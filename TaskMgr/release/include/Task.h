/**********************************
*  @description :
*  @author ：panl - c
*  @date : 2020.02.05
***********************************/

#ifndef _TASK_H_
#define _TASK_H_

#include <QObject>
#include "taskmgr_global.h"

class TASKMGR_EXPORT Task : public QObject
{
	Q_OBJECT
public:
	Task(QObject* parent = nullptr);
	~Task();

	void cancel();

	void setName(const QString& _name);
	const QString& name() const;

	bool isFinished() const;
	bool isRunning() const;

	public slots:
	virtual void run();

signals:
	void sigCancelled();
	void sigFinished();

protected:
	QString m_name;
};
#endif // !_TASK_H_
