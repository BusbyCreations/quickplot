/* QuickPlot.h
 * Class definitions for QuickPlot
 *
 * Copyright (c) 2010, Mark Busby <mark@BusbyFreelance.com>
 * This program is distributed under the terms of the GNU GPL, v2.
 * See License.txt or http://www.opensource.org/licenses/gpl-2.0.php for details.
 */

#ifndef _QUICKPLOT_H
#define	_QUICKPLOT_H

#include <vector>

#include <QtGui>
#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QListWidget>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_canvas.h>

class Data
{
public:
	Data()
	{
		labelX = "";
		labelY = "";
		xIsTime = false;
		yIsTime = false;
	}

	void addPoint(double x, double y)
	{
		this->x.push_back(x);
		this->y.push_back(y);
	}

	void leastSquares()
	{
		double sumXSquared = 0;
		double sumX = 0;
		double sumY = 0;
		double sumXY = 0;

		std::vector<double>::iterator yIt = y.begin();
		for (std::vector<double>::iterator xIt = x.begin(); xIt != x.end(); xIt++)
		{
			double xVal = *xIt;
			double yVal = *yIt;
			sumXSquared += xVal * xVal;
			sumX += xVal;
			sumY += yVal;
			sumXY += xVal * yVal;

			yIt++;
		}

		double delta = double(x.size()) * sumXSquared - (sumX * sumX);
		slope = (double(x.size()) * sumXY - sumX * sumY)/delta;
		intercept = (sumXSquared * sumY - sumX * sumXY)/delta;
	}

	QString labelX;
	QString labelY;
	double intercept;
	double slope;
	std::vector<double> x;
	std::vector<double> y;
	int xColumn;
	int yColumn;
	bool xIsTime;
	bool yIsTime;
	QString title;
};

class Plot : public QwtPlot
{
	Q_OBJECT

public:
	Plot(QWidget *parent = 0) : QwtPlot(parent)
	{
		rawDataSymbol = new QwtSymbol(QwtSymbol::XCross, QBrush(QColor(Qt::black)), QPen(QColor(Qt::black)), QSize(5, 5));

		rawData = new QwtPlotCurve("Raw Data");
		rawData->setStyle(QwtPlotCurve::NoCurve);
		rawData->setPaintAttribute(QwtPlotCurve::PaintFiltered); // don't draw duplicates
		rawData->setSymbol(*rawDataSymbol);
		rawData->attach(this);

		fit = new QwtPlotCurve("Least-Squares Fit");
		fit->setStyle(QwtPlotCurve::Lines);
		fit->setRenderHint(QwtPlotItem::RenderAntialiased);
		fit->setPen(QPen(Qt::red));
		//fit->setBrush(QBrush(QColor(Qt::red)));
		fit->setPaintAttribute(QwtPlotCurve::PaintFiltered); // don't draw duplicates
		fit->attach(this);

		// horizontal line at y = 0
		QwtPlotMarker *y0 = new QwtPlotMarker();
		y0->setLineStyle(QwtPlotMarker::HLine);
		y0->setYValue(0.0);
		y0->setLinePen(QPen(Qt::gray));
		y0->attach(this);

		// vertical line at x = 0
		QwtPlotMarker *x0 = new QwtPlotMarker();
		x0->setLineStyle(QwtPlotMarker::VLine);
		x0->setXValue(0.0);
		x0->setLinePen(QPen(Qt::gray));
		x0->attach(this);
	}

	void drawPlot()
	{
		// TODO: what does this do?  Taken fromincrementalPlot.cpp (Qwt example)
		const bool cacheMode = canvas()->testPaintAttribute(QwtPlotCanvas::PaintCached);
		canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
		rawData->draw(0, -1);
		fit->draw(0, -1);
		canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, cacheMode);
	}

	QwtPlotCurve *rawData;
	QwtSymbol *rawDataSymbol;
	QwtPlotCurve *fit;

private:
	QwtPlotMarker *y0;
	QwtPlotMarker *x0;
};

class QuickPlot : public QWizard
{
	Q_OBJECT

public:
	QuickPlot(QWidget *parent = 0);

	Data data;
};

class Page1 : public QWizardPage
{
	Q_OBJECT

public:
	Page1(QWidget *parent = 0);

private:
	QLabel *label;
};

class Page2 : public QWizardPage
{
	Q_OBJECT

public:
	Page2(QWidget *parent = 0);
	bool validatePage();

public slots:
	void on_pushButtonChooseFile_clicked();

private:
	QLabel *label1;
	QLabel *label2;
	QLineEdit *lineEditFileName;
	QPushButton *pushButtonChooseFile;
};

class Page3 : public QWizardPage
{
	Q_OBJECT

public:
	Page3(QWidget *parent = 0);

protected:
	void initializePage();

private:
	QLabel *label1;
	QLabel *label2;
	QListWidget *listWidget1;
	QListWidget *listWidget2;
};

class Page4 : public QWizardPage
{
	Q_OBJECT

public:
	Page4(QWidget *parent = 0);
	virtual bool isComplete() const;

public slots:
	void on_tableWidgetX_itemSelectionChanged();
	void on_tableWidgetY_itemSelectionChanged();

protected:
	void initializePage();

private:
	QLabel *labelX;
	QLabel *labelY;
	QTableWidget *tableWidgetX;
	QTableWidget *tableWidgetY;
	bool tableWidgetXSelected;
	bool tableWidgetYSelected;
};

class Page5 : public QWizardPage
{
	Q_OBJECT

public:
	Page5(QWidget *parent = 0);

protected:
	void initializePage();

private:
	void fillData();
	QLabel *label;
	QLabel *slope;
	QLabel *intercept;
	Plot *plot;
};

#endif	/* _QUICKPLOT_H */

