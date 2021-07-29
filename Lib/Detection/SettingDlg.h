#pragma once

#include <QWidget>
#include <QMessageBox>
#include <QTimer>
#include <QTreeWidgetItem>
#include <Can/Can.h>

#include "JsonTool.h"
#include "GeneratedFiles/ui_SettingDlg.h"

#define ITEM_TO_STR(I,N) I->data(N, Qt::EditRole).toString()

enum DeviceButton {
	DB_POWER_CONN,
	DB_POWER_CTRL,
	DB_RELAY_CONN,
	DB_CURRE_CONN,
	DB_VOLTA_CONN,
};

#define BUTTON_COUNT 5

class SettingDlg : public QWidget
{
	Q_OBJECT
public:
	SettingDlg(QWidget *parent = Q_NULLPTR);

	~SettingDlg();

	const QString& getLastError();

	bool setAuthDlg(const int& flag = 1);

	bool initInstance();

	void setIsExistDlg(bool* existDlg);

	void setConnected(bool connected);

	//����ҳ
	void configExpand();

	void configAddNode();

	void configDelNode();

	void configSaveData();

	void configExitDlg();

	//��ͼҳ,�ⲿ������,���û���ָ��
	void setBasePointer(void* pointer);
public slots:
	/*����ҳ*/
	void configTreeItemPressedSlot(QTreeWidgetItem* item, int column);

	void configTreeItemDoubleClickedSlot(QTreeWidgetItem* item, int column);

	void configTreeItemChangedSlot(QTreeWidgetItem* item, int column);

	//Ӳ��ҳ
	//��Դ
	void powerConnectSlot();

	void powerControlSlot();

	//�̵���
	void relayConnectSlot();

	void relayControlSlot(bool checked);

	//������
	void currentConnectSlot();

	void currentGetValueSlot();

	//��ѹ��
	void voltageConnectSlot();

	void voltageGetValueSlot();

	/*CANҳ*/
	void addCanTableItemSlot(const char* type, const MsgNode& msg);

	void canBaseSendSlot();

	void canBaseSendSlot2();

	void canBaseStopSlot();

	void canStartupSlot();

	void canConnectSlot();

	void canMatrixTypeSlot(const QString& text);

	void canFilterEnableSlot();

	/*��ͼҳ*/
	void updateImageSlot(const QImage& image);

	void startCaptureSlot();

	void stopCaptureSlot();

	void saveCoordSlot();

	void connectCaptureSlot();
protected:
	void setLastError(const QString& error);

	bool initConfigTreeWidget();

	bool initHardwareWidget();

	bool initCanTableWidget();

	bool initPaintWidget();

	bool initAboutWidget();
private:
	const int getComNumber(const QString& comName);

signals:
	void setAuthDlgSignal(bool* result, const int& flag);

	void addCanTableItemSignal(const char* type, const MsgNode& msg);

	void restartSignal(const QString& name);
private:
	Ui::SettingDlg ui;

	CanMatrix m_matrix;

	JsonTool* m_jsonTool = nullptr;

	QString m_lastError = "No Error";

	bool m_configItemOpen = false;

	QTreeWidgetItem* m_currentConfigItem = nullptr;

	QString m_currentConfigValue = "";

	int m_currentConfigColumn = 0;

	enum UpdateWarn {
		UW_NO,
		UW_RESTART,
		UW_RECONNECT,
		UW_EMPTY,
	}m_updateWarn = UW_NO;

	MsgNode m_msg, m_msg2;

	QTimer m_canBaseSendTimer;
	QTimer m_canBaseSendTimer2;

	bool m_startCapture = false;

	void* m_basePointer = nullptr;

	bool m_canThreadStart = false;

	bool* m_isExistDlg = nullptr;

	QList<bool> m_buttonList;

	bool m_connected = false;

	QList<QCheckBox*> m_relayBoxList;

	bool m_canConnect = false;

	bool m_captureStatus = false;

	int m_canLogCount = 0;
};
