#pragma once

/************************************************************************/
/* ���ļ����ڹ������������ļ���д�޸�                               */
/************************************************************************/
#pragma execution_character_set("utf-8")
#include <functional>
#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <memory>

#define GET_JSON() JsonTool::getInstance()

#define RUN_BREAK(success,error) \
if ((success))\
{\
	setLastError(error);\
	break;\
}

//��ܰ汾��
#define LIB_VERSION "1.0.0.9"

//DCF�ļ��汾��
#define DCF_VERSION "1.0.0.0"

//JSON�ļ��汾��
#define JSON_VERSION "1.0.0.5"

////������ѯ��վ�������
//#define SKIP_QS_SYMBOL "$^"
//
////����������к��������
//#define SKIP_SN_SYMBOL "&^"
//
////������������������
//#define SKIP_DATE_SYMBOL "@^"

//��Ƶ�ؼ����
#define VIDEO_WIDGET_WIDTH 720

//��Ƶ�ؼ��߶�
#define VIDEO_WIDGET_HEIGHT 480

//QStringתchar*
#define Q_TO_C_STR(X) X.toStdString().c_str()

//���״�new
#define NO_THROW_NEW new(std::nothrow)

//��ȫɾ������
#define SAFE_DELETE_A(X)\
if (X)\
{\
	delete[] X;\
	X = nullptr;\
}

//��ȫɾ������
#define SAFE_DELETE(X)\
if (X)\
{\
	delete X;\
	X = nullptr;\
}

//Ŀǰ֧�ֵĲɼ���

//MV800�ɼ�������
#define MV800_CC "MV800"

//MOR�ɼ�������
#define MOR_CC "MOR"

//�κβɼ���
#define ANY_CC "ANY"

//����������
enum SkipItem {
	//���������ж�
	SI_JC,//JC[JUDGE CODE]
	
	//������ѯ��վ
	SI_QS,//QS[QUERY STATION]

	//�������к�
	SI_SN,

	//��������
	SI_DATE
};

/************************************************************************/
/* �豸����														*/
/************************************************************************/
struct DeviceConfig
{
	/*������*/
	QString modelName;

	/*UDS����*/
	QString udsName;

	/*CAN����*/
	QString canName;

	/*CAN������*/
	QString canBaudrate;

	/*CAN��չ֡*/
	QString canExtFrame;

	/*�ɼ�������*/
	QString cardName;

	/*�ɼ���ͨ����*/
	QString cardChannelCount;

	/*�ɼ���ͨ����*/
	QString cardChannelId;

	/*�����ж�*/
	QString codeJudge;

	/*���볤��*/
	QString codeLength;
};

/************************************************************************/
/* Ӳ������                                                             */
/************************************************************************/
struct HardwareConfig
{
	/*��Դ���ں�*/
	int powerPort;

	/*��Դ������*/
	int powerBaud;

	/*��Դ��ѹ*/
	float powerVoltage;

	/*��Դ����*/
	float powerCurrent;

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
};

/************************************************************************/
/* �̵���IO����������JSON                                               */
/************************************************************************/
struct RelayConfig {
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

	/*�źŵư�*/
	int white;

	/*�źŵƺ�*/
	int red;

	/*�źŵ���*/
	int green;
};

/************************************************************************/
/* ���ο�ṹ�嶨��,����ǰ����������ͷ���м����ο�                    */
/************************************************************************/
struct RectConfig
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
};

#define SMALL_RECT_  4

#define BIG_RECT_ 4

#define IMAGE_CHECK_COUNT  9

/************************************************************************/
/* ͼ�����ýṹ�嶨��                                                    */
/************************************************************************/
struct ImageConfig
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
	RectConfig smallRect[SMALL_RECT_];

	/*����ο�����*/
	RectConfig bigRect[BIG_RECT_];
};

/************************************************************************/
/* ��Χ���ýṹ�嶨��                                                   */
/************************************************************************/
struct RangeConfig
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

	/*��С����*/
	float minCurrent0;
	float minCurrent1;

	/*������*/
	float maxCurrent0;
	float maxCurrent1;
};

struct ThresholdConfig
{
	/*������ʱ*/
	float startDelay;

	/*CAN���ѵ�����ֵ*/
	float canRouse;

	/*CAN���ߵ�����ֵ*/
	float canSleep;
};

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

	/*�����źŵ�*/
	int signalLight;
	
	//�����ж�
	int codeJudge;

	//��ѯ��վ
	int queryStation;

	//���кŶ�д
	int snReadWrite;

	//���ڶ�д
	int dateReadWrite;
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

struct HwdConfig
{
	VoltageConfig* voltage;
	ResConfig* res;
	CurrentConfig* current;
	StaticConfig staticCurrent;
	KeyVolConfig keyVol;
};

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

struct UdsConfig
{
	VersonConfig* ver;
	DtcConfig* dtc;
};

/************************************************************************/
/* JsonTool����                                                         */
/************************************************************************/
class JsonTool : public QObject
{
	Q_OBJECT
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

	/*��ȡ�����б�*/
	const QStringList& getErrorList();

	/*�����ļ�������*/
	void setFolderName(const QString& name);

	/*��ʼ��ʵ��*/
	bool initInstance(bool update = false, const QString& folderName = "Config",
		const QStringList& fileName = { "def.json","hwd.json","uds.json","img.json","oth.json" });

	/*��ȡ��������*/
	QStringList getAllMainKey();

	/*��ȡ��汾*/
	static QString getLibVersion();

	/*��ȡJSON�ļ��汾*/
	static QString getJsonVersion();

	/*��ȡDCF�ļ��汾*/
	static QString getDcfVersion();

	/************************************************************************/
	/*��д�����ļ�����                                                      */
	/************************************************************************/

	bool writeDcfFile(const QString& name);

	bool readJsonFile(const QString& name, QJsonObject& rootObj);

	bool writeJsonFile(const QString& name, const QJsonObject& rootObj);

	/************************************************************************/
	/* DEF                                                                  */
	/************************************************************************/

	/*��ȡjson�����ļ�*/
	bool testDefJsonFile(const QString& name = "def.json");

	/*��ȡjson�����ļ�*/
	bool readDefJsonFile(const QString& name = "def.json");

	/*д��Ĭ��json�����ļ�*/
	bool writeDefJsonFile(const QString& name = "def.json");

	/*����Ĭ��json�����ļ�*/
	bool updateDefJsonFile(const QString& name = "def.json");

	/************************************************************************/
	/* HWD                                                                  */
	/************************************************************************/
	/*��ȡӲ��json�����ļ�*/
	bool readHwdJsonFile(const QString& name = "hwd.json");

	/*д��Ӳ��json�����ļ�*/
	bool writeHwdJsonFile(const QString& name = "hwd.json");

	/*����Ӳ��json�����ļ�*/
	bool updateHwdJsonFile(const QString& name = "hwd.json");

	//IMG
	bool testImgJsonFile(const QString& name = "img.json");

	bool readImgJsonFile(const QString& name = "img.json");

	bool writeImgJsonFile(const QString& name = "img.json");

	bool updateImgJsonFile(const QString& name = "img.json");

	/*
	* OTH(other)
	* @notice,�����Ϊ�˸�һЩ������������,��д����ļ�
	* @example,��������Ҫ���һ���������ڲ��汾��,��Ҫ��scp
	* �������ص�����,Ȼ��Ա��ļ��ڲ�������,���Աȵ���ȷ����,
	* ��д����oth.json��.
	*/
	bool readOthJsonFile(const QString& name = "oth.json");

	bool writeOthJsonFile(const QString& name = "oth.json");

	bool updateOthJsonFile(const QString& name = "oth.json");

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
	/* �豸���ò���                                                         */
	/************************************************************************/

	/*��ȡ�豸����Ԫ������*/
	const int getDeviceConfigCount();

	/*ͨ������ȡ�豸����ֵ*/
	const QString getDeviceConfigValue(const QString& key);

	/*��ȡ�豸�����*/
	const QStringList& getDeviceConfigKeyList();

	/*��ȡ�ѽ������豸����*/
	const DeviceConfig& getParsedDeviceConfig();

	/*��ȡJson����*/
	const QJsonObject& getDeviceConfigObj();

	/*�����豸����ֵ*/
	bool setDeviceConfigValue(const QString& key, const QString& value);

	/*��ȡ�豸˵��*/
	const QStringList& getDeviceConfigExplain();

	/*��ȡ�豸Ĭ��ֵ*/
	const QString getDeviceConfigDefaultValue(const QString& key);

	/************************************************************************/
	/* Ӳ�����ò���                                                         */
	/************************************************************************/

	/*ͨ������ȡӲ������ֵ*/
	const QString getHardwareConfigValue(const QString& key);

	/*��ȡӲ������Ԫ������*/
	const int getHardwareConfigCount();

	/*��ȡӲ�������*/
	const QStringList& getHardwareConfigKeyList();

	/*��ȡ�ѽ�����Ӳ������*/
	const HardwareConfig& getParseHardwareConfig();

	/*����Ӳ������ֵ*/
	bool setHardwareConfigValue(const QString& key, const QString& value);

	/*��ȡӲ��˵��*/
	const QStringList& getHardwareConfigExplain();

	/*��ȡӲ��Ĭ��ֵ*/
	const QString getHardwareConfigDefaultValue(const QString& key);

	/************************************************************************/
	/* �̵������ò���                                                       */
	/************************************************************************/

	/*ͨ������ȡ�̵���IO����ֵ*/
	const QString getRelayConfigValue(const QString& key);

	/*��ȡ�̵�������Ԫ������*/
	const int getRelayConfigCount();

	/*��ȡ�ѽ����ļ̵���IO����*/
	const RelayConfig& getParsedRelayConfig();

	/*��ȡ�̵��������*/
	const QStringList& getRelayConfigKeyList();

	/*���ü̵�������ֵ*/
	bool setRelayConfigValue(const QString& key, const QString& value);

	/*��ȡ�̵���˵��*/
	const QStringList& getRelayConfigExplain();

	/*��ȡ�̵���Ĭ��ֵ*/
	const QString getRelayConfigDefaultValue(const QString& key);

	/************************************************************************/
	/* �û����ò���                                                         */
	/************************************************************************/

	/*��ȡ�û����ü��б�*/
	const QStringList& getUserConfigKeyList();

	/*��ȡ�û�����˵��*/
	const QStringList& getUserConfigExplain();

	/*ͨ������ȡ�û�����ֵ*/
	const QString getUserConfigValue(const QString& key);

	/*��ȡ�û���������*/
	const int getUserConfigCount();

	/*�����û�����ֵ*/
	bool setUserConfigValue(const QString& key, const QString& value);

	/*��ȡ�û�Ȩ��*/
	bool getUserPrivileges();

	/*��ȡ�û�Ĭ��ֵ*/
	const QString getUserConfigDefaultValue(const QString& key);

	/************************************************************************/
	/* ��Χ���ò���                                                         */
	/************************************************************************/

	/*ͨ������ȡ��Χ����ֵ*/
	const QString getRangeConfigValue(const QString& key);

	/*��ȡ��Χ����Ԫ������*/
	const int getRangeConfigCount();

	/*��ȡ��Χ��*/
	const QStringList& getRangeConfigKeyList();

	/*��ȡ�ѽ����ķ�Χ����*/
	const RangeConfig& getParsedRangeConfig();

	/*���÷�Χ����ֵ*/
	bool setRangeConfigValue(const QString& key, const QString& value);

	/*��ȡ��Χ����˵��*/
	const QStringList& getRangeConfigExplain();

	/*��ȡ��ΧĬ��ֵ*/
	const QString getRangeConfigDefaultValue(const QString& key);

	/************************************************************************/
	/* ��ֵ���ò���                                                         */
	/************************************************************************/

	/*ͨ������ȡ��ֵ����ֵ*/
	const QString getThresholdConfigValue(const QString& key);

	/*��ȡ��ֵ����Ԫ������*/
	const int getThresholdConfigCount();

	/*��ֵ��*/
	const QStringList& getThresholdConfigKeyList();

	/*��ȡ�ѽ�������ֵ����*/
	const ThresholdConfig& getParsedThresholdConfig();

	/*������ֵ����ֵ*/
	bool setThresholdConfigValue(const QString& key, const QString& value);

	/*��ȡ��ֵ����˵��*/
	const QStringList& getThresholdConfigExplain();

	/*��ȡ��ֵĬ��ֵ*/
	const QString getThresholdConfigDefaultValue(const QString& key);

	/************************************************************************/
	/* ͼ�����ò���                                                         */
	/************************************************************************/

	/*��ȡ�ѽ�����ͼ������*/
	const ImageConfig& getParsedImageConfig();

	/*��ȡ��ͼ����������*/
	const int getImageConfigCount();

	/*��ȡ��ͼ����б�*/
	const QStringList getParentImageKeyList();

	/*������ͼ����б��±�*/
	void setChildImageKeyListSubscript(const int& subscript);

	/*��ȡ��ͼ����б�[����1]*/
	const QStringList getChildImageKeyList(const int& id);

	/*��ȡ��ͼ����б�[����2]*/
	const QStringList& getChildImageKeyList();

	/*��ȡ��ͼ������ֵ*/
	const QString getImageConfigValue(const QString& parentKey, const QString& childKey);

	/*����������ͼ�����ü�*/
	inline void setImageConfigKey(const QString& oldParentKey, const QString& newParentKey) {};

	/*����ͼ������ֵ*/
	bool setImageConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡͼ������˵��[����1]*/
	const QStringList getImageConfigExplain(const int& i);

	/*��ȡͼ������˵��[����2]*/
	const QStringList& getImageConfigExplain();

	/************************************************************************/
	/* �������ò���                                                         */
	/************************************************************************/
	/*��ȡ������������*/
	const int getEnableConfigCount();

	/*��ȡ�������ü��б�*/
	const QStringList& getEnableConfigKeyList();

	/*��ȡ��������ֵ�б�*/
	const QStringList getEnableConfigValueList();

	/*��ȡ��������ֵ*/
	const QString getEnableConfigValue(const QString& key);

	/*������������ֵ*/
	bool setEnableConfigValue(const QString& key, const QString& value);

	/*��ȡ��������˵��*/
	const QStringList& getEnableConfigExplain();

	/*��ȡ��������Ĭ��ֵ*/
	const QString getEnableConfigDefaultValue(const QString& key);

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
	const QStringList& getChildVoltageConfigKeyList();

	/*��ȡ�ӵ�ѹ����Ĭ��ֵ*/
	const QStringList& getChildVoltageConfigValueList();

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
	const QStringList& getVoltageConfigExplain();

	/************************************************************************/
	/* ������ѹ���ò���                                                     */
	/************************************************************************/
	/*��ȡ������ѹ����*/
	const int getKeyVolConfigCount();

	/*��ȡ������ѹ���ü�*/
	const QStringList& getKeyVolConfigKeyList();

	/*��ȡ������ѹ����ֵ*/
	const QStringList getKeyVolConfigValueList();

	/*��ȡ������ѹ����ֵ*/
	const QString getKeyVolConfigValue(const QString& key);

	/*���ð�����ѹֵ*/
	bool setKeyVolConfigValue(const QString& key, const QString& value);

	/*��ȡ������ѹ����˵��*/
	const QStringList& getKeyVolConfigExplain();

	/************************************************************************/
	/* �������ò���                                                         */
	/************************************************************************/

	/*��ȡ���������ü��б�*/
	const QStringList getParentCurrentConfigKeyList();

	/*��ȡ�ӵ������ü��б�*/
	const QStringList& getChildCurrentConfigKeyList();

	/*��ȡ��������ֵ�б�*/
	const QStringList& getChildCurrentConfigValueList();

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
	const QStringList& getCurrentConfigExplain();

	/************************************************************************/
	/* ��̬�������ò���                                                     */
	/************************************************************************/

	/*��ȡ��̬��������*/
	const int getStaticConfigCount();

	/*��̬�������ü��б�*/
	const QStringList& getStaticConfigKeyList();

	/*��̬��������ֵ�б�*/
	const QStringList getStaticConfigValueList();

	/*��ȡ��̬������ֵ*/
	const QString getStaticConfigValue(const QString& key);

	/*���þ�̬��������ֵ*/
	bool setStaticConfigValue(const QString& key, const QString& value);

	/*��ȡ��̬����˵��*/
	const QStringList& getStaticConfigExplain();

	/************************************************************************/
	/* �������ò���                                                         */
	/************************************************************************/

	/*��ȡ���������ü�*/
	const QStringList getParentResConfigKeyList();

	/*��ȡ�ӵ������ü�*/
	const QStringList& getChildResConfigKeyList();

	/*��ȡ��������ֵ*/
	const QStringList& getChildResConfigValueList();

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
	const QStringList& getResConfigExplain();

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
	const QStringList& getChildVerConfigKeyList();

	/*��ȡ�Ӱ汾����ֵ*/
	const QStringList& getChildVerConfigValueList();

	/*��ȡ�汾����ֵ*/
	const QString getVerConfigValue(const QString& parentKey, const QString& childKey);

	/*���ð汾���ü�*/
	void setVerConfigKey(const QString& oldParentKey, const QString& newParentKey);

	/*���ð汾����ֵ*/
	bool setVerConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ�汾JSON���ö���*/
	QJsonObject& getVerConfigObj();

	/*��ȡ�汾����˵��*/
	const QStringList& getVerConfigExplain();

	/************************************************************************/
	/* ��Ϲ��������ò���                                                   */
	/************************************************************************/

	/*��ȡ��������������*/
	const int getDtcConfigCount();

	/*��ȡ���������*/
	const QStringList getParentDtcConfigKeyList();

	/*��ȡ�ӹ������*/
	const QStringList& getChildDtcConfigKeyList();

	/*��ȡ�ӹ�����ֵ*/
	const QStringList& getChildDtcConfigValueList();

	/*��ȡ������ֵ*/
	const QString getDtcConfigValue(const QString& parentKey, const QString& childKey);

	/*������Ϲ��������ü�*/
	void setDtcConfigKey(const QString& oldParentKey, const QString& newParentKey);

	/*������Ϲ���������ֵ*/
	bool setDtcConfigValue(const QString& parentKey, const QString& childKey, const QString& value);

	/*��ȡ��Ϲ�����JSON���ö���*/
	QJsonObject& getDtcConfigObj();

	/*��ȡ��Ϲ�����˵��*/
	const QStringList& getDtcConfigExplain();

	/************************************************************************/
	/* ��ȡ����������                                                       */
	/************************************************************************/

	/*��ȡ�ѽ�����UDS��Ϣ����*/
	UdsConfig* getParsedUdsConfig();

	/*��ȡ��������ֵ*/
	const QString getOthConfigValue(const QString& key);

	/*��ȡ������������*/
	const int getOthConfigCount();

	/*����������Ŀ*/
	//void setSkipItem(const SkipItem& item, bool skip);

	/*��ȡ������Ŀ*/
	bool getSkipItem(const SkipItem& item);

	/*ɾ����������*/
	//void deleteSkipSymbol(QString& code);

	/*��ȡ��������*/
	//bool getSkipCode();
protected:
	/*���ô���*/
	void setLastError(const QString& error);

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
	QString dtcCategoryConvert(const QString& dtc);

private:
	/*
	* @notice,�˴������Ӽ�/ֵ,�������ṹ������ӱ���,
	* ��ӱ�����ע��,���������Ƿ�һ��,���򽫻ᵼ�³������.
	*/

	/*�豸���ü��б�*/
	QStringList m_deviceConfigKeyList = {
		"��������",
		"UDS����",//0
		"CAN����",//1
		"CAN������",
		"CAN��չ֡",
		"�ɼ�������",
		"�ɼ���ͨ����",
		"�ɼ���ͨ����",
		"�����ж�",
		"���볤��"
	};

	/*�豸����ֵ�б�*/
	QStringList m_deviceConfigValueList = {
		"δ֪",
		"GACA56",//0
		"ZLG",//1
		"500",
		"0",
		"ANY",
		"2",
		"0",
		"NULL",
		"0"
	};

	/*Ӳ�����ü��б�*/
	QStringList m_hardwareConfigKeyList{
		"��Դ����",//2
		"��Դ������",//3
		"��Դ��ѹ",//4
		"��Դ����",
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
		"1",//2
		"19200",//3
		"12.0",//4
		"1.0",
		"2",//6
		"19200",//7
		"3",//8
		"9600",//9	
		"4",//10
		"9600",//11
		"5",//12
		"9600",//13
		"6",//14
		"9600",//15
		"7",//16
		"9600",//17
		"8",
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
		"����",
		"�׵�",
		"���",
		"�̵�"
	};

	/*�̵���IO����ֵ�б�*/
	QStringList m_relayConfigValueList = {
		"0",//10
		"2",//11
		"1",//12
		"3",//13
		"4",//14
		"6",
		"7",
		"15",
		"14",
		"13"
	};

	/*��Χ���ü��б�*/
	QStringList m_rangeConfigKeyList = {
		"����",//1
		"����X����",//2
		"����Y����",//3
		"����Ƕ�",//4
		"�����",//5
		"��С����",//6
		"������"//7
	};

	/*��Χ����ֵ�б�*/
	QStringList m_rangeConfigValueList = {
		"0.0~9999.0",//1
		"-9999.0~9999.0",//2
		"-9999.0~9999.0",//3
		"-9999.0~9999.0",//4
		"-9999.0~9999.0",//5
		"0.0~1000.0",//6
		"0.0~1000.0"//7
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
		"���ͼ���ο�",
		"�Ҵ�ͼ���ο�",
		"�������״̬"
	};

	/*��ͼ����б�*/
	QStringList m_childImageKeyList[IMAGE_CHECK_COUNT] = {
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"��ɫ","R","G","B","���","X����","Y����","��","��"},
		{"����RGB","��ʾСͼ","��ʾ��ͼ","������־"}
	};

	/*��ͼ��ֵ�б�*/
	QStringList m_childImageValueList[IMAGE_CHECK_COUNT] = {
		{"!=��ɫ","201","212","85","100","80","48","64","76"},
		{"!=��ɫ","255","255","255","100","88","352","51","67"},
		{"!=��ɫ","176","58","177","100","4","147","53","159"},
		{"!=��ɫ","164","78","7","100","155","149","60","157"},
		{"!=��ɫ","153","212","81","100","391","130","148","114"},
		{"!=��ɫ","113","50","34","100","391","130","148","114"},
		{"!=��ɫ","100","108","30","18","391","130","148","114"},
		{"!=��ɫ","168","55","66","77","391","130","148","114"},
		{"1","1","1","0"}
	};

	/*��ͼ���±�*/
	int m_childImageSubscript = 0;

	/*��ֵ���б�*/
	QStringList m_thresholdConfigKeyList =
	{
		"������ʱ",
		"CAN���ѵ���",
		"CAN���ߵ���"
	};

	/*��ֵֵ�б�*/
	QStringList m_thresholdConfigValueList = {
		"15000",
		"0.3",
		"0.005"
	};

	/*�������ü��б�*/
	QStringList m_enableConfigKeyList = {
		"�����Ի���",//1
		"����Ի���",//2
		"����CAN��־",//3
		"����ʧ��",//4
		"���������־",//5
		"����������־",//6
		"�źŵ���ʾ",//7
		"�����ж�",//8
		"��ѯ��վ",//9
		"���кŶ�д",//10
		"���ڶ�д"//11
	};

	/*��������ֵ�б�*/
	QStringList m_enableConfigValueList = {
		"0",//1
		"1",//2
		"0",//3
		"0",//4
		"0",//5
		"0",//6
		"0",//7
		"1",//8
		"1",//9
		"1",//10
		"1"//11
	};

	/*����*/
	JsonTool(QObject* parent = nullptr);

	/*����*/
	~JsonTool();

	/*���������Ϣ*/
	QString m_lastError = "No Error";

	/*��������б�*/
	QStringList m_errorList;

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

	/*������Ŀ����*/
	//QVector<bool> m_skipItemVec = QVector<bool>(256, false);

	/*��������*/
	QJsonObject m_othConfigObj;

	//��ʼ���ļ�������
	QString m_folderName;
};

