//VS2015 + QT5.8 + matlab2017的一个小麦麦穗分类检测软件
#include "imgProc_matlab17.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	imgProc_matlab17 w;
	w.show();
	return a.exec();
}
