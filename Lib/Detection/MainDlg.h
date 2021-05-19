#pragma once

/*
* MainDlg.h����ļ���������������
*/

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
#include "UpdateDlg.h"

/*
* @NEW_MAIN_DLG,����һ���Ի���
* @param1,�̳�Dt::Base���߳�
*/
#define NEW_MAIN_DLG(THREAD)\
(NO_THROW_NEW MainDlg(NO_THROW_NEW THREAD))->show()


/*
* @RUN_MAIN_FNC,����������
* @param1,�̳�Dt::Base���߳�
*/
#define RUN_MAIN_FNC(THREAD)\
int main(int argc,char* argv[])\
{\
QApplication a(argc,argv);\
NEW_MAIN_DLG(THREAD);\
return a.exec();\
}

/*
* @MainDlg,������Ի���
*/
class MainDlg : public QWidget
{
	Q_OBJECT
public:
	/*
	* @MainDlg,���Ի�����
	* @param1,������ָ��
	* @param2,�ؼ�����
	*/
	MainDlg(Dt::Base* thread, QWidget* parent = nullptr);

	/*
	* @~MainDlg,���Ի�������
	*/
	~MainDlg();

	/*
	* @initInstance,��ʼ��ʵ��
	* @return,bool
	*/
	bool initInstance();

	/*
	* @getLastError,��ȡ���մ���
	* @return,QString
	*/
	const QString& getLastError();

public slots:
	/*
	* @setScanCodeDlgSlot,����ɨ��Ի����
	* @param1,�Ƿ���ʾ
	* @return,vid
	*/
	void setScanCodeDlgSlot(bool show);

	/*
	* @setUnlockDlgSlot,���ý����Ի����
	* @param1,�Ƿ���ʾ
	* @return,void
	*/
	void setUnlockDlgSlot(bool show);

	/*
	* @setAuthDlgSlot,������֤�Ի����
	* @param1,���ָ��
	* @return,void
	*/
	void setAuthDlgSlot(bool* result);

	/*
	* @setDownloadDlgSlot,�������ضԻ����
	* @param1,������Ϣ�ṹ��ָ��
	* @return,void
	*/
	void setDownloadDlgSlot(BaseTypes::DownloadInfo* info);

	/*
	* @settingButtonSlot,���ð�ť��
	* @return,void
	*/
	void settingButtonSlot();

	/*
	* @connectButtonSlot,���Ӱ�ť��
	* @return,void
	*/
	void connectButtonSlot();

	/*
	* @exitButtonSlot,�˳���ť��
	* @return,void
	*/
	void exitButtonSlot();

	/*
	* @setMessageBoxSlot,������Ϣ�Ի���
	* @notice,���̷߳����źŵ����߳���ʹ��
	* @param1,����
	* @param2,�ı�
	* @return,void
	*/
	void setMessageBoxSlot(const QString& title, const QString& text);

	/*
	* @setMessageBoxSlot,������Ϣ�Ի���
	* @notice,���̷߳����źŵ����߳���ʹ��
	* @param1,����
	* @param2,�ı�
	* @param3,����
	* @return,void
	*/
	void setMessageBoxExSlot(const QString& title, const QString& text, const QPoint& point);

	/*
	* @setQuestionBoxSlot,����ѯ�ʶԻ���
	* @notice,���̷߳����źŵ����߳���ʹ��
	* @param1,����
	* @param2,�ı�
	* @param3,���
	* @return,void
	*/
	void setQuestionBoxSlot(const QString& title, const QString& text, bool* result);

	/*
	* @setQuestionBoxSlot,����ѯ�ʶԻ���
	* @notice,���̷߳����źŵ����߳���ʹ��
	* @param1,����
	* @param2,�ı�
	* @param3,���
	* @param4,��recordList�е�����
	* @return,void
	*/
	void setQuestionBoxExSlot(const QString& title, const QString& text, bool* result, const QPoint& point);


	/*
	* @setPlayQuestionBoxSlot,���ò���ѯ�ʶԻ���
	* @notice,���̷߳����źŵ����߳���ʹ��
	* @param1,����
	* @param2,�ı�
	* @param3,���
	* @param4,��recordList�е�����
	* @return,void
	*/
	void setPlayQuestionBoxSlot(const QString& title, const QString& text, int* result, const QPoint& point);

	/*
	* @setCurrentStatusSlot,���õ�ǰ״̬��
	* @param1,��ǰ״̬����
	* @param2,�Ƿ�Ϊϵͳ״̬
	* @return,void
	*/
	void setCurrentStatusSlot(const QString& status, bool systemStatus);

	/*
	* @setTestResultSlot,���ò��Խ����
	* @param1,���Խ��ö��
	* @return,void
	*/
	void setTestResultSlot(const BaseTypes::TestResult& result);

	/*
	* @addListItemSlot,����б���Ŀ��
	* @param1,��Ŀ����
	* @param2,�Ƿ�Ϊ��־��Ŀ
	* @return,void
	*/
	void addListItemSlot(const QString& item, bool logItem);

	/*
	* @clearListItemSlot,����б���Ŀ��
	* @return,void
	*/
	void clearListItemSlot();

	/*
	* @updateImageSlot,����ͼ���
	* @param1,ͼ��
	* @return,void
	*/
	void updateImageSlot(const QImage& image);

	/*
	* @coordinateSlot,�����
	* @param1,videoLabel�е�����
	* @return,void
	*/
	void coordinateSlot(const QPoint& point);

	/*
	* @usageRateTimerSlot,ʹ���ʶ�ʱ����
	* @return,void
	*/
	void usageRateTimerSlot();

	/*
	* @updateSfrSlot,���½���Ȳ�
	* @return,void
	*/
	void updateSfrSlot();

	/*
	* @restartSlot,������
	* @param1,������
	* @return,void
	*/
	void restartSlot(const QString& name);
protected:
	/*
	* @closeEvent,��д�ر��¼�
	* @param1,�ر��¼�
	* @return,void
	*/
	virtual void closeEvent(QCloseEvent* event);

	/*
	* @setLastError,���ô���
	* @param1,��������
	* @return,void
	*/
	void setLastError(const QString& error);
private:
	//���Ի���ؼ���
	Ui::MainDlgClass ui;

	//������
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

	/*���¶Ի���*/
	UpdateDlg* m_updateDlg = nullptr;

	/*�������*/
	QString m_lastError = "No Error";

	/*ʹ���ʶ�ʱ��*/
	QTimer m_usageRateTimer;

	/*�Ƿ�������öԻ���*/
	bool m_isExistSettingDlg = false;

	/*�Ƿ�������*/
	bool m_connected = false;
};
