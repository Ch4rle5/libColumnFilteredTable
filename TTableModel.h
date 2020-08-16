#ifndef TTABLEMODEL_H
#define TTABLEMODEL_H

#include "libColumnFilteredTableGlobal.h"
#include <QAbstractTableModel>
#include <QTableWidgetItem>
#include <QFile>
#include <QDebug>

template<typename rowModel>
class LIBCOLUMNFILTEREDTABLE_EXPORT TTableModel : public QAbstractTableModel
{
public:
    TTableModel(QString _qs_filename, QObject *_p_parent = nullptr)
        : QAbstractTableModel(_p_parent)
    {
        QFile qf_icd(_qs_filename);
        if (qf_icd.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QString qs_header = QString(qf_icd.readLine())
                                    .remove('\n')
                                    .remove('\r');
            if(qs_header.count(',') > qs_header.count(';'))
            {
                m_c_separator = ',';
            }
            else
            {
                m_c_separator = ';';
            }
            m_qsl_header = qs_header.split(m_c_separator);
        }
        else
        {
            qWarning() << "failed to open" << _qs_filename;
        }
        v_addFile(_qs_filename);
    }

    void v_addFile(QString _qs_filename)
    {
        QFile qf_icd(_qs_filename);
        if (qf_icd.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QStringList qsl_header = QString(qf_icd.readLine())
                                         .remove('\n')
                                         .remove('\r')
                                         .split(m_c_separator);
            if (m_qsl_header != qsl_header)
            {
                qWarning() << "file" << _qs_filename << "has unexpected header";
            }
            else
            {
                /* nothing to do */
            }

            int i_oldSize = m_ql_params.size();
            while (qf_icd.atEnd() == false)
            {
                QStringList qsl_param = QString(qf_icd.readLine())
                                            .remove('\n')
                                            .remove('\r')
                                            .split(m_c_separator);
                m_ql_params.append(new rowModel(qsl_param));
            }
            qDebug() << _qs_filename
                     << "parsed" << m_ql_params.size() << "lines";
            beginInsertRows(QModelIndex(), i_oldSize, m_ql_params.size());
            endInsertRows();
        }
        else
        {
            qWarning() << "failed to open" << _qs_filename;
        }
    }

    const QStringList &qsl_header()
    {
        return m_qsl_header;
    }

    int rowCount(const QModelIndex &_h_parent) const
    {
        Q_UNUSED(_h_parent)
        return m_ql_params.size()+1;/* TBR +1 should not be here */
    }

    int columnCount(const QModelIndex &_h_parent) const
    {
        Q_UNUSED(_h_parent)
        return m_qsl_header.size();
    }

    QModelIndex parent(const QModelIndex &_h_child) const
    {
        Q_UNUSED(_h_child)
        return QModelIndex();
    }

    QVariant data(const QModelIndex &_h_index, int _i_role) const
    {
        QVariant h_ret;
        if (_h_index.isValid() == true)
        {
            if (_h_index.row() < m_ql_params.size() && _h_index.row() >= 0)
            {
                rowModel const * const p_param = m_ql_params.at(_h_index.row());
                if (p_param != nullptr)
                {
                    if (_h_index.column() < p_param->size() && _h_index.column() >= 0)
                    {
                        if (_i_role == Qt::DisplayRole)
                        {
                            h_ret = p_param->at(_h_index.column());
                        }
                        else
                        {
                            /* role not implemented */
                        }
                    }
                    else
                    {
                        qWarning() << "invalid column" << _h_index.column();
                    }
                }
                else
                {
                    qWarning() << "param is null" << _h_index.row();
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

    bool setData(const QModelIndex &_h_index, const QVariant &_qt_value, int _i_role)
    {
        bool b_change = false;
        if (_h_index.isValid() == true)
        {
            if (_h_index.row() < m_ql_params.size() && _h_index.row() >= 0)
            {
                rowModel * const p_param = m_ql_params[_h_index.row()];
                if (p_param != nullptr)
                {
                    if (_h_index.column() < p_param->size() && _h_index.column() >= 0)
                    {
                        if (_i_role == Qt::EditRole)
                        {
                            if (_qt_value.canConvert<QString>() == true)
                            {
                                QString qs_new = _qt_value.toString();
                                if (qs_new != p_param->at(_h_index.column()))
                                {
                                    (*p_param)[_h_index.column()] = qs_new;
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
                    qWarning() << "param is null" << _h_index.row();
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

    Qt::ItemFlags flags(const QModelIndex &_h_index) const
    {
        Qt::ItemFlags h_flags = Qt::ItemIsEnabled;/* it seems that even if index is not valid we should return "Enabled" */
        if (_h_index.isValid() ==true)
        {
            h_flags = QAbstractTableModel::flags(_h_index);
            h_flags &= ~Qt::ItemIsEditable;
        }
        else
        {
            /* nothing to do */
        }
        return h_flags;
    }

    QVariant headerData(int _i_section, Qt::Orientation _qt_orientation,
                                     int _i_role) const
    {
        QVariant qt_ret;
        if (_i_role == Qt::DisplayRole)
        {
            if (_qt_orientation == Qt::Horizontal)
            {
                if ((_i_section < m_qsl_header.size()) && (_i_section >=0))
                {
                    qt_ret = m_qsl_header.at(_i_section);
                }
                else
                {
                    qWarning() << "invalid section" << _i_section;
                }
            }
            else
            {
                /* nothing to do */
            }
        }
        else
        {
            /* role not implemented */
        }
        return qt_ret;
    }

private:
    QStringList m_qsl_header;
    QList<rowModel *> m_ql_params;
    char m_c_separator;
};

#endif // TTABLEMODEL_H
