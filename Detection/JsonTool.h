#pragma once
/************************************************************************/
/* ���ļ����ڹ������������ļ���д�޸�                               */
/************************************************************************/
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <memory>
#include <Common/Types.h>

#define Q_TO_C_STR(X) X.toStdString().c_str()

#define NO_THROW_NEW new(std::nothrow)

#define SAFE_DELETE_A(X)\
if (X)\
{\
	delete[] X;\
	X = nullptr;\
}

#define SAFE_DELETE(X)\
if (X)\
{\
	delete X;\
	X = nullptr;\
}

/************************************************************************/
/* �豸��������															*/
/************************************************************************/
typedef struct DeviceConfig
{
	/*������*/
	QString modelName;

	/*UDS����*/
	QString udsName;

	/*CAN����*/
	QString canName;

	/*�ɼ�������*/
	QString captureName;

	/*�������*/
	QString detectionName;

	/*�����ж�*/
	QString codeJudge;

	/*���볤��*/
	QString codeLength;
}deviceConfig_t;

/************************************************************************/
/* Ӳ������                                                             */
/************************************************************************/
typedef struct HardwareConfig
{
	/*��Դ���ں�*/
	int powerPort;

	/*��Դ������*/
	int powerBaud;

	/*��Դ��ѹ*/
	float powerVoltage;

	/*�̵������ں�,COM20Ϊ20*/
	int relayPort;

	/*�̵���������,Ĭ��:19200*/
	int relayBaud;

	/*��ѹ���ں�*/
	int voltagePort;

	/*��ѹ������,Ĭ��:9600*/
	int voltageBaud;

	/*��̬�������ں�*/
	int staticPort;

	/*��̬����������*/
	int staticBaud;

	/*��չ����1*/
	int expandCom1;

	/*��չ������1*/
	int expandBaud1;

	/*��չ����2*/
	int expandCom2;

	/*��չ������2*/
	int expandBaud2;

	/*��չ����3*/
	int expandCom3;

	/*��չ������3*/
	int expandBaud3;

	/*��չ����4*/
	int expandCom4;

	/*��չ������*/
	int expandBaud4;
}hardwareConfig_t;

/************************************************************************/
/* �̵���IO����������JSON                                               */
/************************************************************************/
typedef struct RelayConfig {
	/*GND IO��*/
	int gnd;

	/*ACC/IG IO��*/
	int acc;

	/*��̬����IO��*/
	int staticCur;

	/*Ӳ����IO��*/
	int key;

	/*ת�Ӱ�IO��*/
	int pinboard;

	/*LED����*/
	int led;

	/*����*/
	int sound;
}relayConfig_t;

/************************************************************************/
/* ���ο�ṹ�嶨��,����ǰ����������ͷ���м����ο�                    */
/************************************************************************/
typedef struct RectConfig
{
	/*��ɫ*/
	QString color;

	/*��*/
	int red;

	/*��*/
	int green;

	/*��*/
	int blue;

	/*���*/
	int deviation;

	/*��ʼX*/
	int startX;

	/*��ʼY*/
	int startY;

	/*��*/
	int width;

	/*��*/
	int height;
}rectConfig_t;

#define SMALL_RECT_  4

#define BIG_RECT_ 2

#define IMAGE_CHECK_COUNT  7
/************************************************************************/
/* ͼ�����ýṹ�嶨��                                                    */
/************************************************************************/
typedef struct ImageConfig
{
	/*����RGB*/
	int ignoreRgb;

	/*�Ƿ���ʾСͼ���ο�*/
	int showSmall;

	/*�Ƿ���ʾ��ͼ���ο�*/
	int showBig;

	/*�Ƿ񱣴���־*/
	int saveLog;

	/*С���ο�����*/
	rectConfig_t smallRect[SMALL_RECT_];

	/*����ο�����*/
	rectConfig_t bigRect[BIG_RECT_];
}imageConfig_t;

/************************************************************************/
/* ��Χ���ýṹ�嶨��                                                   */
/************************************************************************/
typedef struct RangeConfig
{
	/*DVR��������*/
	float minNetworkSpeed;
	float maxNetworkSpeed;

	/*DVR����X*/
	float minRayAxisX;
	float maxRayAxisX;

	/*DVR����Y*/
	float minRayAxisY;
	float maxRayAxisY;

	/*DVR�Ƕ�*/
	float minRayAxisA;
	float maxRayAxisA;

	/*�����*/
	float minSfr;
	float maxSfr;
}rangeConfig_t;

typedef struct ThresholdConfig
{
	/*CAN���ѵ�����ֵ*/
	float canRouse;

	/*CAN���ߵ�����ֵ*/
	float canSleep;
}thresholdConfig_t;

/*��������*/
struct EnableConfig
{
	/*�����Ի���*/
	int unlockDlg;

	/*������Ϣ�Ի���,����ʧ��������ʾ*/
	int errorDlg;

	/*CAN��־*/
	int saveCanLog;

	/*����ʧ��,�����в�����ʧ��*/
	int ignoreFailure;

	/*���CAN��־*/
	int outputRunLog;

	/*������־,�������������е���־*/
	int saveRunLog;
};

/*Ĭ������*/
struct DefConfig {
	DeviceConfig device;

	HardwareConfig hardware;
	
	RelayConfig relay;
	
	ImageConfig image;
	
	RangeConfig range;
	
	ThresholdConfig threshold;

	EnableConfig enable;
};

#define HWD_BUF 64
struct VoltageConfig
{
	/*�����*/
	bool result;

	/*���ֵ*/
	float read;

	/*����*/
	float high;

	/*����*/
	float low;

	/*�̵���IO*/
	int relay;

	/*����*/
	char name[HWD_BUF];
};

struct CurrentConfig
{
	/*�����*/
	bool result;

	/*���ֵ*/
	float read;

	/*����*/
	float high;

	/*����*/
	float low;

	/*��Դ��ѹ*/
	float voltage;

	/*����*/
	char name[HWD_BUF];
};

struct StaticConfig
{
	/*�����*/
	bool result;

	/*���ֵ*/
	float read;

	/*����*/
	float high;

	/*����*/
	float low;
};

struct KeyVolConfig
{
	/*�ߵ�ƽ���*/
	bool hResult;

	/*�͵�ƽ���*/
	bool lResult;

	/*�ߵ�ƽ���ֵ*/
	float hRead;

	/*�͵�ƽ���ֵ*/
	float lRead;

	/*�ߵ�ƽ����*/
	float hULimit;

	/*�ߵ�ƽ����*/
	float hLLimit;

	/*�͵�ƽ����*/
	float lULimit;

	/*�͵�ƽ����*/
	float lLLimit;
};

struct ResConfig
{
	/*�����*/
	bool result;

	/*���ֵ*/
	float read;

	/*����*/
	float high;

	/*����*/
	float low;

	/*�̵���IO*/
	int relay;

	/*����*/
	char name[HWD_BUF];
};

typedef struct HwdConfig
{
	VoltageConfig* voltage;
	ResConfig* res;
	CurrentConfig* current;
	StaticConfig	staticCurrent;
	KeyVolConfig	keyVol;
}hwdConfig_t;

/************************************************************************/
/* UDS����                                                              */
/************************************************************************/

struct VersonConfig
{
	/*DID*/
	uchar did[4];

	/*����*/
	char encode[HWD_BUF];

	/*����ֵ*/
	char setup[HWD_BUF];

	/*����*/
	char name[HWD_BUF];

	/*��С*/
	int size;

	/*��ȡֵ*/
	char read[HWD_BUF];

	/*���*/
	bool result;
};

struct DtcConfig
{
	/*�Ƿ����*/
	bool ignore;

	/*�Ƿ����*/
	bool exist;

	/*DTC*/
	uchar dtc[4];

	/*����*/
	char name[HWD_BUF];
};

typedef struct UdsConfig
{
	VersonConfig* ver;
	DtcConfig* dtc;
}udsConfig_t;


/************************************************************************/
/* CanSender����                                                        */
/************************************************************************/
#define MAX_MSG_COUNT 256

enum SendType {
	ST_Period,

	ST_Event,

	ST_PE,
};

struct CanMsg {
	bool bValid;

	SendType emST;

	int iCycle;

	MsgNode msg;

	int iTime;

	int iSendCount;
};


/************************************************************************/
/* JsonTool����                                                         */
/************************************************************************/
class JsonTool : public QObject
{
	Q_OBJECT
private:
	/*����*/
	JsonTool(QObject* parent = nullptr);

	/*����*/
	~JsonTool();

	/*���������Ϣ*/
	QString m_lastError = "No Error";

	/*��ָ̬��*/
	static JsonTool* m_self;

	/*�豸���ö���*/
	QJsonObject m_deviceConfigObj;

	/*Ӳ�����ö���*/
	QJsonObject m_hardwareConfigObj;

	/*�̵���IO����*/
	QJsonObject m_relayConfigObj;

	/*��Χ����*/
	QJsonObject m_rangeConfigObj;

	/*�û����ö���*/
	QJsonObject m_userConfigObj;

	/*ͼ�����ö���*/
	QJsonObject m_imageConfigObj;

	/*��ֵ���ö���*/
	QJsonObject m_thresholdConfigObj;

	/*�������ö���*/
	QJsonObject m_enableConfigObj;

	/*Ĭ������*/
	DefConfig m_defConfig;

	/*Ӳ�����ṹ��*/
	HwdConfig m_hwdConfig = { 0 };

	/*��ѹ����*/
	QJsonObject m_voltageConfigObj;

	/*��������*/
	QJsonObject m_keyVolConfigObj;

	/*��������*/
	QJsonObject m_currentConfigObj;

	/*��̬��������*/
	QJsonObject m_staticConfigObj;

	/*��������*/
	QJsonObject m_resConfigObj;

	/*UDS�ṹ��*/
	UdsConfig m_udsConfig = { 0 };

	/*�汾����*/
	QJsonObject m_verConfigObj;

	/*DTC����*/
	QJsonObject m_dtcConfigObj;

	/*CanSender����*/
	QJsonObject m_canMsgObj;

	CanMsg m_canMsg[MAX_MSG_COUNT] = { 0 };
protected:
	/*��ѹ*/
	/*�豸���ü��б�*/
	QStringList m_deviceConfigKeyList = {
		"��������",
		"UDS����",//0
		"CAN����",//1
		"�ɼ�������",
		"�������",
		"�����ж�",
		"���볤��"
	};

	/*�豸����ֵ�б�*/
	QStringList m_deviceConfigValueList = {
		"δ֪",
		"GuangQiA56",//0
		"ZLG",//1
		"MV800",
		"����",
		"ABC",
		"6"
	};

	/*Ӳ�����ü��б�*/
	QStringList m_hardwareConfigKeyList{
		"��Դ����",//2
		"��Դ������",//3
		"��Դ��ѹ",//4
		"�̵�������",//6
		"�̵���������",//7
		"��ѹ����",//8
		"��ѹ������",//9
		"��������",//10
		"����������",//11
		"��չ����1",//12
		"��չ������1",//13
		"��չ����2",//14
		"��չ������2",//15
		"��չ����3",//16
		"��չ������3",//17
		"��չ����4",
		"��չ������4"
	};

	/*Ӳ������ֵ�б�*/
	QStringList m_hardwareConfigValueList{
		"4",//2
		"19200",//3
		"12",//4
		"5",//6
		"19200",//7
		"2",//8
		"9600",//9	
		"3",//10
		"9600",//11
		"6",//12
		"9600",//13
		"7",//14
		"9600",//15
		"8",//16
		"9600",//17
		"9",
		"9600"
	};

	/*�̵���IO���ü��б�*/
	QStringList m_relayConfigKeyList = {
		"GND",//10
		"ACC",//11
		"��̬������",//12
		"Ӳ����",//13
		"ת�Ӱ�",//14
		"�����",
		"����"
	};

	/*�̵���IO����ֵ�б�*/
	QStringList m_relayConfigValueList = {
		"0",//10
		"2",//11
		"1",//12
		"3",//13
		"4",//14
		"6",
		"7"
	};

	/*��Χ���ü��б�*/
	QStringList m_rangeConfigKeyList = {
		"����",//1
		"����X����",//2
		"����Y����",//3
		"����Ƕ�",//4
		"�����"//5
	};

	/*��Χ����ֵ�б�*/
	QStringList m_rangeConfigValueList = {
		"2.00~5.00",//1
		"800.0~1100.0",//2
		"400.0~600.0",//3
		"5.0~20.0",//4
		"0.0~1000.0"//5
	};

	/*�û����ü��б�*/
	/*ROOT�û�Ȩ��0,���ڴ˳��򿪷���*/
	/*INVO�û�Ȩ��1,�����ֳ�������*/
	/*TEST�û�Ȩ��2,������ҵԱ*/

	QStringList m_userConfigKeyList = {
		"�û���",
		"����"
	};

	/*�û�����ֵ�б�*/
	QStringList m_userConfigValueList = {
		"INVO",
		"1."
	};

	/*��ͼ����б�*/
	QStringList m_parentImageKeyList = {
		"ǰСͼ���ο�",
		"��Сͼ���ο�",
		"��Сͼ���ο�",
		"��Сͼ���ο�",
		"ǰ��ͼ���ο�",
		"���ͼ���ο�",
		"�������״̬"
	};

	/*��ͼ����б�*/
	QStringList m_childImageKeyList[IMAGE_CHECK_COUNT] = {
		QStringList{"��ɫ",("R"),("G"),("B"),("���"),("X����"),("Y����"),("��"),("��")},
		QStringList{"��ɫ",("R"),("G"),("B"),("���"),("X����"),("Y����"),("��"),("��")},
		QStringList{"��ɫ",("R"),("G"),("B"),("���"),("X����"),("Y����"),("��"),("��")},
		QStringList{"��ɫ",("R"),("G"),("B"),("���"),("X����"),("Y����"),("��"),("��")},
		QStringList{"��ɫ",("R"),("G"),("B"),("���"),("X����"),("Y����"),("��"),("��")},
		QStringList{"��ɫ",("R"),("G"),("B"),("���"),("X����"),("Y����"),("��"),("��")},
		QStringList{"����RGB","��ʾСͼ","��ʾ��ͼ","������־"}
	};

	/*��ͼ��ֵ�б�*/
	QStringList m_childImageValueList[IMAGE_CHECK_COUNT] = {
		QStringList{"!=��ɫ",("201"),("212"),("85"),("100"),("55"),("20"),("50"),("80")},
		QStringList{"!=��ɫ",("255"),("255"),("255"),("100"),("75"),("380"),("40"),("60")},
		QStringList{"!=��ɫ",("176"),("58"),("177"),("100"),("5"),("100"),("40"),("180")},
		QStringList{"!=��ɫ",("164"),("78"),("7"),("100"),("120"),("130"),("40"),("180")},
		QStringList{"!=��ɫ",("153"),("212"),("81"),("100"),("320"),("80"),("110"),("250")},
		QStringList{"!=��ɫ",("113"),("50"),("34"),("100"),("300"),("50"),("110"),("250")},
		QStringList{"1","0","0","0"}
	};

	int m_childImageSubscript = 0;

	/*��ֵ���б�*/
	QStringList m_thresholdKeyList =
	{
		"CAN���ѵ���",
		"CAN���ߵ���"
	};

	/*��ֵֵ�б�*/
	QStringList m_thresholdValueList = {
		"0.3",
		"0.005"
	};

	/*�������ü��б�*/
	QStringList m_enableConfigKeyList = {
		"�����Ի���",//1
		"����Ի���",//2
		"����CAN��־",//3
		"����ʧ��",//4
		"���������־",
		"����������־"//5
	};

	/*��������ֵ�б�*/
	QStringList m_enableConfigValueList = {
		"0",//1
		"1",//2
		"0",//3
		"0",//4
		"0",//5
		"0"
	};
protected:
	/*���ô���*/
	void setLastError(const QString& err);

	/*������Χ*/
	bool parseRangeValue(const QString& value, float& min, float& max);

	/*�����豸��������*/
	bool parseDeviceConfigData();

	/*����Ӳ����������*/
	bool parseHardwareConfigData();

	/*�����̵���IO������*/
	bool parseRelayPortConfigData();

	/*�����û�����*/
	bool parseUserConfigData();

	/*����ͼ������������*/
	bool parseImageConfigData();

	/*������Χ��������*/
	bool parseRangeConfigData();

	/*������ֵ��������*/
	bool parseThresholdConfigData();

	/*����������������*/
	bool parseEnableConfigData();

	/************************************************************************/
	/* Ӳ���������                                                         */
	/************************************************************************/
	/*������ѹ����*/
	bool parseVoltageConfigData();

	/*����LED����*/
	bool parseKeyVolConfigData();

	/*������������*/
	bool parseCurrentConfigData();

	/*������̬��������*/
	bool parseStaticConfigData();

	/*������������*/
	bool parseResConfigData();

	/************************************************************************/
	/* UDS�������                                                          */
	/************************************************************************/
	/*�����汾����*/
	bool parseVerConfigData();

	/*����DTC����*/
	bool parseDtcConfigData();

	/*DTC����ת��*/
	const QString dtcCategoryConvert(const QString& dtc);

	/************************************************************************/
	/* CanSender����                                                        */
	/************************************************************************/

	bool parseCanMsgData();
public:
	/************************************************************************/
	/* ��������                                                             */
	/************************************************************************/
	/*��������ɾ��*/
	JsonTool(const JsonTool&) = delete;

	/*��ֵ����ɾ��*/
	JsonTool& operator=(const JsonTool&) = delete;

	/*��ȡ����*/
	static JsonTool* getInstance();

	/*ɾ������*/
	static void deleteInstance();

	/*��ȡ������Ϣ*/
	const QString& getLastError();

	/*��ʼ��ʵ��*/
	bool initInstance(bool update = false, const QString& folderName = "Config", const QStringList& fileName = { "def.json","hwd.json","uds.json","can.json" });

	/*��ȡ��������*/
	const QStringList getAllMainKey();

	/************************************************************************/
	/*��д�����ļ�����                                                      */
	/************************************************************************/

	/************************************************************************/
	/* DEF                                                                  */
	/************************************************************************/
	/*��ȡjson�����ļ�*/
	bool readDefJsonFile(const QString& name = QString("def.json"));

	/*д��Ĭ��json�����ļ�*/
	bool writeDefJsonFile(const QString& name = QString("def.json"));

	/*����Ĭ��json�����ļ�*/
	bool updateDefJsonFile(const QString& name = QString("def.json"));

	/************************************************************************/
	/* HWD                                                                  */
	/************************************************************************/
	/*��ȡӲ��json�����ļ�*/
	bool readHwdJsonFile(const QString& name = QString("hwd.json"));

	/*д��Ӳ��json�����ļ�*/
	bool writeHwdJsonFile(const QString& name = QString("hwd.json"));

	/*����Ӳ��json�����ļ�*/
	bool updateHwdJsonFile(const QString& name = QString("hwd.json"));

	/************************************************************************/
	/* UDS                                                                  */
	/************************************************************************/
	/*��ȡuds json �����ļ�*/
	bool readUdsJsonFile(const QString& name = QString("uds.json"));

	/*д��uds json �����ļ�*/
	bool writeUdsJsonFile(const QString& name = QString("uds.json"));

	/*����uds json �����ļ�*/
	bool updateUdsJsonFile(const QString& name = QString("uds.json"));

	/************************************************************************/
	/* CAN                                                                  */
	/************************************************************************/
	/*��ȡcan json �����ļ�*/
	bool readCanJsonFile(const QString& name = QString("can.json"));

	/*д��can json �����ļ�*/
	bool writeCanJsonFile(const QString& name = QString("can.json"));

	/*����can json �����ļ�*/
	bool updateCanJsonFile(const QString& name = QString("can.json"));

	/************************************************************************/
	/* �豸���ò���                                                         */
	/************************************************************************/

	/*��ȡ�豸����Ԫ������*/
	const int getDeviceConfigCount();

	/*ͨ������ȡ�豸����ֵ*/
	const QString getDeviceConfigValue(const QString& key);

	/*��ȡ�豸�����*/
	const QStringList getDeviceConfigKeyList();

	/*��ȡ�ѽ������豸����*/
	const deviceConfig_t& getParsedDeviceConfig();

	/*��ȡJson����*/
	const QJsonObject& getDeviceConfigObj();

	/*�����豸����ֵ*/
	bool setDeviceConfigValue(const QString& key, const QString& value);

	/*��ȡ�豸˵��*/
	const QStringList getDeviceConfigExplain();
	/************************************************************************/
	/* Ӳ�����ò���                                                         */
	/************************************************************************/

	/*ͨ������ȡӲ������ֵ*/
	const QString getHardwareConfigValue(const QString& key);

	/*��ȡӲ������Ԫ������*/
	const int getHardwareConfigCount();

	/*��ȡӲ�������*/
	const QStringList getHardwareConfigKeyList();

	/*��ȡ�ѽ�����Ӳ������*/
	const hardwareConfig_t& getParseHardwareConfig();

	/*����Ӳ������ֵ*/
	bool setHardwareConfigValue(const QString& key, const QString& value);

	/*��ȡӲ��˵��*/
	const QStringList getHardwareConfigExplain();
	/************************************************************************/
	/* �̵������ò���                                                       */
	/************************************************************************/
	/*ͨ������ȡ�̵���IO����ֵ*/
	const QString getRelayConfigValue(const QString& key);

	/*��ȡ�̵�������Ԫ������*/
	const int getRelayConfigCount();

	/*��ȡ�ѽ����ļ̵���IO����*/
	const relayConfig_t& getParsedRelayConfig();

	/*��ȡ�̵��������*/
	const QStringList getRelayConfigKeyList();

	/*���ü̵�������ֵ*/
	bool setRelayConfigValue(const QString& key, const QString& value);

	/*��ȡ�̵���˵��*/
	const QStringList getRelayConfigExplain();
	/************************************************************************/
	/* �û����ò���                                                         */
	/************************************************************************/
	/*ͨ������ȡ�û�����ֵ*/
	const QString getUserConfigValue(const QString& key);

	/*��ȡ�û���������*/
	const int getUserConfigCount();

	void setUserConfigValue(const QString& key, const QString& value);
	/************************************************************************/
	/* ��Χ���ò���                                                         */
	/************************************************************************/
	/*ͨ������ȡ��Χ����ֵ*/
	const QString getRangeConfigValue(const QString& key);

	/*��ȡ��Χ����Ԫ������*/
	const int getRangeConfigCount();

	/*��ȡ��Χ��*/
	const QStringList getRangeConfigKeyList();

	/*��ȡ�ѽ����ķ�Χ����*/
	const rangeConfig_t& getParsedRangeConfig();

	/*���÷�Χ����ֵ*/
	bool setRangeConfigValue(const QString& key, const QString& value);

	/*��ȡ��Χ����˵��*/
	const QStringList getRangeConfigExplain();
	/************************************************************************/
	/* ��ֵ���ò���                                                         */
	/************************************************************************/
	/*ͨ������ȡ��ֵ����ֵ*/
	const QString getThresholdConfigValue(const QString& key);

	/*��ȡ��ֵ����Ԫ������*/
	const int getThresholdConfigCount();

	/*��ֵ��*/
	const QStringList getThresholdConfigKeyList();

	/*��ȡ�ѽ�������ֵ����*/
	const thresholdConfig_t& getParsedThresholdConfig();

	/*������ֵ����ֵ*/
	bool setThresholdConfigValue(const QString& key, const QString& value);

	/*��ȡ��ֵ����˵��*/
	const QStringList getThresholdConfigExplain();
	/************************************************************************/
	/* ͼ�����ò���                                                         */
	/************************************************************************/
	/*��ȡ�ѽ�����ͼ������*/
	const imageConfig_t& getParsedImageConfig();

	/*��ȡ��ͼ����������*/
	const int getImageConfigCount();

	/*��ȡ��ͼ����б�*/
	const QStringList getParentImageKeyList();

	void setChildImageKeyListSubscript(const int& subscript);

	/*��ȡ��ͼ����б�*/
	const QStringList getChildImageKeyList(const int& id);

	const QStringList getChildImageKeyList();

	/*��ȡ��ͼ������ֵ*/
	const QString getImageConfigValue(const QString& parentKey, const QString& childKey);

	/*����������ͼ�����ü�*/
	inline void setImageConfigKey(const QString& oldParentKey, const QString& newParentKey) {};

	/*����ͼ������ֵ*/
	bool setImageConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡͼ������˵��*/
	const QStringList getImageConfigExplain(const int& i);

	const QStringList getImageConfigExplain();
	/************************************************************************/
	/* �������ò���                                                         */
	/************************************************************************/
	/*��ȡ������������*/
	const int getEnableConfigCount();

	/*��ȡ�������ü��б�*/
	const QStringList getEnableConfigKeyList();

	/*��ȡ��������ֵ�б�*/
	const QStringList getEnableConfigValueList();

	/*��ȡ��������ֵ*/
	const QString getEnableConfigValue(const QString& key);

	/*������������ֵ*/
	bool setEnableConfigValue(const QString& key, const QString& value);

	/*��ȡ��������˵��*/
	const QStringList getEnableConfigExplain();

	/************************************************************************/
	/* ��ȡ����������                                                       */
	/************************************************************************/
	DefConfig* getParsedDefConfig();

	/************************************************************************/
	/* ��ѹ���ò���                                                         */
	/************************************************************************/
	/*��ȡ��ѹ��������*/
	const int getVoltageConfigCount();

	/*��ȡ�ӵ�ѹ���ü�*/
	const QStringList getChildVoltageConfigKeyList();

	/*��ȡ�ӵ�ѹ����Ĭ��ֵ*/
	const QStringList getChildVoltageConfigValueList();

	/*��ȡ����ѹ���ü�*/
	const QStringList getParentVoltageConfigKeyList();

	/*��ȡ��ѹ����ֵ*/
	const QString getVoltageConfigValue(const QString& parentKey, const QString& childKey);
	
	/*���õ�ѹ���ü�*/
	void setVoltageConfigKey(const QString& oldParentKey, const QString& newParentKey);

	/*���õ�ѹ����ֵ*/
	bool setVoltageConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ��ѹJSON����*/
	QJsonObject& getVoltageConfigObj();

	/*��ȡ��ѹ����˵��*/
	const QStringList getVoltageConfigExplain();

	/************************************************************************/
	/* ������ѹ���ò���                                                     */
	/************************************************************************/
	/*��ȡ������ѹ����*/
	const int getKeyVolConfigCount();

	/*��ȡ������ѹ���ü�*/
	const QStringList getKeyVolConfigKeyList();

	/*��ȡ������ѹ����ֵ*/
	const QStringList getKeyVolConfigValueList();

	/*��ȡ������ѹ����ֵ*/
	const QString getKeyVolConfigValue(const QString& key);

	/*���ð�����ѹֵ*/
	bool setKeyVolConfigValue(const QString& key, const QString& value);

	/*��ȡ������ѹ����˵��*/
	const QStringList getKeyVolConfigExplain();

	/*������ѹ*/
	/************************************************************************/
	/* �������ò���                                                         */
	/************************************************************************/
	/*��ȡ���������ü��б�*/
	const QStringList getParentCurrentConfigKeyList();

	/*��ȡ�ӵ������ü��б�*/
	const QStringList getChildCurrentConfigKeyList();
	
	/*��ȡ��������ֵ�б�*/
	const QStringList getChildCurrentConfigValueList();

	/*��ȡ��������ֵ�б�*/
	const QStringList getChildCurrentConfigValueList(const int& i);

	/*��ȡ������������*/
	const int getCurrentConfigCount();

	/*��ȡ��������ֵ*/
	const QString getCurrentConfigValue(const QString& parentKey, const QString& childKey);

	/*���õ������ü�*/
	void setCurrentConfigKey(const QString& oldParentKey, const QString& newParentKey);

	/*���õ�������ֵ*/
	bool setCurrentConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ����JSON���ö���*/
	QJsonObject& getCurrentConfigObj();

	/*��ȡ��������˵��*/
	const QStringList getCurrentConfigExplain();
	/************************************************************************/
	/* ��̬�������ò���                                                     */
	/************************************************************************/

	/*��ȡ��̬��������*/
	const int getStaticConfigCount();

	/*��̬�������ü��б�*/
	const QStringList getStaticConfigKeyList();

	/*��̬��������ֵ�б�*/
	const QStringList getStaticConfigValueList();

	/*��ȡ��̬������ֵ*/
	const QString getStaticConfigValue(const QString& key);

	/*���þ�̬��������ֵ*/
	bool setStaticConfigValue(const QString& key, const QString& value);

	/*��ȡ��̬����˵��*/
	const QStringList getStaticConfigExplain();

	/************************************************************************/
	/* �������ò���                                                         */
	/************************************************************************/
	/*��ȡ���������ü�*/
	const QStringList getParentResConfigKeyList();

	/*��ȡ�ӵ������ü�*/
	const QStringList getChildResConfigKeyList();

	/*��ȡ��������ֵ*/
	const QStringList getChildResConfigValueList();

	/*��ȡ������������*/
	const int getResConfigCount();

	/*��ȡ��������ֵ*/
	const QString getResConfigValue(const QString& parentKey, const QString& childKey);

	/*���õ������ü�*/
	void setResConfigKey(const QString& oldParentKey, const QString& newParentKey);

	/*���õ�������ֵ*/
	bool setResConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ����JSON���ö���*/
	QJsonObject& getResConfigObj();

	/*��ȡ��������˵��*/
	const QStringList getResConfigExplain();

	/************************************************************************/
	/* ��ȡ����������                                                       */
	/************************************************************************/
	/*��ȡ�ѽ�����Ӳ�������Ϣ����*/
	HwdConfig* getParsedHwdConfig();

	/************************************************************************/
	/* �汾���ò���                                                          */
	/************************************************************************/
	/*��ȡ�汾��������*/
	const int getVerConfigCount();

	/*��ȡ���汾���ü�*/
	const QStringList getParentVerConfigKeyList();

	/*��ȡ�Ӱ汾���ü�*/
	const QStringList getChildVerConfigKeyList();

	/*��ȡ�Ӱ汾����ֵ*/
	const QStringList getChildVerConfigValueList();

	/*��ȡ�汾����ֵ*/
	const QString getVerConfigValue(const QString& parentKey, const QString& childKey);

	/*���ð汾���ü�*/
	void setVerConfigKey(const QString& oldParentKey, const QString& newParentKey);

	/*���ð汾����ֵ*/
	bool setVerConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ�汾JSON���ö���*/
	QJsonObject& getVerConfigObj();

	/*��ȡ�汾����˵��*/
	const QStringList getVerConfigExplain();

	/************************************************************************/
	/* ��Ϲ��������ò���                                                   */
	/************************************************************************/
	/*��ȡ��������������*/
	const int getDtcConfigCount();

	/*��ȡ���������*/
	const QStringList getParentDtcConfigKeyList();

	/*��ȡ�ӹ������*/
	const QStringList getChildDtcConfigKeyList();

	/*��ȡ�ӹ�����ֵ*/
	const QStringList getChildDtcConfigValueList();

	/*��ȡ������ֵ*/
	const QString getDtcConfigValue(const QString& parentKey, const QString& childKey);

	/*������Ϲ��������ü�*/
	void setDtcConfigKey(const QString& oldParentKey, const QString& newParentKey);

	/*������Ϲ���������ֵ*/
	bool setDtcConfigValue(const QString& parentKey, const QString& childKey, const QString& value);
	
	/*��ȡ��Ϲ�����JSON���ö���*/
	QJsonObject& getDtcConfigObj();

	/*��ȡ��Ϲ�����˵��*/
	const QStringList getDtcConfigExplain();


	/************************************************************************/
	/* ��ȡ����������                                                       */
	/************************************************************************/
	/*��ȡ�ѽ�����UDS��Ϣ����*/
	UdsConfig* getParsedUdsConfig();

	/************************************************************************/
	/* CanSender����,������ΪԤ������,����ʹ����������                      */
	/************************************************************************/
	/*��ȡCAN��������*/
	const int getCanMsgCount();

	/*��ȡCAN���ļ��б�*/
	const QStringList getCanMsgKeyList();

	/*��ȡCAN����ֵ*/
	const QString getCanMsgValue(const QString& parentKey, const QString& childKey);

	/*����CAN���ļ�*/
	void setCanMsgKey(const QString& oldParentKey, const QString& newParentKey);

	/*����CAN����ֵ*/
	void setCanMsgValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ�ѽ�����CAN����*/
	const CanMsg* getParsedCanMsg();

	/*��ȡCAN����JSON���ö���*/
	QJsonObject& getCanMsgObj();
};

