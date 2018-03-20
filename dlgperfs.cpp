#include "dlgperfs.h"
#include "ui_dlgperfs.h"

DlgPerfs::DlgPerfs(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DlgPerfs)
{
	ui->setupUi(this);
	QSettings settings;
	QList <QVariant> buffer = settings.value("performances").toList();
	QHash <int, double> perfs;
	double maxX, maxY;
	foreach (QVariant x, buffer) {
		QPointF value = x.toPointF();
		perfs.insert((int)value.x(), value.y());
		maxX = qMax(maxX, value.x());
		maxY = qMax(maxY, value.y());
	}
	// set dark background gradient:
	QLinearGradient gradient(0, 0, 0, 400);
	gradient.setColorAt(0, QColor(90, 90, 90));
	gradient.setColorAt(0.38, QColor(105, 105, 105));
	gradient.setColorAt(1, QColor(70, 70, 70));
	ui->customPlot->setBackground(QBrush(gradient));
	QCPBars *regen = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
	regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
	regen->setName("Temps par mot");
	regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
	regen->setBrush(QColor(0, 168, 140));

	// prepare x axis with country labels:
	ui->customPlot->xAxis->setSubTicks(false);
	ui->customPlot->xAxis->setTickLength(0, 4);
	ui->customPlot->xAxis->setRange(0, maxX+1);
	ui->customPlot->xAxis->setBasePen(QPen(Qt::white));
	ui->customPlot->xAxis->setTickPen(QPen(Qt::white));
	ui->customPlot->xAxis->grid()->setVisible(true);
	ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
	ui->customPlot->xAxis->setTickLabelColor(Qt::white);
	ui->customPlot->xAxis->setLabelColor(Qt::white);

	// prepare y axis:
	ui->customPlot->yAxis->setRange(0, maxY);
	ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
	ui->customPlot->yAxis->setLabel("Temps par mot");
	ui->customPlot->yAxis->setBasePen(QPen(Qt::white));
	ui->customPlot->yAxis->setTickPen(QPen(Qt::white));
	ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white));
	ui->customPlot->yAxis->grid()->setSubGridVisible(true);
	ui->customPlot->yAxis->setTickLabelColor(Qt::white);
	ui->customPlot->yAxis->setLabelColor(Qt::white);
	ui->customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
	ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

	// Add data:
	regen->setData(perfs.keys().toVector(), perfs.values().toVector());

	// setup legend:
	ui->customPlot->legend->setVisible(false);
/*	ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
	ui->customPlot->legend->setBrush(QColor(255, 255, 255, 100));
	ui->customPlot->legend->setBorderPen(Qt::NoPen);
	QFont legendFont = font();
	legendFont.setPointSize(10);
	ui->customPlot->legend->setFont(legendFont);*/
	ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);}

DlgPerfs::~DlgPerfs()
{
	delete ui;
}
