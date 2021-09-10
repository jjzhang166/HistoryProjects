#pragma once

#pragma execution_character_set("utf-8")

#include "CanFactory.h"

//���������
#define MAX_MSG_COUNT 256

//���ʹ���������
#define SEND_PROC_FNC(...) [__VA_ARGS__](MsgNode& FMSG)mutable->void

//���ʹ�����
typedef const std::function<void(MsgNode&)>& SendProc;

/*
* @SendType,��������
*/
enum SendType {
	//����
	ST_PERIOD,

	//�¼�
	ST_EVENT,

	//����/�¼�
	ST_PE,
};

/*
* @CanMsg,CAN����
* @notice,����ṹ��˳�򲻿�����ı�,
* ���򽫻ᵼ�³������
*/

struct CanMsg {
	//���Ľڵ�
	MsgNode msg;

	//������ʱ
	int delay;

	//��������
	SendType type;

	//���ʹ���
	int count;

	//���ʹ���
	std::function<void(MsgNode& msg)> fnc;

	//�Ƿ���Ч
	bool valid;

	//ʱ���
	int time;
};

class CanSender
{
public:
	/*
	* @CanSender,����
	*/
	CanSender();

	/*
	* @CanSender,����
	* @param1,CAN����
	*/
	CanSender(CanTransfer* transfer);

	/*
	* @~CanSender,����
	*/
	~CanSender();

	/*
	* @getLastError,��ȡ���մ���
	* @return,const QString&
	*/
	const QString& getLastError();

	/*
	* @init,��ʼ��
	* @param1,CAN����
	* @return,bool
	*/
	bool init(CanTransfer* transfer);

	/*
	* @getMsgData,��ȡ��������
	* @param1,����ID
	* @param2,��������
	* @return,bool
	*/
	bool getMsgData(int id, UCHAR* data);

	/*
	* @setMsgData,���ñ�������
	* @param1,����ID
	* @param2,��������
	* @return,bool
	*/
	bool setMsgData(int id, const UCHAR* data);

	/*
	* @addMsg,��ӱ���[����1]
	* @param1,����
	* @param2,��ʱ
	* @param3,��������
	* @param4,����
	* @param5,���ʹ���[lambda]
	* @return,bool
	*/
	bool addMsg(const MsgNode& msg, int delay, SendType type = ST_PERIOD, int count = 0, SendProc proc = nullptr);

	/*
	* @addMsg,��ӱ���[����2]
	* @param1,���ʹ���[lambda]
	* @param2,��ʱ
	* @param3,��������
	* @param4,���ʹ���
	* @return,bool
	*/
	bool addMsg(SendProc proc, int delay, SendType type = ST_PERIOD, int count = 0);

	/*
	* @addMsg,��ӱ���[����3]
	* @param1,����
	* @return,bool
	*/
	bool addMsg(const CanMsg& msg);

	/*
	* @addMsg,��ӱ���[����4]
	* @param1,���ĳ�ʼ���б�
	* @return,bool
	*/
	bool addMsg(const std::initializer_list<CanMsg>& msg);

	/*
	* @addMsg,��ӱ���[����5]
	* @param1,����
	* @notice,param1��ʽ:{{id},delay,type,count}
	* @param2,��ʼλ��
	* @param3,���ݳ���
	* @param4,����
	* @return,bool
	*/
	bool addMsg(const CanMsg& msg, int start, int length, ULONGLONG data);

	/*
	* @addPeriodEventMsg,��������¼�����
	* @param1,CAN����
	* @return,void
	*/
	bool addPeriodEventMsg(const CanMsg& msg);

	/*
	* @deleteMsgs,ɾ������[����1]
	* @param1,���ĳ�ʼ���б�
	* @return,void
	*/
	void deleteMsgs(const std::initializer_list<MsgNode>& msgs);

	/*
	* @deleteMsgs,ɾ������[����2]
	* @param1,ID��ʼ���б�
	* @return,void
	*/
	void deleteMsgs(const std::initializer_list<int>& ids);

	/*
	* @deleteOnMsg,ɾ��һ������[����1]
	* @param1,����
	* @return,void
	*/
	void deleteOneMsg(const MsgNode& msg);

	/*
	* @deleteOnMsg,ɾ��һ������[����2]
	* @param1,����
	* @return,void
	*/
	void deleteOneMsg(const CanMsg& msg);

	/*
	* @deleteOneMsg,ɾ��һ������
	* @param1,����ID
	* @return,void
	*/
	void deleteOneMsg(int id);

	/*
	* @deleteAllMsgs,ɾ�����б���
	* @return,void
	*/
	void deleteAllMsgs();

	/*
	* @pauseMsg,��ͣ����
	* @param1,���ĳ�ʼ���б�
	* @return,void
	*/
	void pauseMsg(const std::initializer_list<MsgNode>& msg);

	/*
	* @continueMsg,��������
	* @param1,���ĳ�ʼ���б�
	* @return,void
	*/
	void continueMsg(const std::initializer_list<MsgNode>& msg);

	/*
	* @start,��ʼ(���ͱ���)
	* @return,void
	*/
	void start();

	/*
	* @stop,ֹͣ(���ͱ���)
	* @return,void
	*/
	void stop();

protected:
	/*
	* @setLastError,�������մ���
	* @param1,���մ���
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @sendProcThread,���ʹ����߳�
	* @param1,����
	* @return,void
	*/
	static UINT WINAPI sendProcThread(void* args);
private:
	//�Ƿ��˳�
	bool m_quit = false;

	//�Ƿ�ʼ
	bool m_start = false;

	//CAN����
	CanTransfer* m_transfer = nullptr;

	//��ʱ��
	Timer m_timer;

	//��������
	CanMsg m_msgs[MAX_MSG_COUNT] = { 0 };

	//�������鱸��
	CanMsg m_msgsBackup[MAX_MSG_COUNT] = { 0 };

	//���
	HANDLE m_handle = nullptr;

	//���մ���
	QString m_lastError = "No Error";
};
