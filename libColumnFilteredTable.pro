TARGET = columnFilteredTable
TEMPLATE = lib
DEFINES += LIBCOLUMNFILTEREDTABLE_LIBRARY

CONFIG += c++11

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TTableFilterModel.cpp \
    TTableFilter.cpp \
    TTable.cpp \

HEADERS += \
    libColumnFilteredTableGlobal.h \
    TTableModel.h \
    TTableFilterModel.h \
    TTableFilter.h \
    TTableView.h \
    TTable.h \

headers.files = $$HEADERS
headers.path = /usr/include
INSTALLS += headers

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
