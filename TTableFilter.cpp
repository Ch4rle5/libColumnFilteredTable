#include "TTableFilter.h"
#include <QDebug>

TTableFilter::TTableFilter(QObject *_p_parent)
//    : QAbstractProxyModel(_p_parent)
    : QSortFilterProxyModel(_p_parent)
    , m_i_sortColumn(-1)
{}

void TTableFilter::setSourceModel(QAbstractItemModel *_p_sourceModel)
{
    if (_p_sourceModel != nullptr)
    {
        m_h_filterModel.setColumnCount(_p_sourceModel->columnCount());
        for (int i=0; i<_p_sourceModel->rowCount()-1; i++)/* TBR -1 should not be here */
        {
            m_ql_rowsDisp.append(i);
        }
    }
    else
    {
        qWarning() << "source is null";
    }
    QSortFilterProxyModel::setSourceModel(_p_sourceModel);
}

QModelIndex TTableFilter::parent(const QModelIndex &_h_child) const
{
    Q_UNUSED(_h_child)
    return QModelIndex();
}

QModelIndex TTableFilter::mapFromSource(const QModelIndex &_h_sourceIndex) const
{
    QModelIndex h_index;
    if (_h_sourceIndex.model() == sourceModel())
    {
        int i_viewIdx = m_ql_rowsDisp.indexOf(_h_sourceIndex.row());
        if(i_viewIdx != -1)
        {
            h_index = index(i_viewIdx +1,
                            _h_sourceIndex.column());
        }
        else
        {
            qWarning() << "source row" <<_h_sourceIndex.row()
                       << "is not on the view";
        }
    }
    else if (_h_sourceIndex.model() == &m_h_filterModel)
    {
        h_index = index(_h_sourceIndex.row(),
                        _h_sourceIndex.column());
    }
    else
    {
        qWarning() << "unknown index source" << _h_sourceIndex.model();
    }
    return h_index;
}

QModelIndex TTableFilter::mapToSource(const QModelIndex &_h_proxyIndex) const
{
    QModelIndex h_index;
    if ((_h_proxyIndex.row() <0) || (_h_proxyIndex.column() <0))
    {
        /* nothing to do */
    }
    else if (_h_proxyIndex.row() == 0)
    {
        h_index = m_h_filterModel.index(_h_proxyIndex.row(),
                                        _h_proxyIndex.column());
    }
    else
    {
        if (sourceModel() != nullptr)
        {
            if (_h_proxyIndex.row() -1 < m_ql_rowsDisp.size())
            {
                int i_srcRow = m_ql_rowsDisp.at(_h_proxyIndex.row() -1);
                h_index = sourceModel()->index(i_srcRow,
                                               _h_proxyIndex.column());
            }
            else
            {
                qWarning() << "unexpected row index"
                           << _h_proxyIndex.row() -1 << m_ql_rowsDisp.size();
            }
        }
        else
        {
            qWarning() << "source is null";
        }
    }
    return h_index;
}

int TTableFilter::rowCount(const QModelIndex &_h_parent) const
{
    Q_UNUSED(_h_parent)

    return m_ql_rowsDisp.size() +1;
}

int TTableFilter::columnCount(const QModelIndex &_h_parent) const
{
    Q_UNUSED(_h_parent)

    int i=0;
    if (sourceModel() != nullptr)
    {
        i = sourceModel()->columnCount();
    }
    else
    {
        qWarning() << "source is null";
    }
    return i;
}

QVariant TTableFilter::data(const QModelIndex &_h_index, int _i_role) const
{
    return toSourceConst<QVariant>(_h_index,
                                   [_i_role](const QAbstractItemModel* _p_model,
                                             const QModelIndex &_h_srcIdx)
                                   {
                                       return _p_model->data(_h_srcIdx, _i_role);
                                   });
}

bool TTableFilter::setData(const QModelIndex &_h_index, const QVariant &_qt_value, int _i_role)
{
    bool b_change = toSource<bool>(_h_index,
                                   [&_qt_value, _i_role](QAbstractItemModel* _p_model,
                                                         const QModelIndex &_h_srcIdx)
                                   {
                                       return _p_model->setData(_h_srcIdx, _qt_value, _i_role);
                                   });

    if (b_change == true)
    {
        QStringList qsl_filters = m_h_filterModel.filters();
        QList<QRegularExpression> ql_regEx;
        for(const auto &filter : qsl_filters)
        {
            ql_regEx.append(QRegularExpression(filter));
        }
        QList<int> ql_srcRowsToDisplay;
        for(int i_srcRow =0; i_srcRow < sourceModel()->rowCount()-1; i_srcRow++)/* TBR -1 should not be here */
        {
            bool b_keep = true;
            for(int i_col=0; i_col < columnCount(); i_col++)
            {
                QVariant qv_data = sourceModel()->data(sourceModel()->index(i_srcRow, i_col));
                if (qv_data.canConvert<QString>() == true)
                {
                    QString qs_val = qv_data.toString();
    qDebug() << "val" << qs_val << "regex" << ql_regEx.at(i_col).pattern()
             << "match" << ql_regEx.at(i_col).match(qs_val).hasMatch();
                    if (ql_regEx.at(i_col).match(qs_val).hasMatch() == false)
                    {
                        b_keep = false;
                        break;
                    }
                    else
                    {
                        /* continue checking filters */
                    }
                }
                else
                {
                    qWarning() << "value is not a string"
                               << i_srcRow << i_col;
                }
            }
            if (b_keep == true)
            {
qDebug() << "append" << i_srcRow;
                ql_srcRowsToDisplay.append(i_srcRow);
            }
            else
            {
                /* filter out this line */
            }
        }
        beginResetModel();
        m_ql_rowsDisp = ql_srcRowsToDisplay;
        sort(m_i_sortColumn, sortOrder());
    }
    else
    {
        /* nothing to do */
    }
    return b_change;
}

Qt::ItemFlags TTableFilter::flags(const QModelIndex &_h_index) const
{
    return toSourceConst<Qt::ItemFlags>(_h_index,
                                        [](const QAbstractItemModel* _p_model,
                                           const QModelIndex &_h_srcIdx)
                                        {
                                            return _p_model->flags(_h_srcIdx);
    });
}

void TTableFilter::sort(int _i_column, Qt::SortOrder _qt_order)
{
    qDebug() << _i_column << _qt_order;
    QMap<QString, int> qm_order;
    for(int i_srcRow : m_ql_rowsDisp)
    {
        QVariant qv_data = sourceModel()->data(
            sourceModel()->index(i_srcRow, _i_column), sortRole());

        if (qv_data.canConvert<QString>() == true)
        {
            QString qs_new = qv_data.toString();
            qm_order.insert(qs_new, i_srcRow);
        }
        else
        {
            qWarning() << "value is not a string"
                       << i_srcRow << _i_column << sortRole();
        }
    }
    beginResetModel();
    m_ql_rowsDisp = qm_order.values();
    if (_qt_order == Qt::DescendingOrder)
    {
        int j = m_ql_rowsDisp.size()-1;
        for(int i =0; i < m_ql_rowsDisp.size()/2; i++,j--)
        {
            m_ql_rowsDisp.swap(i,j);
        }
    }
    else
    {
        /* nothing to do */
    }
    m_i_sortColumn = _i_column;
    endResetModel();
    qDebug() << m_ql_rowsDisp;
}
