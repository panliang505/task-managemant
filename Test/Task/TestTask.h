/**********************************
*  @description : test task
*  @author £ºpanl - c
*  @date : 2020.03.23
***********************************/

#ifndef _TESTTASK_H_
#define _TESTTASK_H_

#include "Task.h"

class TestTask : public Task
{
Q_OBJECT

public:
    TestTask(QObject* parent = nullptr);
    ~TestTask();

    void setParam(const QString& logPath);
    virtual void run();

signals:
    void sigTestError(const QString& errMsg);

private:
    QString m_logPath;
};

#endif // !_TESTTASK_H_

