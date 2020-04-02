#include "test.h"
#include <QtWidgets/QApplication>
#include "Task\TestTask.h"
#include "TaskScheduler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*Test w;
    w.show();*/
    TestTask tt;
    tt.setParam(qApp->applicationDirPath());
    TaskScheduler taskScheduler;
    taskScheduler.schedule(&tt);
    taskScheduler.waitForFinished();
    return a.exec();
}
