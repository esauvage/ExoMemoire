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
	void on_btnCorrection_clicked();
	void on_btnNext_clicked();
	void on_actionImporter_des_items_triggered();

private:
	void chargeItems(QFile &fDict);

	Ui::Memoire *ui;
	QTimer *_chrono;
	QList < QString> _dictionnaire;
	QList <Resultat> _resultats;
	int _elapsedSeconds;
};

#endif // MEMOIRE_H
