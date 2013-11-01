#include "DebugFileLogger.h"
#include "debugHelper.h"
#include "DebugFileLoggerRunable.h"

#include <QtCore>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <cstdlib>
#include <QMutex>

#ifdef Q_WS_WIN
#include "Windows.h"
#include <Lmcons.h>
#include <shlwapi.h>
#include "shlobj.h"
#endif

/// @note
/// We want to clear visual effect in notepad also ,
/// so we need to compare break beetwen column,
/// 19-This is the lentgh of the lentghest component name -Client Control Center.
static const int theLongestTiitleSize = 19;

QString DebugFileLogger::m_FileName;
QString DebugFileLogger::m_pathToLogFolder;

/*! General debug log handling. */
void LogMessage(QtMsgType type, const char* msg) {
    DebugFileLogger::getInstance().logMessage(type,msg);
}

void DebugFileLogger::InitializeLogger(QString fileName) {
    static bool isInitialized = false;

    m_FileName = fileName;

    if (isInitialized) {
        return;
    }
    isInitialized = true;
    qInstallMsgHandler(LogMessage);
}

void DebugFileLogger::logMessage(QtMsgType type, const char* msg) {
    m_Mutex.lock();
    DebugFileLoggerRunnable *r = new DebugFileLoggerRunnable(QString(msg), type);
    m_logThreadPool.start(r);
    m_Mutex.unlock();
}

///
/// \brief DebugFileLogger::instance
/// \return main debugFileLogger instance.
///
DebugFileLogger& DebugFileLogger::getInstance()
{
    static DebugFileLogger instance;
    return instance;
}

DebugFileLogger::DebugFileLogger(QObject *parent)
    :QObject(parent)
{

    m_logThreadPool.setMaxThreadCount(1);
    m_logThreadPool.setExpiryTimeout(-1);

    /// @note Part for getting user name from windows enviroment.
#ifdef Q_WS_WIN
    DWORD size = UNLEN + 1;
    wchar_t username[UNLEN+1];
    GetUserName(username, &size);
    m_userName = QString::fromWCharArray(username);
#endif /// endif windows operation system functions.

#ifdef Q_WS_MAC /// Getting username for mac.
    m_userName = getenv("USERNAME");
#endif

    if(m_userName.isEmpty())
        m_userName = "Anonymous";
}

void DebugFileLogger::logMessageToFile(QString mMsg,QtMsgType mType) {

    QFile file;
    QTextStream out;

    file.setFileName(DebugFileLogger::getFileName());

    /// @note If not possible to open file, escape function.
    if( openFile(&file) == false )
        return;

    out.setDevice(&file);  /// Open Stream

    QString msgTemp = mMsg.remove("\"");

    switch (mType) {

    case QtDebugMsg: {
        if( msgTemp !=  m_earlierMsg)
            out <<m_FileName<<"\t"<<
                  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:z")<<"\tInfo\t"<<
                  m_userName+"\t"<<msgTemp<<"\t"<<"\n";
        break;
    }
    case QtWarningMsg:
        if( msgTemp !=  m_earlierMsg)
            out <<m_FileName<<"\t" <<
                  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:z") <<
                  "\tWarning\t" <<m_userName+"\t"<< msgTemp << "\t\n";
        break;
    case QtCriticalMsg:
        if( msgTemp !=  m_earlierMsg)
            out <<m_FileName<<"\t" <<
                  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:z") <<
                  "\tError\t" <<m_userName+"\t"<< msgTemp << "\t\n";
        break;
    case QtFatalMsg:
        if( msgTemp !=  m_earlierMsg)
            out <<m_FileName<<"\t" <<
                  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:z") <<
                  "\tFatal\t" <<m_userName+"\t"<< msgTemp << "\t\n";
        abort();
    default :
        if( msgTemp !=  m_earlierMsg)
            out <<m_FileName<<"\t" <<
                  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss:z") <<
                  "\tNo qt error\t" <<m_userName+"\t"<< msgTemp << "\t\n";

    }
    m_earlierMsg = msgTemp;
    out.flush();
}

/*! using for getting file name and path ,when open file handle */
QString DebugFileLogger::getFileName() {

#ifdef Q_WS_WIN /// @todo MAC This is for windows version, when port to mac, need to found mac file path.
    TCHAR szPath[MAX_PATH];
    SHGetFolderPath( NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath );
    QString programData = QString::fromWCharArray(szPath);
#endif /// End Windows specific part.

#ifdef Q_WS_MAC
    QString programData;
#endif

    QDir dir(programData);
    if ( dir.exists() == false)
        QDir().mkdir(programData);

    programData.append("/DebugLogs");

    QDir dir1(programData);
    if ( dir1.exists() == false)
        QDir().mkdir(programData);

    m_pathToLogFolder = programData;

    QString name = programData.append("/DebugInfo.");
    name.append(QDate::currentDate().toString("yyyy.MM.dd"));
    name.append(".log");
    return name;
}

/// @brief General open handler application.
/// @note If file with orginal name is open ,
/// try open temp.log file.
/// @elseif
/// return True if open file ( temp or normal )
/// return false if not possible to open file.
bool DebugFileLogger::openFile( QFile *file) {

    if (!file->open(QIODevice::WriteOnly  | QIODevice::Append | QIODevice::Text)) {
        file->close();

        /// @note Using this only when any other application
        /// read debug log file and file is in exclusive mode.
        /// http://stackoverflow.com/questions/8301125/cannot-write-to-file-while-it-is-opened-in-ms-excel
        /// http://stackoverflow.com/questions/947172/write-to-file-that-is-open
        QString temp = DebugFileLogger::getFileName().replace(".log","temp.log");

        file->setFileName(temp);

        if(  file->open(QIODevice::ReadWrite  | QIODevice::Append | QIODevice::Text) == false )
            return false;
    }
    return true;
}

QString DebugFileLogger::getPathToLogFolder() {
    /// @note Get file name and Set it in pathToLogFolder variable.
    if ( m_pathToLogFolder == "")
        getFileName();
    return m_pathToLogFolder;
}

