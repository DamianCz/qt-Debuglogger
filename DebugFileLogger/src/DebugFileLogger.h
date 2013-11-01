#pragma once

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QObject>
#include <QThreadPool>
#include <QRunnable>

///
///   @brief
///   A logger for our  Qt applications. Singleton class.
///   @note
///   Before logging initialization make sure that QCoreApplication instance has been created and
///   OrganizationName and ApplicationName has been set.
///

class DebugFileLogger : public QObject
{
    Q_OBJECT
public:
    static DebugFileLogger& getInstance();

    void logMessage(QtMsgType type, const char *msg);
    void logMessageToFile(QString mMsg,QtMsgType mType);

public:
    static void InitializeLogger(QString);

    static QString getFileName();
    static QString getPathToLogFolder();

private:
    DebugFileLogger(QObject *parent = 0);
    DebugFileLogger(const DebugFileLogger &);
    DebugFileLogger& operator=(const DebugFileLogger&);

    bool openFile(QFile*);

private:
    ///@note Store username getting from  getenv()
    QString m_userName;

    ///@note Store earlier message, this way we can avoid save duplicate communicate to file.
    QString m_earlierMsg;

    ///@note Using to store name for file where we save all debug info.
    static QString m_FileName;

    ///@note Store general path to debug folder, without debug file name.
    ///Use in debug cleaner program to get debug folder path
    static QString m_pathToLogFolder;

    QThreadPool m_logThreadPool;
    QMutex m_Mutex;
};
