/* main.cpp
 * 
 * Copyright (c) 2010, Mark Busby <mark@BusbyFreelance.com>
 * This program is distributed under the terms of the GNU GPL, v2.
 * See License.txt or http://www.opensource.org/licenses/gpl-2.0.php for details.
 */

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "QuickPlot.h"

int main(int argc, char *argv[])
{
	// initialize resources, if needed
	// Q_INIT_RESOURCE(QuickPlot);

	QApplication app(argc, argv);

	// create and show your widgets here
	QuickPlot quickPlot;
	quickPlot.show();

	return app.exec();
}
