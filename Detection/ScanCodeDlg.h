#pragma once
#pragma execution_character_set("utf-8")

#include <Windows.h>
#include <QDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>
#include "GeneratedFiles/ui_ScanCodeDlg.h"
#include "JsonTool.h"
//9AMS30-7930010E2190315001
typedef enum WorkstationType
{
	WT_HARDWARE_TEST,   //Ӳ������
	WT_AGING_TEST,      //�ϻ�����
	WT_VERIFY_TEST,     //����ȷ��
}workstationType_t;

typedef enum QueryResult
{
	QR_OK,
	QR_NG,
	QR_PRE_NG,     //��վNG
	QR_PRE_NONE,   //��վδ��
	QR_CUR_NG,     //��վ��������NG
	QR_CUR_OK,     //��վ��������OK
}queryResult_t;

class ScanCodeDlg : public QDialog
{
	Q_OBJECT
private:
	Ui::ScanCodeDlg ui;

	const DeviceConfig& m_deviceConfig;

	QLabel m_minimize;
protected:
	/*��д�鷽��ʵ���ƶ�*/
	bool m_isPress = false;

	QPoint m_point;
	
	virtual void mousePressEvent(QMouseEvent* event);
	
	virtual void mouseReleaseEvent(QMouseEvent* event);
	
	virtual void mouseMoveEvent(QMouseEvent* event);
	
	virtual bool eventFilter(QObject* obj, QEvent* event);

	bool judgeCode(const QString& code);
	
	bool nativeEvent(const QByteArray& eventType, void* message, long* result);
	
	bool sendCode(const QString& code);
public:
	ScanCodeDlg(QWidget* parent = Q_NULLPTR);

	~ScanCodeDlg();
	void setLineEditText(const QString& text);
private slots:
	void returnPressedSlot();
};
