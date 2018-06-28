#include "GLGuiApplication.h"
#include <QtWidgets/QApplication>
#include "VideoPlayer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QString sDir = QCoreApplication::applicationDirPath();
	a.addLibraryPath(sDir + "/plugins");
	GLGuiApplication w;
	w.show();
	VideoPlayer v;
	v.show();
	return a.exec();
}
