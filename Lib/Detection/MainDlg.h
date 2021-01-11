#pragma once

#include <QtWidgets/QWidget>
#include <QDebug>
#include "GeneratedFiles/ui_MainDlg.h"
#include "SettingDlg.h"
#include "ScanCodeDlg.h"
#include "UnlockDlg.h"
#include "AuthDlg.h"
#include "DownloadDlg.h"
#include "Detection.h"
#include "jqcpumonitor.h"

#define NEW_MAIN_DLG(THREAD)\
(NO_THROW_NEW MainDlg(NO_THROW_NEW THREAD))->show()

#define RUN_MAIN_FNC(THREAD)\
int main(int argc,char* argv[])\
{\
QApplication a(argc,argv);\
NEW_MAIN_DLG(THREAD);\
return a.exec();\
}

class MainDlg : public QWidget
{
	Q_OBJECT
public:
	/*����*/
	MainDlg(Dt::Base* thread, QWidget* parent = nullptr);

	/*����*/
	~MainDlg();

	/*��ʼ��ʵ��*/
	bool initInstance();

	/*��ȡ����*/
	const QString& getLastError();
public slots:
	/*ɨ��Ի����*/
	void setScanCodeDlgSlot(bool show);

	/*�����Ի����*/
	void setUnlockDlgSlot(bool show);

	/*��֤�Ի����*/
	void setAuthDlgSlot(bool* result);

	/*�������ضԻ����*/
	void setDownloadDlgSlot(BaseTypes::DownloadInfo* info);

	/*���ð�ť��*/
	void settingButtonSlot();

	/*���Ӱ�ť��*/
	void connectButtonSlot();

	/*�˳���ť��*/
	void exitButtonSlot();

	/*��ͨ��Ϣ�Ի����*/
	void setMessageBoxSlot(const QString& title, const QString& text);

	void setMessageBoxExSlot(const QString& title, const QString& text, const QPoint& point);

	/*ѯ�ʶԻ����*/
	void setQuestionBoxSlot(const QString& title, const QString& text, bool* result);

	void setQuestionBoxExSlot(const QString& title, const QString& text, bool* result, const QPoint& point);

	/*���õ�ǰ״̬��*/
	void setCurrentStatusSlot(const QString& status, bool systemStatus);

	/*���ý����*/
	void setTestResultSlot(const BaseTypes::TestResult& result);

	/*����б�Ԫ�ز�*/
	void addListItemSlot(const QString& item, bool logItem);

	/*����б�Ԫ�ز�*/
	void clearListItemSlot();

	/*����ͼ���*/
	void updateImageSlot(const QImage& image);

	void coordinateSlot(const QPoint& point);

	void usageRateTimerSlot();

	void updateSfrSlot();
protected:
	/*��д�ر��¼�*/
	virtual void closeEvent(QCloseEvent* event);

	/*���ô���*/
	void setLastError(const QString& error);
private:
	Ui::MainDlgClass ui;

	Dt::Base* m_base = nullptr;

	/*ɨ��Ի���*/
	ScanCodeDlg* m_scanCodeDlg = nullptr;

	/*�����Ի���*/
	UnlockDlg* m_unlockDlg = nullptr;

	/*��֤�Ի���*/
	AuthDlg* m_authDlg = nullptr;

	/*���öԻ���*/
	SettingDlg* m_settingDlg = nullptr;

	/*���ضԻ���*/
	DownloadDlg* m_downloadDlg = nullptr;

	/*�������*/
	QString m_lastError = "No Error";

	/*ʹ���ʶ�ʱ��*/
	QTimer m_usageRateTimer;

	/*�Ƿ�������öԻ���*/
	bool m_isExistSettingDlg = false;

	/*�Ƿ�������*/
	bool m_connected = false;
};
