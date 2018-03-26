#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(QWidget *parent = 0);
	~PreferencesDialog();
	void setOrdre(bool ordre);
	bool ordre();
private:
	Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
