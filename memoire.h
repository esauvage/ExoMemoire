#ifndef MEMOIRE_H
#define MEMOIRE_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QTimer>
#include <QFile>

namespace Ui {
class Memoire;
}

struct Resultat {
	int niveau;
	int temps;
	int reussites;
};

class Memoire : public QMainWindow
{
	Q_OBJECT

public:
	explicit Memoire(QWidget *parent = 0);
	~Memoire();

private slots:
	void on_btnNouveau_clicked();
	void on_btnRestitution_clicked();
	void update();
	void correctionOrdre();
	void correctionDesordre();
	void on_btnNext_clicked();
	void on_actionImporter_des_items_triggered();
	void on_btnPerfs_clicked();
	void on_btnQuit_clicked();
	void on_bxDifficulte_valueChanged(int arg1);
	void on_actionPr_f_rences_triggered();

	void on_btnCorrection_clicked();

private:
	void chargeItems(QFile &fDict);

	Ui::Memoire *ui;
	QTimer *_chrono;
	QList < QString> _dictionnaire;
	QList <Resultat> _resultats;
	void (Memoire::*_fctCorrection)();
	int _elapsedSeconds;
};

#endif // MEMOIRE_H
