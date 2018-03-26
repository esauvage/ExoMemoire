#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::setOrdre(bool ordre)
{
	ui->rbtOrdre->setChecked(ordre);
	ui->rbtSansOrdre->setChecked(!ordre);
}

bool PreferencesDialog::ordre()
{
	return ui->rbtOrdre->isChecked();
}
