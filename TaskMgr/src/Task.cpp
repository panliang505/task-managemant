#include "Task.h"

Task::Task(QObject* parent)
	: QObject(parent)
{
}

Task::~Task()
{
}

void Task::cancel()
{
	emit sigCancelled();
}

void Task::setName(const QString& _name)
{
	this->m_name = _name;
}

const QString& Task::name() const
{
	return this->m_name;
}

bool Task::isFinished() const
{
	return false;
}

bool Task::isRunning() const
{
	return false;
}

void Task::run()
{
	// 空实现
}