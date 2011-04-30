# QuickPlot.pro
# Copyright (c) 2010, Mark Busby <mark@BusbyFreelance.com>
# This program is distributed under the terms of the GNU GPL, v2.
# See License.txt or http://www.opensource.org/licenses/gpl-2.0.php for details.
HEADERS += QuickPlot.h
SOURCES += QuickPlot.cpp \
    main.cpp
CONFIG += qt \
    warn_on \
    debug
INCLUDEPATH += /home/mobusby/downloads/qwt/include
LIBS += -L/home/mobusby/downloads/qwt/lib/static \
    -lqwt
RESOURCES += resources.qrc
OTHER_FILES += README.txt \
    License.txt \
    EXAMPLE.txt
