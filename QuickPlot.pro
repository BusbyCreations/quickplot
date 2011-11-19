# QuickPlot.pro
# Copyright (c) 2010, Mark Busby <mark@BusbyFreelance.com>
# This program is distributed under the terms of the GNU GPL, v2.
# See License.txt or http://www.opensource.org/licenses/gpl-2.0.php for details.
HEADERS += QuickPlot.h
SOURCES += QuickPlot.cpp \
    main.cpp
CONFIG += qt \
	warn_on
RESOURCES += resources.qrc
OTHER_FILES += README.txt \
    License.txt \
    EXAMPLE.txt

VERSION_MAJ = 0
VERSION_MIN = 1
VERSION_PAT = 1
VERSION = $${VERSION_MAJ}.$${VERSION_MIN}.$${VERSION_PAT}
DEFINES += APP_VERSION_MAJ=$${VERSION_MAJ} APP_VERSION_MIN=$${VERSION_MIN} APP_VERSION_PAT=$${VERSION_PAT}

QWT_VERSION_MAJ = 5
QWT_VERSION_MIN = 2
QWT_VERSION_PAT = 1
QWT_VERSION = $${QWT_VERSION_MAJ}.$${QWT_VERSION_MIN}.$${QWT_VERSION_PAT}

win32 {
	isEmpty(QWT_LIB_DIR):QWT_LIB_DIR = C:/Qwt-$${QWT_VERSION}/lib
	isEmpty(QWT_INC_DIR):QWT_INC_DIR = C:/Qwt-$${QWT_VERSION}/include
	isEmpty(QWT_LIB_FILENAME):QWT_LIB_FILENAME = qwt5.dll
	isEmpty(QWT_LIB_FILENAME_D):QWT_LIB_FILENAME_D = qwtd5.dll

	INCLUDEPATH += $${QWT_INC_DIR}

	OUT_PWD_R = $$OUT_PWD/release
	OUT_PWD_D = $$OUT_PWD/debug
	OUT_PWD_A = $$OUT_PWD/all # here just so it has correct scope

	PWD_W = $$replace(PWD, /, \\)
	OUT_PWD_R_W = $$replace(OUT_PWD_R, /, \\)
	OUT_PWD_D_W = $$replace(OUT_PWD_D, /, \\)
	QWT_LIB_DIR_W = $$replace(QWT_LIB_DIR, /, \\)

	DEFINES += QWT_DLL
	LIBS += -L\"$$QWT_LIB_DIR_W\"

	QT_BIN_DIR = $$dirname(QMAKE_QMAKE)
	QT_BIN_DIR_W = $$replace(QT_BIN_DIR, /, \\)

	!exists($$OUT_PWD_R):system(mkdir $$quote($$OUT_PWD_R_W))
	!exists($$OUT_PWD_D):system(mkdir $$quote($$OUT_PWD_D_W))

	CONFIG(debug, debug|release) {
		message("Debug build")
		OUT_PWD_A = $$OUT_PWD_D
		OUT_PWD_A_W = $$OUT_PWD_D_W

		LIBS += -lqwtd5

		message("Copying Qt debug dlls")
		!exists($$OUT_PWD_D/QtGuid4.dll):system(copy /Y \"$$QT_BIN_DIR_W\\QtGuid4.dll\" \"$$OUT_PWD_D_W\\\")
		!exists($$OUT_PWD_D/QtCored4.dll):system(copy /Y \"$$QT_BIN_DIR_W\\QtCored4.dll\" \"$$OUT_PWD_D_W\\\")

		message("Copying Qwt debug dll")
		!exists($$OUT_PWD_A/$$QWT_LIB_FILENAME_D):system(copy /Y \"$$QWT_LIB_DIR_W\\$$QWT_LIB_FILENAME_D\" \"$$OUT_PWD_D_W\\\")
	}
	CONFIG(release, debug|release) {
		message("Release build")
		OUT_PWD_A = $$OUT_PWD_R
		OUT_PWD_A_W = $$OUT_PWD_R_W

		LIBS += -lqwt5
	}

	message("Copying Qt dlls")
	!exists($$OUT_PWD_A/QtGui4.dll): system(copy /Y \"$$QT_BIN_DIR_W\\QtGui4.dll\" \"$$OUT_PWD_A_W\\\")
	!exists($$OUT_PWD_A/QtCore4.dll):system(copy /Y \"$$QT_BIN_DIR_W\\QtCore4.dll\" \"$$OUT_PWD_A_W\\\")

	message("Copying Qwt dll")
	!exists($$OUT_PWD_A/$$QWT_LIB_FILENAME):system(copy /Y \"$$QWT_LIB_DIR_W\\$$QWT_LIB_FILENAME\" \"$$OUT_PWD_A_W\\\")
}

linux-g++ {
	message("This is linux")
	LIBS += -lqwt-qt4
	INCLUDEPATH += $$quote(/usr/include/qwt-qt4)
}

message(QMAKESPEC = $$QMAKESPEC)
message(PWD = $$PWD)
message(OUT_PWD = $$OUT_PWD)
message(QT = $$QT)
message(DEFINES = $$DEFINES)
message(CONFIG = $$CONFIG)
message(QMAKE_CXX = $$QMAKE_CXX)
message(MAKEFILE_GENERATOR = $$MAKEFILE_GENERATOR)
message(INCLUDEPATH = $$INCLUDEPATH)
message(LIBS = $$LIBS)

