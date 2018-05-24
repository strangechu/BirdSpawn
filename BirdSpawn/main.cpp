#include "GLGuiApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QString sDir = QCoreApplication::applicationDirPath();
	a.addLibraryPath(sDir + "/plugins");
	GLGuiApplication w;
	w.show();
	return a.exec();
}
