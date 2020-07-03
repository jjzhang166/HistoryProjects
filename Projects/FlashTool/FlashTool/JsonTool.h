#pragma once
#pragma execution_character_set("utf-8")

/************************************************************************/
/* JSON�����ļ�������                                                   */
/************************************************************************/
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>

#define SUPER_PASSWORD "i am superman"

/*��ͨ����Դ*/
#define IT6832 "IT6832"

/*��ͨ����Դ*/
#define IT6302 "IT6302"

#define NO_THROW_NEW new(std::nothrow)

#define SAFE_DELETE(X)\
if (X)\
{\
delete X;\
X = nullptr;\
}

#define SAFE_DELETE_A(X) \
if (X)\
{\
delete[] X;\
X = nullptr;\
}

/*���½ṹ��ȷ����������һֱ,�������*/

/*�豸����*/
struct DeviceConfig {
	/*��Դ����*/
	QString powerType;

	/*�Ƿ����õ�Դ*/
	QString openPower;

	/*�Ƿ����õ�������*/
	QString currentLimit;

	/*�Ƿ����ü̵���*/
	QString openRelay;

	/*�Ƿ�����ȥ��*/
	QString codeRepeat;

	/*����0xff*/
	QString jump0xff;
};

/*Ӳ������*/
struct HardwareConfig {
	/*��Դ���ں�,Ĭ��:19200*/
	int powerPort;

	/*��Դ������*/
	int powerBaud;

	/*��Դͨ����*/
	int powerChannel;

	/*��Դ��ʱ*/
	int powerDelay;

	/*�̵������ں�,COM20Ϊ20*/
	int relayPort;

	/*�̵���������,Ĭ��:19200*/
	int relayBaud;

	/*�̵������µ���ʱ*/
	int relayDelay;
};

/*��ֵ����*/
struct ThresholdConfig {
	/*��Դ��ѹ*/
	float powerVoltage;

	/*��Դ����*/
	float powerCurrent;
};

/************************************************************************/
/* �̵���IO����������JSON                                               */
/************************************************************************/
struct RelayConfig {
	/*�̵���IO��*/
	int port;

	/*�Ƿ�����IO*/
	int enable;
};

/************************************************************************/
/* �ļ�����                                                          */
/************************************************************************/
struct FileConfig {
	/*�ļ�·��*/
	QString path;

	/*�ڵ�����*/
	QString nodeName;

	/*���볤��*/
	int codeLength;

	/*�������*/
	QString codeRule;

	/*����BIN����*/
	QByteArray data;

	/*��ַ����*/
	int addressLength;

	/*����slave*/
	int dataSlave;

	/*У��slave*/
	int checkSlave;

	/*��¼ģʽ*/
	int burnMode;

	/*���ģʽ*/
	int speedMode;

	/*�ײ���ȡ��ʱ*/
	int libReadDelay;

	/*�ײ��д����ʱ*/
	int libWriteDelay;

	/*Ӧ�ò��ȡ��ʱ*/
	int appReadDelay;

	/*Ӧ�ò�д����ʱ*/
	int appWriteDelay;

	/*������ʱ*/
	int rebootDelay;

	/*�豸�ٶ�*/
	int deviceSpeed;

	/*�豸��ʱ*/
	int deviceTimeout;

	/*�����Ƿ���Ч*/
	bool valid;

	/*У��ϵ�*/
	int checkOutage;
};

/************************************************************************/
/* JsonTool��                                                           */
/************************************************************************/
class JsonTool : public QObject
{
	Q_OBJECT
public:
	/*��������ɾ��*/
	JsonTool(const JsonTool&) = delete;

	/*��ֵ����ɾ��*/
	JsonTool& operator=(const JsonTool&) = delete;

	/*��ȡ����*/
	static JsonTool* getInstance();

	/*ɾ������*/
	static void deleteInstance();

	/*��ʼ��Json����*/
	bool initInstance(bool update = false, const QString& fileName = "FlashConfig.json");

	/*��ȡ����*/
	const QString& getLastError();

	/*��ȡ��������*/
	const QStringList& getAllMainKey();

	/*��ȡ��¼ģʽ*/
	const QString& getBurnModeTips();

	const QString& getFilePathTips();
	/************************************************************************/
	/* DeviceConfig                                                         */
	/************************************************************************/

	/*��ȡ�豸���ö���*/
	const QJsonObject& getDeviceConfigObj();

	/*ͨ������ȡ�豸����ֵ*/
	const QString getDeviceConfigValue(const QString& key);

	/*ͨ���������豸����ֵ*/
	bool setDeviceConfigValue(const QString& key, const QString& value);

	/*��ȡ�豸���ü��б�*/
	const QStringList& getDeviceConfigKeyList();

	/*��ȡ�豸����ֵ�б�*/
	const QStringList& getDeviceConfigValueList();

	/*��ȡ�豸����Ԫ������*/
	const int getDeviceConfigCount();

	/*��ȡ�ѽ������豸����*/
	DeviceConfig* getParsedDeviceConfig();

	/*��ȡ�豸����˵��*/
	const QStringList& getDeviceConfigExplain();

	/*��ӡ�ѽ������豸����*/
	void printDeviceConfigData();

	/*��ӡ�ѽ������豸����*/
	void printDeviceConfigData(const DeviceConfig& deviceConfig);

	/************************************************************************/
	/* HardwareConfig                                                       */
	/************************************************************************/
	/*��ȡӲ����������*/
	const int getHardwareConfigCount();
	
	/*��ȡӲ������ֵ*/
	const QString getHardwareConfigValue(const QString& key);

	/*��ȡӲ�����ü�*/
	const QStringList& getHardwareConfigKeyList();

	/*��ȡӲ������ֵ*/
	const QStringList& getHardwareConfigValueList();

	/*��ȡӲ������˵��*/
	const QStringList& getHardwareConfigExplain();

	/*����Ӳ������ֵ*/
	bool setHardwareConfigValue(const QString& key, const QString& value);

	/*��ȡ�ѽ�����Ӳ������*/
	HardwareConfig* getParsedHardwareConfig();

	/************************************************************************/
	/* ThresholdConfig                                                      */
	/************************************************************************/
	/*��ȡ��ֵ��������*/
	const int getThresholdConfigCount();

	/*��ȡ��ֵ��ֵ*/
	const QString getThresholdConfigValue(const QString& key);

	/*��ȡ��ֵ���ü��б�*/
	const QStringList& getThresholdConfigKeyList();

	/*��ȡ��ֵ����ֵ�б�*/
	const QStringList& getThresholdConfigValueList();

	/*������ֵ��ֵ*/
	bool setThresholdConfigValue(const QString& key, const QString& value);

	/*��ȡ��ֵ����˵��*/
	const QStringList& getThresholdConfigExplain();

	/*��ȡ�ѽ�������ֵ����*/
	ThresholdConfig* getParsedThresholdConfig();

	/************************************************************************/
	/* RelayConfig                                                    */
	/************************************************************************/

	/*��ȡ�̵���IO�ڶ���*/
	const QJsonObject& getRelayConfigObj();

	/*ͨ������ȡ�̵�������ֵ*/
	const QString getRelayConfigValue(const QString& key);

	/*ͨ�������ü̵�������ֵ*/
	bool setRelayConfigValue(const QString& key, const QString& value);

	/*��ȡ�̵������б�*/
	const QStringList& getRelayConfigKeyList();

	/*��ȡ�̵���ֵ�б�*/
	const QStringList& getRelayConfigValueList();

	/*��ȡ�̵�����������*/
	const int getRelayConfigCount();

	/*��ȡ�̵�������˵��*/
	const QStringList& getRelayConfigExplain();

	/*��ȡ�ѽ�����IO������*/
	RelayConfig* getParsedRelayConfig();


	/************************************************************************/
	/* UserConfig                                                           */
	/************************************************************************/

	/*��ȡ�û����ü��б�*/
	const QStringList& getUserConfigKeyList();

	/*��ȡ�û�����ֵ�б�*/
	const QStringList& getUserConfigValueList();

	/*ͨ������ȡ�û�����ֵ*/
	const QString getUserConfigValue(const QString& key);

	/*��ȡ�û���������*/
	const int getUserConfigCount();

	/*�����û�ֵ*/
	bool setUserConfigValue(const QString& key, const QString& value);

	/*��ȡ�û�����˵��*/
	const QStringList& getUserConfigExplain();

	/************************************************************************/
	/* FileConfig                                                           */
	/************************************************************************/
	
	/*��ȡ�ļ���������*/
	const int getFileConfigCount();

	/*��ȡ�ļ����ö���*/
	QJsonObject& getFileConfigObj();

	/*��ȡ�����б�*/
	const QStringList getParentFileConfigKeyList();

	/*��ȡ�ļ����ü��б�*/
	const QStringList& getFileConfigKeyList();

	/*��ȡ�ļ�����ֵ�б�*/
	const QStringList& getFileConfigValueList();

	/*��ȡ�ļ�����ֵ*/
	const QString getFileConfigValue(const QString& parentKey, const QString& childKey);

	/*�����ļ����ü�*/
	bool setFileConfigKey(const QString& oldKey, const QString& newKey);

	/*�����ļ�����ֵ*/
	bool setFileConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ�ѽ������ļ�����*/
	bool getParsedFileConfig(const QString& nodeName);

	/*��ȡ�ļ�����˵��*/
	const QStringList& getFileConfigExplain();

	FileConfig* getParsedFileConfig();

	FileConfig* getParsedAllFileConfig();

	void printFileConfig();
protected:
	/*���ô���*/
	void setLastError(const QString& err);

	/*��ȡJson�����ļ�*/
	bool readJsonFile(const QString& name);

	/*д��Ĭ��Json�����ļ�*/
	bool writeJsonFile(const QString& name);

	/*����Json�����ļ�*/
	bool updateJsonFile(const QString& name);

	/*�����豸��������*/
	bool parseDeviceConfigData();

	/*����Ӳ����������*/
	bool parseHardwareConfigData();

	/*������ֵ��������*/
	bool parseThresholdConfigData();

	/*�����̵�����������*/
	bool parseRelayConfigData();

	/*�����ļ���������*/
	bool parseFileConfigData();

	/*�����û���������*/
	bool parseUserConfigData() { return true; };
private:
	/*����*/
	inline JsonTool(QObject* parent = nullptr) {};

	/*����*/
	inline ~JsonTool() {};

	/*��ָ̬��*/
	static JsonTool* m_self;

	/*ROOT����*/
	QJsonObject m_root;

	/*�豸���ö���*/
	QJsonObject m_deviceConfigObj;

	/*Ӳ�����ö���*/
	QJsonObject m_hardwareConfigObj;

	/*��ֵ���ö���*/
	QJsonObject m_thresholdConfigObj;

	/*�̵������ö���*/
	QJsonObject m_relayConfigObj;

	/*�ļ����ö���*/
	QJsonObject m_fileConfigObj;

	/*��BIN�ļ����б�*/
	QStringList m_parentFileKeyList;

	/*�û����ö���*/
	QJsonObject m_userConfigObj;

	/*��ȡ����*/
	QString m_lastError = "No Error";

	/*�豸����*/
	DeviceConfig m_deviceConfig;

	/*Ӳ������*/
	HardwareConfig m_hardwareConfig;

	/*��ֵ����*/
	ThresholdConfig m_thresholdConfig;

	/*�̵������ý�*/
	RelayConfig m_relayConfig[15];

	/*�ļ�����,������¼ʹ��*/
	FileConfig m_fileConfig;

	/*�ļ���������,ֻ������ʾ*/
	FileConfig* m_allFileConfig = nullptr;

	/*�豸���ü�*/
	QStringList m_deviceConfigKeyList = {
		"��Դ����",//0
		"���õ�Դ",//1
		"��������",//2
		"���ü̵���",//3
		"����ȥ��",//4
		"����0xFF"
	};

	/*�豸����ֵ*/
	QStringList m_deviceConfigValueList = {
		IT6302,//0
		"1",//1
		"0",//2
		"0",//3
		"0",//4
		"0"
	};

	/*Ӳ�����ü�*/
	QStringList m_hardwareConfigKeyList = {
		"��Դ����",//2
		"��Դ������",//3
		"��Դͨ����",//4
		"��Դ��ʱ",//5
		"�̵�������",//6
		"�̵���������",//7
		"�̵�����ʱ"//8
	};

	/*Ӳ������ֵ*/
	QStringList m_hardwareConfigValueList = {
		"1",//2
		"9600",//3
		"2",//4
		"300",//5
		"2",//6
		"19200",//7
		"300"//8
	};

	/*��ֵ���ü�*/
	QStringList m_thresholdConfigKeyList = {
		"��Դ��ѹ",//1
		"��Դ����"//2
	};

	/*��ֵ����ֵ*/
	QStringList m_thresholdConfigValueList = {
		"12.0",//1
		"0.1"//2
	};

	/*�̵�����*/
	QStringList m_relayConfigKeyList = {
		"�˿�0",//1
		"�˿�1",//2
		"�˿�2",//3
		"�˿�3",//4
		"�˿�4",//5
		"�˿�5",//6
		"�˿�6",//7
		"�˿�7",//8
		"�˿�8",//9
		"�˿�9",//10
		"�˿�10",//11
		"�˿�11",//12
		"�˿�12",//13
		"�˿�13",//14
		"�˿�14",//15
	};

	/*�̵���ֵ*/
	QStringList m_relayConfigValueList = {
		"0",//1
		"0",//2
		"0",//3
		"0",//4
		"0",//5
		"0",//6
		"0",//7
		"0",//8
		"0",//9
		"0",//10
		"0",//11
		"0",//12
		"0",//13
		"0",//14
		"0"//15
	};

	QStringList m_fileConfigKeyList = {
		"�ļ�·��",//1
		"���볤��",//7
		"�������",//2
		"��ַ����",//3
		"��д��ַ",//4
		"У���ַ",//5
		"��¼ģʽ",//6
		"����ģʽ",//13
		"�ײ��ȡ",//8
		"�ײ�д��",//9
		"Ӧ���ȡ",//10
		"Ӧ��д��",//12
		"������ʱ",//11
		"�豸����",
		"�豸��ʱ",
		"У��ϵ�"
	};

	QStringList m_fileConfigValueList = {
		"Config\\Bin\\A39-HSYNC.bin",//1
		"3",//7
		"HD",//2
		"7",//3
		"0x34",//4
		"0x34",//5
		"1",//6
		"0",//13
		"2",//8
		"2",//9
		"1",//10
		"1",//12
		"2000",//11
		"400",
		"150",
		"1"
	};

	QStringList m_userConfigKeyList = {
		"�û���",
		"����"
	};

	QStringList m_userConfigValueList = {
		"invo",
		"123456"
	};
};
