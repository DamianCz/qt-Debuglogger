#ifndef DEBUGFILELOGGERRUNABLE_H
#define DEBUGFILELOGGERRUNABLE_H
#include <QString>
#include <QRunnable>

class DebugFileLoggerRunnable : public QRunnable {

public:
    DebugFileLoggerRunnable(const QString &message, QtMsgType level)
        : mMessage(message)
        , mLevel(level) {}

    virtual void run()
    {
        DebugFileLogger::getInstance().logMessageToFile(mMessage, mLevel);
    }

private:
    QString mMessage;
    QtMsgType mLevel;

};
#endif // DEBUGFILELOGGERRUNABLE_H
