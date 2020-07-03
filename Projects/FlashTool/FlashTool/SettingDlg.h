#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QDateTime>
#include <QComboBox>
#include "QMessageBoxEx.h"
#include "JsonTool.h"
#include "ui_SettingDlg.h"

#define ITEM_TO_STR(ITEM,I) \
ITEM->data(I,Qt::EditRole).toString()

class SettingDlg : public QWidget
{
	Q_OBJECT
public:
	/*����*/
	SettingDlg(QWidget* parent = nullptr);
	
	/*����*/
	~SettingDlg();
	
	/*��ȡ����*/
	const QString& getLastError();

	/*��ʼ��*/
	bool initInstance();
	
	/*��ʼ�����ؼ�*/
	bool initTreeWidget();
	
	/*׷���ļ����ýڵ�*/
	bool appendFileConfigNode();
	
	/*ɾ���ļ����ýڵ�*/
	bool deleteFileConfigNode();
	
	bool setAuthDlg();

	void setTypeName(const QString& name) { m_typeName = name; };

	void setUpdateTypeName() { emit updateTypeNameSignal(); };
public slots:
	void itemPressedSlot(QTreeWidgetItem* item, int column);
	
	void itemDoubleClickedSlot(QTreeWidgetItem* item, int column);
	
	void itemChangedSlot(QTreeWidgetItem* item, int column);
	
	void saveDataSlot();

	void expandAllSlot();

	void closeSlot();

	void appendFileConfigNodeSlot();

	void deleteFileConfigNodeSlot();
protected:
	virtual void showEvent(QShowEvent* event);

	void setLastError(const QString& err);
private:
	Ui::SettingDlg ui;

	JsonTool* m_jsonTool = nullptr;
	
	QString m_lastError = "None Error";
	
	QTreeWidgetItem* m_currentItem = nullptr;
	
	QString m_currentValue;
	
	int m_currentColumn = 0;
	
	bool m_itemOpen = false;

	QString m_typeName;
signals:
	void setAuthDlgSignal(bool*);

	void updateTypeNameSignal();
};
