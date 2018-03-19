#include "dlgperfs.h"
#include "ui_dlgperfs.h"

DlgPerfs::DlgPerfs(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DlgPerfs)
{
	ui->setupUi(this);
}

DlgPerfs::~DlgPerfs()
{
	delete ui;
}
