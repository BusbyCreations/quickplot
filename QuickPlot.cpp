/* QuickPlot.cpp
 * 
 * Copyright (c) 2010, Mark Busby <mark@BusbyFreelance.com>
 * This program is distributed under the terms of the GNU GPL, v2.
 * See License.txt or http://www.opensource.org/licenses/gpl-2.0.php for details.
 */

#include "QuickPlot.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>

QuickPlot::QuickPlot(QWidget* parent) : QWizard(parent)
{
	addPage(new Page1);
	addPage(new Page2);
	addPage(new Page3);
	addPage(new Page4);
	addPage(new Page5);

	setWindowTitle("QuickPlot");
	resize(QSize(640, 480));
}

//------------------------------------------------------------------------------

Page1::Page1(QWidget* parent) : QWizardPage(parent)
{
	setTitle("QuickPlot");
	
	label = new QLabel(tr("This little program will show a plot of a two dimensional data set "
						  "of your choosing.\n\n"
						  "The data must be in a tab delimited text "
						  "file, such as may be created using the export options in OpenOffice.org "
						  "Calc or Microsoft Excel.\n\n"
						  "This program is copyright (c) Mark Busby, 2010, and is distributed under "
						  "the terms of the GNU GPL version 2 or later.  You are free to use the "
						  "software, see the source code, and pass it on.  Quickplot and it's "
						  "source code may be downloaded from www.BusbyFreelance.com.\n\n"
						  "BusbyFreelance.com, home of your data collection and analysis specialist!"));
	label->setWordWrap(true);
	
	QLabel *logoLabel = new QLabel;
	QPixmap logo = QPixmap(":/images/BusbyFreelanceLogo.png").scaledToWidth(180, Qt::SmoothTransformation);
	logoLabel->setPixmap(logo);
	logoLabel->setScaledContents(false);
	
	QGridLayout *mainlayout = new QGridLayout;
//	QVBoxLayout *layout = new QVBoxLayout;
//	layout->addWidget(label);
//	QHBoxLayout *layout2 = new QHBoxLayout;
//	layout2->addWidget(label2);
//	layout2->addWidget(logoLabel);
//	layout->addLayout(layout2);
	mainlayout->addWidget(logoLabel,0,0,1,1,Qt::AlignTop);
	mainlayout->addWidget(label, 0, 1);
	setLayout(mainlayout);
}

//------------------------------------------------------------------------------

Page2::Page2(QWidget* parent) : QWizardPage(parent)
{
	setTitle("QuickPlot");
	setSubTitle(tr("Select a data set"));

	label1 = new QLabel(tr("The data may be in any text file on your computer."));
	label1->setWordWrap(true);

	label2 = new QLabel(tr("Please choose a data file:"));
	lineEditFileName = new QLineEdit;
	registerField("lineEditFileName*", lineEditFileName);

	pushButtonChooseFile = new QPushButton;
	pushButtonChooseFile->setText(tr("Browse"));
	connect(pushButtonChooseFile, SIGNAL(clicked()), this, SLOT(on_pushButtonChooseFile_clicked()));

	QHBoxLayout *hBoxLayout = new QHBoxLayout;
	hBoxLayout->addWidget(lineEditFileName);
	hBoxLayout->addWidget(pushButtonChooseFile);

	QVBoxLayout *vBoxLayout = new QVBoxLayout;
	vBoxLayout->addWidget(label1);
	vBoxLayout->addWidget(label2);
	vBoxLayout->addLayout(hBoxLayout);
	setLayout(vBoxLayout);
}

void Page2::on_pushButtonChooseFile_clicked()
{
	lineEditFileName->setText(QFileDialog::getOpenFileName(this, tr("Choose a text file"), "",
														   "Text file (*.txt);;All Files (*)"));
}

bool Page2::validatePage()
{
	std::ifstream *inFile = new std::ifstream(lineEditFileName->text().toStdString().c_str());
	if (!inFile)
	{
		QMessageBox::critical(wizard(), tr("Can't read file"), tr("The file cannot be opened for reading"));
		return false;
	}
	
	std::string input;
	std::getline(*inFile,input);
	
	if (inFile->bad())
	{
		QMessageBox::critical(wizard(), tr("Can't read file"), tr("Can't read a line."));
		return false;
	}

	std::string input2;
	for (int i = 0; i < 3; i++)
	{
		std::getline(*inFile, input2);
		if (inFile->bad())
		{
			QMessageBox::critical(wizard(), tr("Not enough information in file"),
								  tr("The file must have at least three lines"));
			return false;
		}
	}

	return true;
}

//------------------------------------------------------------------------------

Page3::Page3(QWidget* parent) : QWizardPage(parent)
{
	setTitle("QuickPlot");
	setSubTitle(tr("Data details"));

	label1 = new QLabel(tr("Select the line containing the data (column) labels"));
	listWidget1 = new QListWidget(this);
	listWidget1->setSelectionMode(QAbstractItemView::SingleSelection);

	label2 = new QLabel(tr("Select the first line (row) of data.  All lines of text before the "
						   "selected line will be ignored."));
	listWidget2 = new QListWidget(this);
	listWidget2->setSelectionMode(QAbstractItemView::SingleSelection);

	registerField("dataLabelsLine*", listWidget1);
	registerField("dataStartLine*", listWidget2);

	QVBoxLayout *vBoxLayout = new QVBoxLayout;
	vBoxLayout->addWidget(label1);
	vBoxLayout->addWidget(listWidget1);
	vBoxLayout->addWidget(label2);
	vBoxLayout->addWidget(listWidget2);
	setLayout(vBoxLayout);
}

void Page3::initializePage()
{
	std::ifstream *dataFile = new std::ifstream(field("lineEditFileName").toString().toStdString().c_str());
	if (!dataFile)
	{
		QMessageBox::critical(wizard(), tr("Can't read file"), tr("The file cannot be opened for reading"));
	}

	for (int i = 0; i < 50; i++)
	{
		std::string input;
		std::getline(*dataFile, input);
		if (!dataFile->bad())
		{
			QString line(input.c_str());
			line.remove(QChar('\r'));
			listWidget1->addItem(line);
			listWidget2->addItem(line);
		}
		else
		{
			if (i == 0) QMessageBox::critical(wizard(), tr("Can't read file"), tr("The file cannot be read"));

			break;
		}
	}
}

//------------------------------------------------------------------------------

Page4::Page4(QWidget *parent) : QWizardPage(parent)
{
	tableWidgetXSelected = false;
	tableWidgetYSelected = false;

	labelX = new QLabel(tr("Choose the column containing the \"x\" variables."));
	labelX->setWordWrap(true);

	tableWidgetX = new QTableWidget(1000, 2);
	tableWidgetX->setSelectionBehavior(QAbstractItemView::SelectColumns);
	tableWidgetX->setSelectionMode(QAbstractItemView::SingleSelection);

	labelY = new QLabel(tr("Choose the column containing the \"y\" variables."));
	labelY->setWordWrap(true);

	tableWidgetY = new QTableWidget(1000, 2);
	tableWidgetY->setSelectionBehavior(QAbstractItemView::SelectColumns);
	tableWidgetY->setSelectionMode(QAbstractItemView::SingleSelection);

	QVBoxLayout *vBoxLayout = new QVBoxLayout;
	vBoxLayout->addWidget(labelX);
	vBoxLayout->addWidget(tableWidgetX);
	vBoxLayout->addWidget(labelY);
	vBoxLayout->addWidget(tableWidgetY);
	setLayout(vBoxLayout);
}

void Page4::initializePage()
{
	std::ifstream *dataFile = new std::ifstream(field("lineEditFileName").toString().toStdString().c_str());
	if (!dataFile)
	{
		QMessageBox::critical(wizard(), tr("Can't read file"), tr("The file cannot be opened for reading"));
	}

	bool canConvert;
	int dataLabelsLine = field("dataLabelsLine").toInt(&canConvert);
	QString dataLabels;
	QStringList dataLabelsList;

	if (!canConvert)
	{
		QMessageBox::critical(wizard(), tr("Can't covert token"),
							  tr("The line selected for Labels can't be understood as an int"));
	}

	std::string input;
	
	for (int i = 0; i <= dataLabelsLine; i++)
	{	
		std::getline(*dataFile, input);
	}

	dataLabels = QString(input.c_str());
	dataLabels.remove(QChar('\r'));
	
	dataLabelsList = dataLabels.split("\t");
	int size = dataLabelsList.size();
	tableWidgetX->setColumnCount(size);
	tableWidgetX->setHorizontalHeaderLabels(dataLabelsList);
	tableWidgetY->setColumnCount(size);
	tableWidgetY->setHorizontalHeaderLabels(dataLabelsList);

	dataFile->close();
	delete dataFile;
	dataFile = new std::ifstream(field("lineEditFileName").toString().toStdString().c_str());
	if (!dataFile)
	{
		QMessageBox::critical(wizard(), tr("Can't read file"), tr("The file cannot be opened for reading"));
	}

	int dataStartLine = field("dataStartLine").toInt(&canConvert);
	QString dataLine;
	QStringList dataLineList;

	if (!canConvert)
	{
		QMessageBox::critical(wizard(), tr("Can't covert token"),
							  tr("The line selected for Data can't be understood as an int"));
	}

	int row = 0;
	while (dataFile->good())
	{
		if (row < dataStartLine) 
		{
			std::getline(*dataFile, input); // ignore the entire line, not just 1000 characters
		}
		else
		{
			std::getline(*dataFile, input);
			dataLine = input.c_str();
			dataLine.remove(QChar('\r'));
			dataLineList = dataLine.split("\t");

			if (dataLineList.size() <= 1) break;

			int column = 0;
			for (QStringList::iterator it = dataLineList.begin(); it != dataLineList.end(); it++)
			{
				QTableWidgetItem *tableItemX = new QTableWidgetItem(*it);
				QTableWidgetItem *tableItemY = new QTableWidgetItem(*it);
				tableWidgetX->setItem(row-dataStartLine, column, tableItemX);
				tableWidgetY->setItem(row-dataStartLine, column, tableItemY);
				column++;

//				std::cout << "data item: " << (*it).toStdString() << "\n"
//						<< "       row: " << i - dataStartLine << "\n"
//						<< "    column: " << column << std::endl;
			}
		}
		row++;
	}

	tableWidgetX->setRowCount(row);
	tableWidgetY->setRowCount(row);

	tableWidgetX->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableWidgetY->setEditTriggers(QAbstractItemView::NoEditTriggers);

	tableWidgetXSelected = false;
	tableWidgetYSelected = false;

	connect(tableWidgetX, SIGNAL(itemSelectionChanged()), SLOT(on_tableWidgetX_itemSelectionChanged()));
	connect(tableWidgetY, SIGNAL(itemSelectionChanged()), SLOT(on_tableWidgetY_itemSelectionChanged()));
}

void Page4::on_tableWidgetX_itemSelectionChanged()
{
//	if (tableWidgetX->currentColumn() != tableWidgetY->currentColumn())
	{
		tableWidgetXSelected = true;
		static_cast<QuickPlot *>(wizard())->data.xColumn = tableWidgetX->currentColumn();
		//std::cout << "X ok" << tableWidgetX->currentColumn() << std::endl;
	}
//	else tableWidgetXSelected = false;
	emit completeChanged();
}

void Page4::on_tableWidgetY_itemSelectionChanged()
{
//	if (tableWidgetX->currentColumn() != tableWidgetY->currentColumn())
	{
		tableWidgetYSelected = true;
		static_cast<QuickPlot *>(wizard())->data.yColumn = tableWidgetY->currentColumn();
		//std::cout << "Y ok at " << tableWidgetY->currentColumn() << std::endl;
	}
//	else tableWidgetYSelected = false;
	emit completeChanged();
}

bool Page4::isComplete() const
{
	if (tableWidgetXSelected
		&& tableWidgetYSelected
		&& tableWidgetY->currentColumn() >= 0
		&& tableWidgetX->currentColumn() >= 0
		/*&& tableWidgetX->currentColumn() != tableWidgetY->currentColumn()*/)
		return true;
	else return false;
}

//------------------------------------------------------------------------------

Page5::Page5(QWidget *parent) : QWizardPage(parent)
{
	label = new QLabel(tr("Each data point plotted, together with a least-squares fit of the data."));
	plot = new Plot(this);
	slope = new QLabel;
	intercept = new QLabel;
	QVBoxLayout *vBoxLayout = new QVBoxLayout;
	vBoxLayout->addWidget(label);
	vBoxLayout->addWidget(plot);
	vBoxLayout->addWidget(slope);
	vBoxLayout->addWidget(intercept);
	setLayout(vBoxLayout);
}

void Page5::initializePage()
{
	fillData();
	Data *data = static_cast<Data *>(&(static_cast<QuickPlot *>(wizard())->data));
	data->leastSquares();

	slope->setText(QString("slope: ") + QString::number(data->slope, 'g', 10));
	intercept->setText(QString("intercept: ") + QString::number(data->intercept, 'g', 10));

	plot->setAxisTitle(QwtPlot::xBottom, data->labelX);
	plot->setAxisTitle(QwtPlot::yLeft, data->labelY);
	plot->setCanvasBackground(Qt::white);

	double xMin = data->x[0], xMax = data->x[0],
		yMin = data->y[0], yMax = data->y[0],
		xRaw[data->x.size()], yRaw[data->x.size()];
	for (unsigned int i = 0; i < data->x.size(); i++)
	{
		if (data->x[i] > xMax) xMax = data->x[i];
		if (data->x[i] < xMin) xMin = data->x[i];
		if (data->y[i] > yMax) yMax = data->y[i];
		if (data->y[i] < yMin) yMin = data->y[i];

		xRaw[i] = data->x[i];
		yRaw[i] = data->y[i];
	}
	plot->rawData->setData(xRaw, yRaw, data->x.size());

	double x[2], y[2];
		x[0] = xMin - (xMax - xMin)/20.;
		x[1] = xMax + (xMax - xMin)/20.;
	for (int i = 0; i < 2; i++)
		y[i] = data->intercept + x[i] * data->slope;
	plot->fit->setData(x, y, 2);

	plot->replot();
}

void Page5::fillData()
{
	Data *data = static_cast<Data *>(&(static_cast<QuickPlot *>(wizard())->data));
	data->x.clear();
	data->y.clear();

	std::ifstream *dataFile = new std::ifstream(field("lineEditFileName").toString().toStdString().c_str());
	if (!dataFile)
	{
		QMessageBox::critical(wizard(), tr("Can't read file"), tr("The file cannot be opened for reading"));
	}

	bool canConvert;
	int dataLabelsLine = field("dataLabelsLine").toInt(&canConvert);
	QString dataLabels;
	QStringList dataLabelsList;

	if (!canConvert)
	{
		QMessageBox::critical(wizard(), tr("Can't covert token"),
							  tr("The line selected for Labels can't be understood as an int"));
	}

	
	std::string input;
	
	for (int i = 0; i <= dataLabelsLine; i++)
	{
		std::getline(*dataFile, input);
	}
	
	dataLabels = input.c_str();
	dataLabels.remove(QChar('\r'));
	
	dataLabelsList = dataLabels.split("\t");
	data->labelX = dataLabelsList.at(data->xColumn);
	data->labelY = dataLabelsList.at(data->yColumn);

	dataFile->close();
	delete dataFile;
	dataFile = new std::ifstream(field("lineEditFileName").toString().toStdString().c_str());
	if (!dataFile)
	{
		QMessageBox::critical(wizard(), tr("Can't read file"), tr("The file cannot be opened for reading"));
	}

	int dataStartLine = field("dataStartLine").toInt(&canConvert);
	QString dataLine;
	QStringList dataLineList;

	if (!canConvert)
	{
		QMessageBox::critical(wizard(), tr("Can't covert token"),
							  tr("The line selected for Data can't be understood as an int"));
	}

	int row = 0;
	while (dataFile->good())
	{
		if (row < dataStartLine)
		{
			std::string tempString;
			std::getline(*dataFile, tempString); // ignore a line
			row++;
			continue;
		}

		std::string input;
		std::getline(*dataFile, input);
		
		dataLine = input.c_str();
		dataLine.remove(QChar('\r'));
		dataLineList = dataLine.split("\t");

		if (dataLineList.size() < data->xColumn + 1) continue;
		if (dataLineList.size() < data->yColumn + 1) continue;

		QString stringValues[2] = {dataLineList.at(data->xColumn), dataLineList.at(data->yColumn)};
		double values[2];
		for (int i = 0; i < 2; i++)
		{
			int timeSeparators = stringValues[i].count(':');
			if (timeSeparators)
			{
				if (timeSeparators > 2)
				{
					QMessageBox::critical(this, tr("Bad data type"),
										  tr("Can't have a data type with more than two \":\" characters"));
					exit(1);
				}

				if (i == 0) data->xIsTime = true;
				else data->yIsTime = true;

				double hours = 0, minutes = 0, seconds = 0; // all will be converted to seconds.

				if (timeSeparators == 2)
				{
					hours = stringValues[i].section(':', 0, 0).toDouble();
					minutes = stringValues[i].section(':', 1, 1).toDouble();
					seconds = stringValues[i].section(':', 2, 2).toDouble();
				}
				else if (timeSeparators == 1)
				{
					minutes = stringValues[i].section(':', 0, 0).toDouble();
					seconds = stringValues[i].section(':', 1, 1).toDouble();
				}

				values[i] = (60. * 60. * hours + 60. * minutes + seconds)/60;
			}
			else
			{
				values[i] = stringValues[i].toDouble();
			}
		}

		//std::cout << "(" << values[0] << " , " << values[1] << ")\t";
		//if (!(row % 10)) std::cout << std::endl;
		data->addPoint(values[0], values[1]);
		row++;
	}
}
