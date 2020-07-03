#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QStyleFactory>

#include "JsonTool.h"
#include "ScanCodeDlg.h"
#include "ui_MainDlg.h"
#include "ThreadHandler.h"
#include "ChannelDlg.h"
#include "SettingDlg.h"
#include "ChoiceDlg.h"
#include "JQCPUMonitor"
#include "AuthDlg.h"
/************************************************************************/
/* ��������                                                             */
/************************************************************************/

class MainDlg : public QWidget
{
	Q_OBJECT
public:
	/*����*/
	explicit MainDlg(QWidget* parent = nullptr);

	/*����*/
	~MainDlg();

	/*��ȡ������Ϣ*/
	const QString& getLastError();

	/*��ʼ��Ui*/
	bool initUi();

	/*��ʼ��*/
	bool initInstance();

	/*��ʼ��CPU���*/
	bool initCpuMonitor();

	/*����ͨ������*/
	bool createChannelInterface();

	/*ɾ��ͨ������*/
	void deleteChannelInterface();

	/*����û���豸����*/
	bool createNoDeviceInterface();

	/*ɾ��û���豸����*/
	void deleteNoDeviceInterface();

	/*�������ÿؼ�*/
	void enableControl(bool enable);

	/*������¼ģʽ�ؼ�*/
	void updateBurnModeControl(const int& burnMode);

	/*�ۺ���*/
public slots:
	/*���ð�ť��*/
	void settingButtonSlot();

	/*���Ӱ�ť��*/
	void connectButtonSlot();

	/*�˳���ť��*/
	void exitButtonSlot();

	/*���¼����豸��*/
	void reloadButtonSlot();

	/*�Ի����*/
	void setMessageBoxSlot(const QString& title, const QString& text);

	/*ɨ��Ի����*/
	void scanCodeDlgSlot(const int& number);

	/*��������ȷ��*/
	void typeNameCorrectSlot(const QString& typeName);

	/*CPUʹ���ʶ�ʱ���ۺ���*/
	void cpuUsageRateTimerSlot();

	void updateTypeNameSlot();
protected:
	/*��д�ر��¼�*/
	virtual void closeEvent(QCloseEvent* event);

	/*���ô�����Ϣ*/
	void setLastError(const QString& err);

	/*ɾ�����ɿؼ�*/
	void deleteStretch();
private:
	Ui::MainDlg ui;

	JsonTool* m_jsonTool = nullptr;

	/*������Ϣ*/
	QString m_lastError = "None Error";

	/*�̹߳�����*/
	ThreadHandler* m_threadHandler = nullptr;

	/*ͨ������*/
	ChannelDlg* m_channelInterface = nullptr;

	/*ɨ��Ի���*/
	ScanCodeDlg* m_scanCodeDlg = nullptr;

	/*��֤�Ի���*/
	AuthDlg* m_authDlg = nullptr;

	/*��ֱ�������ڻ����ؼ�*/
	QVBoxLayout* m_vBoxLayout = nullptr;

	/*����û�������豸��ʾ��Ϣ*/
	QLabel* m_noDevice = nullptr;

	/*Aardvark ����¼��������,���Դ˴�������������¼������*/
	int m_aardvarkCount = 0;

	/*ѡ��Ի���*/
	ChoiceDlg* m_choiceDlg = nullptr;

	/*���ιر��¼�*/
	bool m_maskCloseEvent = false;

	/*CPUʹ���ʼ�ⶨʱ��*/
	QTimer* m_cpuUsageRateTimer = nullptr;

	/*��Ļ��С*/
	QSize m_screenSize;

	/*������*/
	QString m_typeName;

	/**�Ƿ�����ɾ��combobox��Ŀ*/
	bool m_delComboItem = false;
};