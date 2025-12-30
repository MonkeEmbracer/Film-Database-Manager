#include "Tests/Tests.h"
#include "GUI/GUI.h"
#include <QApplication>

int main(int argc, char* argv[]) {
	tests();
	QApplication app(argc, argv);
	GUI ui;
	return QApplication::exec();
}
