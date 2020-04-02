/**********************************
*  @description :
*  @author £ºpanl - c
*  @date : 2020.02.06
***********************************/

#include "HttpTask.h"

HttpTask::HttpTask(QObject* parent /*= nullptr*/)
    : Task(parent)
    , m_error(QNetworkReply::NoError)
    , m_httpCode(0)
{

}

HttpTask::~HttpTask()
{

}

const QString&  HttpTask::url() const
{
    return m_url;
}

const QString& HttpTask::newUrl() const
{
    return m_newUrl;
}

QNetworkReply::NetworkError HttpTask::networkError() const
{
    return m_error;
}

int HttpTask::httpCode() const
{
    return m_httpCode;
}

const QByteArray& HttpTask::content() const
{
    return m_content;
}
