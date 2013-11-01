#ifndef DEBUGHELPER_H
#define DEBUGHELPER_H

#include <QString>
#include <QDebug>

#ifdef CALL_TRACK_ENABLED

class FunctionCallTracker
{
public:
    FunctionCallTracker(const char*method,char *file, int line)
        :mMethod(method),
         file(file),
         line(line)
    {
        qDebug() << "> " << mMethod;
    }
    ~FunctionCallTracker()
    {
        qDebug() << "< " << mMethod;
    }
private:
    const char* mMethod;
    char* file;

    int line;
};

#define CALL_TRACK FunctionCallTracker _CALL_TRACK_functionCallTracker_(__FUNCTION__,__FILE__,__LINE__);
#else
#define CALL_TRACK
#endif

#endif // DEBUGHELPER_H

