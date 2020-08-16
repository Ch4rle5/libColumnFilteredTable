#ifndef TTABLE_H
#define TTABLE_H

#include "TTableFilter.h"
#include "TTableModel.h"
#include "libColumnFilteredTableGlobal.h"
#include <QTableView>

template<typename rowModel>
class LIBCOLUMNFILTEREDTABLE_EXPORT TTable
{
public:
    TTable(QString _qs_filename, QObject *_p_parent = nullptr)
        : m_h_model(_qs_filename, _p_parent)
    {
        m_h_filter.setSourceModel(&m_h_model);
        m_h_view.setModel(&m_h_filter);
        m_h_view.setSortingEnabled(true);
    }

    QTableView *p_view()
    {
        return &m_h_view;
    }

private:
    TTableModel<rowModel> m_h_model;
    TTableFilter m_h_filter;
    QTableView m_h_view;
};

#endif // TTABLE_H
