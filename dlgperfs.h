#ifndef DLGPERFS_H
#define DLGPERFS_H

#include <QDialog>

namespace Ui {
class DlgPerfs;
}

class DlgPerfs : public QDialog
{
	Q_OBJECT

public:
	explicit DlgPerfs(QWidget *parent = 0);
	~DlgPerfs();

private:
	Ui::DlgPerfs *ui;
};

#endif // DLGPERFS_H
