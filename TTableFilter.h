#ifndef TTABLEFILTER_H
#define TTABLEFILTER_H

#include "TTableFilterModel.h"
#include "libColumnFilteredTableGlobal.h"
#include <QSortFilterProxyModel>
//#include <QAbstractProxyModel>
#include <QStandardItem>

class LIBCOLUMNFILTEREDTABLE_EXPORT TTableFilter : public QSortFilterProxyModel
//class LIBCOLUMNFILTEREDTABLE_EXPORT TTableFilter : public QAbstractProxyModel
{
    Q_OBJECT
public:
    TTableFilter(QObject *_p_parent = nullptr);
    void setSourceModel(QAbstractItemModel *_p_sourceModel) override;
    QModelIndex parent(const QModelIndex &_h_child) const override;
    QModelIndex mapFromSource(const QModelIndex &_h_sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &_h_proxyIndex) const override;
    int rowCount(const QModelIndex &_h_parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &_h_parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &_h_index, int _i_role) const override;
    bool setData(const QModelIndex &_h_index, const QVariant &_qt_value, int _i_role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &_h_index) const override;
    void sort(int _i_column, Qt::SortOrder _qt_order = Qt::AscendingOrder) override;
private:
    template<typename typeReturn, typename codeToApply>
    typeReturn toSource(const QModelIndex &_h_idx, codeToApply _h_func)
    {
        typeReturn h_ret;
        if (_h_idx.isValid() == true)
        {
            const QModelIndex h_srcIdx = mapToSource(_h_idx);
            if ((sourceModel() != nullptr)
                && (h_srcIdx.model() == sourceModel()))
            {
                h_ret = _h_func(sourceModel(), h_srcIdx);
            }
            else if (h_srcIdx.model() == &m_h_filterModel)
            {
                h_ret = _h_func(&m_h_filterModel, h_srcIdx);
            }
            else
            {
                qWarning() << "unknown index source" << h_srcIdx.model();
            }
        }
        else
        {
            qWarning() << "invalid index";
        }
        return h_ret;
    }

    template<typename typeReturn, typename codeToApply>
    typeReturn toSourceConst(const QModelIndex &_h_idx, codeToApply _h_func) const
    {
        typeReturn h_ret;
        if (_h_idx.isValid() == true)
        {
            const QModelIndex h_srcIdx = mapToSource(_h_idx);
            if ((sourceModel() != nullptr)
                && (h_srcIdx.model() == sourceModel()))
            {
                h_ret = _h_func(sourceModel(), h_srcIdx);
            }
            else if (h_srcIdx.model() == &m_h_filterModel)
            {
                h_ret = _h_func(&m_h_filterModel, h_srcIdx);
            }
            else
            {
                qWarning() << "unknown index source" << _h_idx.row() << _h_idx.column() << _h_idx.model() << "to" << h_srcIdx.row() << h_srcIdx.column() << h_srcIdx.model();
            }
        }
        else
        {
            qWarning() << "invalid index";
        }
        return h_ret;
    }

    TTableFilterModel m_h_filterModel;
    QList<int> m_ql_rowsDisp;
    int m_i_sortColumn; /* sortColumn always returns -1 - don't know why */
};

#endif // TTABLEFILTER_H
