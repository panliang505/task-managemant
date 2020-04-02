/**********************************
*  @description :
*  @author £ºpanl - c
*  @date : 2020.02.06
***********************************/

#ifndef _HTTPTASK_H_
#define _HTTPTASK_H_

#include "Task.h"
#include <qnetworkreply.h>
#include "taskmgr_global.h"

class TASKMGR_EXPORT HttpTask : public Task
{
    Q_OBJECT
public:
    HttpTask(QObject* parent = nullptr);
    ~HttpTask();

    const QString& url() const;
    const QString& newUrl() const;
    QNetworkReply::NetworkError networkError() const;
    int httpCode() const;
    const QByteArray& content() const;

signals:
    void sigError(QNetworkReply::NetworkError);

protected:

    int m_httpCode;
    QString m_url;
    QString m_newUrl;
    QString m_errorMsg;
    QByteArray m_content;
    QNetworkReply::NetworkError m_error;

};

#endif // !_HTTPTASK_H_
