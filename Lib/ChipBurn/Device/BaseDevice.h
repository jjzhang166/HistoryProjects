#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>

class BaseDevice
{
public:
	BaseDevice();

	virtual ~BaseDevice();

	BaseDevice(const BaseDevice&) = delete;

	BaseDevice& operator=(const BaseDevice&) = delete;

	/*
	* @open,��
	* @param1,�˿�
	* @param2,������
	* @param3,��ʱ
	* @return,bool
	*/
	virtual	bool open(int port, int bitrate = 400, int timeout = 150) = 0;

	/*
	* @close,�ر�
	* @return,bool
	*/
	virtual bool close() = 0;

	/*
	* @readData,��ȡ����
	* @param1,��ȡ���ݵĻ�����
	* @param2,��ȡ���ݵĴ�С
	* @param3,�ӵ�ַ
	* @return,int
	*/
	virtual int readData(uchar* data, ushort size, ushort slave) = 0;

	/*
	* @writeData,д������
	* @param1,д�����ݵĻ�����
	* @param2,д�����ݵĴ�С
	* @param3,�ӵ�ַ
	* @return,int
	*/
	virtual int writeData(const uchar* data, ushort size, ushort slave) = 0;

	/*
	* @isOpened,�Ƿ��
	* @return,bool
	*/
	bool isOpened() const;

	/*
	* @getPort,��ȡ�˿�
	* @return,int
	*/
	int getPort() const;

	/*
	* @getLastError,��ȡ���մ���
	* @return,const QString&
	*/
	const QString& getLastError();

protected:
	/*
	* @setLastError,�������մ���
	* @param1,������Ϣ
	* @return,void
	*/
	void setLastError(const QString& error);
protected:
	bool m_open = false;

	int m_port = -1;

private:
	QString m_lastError = "No Error";
};

