#include "TTableFilterModel.h"

TTableFilterModel::TTableFilterModel(QObject *_p_parent)
    : QAbstractTableModel(_p_parent)
{
}

void TTableFilterModel::setColumnCount(int _i_columnCount)
{
    for(int i =0; i < _i_columnCount; i++)
    {
        m_qsl_filters.append("");
    }
}

QStringList TTableFilterModel::filters() const
{
    return m_qsl_filters;
}

int TTableFilterModel::rowCount(const QModelIndex &_h_parent) const
{
    Q_UNUSED(_h_parent)
    return 1;
}

int TTableFilterModel::columnCount(const QModelIndex &_h_parent) const
{
    Q_UNUSED(_h_parent)
    return m_qsl_filters.size();
}

QVariant TTableFilterModel::data(const QModelIndex &_h_index, int _i_role) const
{
    QVariant h_ret;
    if (_h_index.isValid() == true)
    {
        if (_h_index.row() == 0)
        {
            if (_h_index.column() < m_qsl_filters.size() && _h_index.column() >= 0)
            {
                if (_i_role == Qt::DisplayRole)
                {
                    h_ret = m_qsl_filters.at(_h_index.column());
                }
                else
                {
                    /*nothing to do */
                }
            }
            else
            {
                qWarning() << "invalid column" << _h_index.column();
            }
        }
        else
        {
            qWarning() << "invalid row" << _h_index.row();
        }
    }
    else
    {
        qWarning() << "invalid index";
    }

    return h_ret;
}

bool TTableFilterModel::setData(const QModelIndex &_h_index, const QVariant &_qt_value, int _i_role)
{
    bool b_change = false;
    if (_h_index.isValid() == true)
    {
        if (_h_index.row() == 0)
        {
                if (_h_index.column() < m_qsl_filters.size() && _h_index.column() >= 0)
                {
                    if (_i_role == Qt::EditRole)
                    {
                        if (_qt_value.canConvert<QString>() == true)
                        {
                            QString qs_new = _qt_value.toString();
                            if (qs_new != m_qsl_filters.at(_h_index.column()))
                            {
                                m_qsl_filters[_h_index.column()] = qs_new;
                                b_change = true;
                            }
                            else
                            {
                                /* nothing to do */
                            }
                        }
                        else
                        {
                            qWarning() << "new value is not a string";
                        }
                    }
                    else
                    {
                        qWarning() << "role" << _i_role << "is not implemented";
                    }
                }
                else
                {
                    qWarning() << "invalid column" << _h_index.column();
                }
        }
        else
        {
            qWarning() << "invalid row" << _h_index.row();
        }
    }
    else
    {
        qWarning() << "invalid index";
    }

    return b_change;
}

Qt::ItemFlags TTableFilterModel::flags(const QModelIndex &_h_index) const
{
    Qt::ItemFlags h_flags = Qt::ItemIsEnabled;/* it seems that even if index is not valid we should return "Enabled" - I don't know why */
    if (_h_index.isValid() ==true)
    {
        h_flags = QAbstractTableModel::flags(_h_index);
        h_flags |= Qt::ItemIsEditable;
    }
    else
    {
        /* nothing to do */
    }
    return h_flags;
}
