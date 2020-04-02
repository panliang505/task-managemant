/**********************************
*  @description :
*  @author £ºpanl - c
*  @date : 2020.02.05
***********************************/

#ifndef _TASKSCHEDULER_H_
#define _TASKSCHEDULER_H_
#include <QObject>
#include <qmutex.h>
#include <qeventloop.h>
#include "taskmgr_global.h"

class Task;
class TaskDispatcherPrivate;
class TaskProcessorPrivate;

class TASKMGR_EXPORT TaskScheduler : public QObject
{
    Q_OBJECT

public:
    TaskScheduler(int thread_count = -1, QObject* parent = 0);
    ~TaskScheduler();
    void schedule(Task* task);

    void cancel();

    bool isRunning() const;

    void waitForFinished();

    void cleanUp();

signals:
    void cancelTasks();
    void dispatchTasks();

private:
    mutable QMutex m_locker;

    int m_threadCount;

    QList<Task*> m_tasks;

    QThread* m_dispatchThread;
    QList<QThread*> m_processThreads;

    TaskDispatcherPrivate* m_dispatcher;
    QList<TaskProcessorPrivate*> m_processors;

    bool m_isRunning;

    QEventLoop m_eventLoop;

    volatile short m_taskCount;

    friend class TaskDispatcherPrivate;
    friend class TaskProcessorPrivate;
};


class TaskDispatcherPrivate : public QObject
{
    Q_OBJECT
public:
    TaskDispatcherPrivate(TaskScheduler* scheduler);
    ~TaskDispatcherPrivate();

signals:
    void dispatch(TaskProcessorPrivate* processor, Task* task);
    void idle();

public slots :
    void onDispatchTasks();
    void onTaskFinished();

private:
    TaskScheduler* m_scheduler;

    friend class TaskScheduler;
};

class TaskProcessorPrivate : public QObject
{
    Q_OBJECT

public:
    TaskProcessorPrivate(QObject* parent = 0);
    ~TaskProcessorPrivate();

    void process(TaskProcessorPrivate* processor, Task* task);

signals:
    void sigCancelled();
    void sigFinished();

private:
    friend class TaskScheduler;
};


#endif // !_TASKSCHEDULER_H_

