#ifndef TTABLEFILTERMODEL_H
#define TTABLEFILTERMODEL_H

#include "libColumnFilteredTableGlobal.h"
#include <QAbstractTableModel>
#include <QTableWidgetItem>
#include <QFile>
#include <QDebug>

class LIBCOLUMNFILTEREDTABLE_EXPORT TTableFilterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TTableFilterModel(QObject *_p_parent = nullptr);
    void setColumnCount(int _i_columnCount);
    QStringList filters() const;

private:
    int rowCount(const QModelIndex &_h_parent) const;
    int columnCount(const QModelIndex &_h_parent) const;
    QVariant data(const QModelIndex &_h_index, int _i_role) const;
    bool setData(const QModelIndex &_h_index, const QVariant &_qt_value, int _i_role);
    Qt::ItemFlags flags(const QModelIndex &_h_index) const;
    QStringList m_qsl_filters;

};

#endif // TTABLEFILTERMODEL_H
