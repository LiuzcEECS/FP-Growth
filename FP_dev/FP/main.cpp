#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir dir;
    QString currentPath=dir.currentPath();
    QFile qss("./qss/main.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();
    MainWindow w;

    w.setWindowTitle("FP Growth");
    w.show();
    return a.exec();
}
