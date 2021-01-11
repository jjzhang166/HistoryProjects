#pragma once
#pragma execution_character_set("utf-8")
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QTimer>
#include <QUrl>
#include <QVector>

enum DownloadResult {
	DR_SUCCESS,
	DR_FAILURE,
	DR_ERROR,
	DR_TIMEOUT,
};

/*��������*/
class Downloader : public QObject
{
	Q_OBJECT
public:
	/*����*/
	Downloader();

	/*����*/
	~Downloader();

	/*��ȡ���մ���*/
	const QString& getLastError();

	/*��ȡ�ļ���С*/
	const qint64 getFileSize(const QUrl& url, const int& times = 0);

	/*����*/
	void download(const QUrl& url);

	/*�������س�ʱ*/
	void setTimeout(const int& timeout);

	/*���ñ���·��*/
	void setSavePath(const QString& savePath);

	/*��ȡӦ��*/
	QNetworkReply* getReply();

	/*����ʱ��*/
	const ulong elapsedTime();

	/*ƽ������*/
	const float averageSpeed();
signals:
	/*����ź�*/
	void resultSignal(const DownloadResult& result, const QString& error);

	/*�����ź�*/
	void progressSignal(const int& value, const float& speed);
public slots:
	/*��ɲ�*/
	void finishedSlot(QNetworkReply* reply);

	/*SSL�����*/
	void sslErrorsSlot(const QList<QSslError>& errors);

	/*���Ȳ�*/
	void progressSlot(qint64 recvBytes, qint64 totalBytes);

	/*��ʱ��*/
	void timeoutSlot();
protected:
	/*�������մ���*/
	void setLastError(const QString& error);

	/*�����ļ���*/
	const QString saveFileName(const QUrl& url);

	/*���浽����*/
	bool saveToDisk(const QString& filename, QIODevice* data);
private:
	QString m_lastError = "No Error";

	QNetworkAccessManager m_manager;

	QNetworkReply* m_reply = nullptr;

	QTimer m_timer;

	int m_timeout = 120000;

	ulong m_startTime = 0;

	ulong m_endTime = 0;

	quint64 m_recvBytes = 0;

	QVector<float> m_speedV;

	QString m_savePath = "Downloader";
};

/*Ӧ��ʱ��*/
class QReplyTimeout : public QObject
{
	Q_OBJECT
public:
	QReplyTimeout(QNetworkReply* reply, const int& timeout) : QObject(reply)
	{
		if (reply && reply->isRunning())
		{
			QTimer::singleShot(timeout, this, &QReplyTimeout::onTimeout);
		}
	}
private slots:
	void onTimeout()
	{
		QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
		if (reply->isRunning() && reply->size() == 0)
		{
			reply->abort();
			reply->deleteLater();
			emit timeout();
		}
	}
signals:
	void timeout();
};