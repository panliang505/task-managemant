/**********************************
*  @description : test task
*  @author £ºpanl - c
*  @date : 2020.03.23
***********************************/

#include "TestTask.h"
#include <QThread>
#include <QFile>

TestTask::TestTask(QObject* parent /*= nullptr*/)
    : Task(parent)
{

}

TestTask::~TestTask()
{

}

void TestTask::setParam(const QString& logPath)
{
    m_logPath = logPath;
}

void TestTask::run()
{
    QString logPath = m_logPath + "\\log.txt";
    QFile file(logPath);
    file.open(QIODevice::Append | QIODevice::Unbuffered);
    for (long i = 0; i < 100000; ++i)
    {
        QThread::msleep(10);
        file.write(QString("%1  ").arg(i).toStdString().c_str());
        file.flush();
    }
    file.close();
}

