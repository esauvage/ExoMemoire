#include "memoire.h"
#include "ui_memoire.h"

#include "dlgperfs.h"
#include "preferencesdialog.h"

#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <time.h>

Memoire::Memoire(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Memoire), _chrono(nullptr), _elapsedSeconds(0)
{
	qsrand(time(NULL));
	ui->setupUi(this);
	QString file = QApplication::applicationDirPath() + "/defaut.txt";
	QFile fDict(file);
	if (!fDict.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this, "Échec à l'import", QString("Le fichier %1 n'a pas pu être ouvert.").arg(file));
		return;
	}
	chargeItems(fDict);
	fDict.close();
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Sauvage Software", "ExoMemoire");
	ui->bxDifficulte->setValue(settings.value("Last level", 3).toInt());
	_fctCorrection = settings.value("Ordre", true).toBool() ? &Memoire::correctionOrdre : &Memoire::correctionDesordre;
}

Memoire::~Memoire()
{
	delete ui;
}

void Memoire::on_btnRestitution_clicked()
{
	ui->lstToLearn->setVisible(false);
	delete _chrono;
	_chrono = nullptr;
	for (auto i = 0; i < ui->bxDifficulte->value(); i++)
	{
		auto const item = ui->lstLearned->item(i);
		if (item)
			item->setFlags(item->flags() | Qt::ItemIsEditable);
	}
}

void Memoire::on_btnNouveau_clicked()
{
	_elapsedSeconds = 0;
	ui->lstToLearn->clear();
	ui->lstLearned->clear();
	ui->lstAide->clear();
	ui->lstToLearn->setVisible(true);
	ui->lblResultats->clear();
	ui->btnNext->setEnabled(false);
	auto copydictionnaire = _dictionnaire;
	for (auto i = 0; i < ui->bxDifficulte->value(); i++)
	{
		const auto j = qrand()%copydictionnaire.size();
		const QStringList toLearn = copydictionnaire.at(j).split(':');
		if (toLearn.size() == 2)
			ui->lstAide->addItem(toLearn.last());
		ui->lstToLearn->addItem(toLearn.first());
		ui->lstLearned->addItem(QString("Élément %1").arg(i));
		copydictionnaire.removeAt(j);
	}
	if (!_chrono)
		_chrono = new QTimer(this);
	connect(_chrono, SIGNAL(timeout()), this, SLOT(update()));
	_chrono->start(1000);
}

void Memoire::update()
{
	_elapsedSeconds++;
	ui->lblChrono->setText(QString("%1").arg(_elapsedSeconds));
}

void Memoire::correctionOrdre()
{
	ui->lstToLearn->setVisible(true);
	int nbBons{0};
	for (auto i = 0; i < ui->bxDifficulte->value(); i++)
	{
		if (ui->lstLearned->item(i)->text() != ui->lstToLearn->item(i)->text())
			ui->lstToLearn->item(i)->setTextColor(Qt::red);
		else
		{
			ui->lstToLearn->item(i)->setTextColor(Qt::darkGreen);
			nbBons++;
		}
	}
	Resultat r{ui->bxDifficulte->value(), _elapsedSeconds, nbBons};
	_resultats.push_back(r);
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Sauvage Software", "ExoMemoire");
	QList <QVariant> buffer = settings.value("performances").toList();
	QHash <int, double> perfs;
	foreach (QVariant x, buffer) {
		QPointF value = x.toPointF();
		perfs.insert((int)value.x(), value.y());
	}
	const auto completion = (float)nbBons /(float)r.niveau;
	const auto x = (float)r.temps/(float)r.reussites;
	if ((completion == 1.) && (!perfs.keys().contains(r.niveau) || (x < perfs[r.niveau])))
	{
		perfs[r.niveau] = x;
	}
	buffer.clear();
	foreach (int i, perfs.keys())
	{
		QPointF value(i, perfs[i]);
		buffer.push_back(value);
	}
	settings.setValue("performances", buffer);
	ui->lblResultats->setText(QString("Vous avez retenu %1 % des mots.\nUn mot vous prend en moyenne %2 secondes à apprendre")
							  .arg(completion * 100).arg(x));
	ui->btnNext->setEnabled(true);
}

void Memoire::correctionDesordre()
{
	ui->lstToLearn->setVisible(true);
	int nbBons{0};
	for (auto i = 0; i < ui->bxDifficulte->value(); i++)
	{
		bool fTrouve = false;
		for (auto j = 0; j < ui->bxDifficulte->value(); j++)
		{
			if (ui->lstLearned->item(j)->text() == ui->lstToLearn->item(i)->text())
			{
				ui->lstToLearn->item(i)->setTextColor(Qt::darkGreen);
				nbBons++;
				fTrouve = true;
				break;
			}
		}
		if (!fTrouve)
			ui->lstToLearn->item(i)->setTextColor(Qt::red);
	}
	Resultat r{ui->bxDifficulte->value(), _elapsedSeconds, nbBons};
	_resultats.push_back(r);
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Sauvage Software", "ExoMemoire");
	QList <QVariant> buffer = settings.value("performances").toList();
	QHash <int, double> perfs;
	foreach (QVariant x, buffer) {
		QPointF value = x.toPointF();
		perfs.insert((int)value.x(), value.y());
	}
	const auto completion = (float)nbBons /(float)r.niveau;
	const auto x = (float)r.temps/(float)r.reussites;
	if ((completion == 1.) && (!perfs.keys().contains(r.niveau) || (x < perfs[r.niveau])))
	{
		perfs[r.niveau] = x;
	}
	buffer.clear();
	foreach (int i, perfs.keys())
	{
		QPointF value(i, perfs[i]);
		buffer.push_back(value);
	}
	settings.setValue("performances", buffer);
	ui->lblResultats->setText(QString("Vous avez retenu %1 % des mots.\nUn mot vous prend en moyenne %2 secondes à apprendre")
							  .arg(completion * 100).arg(x));
	ui->btnNext->setEnabled(true);
}

void Memoire::on_btnNext_clicked()
{
	if (!_resultats.isEmpty())
	{
		auto r = _resultats.last();
		if (r.reussites >= r.niveau)
			ui->bxDifficulte->setValue(r.niveau + 1);
	}
	on_btnNouveau_clicked();
}

void Memoire::on_actionImporter_des_items_triggered()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Sauvage Software", "ExoMemoire");
	QString defaultDir = settings.value("Default Directory", "./").toString();
	QString file = QFileDialog::getOpenFileName(this, "Choisissez un dictionnaire", defaultDir);
	if (file.isEmpty())
		return;
	QFile fDict(file);
	if (!fDict.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this, "Échec à l'import", QString("Le fichier %1 n'a pas pu être ouvert.").arg(file));
		return;
	}
	QDir dir(fDict.fileName());
	settings.setValue("Default Directory", dir.absolutePath());
	chargeItems(fDict);
	fDict.close();
}

void Memoire::chargeItems(QFile &fDict)
{
	ui->lstAide->setVisible(false);
	_dictionnaire.clear();
	while (!fDict.atEnd())
	{
		QString s = fDict.readLine();
		s.remove('\n');
		if (s.contains(':'))
			ui->lstAide->setVisible(true);
		_dictionnaire.push_back(s);
	}
}

void Memoire::on_btnPerfs_clicked()
{
	DlgPerfs dlg;
	dlg.exec();
}

void Memoire::on_btnQuit_clicked()
{
	QApplication::quit();
}

void Memoire::on_bxDifficulte_valueChanged(int arg1)
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Sauvage Software", "ExoMemoire");
	settings.setValue("Last level", arg1);
}

void Memoire::on_actionPr_f_rences_triggered()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Sauvage Software", "ExoMemoire");
	PreferencesDialog dlg;
	dlg.setOrdre(settings.value("Ordre", true).toBool());
	if (dlg.exec())
	{
		settings.setValue("Ordre", dlg.ordre());
		if (_fctCorrection)
			disconnect(ui->btnCorrection, SIGNAL(clicked()), this, SLOT(_fctCorrection));
		_fctCorrection = dlg.ordre() ? &Memoire::correctionOrdre : &Memoire::correctionDesordre;
		connect(ui->btnCorrection, SIGNAL(clicked(bool)), this, SLOT(_fctCorrection));
	}
}

void Memoire::on_btnCorrection_clicked()
{
	(this->*_fctCorrection)();
}
