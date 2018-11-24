#include "QPoisson.h"
#include <QtWidgets/QApplication>
#include "CVimg.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QPoisson w;
	w.show();
	return a.exec();
}
