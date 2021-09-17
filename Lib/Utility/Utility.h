#pragma once

#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QProcess>

#include <functional>

#include "File.h"
#include "AuthDlg.h"
#include "UnlockDlg.h"
#include "UpdateDlg.h"
#include "JQCPUMonitor.h"

#define GET_JSON() Json::getInstance()

#define TO_STRING(X) #X

#define BUFF_SIZE 0x1000

#define NO_THROW_NEW new(std::nothrow)

#define S_TO_Q_STR(X) QString::fromStdString(X)

#define WS_TO_Q_STR(X) QString::fromStdWString(X)

#define WC_TO_Q_STR QString::fromWCharArray

#define Q_TO_C_STR(X) X.toStdString().c_str()

#define Q_TO_C_LEN(X) X.toStdString().length()

#define Q_TO_WC_STR(X) X.toStdWString().c_str()

#define Q_TO_WC_LEN(X) X.toStdWString().length()

#define G_TO_Q_STR(X) QString::fromLocal8Bit(X)

#define G_TO_C_STR(X) Q_TO_C_STR(G_TO_Q_STR(X))

#define N_TO_Q_STR QString::number

#define SU_FA(X) X ? "�ɹ�":"ʧ��"

#define OK_NG(X) X ? "OK" : "NG"

#define Q_SPRINTF(format,...) QString().sprintf(format,##__VA_ARGS__)

#define DEC_TO_HEX(num) Q_SPRINTF("%d",num).toInt(nullptr,16)

#define DEBUG_INFO()\
if (Utility::Var::debugInfo)\
	qDebug().noquote() << QString("%1 %2 %3 %4").arg(QString::number(Utility::Var::logCount), 4, '0').arg(Utility::getCurrentTime(), \
	__FUNCTION__, QString::number(__LINE__))

#define DEBUG_INFO_EX(format,...) \
if (Utility::Var::debugInfo)\
	qDebug().noquote() << QString("%1 %2 %3 %4 %5").arg(QString::number(Utility::Var::logCount), 4, '0').arg(Utility::getCurrentTime(), \
	__FUNCTION__, QString::number(__LINE__), Q_SPRINTF(format, ##__VA_ARGS__))

#define RUN_BREAK(success,error) \
if ((success))\
{\
	setLastError(error);\
	break;\
}

//��ȫɾ������
#define SAFE_DELETE_A(X)\
if (X)\
{\
	delete[] X;\
	X = nullptr;\
}

//��ȫɾ������
#define SAFE_DELETE(X)\
if (X)\
{\
	delete X;\
	X = nullptr;\
}


/*
* @Utility,ʵ�ù���
*/
namespace Utility {

	/*
	* @Var,��̬���������ռ�
	*/
	namespace Var {
		static bool debugInfo = false;

		static unsigned int logCount = 0;

		static QString lastError = "δ֪����";

		static QString appendName;

		static bool appendPos = false;
	}

	/*
	* @setDebugInfo,���õ�����Ϣ
	* @param1,�Ƿ�����
	* @return,void
	*/
	void setDebugInfo(bool enable);

	/*
	* @setLastError,���ô�����Ϣ
	* @param1,������Ϣ
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @getLastError,��ȡ������Ϣ
	* @return,const QString&
	*/
	const QString& getLastError();

	/*
	* @getFileNameByUrl,ͨ��URL��ȡ�ļ���
	* @param1,URL
	* @return,QString
	*/
	QString getFileNameByUrl(const QString& url);

	/*
	* @getFileNameByPath,ͨ��·����ȡ�ļ���
	* @param1,·��
	* @return,QString
	*/
	QString getFileNameByPath(const QString& path);

	/*
	* @getCurrentFileName,��ȡ��ǰ�ļ���
	* @return,const QString
	*/
	QString getCurrentFileName();

	/*
	* @getCurrentDir,��ȡ��ǰĿ¼
	* @return,const QString
	*/
	QString getCurrentDir();

	/*
	* @makePath,����·��
	* @param1,·����
	* @return,bool
	*/
	bool makePath(const QString& path);

	/*
	* @existPath,����·��
	* @param1,·����
	* @return,bool
	*/
	bool existPath(const QString& path);

	/*
	* @makeDir,����Ŀ¼
	* @param1,Ŀ¼��
	* @return,bool
	*/
	bool makeDir(const QString& dir);

	/*
	* @existDir,����Ŀ¼
	* @param1,Ŀ¼��
	* @return,bool
	*/
	bool existDir(const QString& dir);

	/*
	* @_getAppVersion,��ȡӦ�ó���汾��
	* @return,const QString
	*/
	QString _getAppVersion();

	/*
	* @getAppVersion,��ȡӦ�ó���汾��
	* @return,const QString
	* @notice,ֻ��ȡһ��
	*/
	const QString& getAppVersion();

	/*
	* @setAppAppendName,����Ӧ�ó��򸽼�����
	* @notice,���APIΪ�ⲿ������ʹ��
	* @param1,������
	* @return,void
	*/
	void setAppAppendName(const QString& appendName);

	/*
	* @setAppAppendPos,����Ӧ�ó��򸽼�λ��
	* @notice,���APIΪ�ⲿ������ʹ��
	* @param1,����λ��,ǰ:ture ��:false
	* @return,void
	*/
	void setAppAppendPos(bool appendPos);

	/*
	* @getAppAppendName,��ȡӦ�ó��򸽼�����
	* @notice,���APIΪ�ⲿ������ʹ��
	* @return,QString
	*/
	QString getAppAppendName();

	/*
	* @getAppAppendPos,��ȡӦ�ó��򸽼�λ��
	* @notice,���APIΪ�ⲿ������ʹ��
	* @return,bool
	*/
	bool getAppAppendPos();

	/*
	* @renameAppByVersion,ͨ���汾��������Ӧ�ó���
	* @notice,���Var::appendName��Ϊ��,��ʹ����
	* @param1,QWidget
	* @param2,������
	* @param3,����λ��,ǰ:true ��:false
	* @return,bool
	*/
	bool renameAppByVersion(QWidget* widget, const QString& appendName = QString(), bool appendPos = false);

	/*
	* @startApp,����Ӧ�ó���
	* @param1,��������
	* @param2,��ʾ��ʽ
	* @param3,�Ƿ�Ϊ����·��
	* @return,bool
	*/
	bool startApp(const QString& name, const int& show, bool absolutely = false);

	/*
	* @finishApp,����Ӧ�ó���
	* @param1,������
	* @return,bool
	*/
	bool finishApp(const QString& name);

	/* 
	* @getCurrentTime,��ȡ��ǰʱ��
	* @param1,�Ƿ�Ϊ�ļ���ʽ
	* @return,const QString
	*/
	QString getCurrentTime(bool fileFormat = false);

	/* 
	* @getCurrentDate,��ȡ��ǰ����
	* @param1,�Ƿ�Ϊ�ļ���ʽ
	* @return,const QString
	*/
	QString getCurrentDate(bool fileFormat = false);

	/*
	* @getCurrentDateTime,��ȡ��ǰʱ������
	* @param1,�Ƿ�Ϊ�ļ���ʽ
	* @return,const QString
	*/
	QString getCurrentDateTime(bool fileFormat = false);

	/*
	* @getFileListByPath,ͨ��·����ȡ�ļ��б�
	* @param1,·��
	* @param2,����ȡ���ļ��б�
	* @return,void
	*/
	void getFileListByPath(const QString& path, QStringList& fileList);

	/* 
	* @getFileListBySuffixName,ͨ����׺����ȡ�ļ��б�
	* @param1,·��
	* @param2,��׺��
	* @return,QStringList
	*/
	QStringList getFileListBySuffixName(const QString& path, const QStringList& suffix);

	/*
	* @wideCharToMultiByte,���ַ���ת���ַ���
	* @param1,���ַ�����
	* @return,char*
	* @notice,��Ҫ�ֶ��ͷ��ڴ�
	*/
	char* wideCharToMultiByte(const wchar_t* wide);

	/*
	* @multiByteToWideChar,���ַ���ת���ַ���
	* @param1,���ַ���
	* @return,wchar_t*
	* @notice,��Ҫ�ֶ��ͷ��ڴ�
	*/
	wchar_t* multiByteToWideChar(const char* multi);

	/*
	* @ping,���������Ƿ�ͨ
	* @param1,��ַ
	* @param2,����
	* @return,bool
	*/
	bool ping(const char* address, const int& times);

	/*
	* @compareList,�Ա��ַ�������
	* @param1,����1
	* @param2,����2
	* return,bool
	*/
	bool compareList(const QStringList& cmp1, const QStringList& cmp2);

	/*
	* @removeList,�Ƴ�����
	* @param1,����1
	* @param2,����2
	* @return,bool
	* @notice,�Ƴ�����2�д�������1�е�����
	*/
	bool removeList(const QStringList& list1, QStringList& list2);

	/*
	* @httpDownload,���ı�����Э������
	* @param1,����
	* @param2,��ʱ
	* @param3,lambda
	*/
	bool httpDownload(const char* url, int timeout, const std::function<void(long, const char*)>&);

	/*
	* ��ʼ������̨����
	* @notice,���ô˺���ǰ,���ɽ����κ����,
	* �����ض�������ʧ��,���޷�������ӡ����.
	*/
	bool initConsoleWindow(const QString& title = "���Կ���̨");

	/*
	* @exitConsoleWindow,�˳�����̨����
	* @return,bool
	*/
	bool exitConsoleWindow();

	/*
	* @getTimeStamp,��ȡʱ���
	* @notice,����ISO DATE��ʽ 2021-05-15 10:00:00.00 
	* @param1,������ʱ��
	* @param2,������ʱ��
	* @return,const uint[�ɹ���������,ʧ��(uint)-1]
	*/
	uint getTimeStamp(const QString& dt1, const QString& dt2);

	/*
	* @secToMinCountdown,���ӵ���ʱ
	* @param1,�ܼƷ���
	* @param2,��ǰ����
	* @param3,����ʱ����
	* @param4,����ʱ��
	* @return,void
	*/
	void minuteCountdown(int totalMinute, int currentSecond, int& minute, int& second);

	/*
	* @getAppNameVersion,��ȡAPP���ư汾
	* @param1,APP����
	* @return,QString 
	* @useage,��C:/Windows/App[1.0.0.1].exe,�򷵻�1.0.0.1
	* �ɹ�,�򷵻ذ汾����1.0.0.0�ַ���
	* ʧ��,�򷵻ؿ��ַ���
	*/
	QString getAppNameVersion(const QString& fileName);

	/*
	* @versionToNumber,�汾ת����
	* @param1,��1.0.0.1��������ɴ�·����C:\Windows\XXXX[1.0.0.1].exe
	* @return,int
	* �ɹ�,���ؼ�����İ汾ֵ
	* ʧ��,-1
	*/
	int versionToNumber(const QString& appName);
}