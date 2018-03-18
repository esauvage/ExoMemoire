#include "memoire.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Memoire w;
	w.show();
	
	return a.exec();
}
