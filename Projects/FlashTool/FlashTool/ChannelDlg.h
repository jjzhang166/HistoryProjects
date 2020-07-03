#pragma once
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QTimer>
#include "ui_ChannelDlg.h"
#include "ThreadHandler.h"

/************************************************************************/
/* ͨ��������,�ڲ���̬���ӵ��������ڿؼ���                              */
/* ��¼��ʱ�ؼ��ڲ�����,�߳��м�ʱ���޷�ʹ�ñ�����execʱ��ѭ��		    */	
/************************************************************************/

class ChannelDlg : public QWidget
{
	Q_OBJECT
private:
	Ui::ChannelDlg ui;

	/*ͨ��ID*/
	int m_channelId = 0;

	/*��¼�����к�*/
	quint32 m_channelSn = 0;

	/*��¼��ʱ��*/
	QTimer* m_burnTimer = nullptr;

	/*��¼��ʱ��ʱ��*/
	quint32 m_burnTimerTime = 0;
public:
	/*����*/
	ChannelDlg(QWidget* parent = Q_NULLPTR);

	/*����*/
	~ChannelDlg();

	/*������ؼ�����*/
	void setGroupTitle(const int& id, const quint32& sn);
	void setGroupTitle(const QString& title);
	void restoreGroupTitle();
signals:
	/*��ȡ��¼��ʱ��ʱ���ź�*/
	void getBurnTimerTimeSignal(const int& data);
public slots:
	/*���½�������*/
	void updateProgressSlot(const int& progress);

	/*���µ�ǰ״̬��*/
	void updateCurrentStatusSlot(const QString& status);

	/*������¼״̬��*/
	void updateBurnStatusSlot(const burnStatus_t& status, const QString& err);

	/*���������*/
	void updateGroupTitleSlot(const QString& title);

	/*������¼��ʱ�����в�*/
	void setBurnTimerRunSlot(bool go);

	/*��¼��ʱ����ʱ��*/
	void burnTimerTimeoutSlot();
};