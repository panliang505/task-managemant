/**********************************
*  @description :
*  @author £ºpanl - c
*  @date : 2020.02.05
***********************************/

#include <QThread>
#include <windows.h>
#include <QApplication>
#include "Task.h"
#include "TaskScheduler.h"

#pragma region TaskScheduler

TaskScheduler::TaskScheduler(int thread_count /*= -1*/, QObject* parent /*= 0*/)
    : QObject(parent)
    , m_threadCount(0)
    , m_isRunning(false)
    , m_taskCount(0)
{
    qRegisterMetaType<Task*>("Task*");
    qRegisterMetaType<TaskDispatcherPrivate*>("TaskDispatcherPrivate*");
    qRegisterMetaType<TaskProcessorPrivate*>("TaskProcessorPrivate*");

    if (thread_count == -1)
    {
        m_threadCount = QThread::idealThreadCount();
    }
    else
    {
        m_threadCount = thread_count;
    }

    m_dispatcher = new TaskDispatcherPrivate(this);
    m_dispatchThread = new QThread(this);
    m_dispatcher->moveToThread(m_dispatchThread);
    connect(m_dispatchThread, &QThread::finished, m_dispatcher, &QObject::deleteLater);
    connect(this, &TaskScheduler::dispatchTasks, m_dispatcher, &TaskDispatcherPrivate::onDispatchTasks);
    connect(m_dispatcher, &TaskDispatcherPrivate::idle, &m_eventLoop, &QEventLoop::quit);

    for (size_t i = 0; i < m_threadCount; i++)
    {
        TaskProcessorPrivate* processor = new TaskProcessorPrivate();
        QThread* thread = new QThread(this);
        processor->moveToThread(thread);
        connect(thread, &QThread::finished, processor, &QObject::deleteLater);
        connect(m_dispatcher, &TaskDispatcherPrivate::dispatch, processor, &TaskProcessorPrivate::process);
        connect(processor, &TaskProcessorPrivate::sigFinished, m_dispatcher, &TaskDispatcherPrivate::onTaskFinished);
        connect(this, &TaskScheduler::cancelTasks, processor, &TaskProcessorPrivate::sigCancelled);

        m_processors.append(processor);
        m_processThreads.append(thread);

        thread->start();
    }

    m_dispatchThread->start();
}

TaskScheduler::~TaskScheduler()
{
    cleanUp();
}

void TaskScheduler::schedule(Task* task)
{
    m_locker.lock();
    m_tasks.append(task);
    m_isRunning = true;
    m_locker.unlock();
    InterlockedIncrement16(&m_taskCount);
    emit dispatchTasks();
}

void TaskScheduler::cancel()
{
    m_locker.lock();
    int size = m_tasks.size();
    m_tasks.clear();
    for (int i = 0; i < size; ++i)
    {
        InterlockedDecrement16(&m_taskCount);
    }
    m_locker.unlock();

    emit cancelTasks();
}

bool TaskScheduler::isRunning() const
{
    return m_taskCount > 0;
}

void TaskScheduler::waitForFinished()
{
    while (m_taskCount > 0)
    {
        Sleep(80);
        QApplication::processEvents();
    }
}

void TaskScheduler::cleanUp()
{
    if (m_dispatchThread)
    {
        m_dispatchThread->quit();
        m_dispatchThread->wait();
        delete m_dispatchThread;
        m_dispatchThread = NULL;
    }

    if (!m_processThreads.isEmpty())
    {
        for (int i = 0; i < m_threadCount; i++)
        {
            m_processThreads[i]->quit();
            m_processThreads[i]->wait();
        }

        m_processThreads.clear();
    }
}

#pragma endregion


#pragma region TaskDispatcherPrivate

TaskDispatcherPrivate::TaskDispatcherPrivate(TaskScheduler* scheduler)
    : QObject(nullptr)
    , m_scheduler(scheduler)
{

}

TaskDispatcherPrivate::~TaskDispatcherPrivate()
{

}

void TaskDispatcherPrivate::onDispatchTasks()
{
    m_scheduler->m_locker.lock();
    if (!m_scheduler->m_tasks.empty())
    {
        while (!m_scheduler->m_tasks.empty())
        {
            if (!m_scheduler->m_processors.empty())
            {
                TaskProcessorPrivate* processor = m_scheduler->m_processors.front();
                m_scheduler->m_processors.pop_front();
                Task* task = m_scheduler->m_tasks.front();
                m_scheduler->m_tasks.pop_front();
                emit dispatch(processor, task);
            }
            else
            {
                break;
            }
        }

        m_scheduler->m_locker.unlock();
    }
    else
    {
        if (m_scheduler->m_processors.count() == m_scheduler->m_threadCount)
        {
            m_scheduler->m_isRunning = false;
            m_scheduler->m_locker.unlock();
            emit idle();
        }
        else
        {
            m_scheduler->m_locker.unlock();
        }
    }
}

void TaskDispatcherPrivate::onTaskFinished()
{
    QObject* signal_sender = sender();
    if (signal_sender)
    {
        TaskProcessorPrivate* processor = dynamic_cast<TaskProcessorPrivate*>(signal_sender);
        m_scheduler->m_processors.append(processor);
    }

    InterlockedDecrement16(&m_scheduler->m_taskCount);

    onDispatchTasks();
}

#pragma endregion


#pragma region TaskProcessorPrivate

TaskProcessorPrivate::TaskProcessorPrivate(QObject* parent /*= 0*/)
    : QObject(parent)
{

}

TaskProcessorPrivate::~TaskProcessorPrivate()
{

}

void TaskProcessorPrivate::process(TaskProcessorPrivate* processor, Task* task)
{
    if (this == processor)
    {
        try
        {
            connect(this, &TaskProcessorPrivate::sigCancelled, task, &Task::sigCancelled, Qt::DirectConnection);
            task->run();
            disconnect(this, &TaskProcessorPrivate::sigCancelled, task, &Task::sigCancelled);
            emit sigFinished();
        }
        catch (std::exception& e)
        {
            emit sigFinished();
            throw e;
        }
        catch (...)
        {
            emit sigFinished();
            throw (QStringLiteral("Î´Öª´íÎó"));
        }
    }
}

#pragma endregion
