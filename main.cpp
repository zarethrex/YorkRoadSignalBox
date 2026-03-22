#include "yrmainwindow.hxx"

#include <QApplication>
#include "scaling.hxx"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Can only initialise window scaling once
    // Qt Application created
    YRB::SCALER.init();

    YRMainWindow w;
    w.show();
    return a.exec();
}
