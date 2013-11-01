#include <QCoreApplication>

#include <QDebug>

#include "DebugFileLogger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DebugFileLogger::InitializeLogger("Test1ApplicationnewLongNameApplicationModule");

    qDebug()<<"First message.";
    qDebug()<<"First message.";
    qDebug()<<"First message.";
    qDebug()<<"Second message.";

    return a.exec();
}
