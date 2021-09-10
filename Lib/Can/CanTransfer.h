#pragma once

#pragma execution_character_set("utf-8")

#include <QObject>

#include <QDir>

#include <QFileInfo>

#include <Windows.h>

#include <functional>

#include <LogMgr/LogMgr.h>
#pragma comment(lib, "LogMgr.lib")

#include <Timer/Timer.h>
#pragma comment(lib, "Timer.lib")

#include "CanMessage.h"

#include "CanMatrix.hpp"

#define MAX_SUPPORTED_BAUDRATE_COUNT 5

#define CONTINUE_RECEIVE_TIMES 2

#define CONTINUE_RECEIVE_INTERVAL 1

#define MAX_FRAME_COUNT 4096

/*
* @CanTransfer,CAN����
* @notice,������ΪCAN���ĸ���,����������CAN����Ҫ�̳д���
*/

class CanTransfer
{
public:
	/*
	* @CanTransfer,����
	*/
	CanTransfer();

	/*
	* @virtual ~CanTransfer,������
	*/
	virtual ~CanTransfer();

	/*
	* @setReceiveID,���ý���ID
	* @param1,��Ҫ���յ�CAN ID
	* @return,void
	*/
	void setReceiveID(unsigned int id)
	{
		m_filter = true;
		m_recvID = id;
	};

	/*
	* @setMatrixType,���þ�������
	* @param1,��������
	* @return,void
	*/
	void setMatrixType(MatrixType matrix);

	/*
	* @open,����CAN��
	* @notice,����Ϊ���麯��,������д
	* @param1,������
	* @param2,��չ֡,0��,1��
	* @param3,�豸��
	* @param4,�˿ں�
	* @return,bool
	*/
	virtual bool open(int baudrate, int extFrame, int device = 0, int port = 0) = 0;

	/*
	* @close,�Ͽ�CAN��
	* @notice,����Ϊ���麯��,������д
	* @return,bool
	*/
	virtual bool close() = 0;

	/*
	* @clearBuffer,��ջ���
	* @notice,����Ϊ���麯��,������д
	* @return,bool
	*/
	virtual bool clearBuffer() = 0;

	/*
	* @send,����CAN����
	* @notice,����Ϊ���麯��,������д
	* @param1,MsgNode�ṹ��
	* @return,bool
	*/
	virtual bool send(const MsgNode* msg) = 0;

	/*
	* @multiSend,���ط���
	* @notice,����Ϊ���麯��,������д
	* @param1,MsgNode�ṹ��
	* @param2,��������
	* @return,bool
	*/
	virtual bool multiSend(const MsgNode* msg, int count) = 0;

	/*
	* @receiveProtected,���ձ���
	* @param1,MsgNode�ṹ��
	* @param2,MsgNode�����С
	* @param3,����
	*/
	virtual int receiveProtected(MsgNode* pMsg, int size, int ms = 200) = 0;

	/*
	* @receive,����CAN����
	* @param1,MsgNode�ṹ��
	* @param2,���ն���MsgNode�ṹ��
	* @param3,����
	* @param4,����ID
	* @param5,����ID
	* @return,const int,����ʵ�ʽ��յ�����
	*/
	int receive(MsgNode* msg, int size, int ms, int recvID = -1, int cmdID = -1);

	/*
	* @quickReceive,���ٽ���CAN����
	* @param1,MsgNode�ṹ��
	* @param2,���ն���MsgNode�ṹ��
	* @param3,����
	* @return,const int,����ʵ�ʽ��յ�����
	*/
	int quickReceive(MsgNode* msg, int size, int ms);

	/*
	* @receive,����CAN����
	* @param1,MsgNode�ṹ��
	* @param2,���ն���MsgNode�ṹ��
	* @param3,����
	* @param4,����ID
	* @param5,����ID
	* @return,const int,����ʵ�ʽ��յ�����
	*/
	int safeReceive(MsgNode* msg, int size, int ms, int recvID = -1, int cmdID = -1);

	/*
	* @enableSaveLog,���ñ�����־
	* @param1,�Ƿ�����
	* @return,void
	*/
	void enableSaveLog(bool on);

	/*
	* @enableDebug,���õ���
	* @param1,�Ƿ�����
	* @return,void
	*/
	void enableDebug(bool on);

	/*
	* @saveLog,������־
	* @param1,��������
	* @param2,MsgNode�ṹ��
	* @param3,MsgNode������
	* @return,bool
	*/
	bool saveLog(const char* type, const MsgNode* msg, int count = 1);

	/*
	* @flushLogFile,ˢ����־�ļ�
	* @return,bool
	*/
	bool flushLogFile();

	/*
	* @clearLogFile,�����־�ļ�
	* @return,bool
	*/
	bool clearLogFile();

	/*
	* @connected,�Ƿ�����
	* @return,bool
	*/
	bool isOpened();

	/*
	* @startReceiveThread,���������߳�
	* @return,bool
	*/
	bool startReceiveThread();

	/*
	* @stopReceiveThread,ֹͣ�����߳�
	* @return,bool
	*/
	bool stopReceiveThread();

	/*
	* @getLastError,��ȡ���մ���
	* @return,const char*
	*/
	const QString& getLastError();

	/*
	* @setSaveLogInfo,���ñ�����־��Ϣ
	* @param1,Ŀ¼��
	* @param2,������
	* @param3,������
	* @return,void
	*/
	void setSaveLogInfo(const QString& dirName, const QString& moduleName = QString(), const QString& codeName = QString());

	/*
	* @setProcessFnc,���ô�����
	* @param1,lambda���ʽ,���ڴ�����(����/����)������
	* @format,std::function<void(const char*, const MsgNode&)>
	* @return,void
	*/
	void setProcessFnc(const std::function<void(const char*, const MsgNode&)>& fnc = nullptr);

	/*
	* @getDataLength,��ȡ���ݳ���
	* @notice,��API��ʹ��FD��ʱ��Żᱻ����.
	* @return,int
	*/
	int getDlc() const;

	/*
	* @m_matrix,CAN�����㷨
	*/
	CanMatrix m_matrix;

	/*
	* @��ȡ�Ƿ�ʹ��flexible data-rate
	* @return,bool
	*/
	bool getUseFd() const;

	/*
	* @����ʹ��flexible data-rate
	* @param1,�Ƿ�ʹ��
	* @return,void
	*/
	void setUseFd(bool on);

protected:

	/*
	* @setLastError,�������մ���
	* @param1,���մ���
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @receiveThread,�����߳�
	* @param1,�������
	* @return,unsigned int
	*/
	static UINT WINAPI receiveThread(void* args);

	/*
	* @getDlc,��ȡ���ݳ��ȴ���,DLC[data length code]
	* @param1,MsgNode�ṹ��
	* @return,ʵ�ʳ���
	*/
	int getDlc(const MsgNode* msg);

	/*
	* @formatMsg,��ʽ������
	* @param1,����[S:SEND,R:RECEIVE,F:FAILED]
	* @param1,MsgNode�ṹ��
	* @param2,��Ҫ��ʽ���Ļ�����
	* @return,void
	*/
	void formatMsg(const char* type, const MsgNode& msg, char* text);

	/*
	* @outputMsg,�������
	* @param1,����[S:SEND,R:RECEIVE,F:FAILED]
	* @param2,MsgNode�ṹ��
	* @param3,MsgNode����
	* @return,void
	*/
	void outputMsg(const char* type, const MsgNode* msg, int count = 1);

	/*
	* @setDlc,�������ݳ���
	* @notice,��API��Ҫ��д�����е���,ʵ�ʳ���ȡ���ڶ���,һ�㳤��Ϊ8
	* @param1,����
	* @return,void
	*/
	void setDlc(int dlc = 8);
protected:
	//�߳��Ƿ��˳�
	bool m_quit = false;

	//�Ƿ��
	bool m_open = false;

	//�豸��
	int m_device = 0;

	//������
	int m_baudrate = 500;

	//�˿ں�
	int m_port = 0;

	//��չ֡
	int m_extFrame = 0;

	//����ID
	int m_recvID = 0;

	//�Ƿ���˱���
	bool m_filter = false;

	//CAN����
	CanMessage m_message;

	//��ʱ��
	Timer m_timer;

	//�Ƿ񱣴���־
	bool m_saveLog = false;

	//�Ƿ����
	bool m_debug = false;

	//ACC����
	int m_accCode = 0;

	//ACC����
	int m_accMask = 0;

	//ʹ��CANFD(CAN flexible data-rate)
	bool m_useFd = false;
private:
	//�߳̾��
	HANDLE m_threadHandle = nullptr;

	//�߳���
	CRITICAL_SECTION m_cs;

	CLogMgr* m_logMgr = nullptr;

	//�ļ�����
	QString m_dirName = "Log";

	//������
	QString m_modelName = "δ֪����";

	//������
	QString m_codeName = "δ֪����";

	//���մ���
	QString m_lastError = "No Error";

	//���Ĵ�����
	std::function<void(const char*, const MsgNode&)> m_fnc = nullptr;

	//���ݳ���
	int m_dlc = 8;
};

