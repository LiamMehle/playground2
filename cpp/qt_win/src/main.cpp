#include <stdio.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

int main( int argc, char** argv ) {
	puts("init");
	QApplication app( argc, argv );
	QPushButton button( "push me" );
	puts("shutdown");
	return 0;
}
