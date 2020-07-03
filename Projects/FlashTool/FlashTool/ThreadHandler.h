#pragma once
#pragma execution_character_set("utf-8")

/************************************************************************/
/* �̹߳�����͹����߳���                                               */
/* WorkThread��̬����,������¼BIN�ļ�,ThreadHandler��������ʼ�ͽ���	*/
/************************************************************************/

#include <QThread>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <ItechSCPIMgr/ItechSCPIMgr.h>
#include <ItechSCPIMgr6302/ItechSCPIMgr6302.h>
#include <MR-DO16-KNMgr/MR-DO16-KNMgr.h>
#include <AardvarkMgr/AardvarkMgr.h>
#include <QMutex>
#include <QTimer>
#include <QDateTime>
#include "JsonTool.h"
#include "Isp.h"

#pragma comment(lib,"MR-DO16-KNMgr.lib")
#pragma comment(lib,"ItechSCPIMgr.lib")
#pragma comment(lib,"ItechSCPIMgr6302.lib")
#pragma comment(lib,"AardvarkMgr.lib")

/*����ת��Lib����������*/
#define From8Bit(x) QString::fromLocal8Bit(x)
#define OK_NG(X) X ? "OK" :"NG"
#define Q_SPRINTF(format,...) QString().sprintf(format,__VA_ARGS__)

/************************************************************************/
/* ��¼״̬                                                             */
/************************************************************************/
typedef enum class BurnStatus
{
	/*д��״̬*/
	BS_WR,

	/*��ȡ״̬*/
	BS_RD,

	/*����״̬*/
	BS_OK,

	/*����״̬*/
	BS_NG,

	/*��״̬*/
	BS_NONE
}burnStatus_t;

/************************************************************************/
/* ��¼˳��                                                             */
/************************************************************************/
typedef enum class BurnSequence
{
	/*ɨ��*/
	BS_SCAN_CODE,

	/*׼����¼*/
	BS_PREP_BURN,

	/*��ʼ����ַ*/
	BS_INIT_ADDR,

	/*��ȡ����*/
	BS_READ_COOR,

	/*�޸�BIN�ļ�*/
	BS_ALTER_BIN_FILE,

	/*д��BIN�ļ�*/
	BS_WRITE_BIN_FILE,

	/*������Դ*/
	BS_RESTART_POWER,

	/*У������*/
	BS_CHECK_DATA,

	/*������־*/
	BS_SAVE_LOG,

	/*�ȴ�ͬ��*/
	BS_WAIT_SYNC,

	/*��*/
	BS_NONE
}burnSequence_t;

/************************************************************************/
/* ��¼ģʽ                                                             */
/************************************************************************/
enum BurnMode
{
	BM_ATC_016_SET,//0
	BM_CTC_016_SET,//1
	BM_CTC_019_SET,//2
	BM_EEP_AXS340,//3
	BM_FLASH_AXS340,//4
	BM_NET_AXS340,//5
	BM_CTC_CHANGAN_IMS,//6
	BM_EEP_GEELY_BX11,//7
	BM_CTC_EP30TAP_DMS,//8
	BM_ATC_BYD_OV7958,//9
};

/*019����ͷ��ʱ����*/
#define TEMP_MODE 0
#if TEMP_MODE
static int HOR = 1;
static int VOR = 3;
#else
static int HOR = 4;
static int VOR = 6;
#endif

/*֧����¼���������*/
#define MAX_DEVICE_COUNT 15

/*���������߳�*/
class WorkThread;

/************************************************************************/
/* ThreadHandler��                                                      */
/************************************************************************/
class ThreadHandler : public QThread
{
	Q_OBJECT
public:
	/*��Ԫ*/
	friend class WorkThread;

	/*����*/
	explicit ThreadHandler(QObject* parent = nullptr);

	/*����*/
	~ThreadHandler();

	/*���õ���ģʽ*/
	void enableDebugMode(bool enable = false);
	
	/*��ȡ����ģʽ*/
	bool getDebugMode();

	/*��ʼ��*/
	bool initInstance();

	/*���豸*/
	bool openDevice();

	/*�ر��豸*/
	bool closeDevice();

	/*��������ѡ��,�����ж�ѡ���˼���ͨ��*/
	void setChannelCount(const int& count);

	/*�����߳��˳�*/
	void threadQuit();

	/*���������߳�*/
	bool createWorkThread();

	/*ɾ�������߳�*/
	void deleteWorkThread();

	/*��ȡ�����߳�*/
	const WorkThread* getWorkThread() const;

	/*��ʼ�����߳�*/
	void startWorkThread();

	/*�˳������߳�*/
	void quitWorkThread();

	/************************************************************************/
	/* Aardvark                                                             */
	/************************************************************************/

	/*��ȡ��¼���˿ںź����к�*/
	const QMap<quint8, quint32>& getAardvarkPortAndSn();

	/*�����¼������*/
	const int getAardvarkCount();

	/*������¼���豸*/
	bool loadAardvarkDevice();

	/*��ȡ����*/
	const QString& getLastError();

	/*ɨ��Ի���*/
	bool scanCodeDlg(const int& number);

	/*�������п���*/
	void setRunSwitch(bool on);

	/*��Ϣ�Ի���*/
	void setMessageBox(const QString& title, const QString& text);

	bool isWorkThreadRunning();
	/*�ź�*/
signals:
	void setMessageBoxSignal(const QString& title, const QString& text);

	void scanCodeDlgSignal(const int& number);
protected:
	/*��дrun*/
	virtual void run();

	/*���ô���*/
	void setLastError(const QString& err,bool msgBox = false);

	/*����¼��*/
	bool openAardvarkDevice();

	/*�ر���¼��*/
	bool closeAardvarkDevice();
private:
	/*�Ƿ�Ϊ����ģʽ*/
	bool m_debugMode = false;

	/*�߳��Ƿ��˳�*/
	bool m_quit = false;

	/*�Ƿ�����,���ڹ���*/
	bool m_connect = false;

	/*���߳��Ƿ����ڹ���*/
	bool m_childWork = false;

	/*���߳��Ƿ��˳�*/
	bool m_childQuit = false;

	/*ɨ��Ի�����ʾ*/
	bool m_scDlgShow = false;

	///*���߳���ֹ,���Ѿ�����������¼״̬*/
	//bool m_childAbort = false;

	/*����ͨ����*/
	int m_channelCount = 0;

	/*��¼������*/
	int m_aardvarkCount = 0;

	/*������Ϣ*/
	QString m_lastError = "No Error";

	/*��ͨ����Դ*/
	CItechSCPIMgr m_power6832;

	/*��ͨ����Դ*/
	CItechSCPIMgr6302 m_power6302;

	/*�̵�����װ��*/
	CMRDO16KNMgr m_relay;

	/*�߳���*/
	QMutex m_mutex;

	/*�����߳�*/
	WorkThread* m_workThread = nullptr;

	/*JSON��װ��*/
	JsonTool* m_jsonTool = nullptr;

	/*�豸����*/
	DeviceConfig* m_deviceConfig = nullptr;

	/*Ӳ������*/
	HardwareConfig* m_hardwareConfig = nullptr;

	/*�ļ�����*/
	FileConfig* m_fileConfig = nullptr;

	/*��ֵ����*/
	ThresholdConfig* m_thresholdConfig = nullptr;

	/*�̵�������*/
	RelayConfig* m_relayConfig = nullptr;

	/*������¼���˿ںź����к�*/
	QMap<quint8, quint32> m_aardvarkPortAndSn;
};


/************************************************************************/
/* WorkThread��                                                         */
/************************************************************************/
class WorkThread : public QThread
{
	Q_OBJECT
public:
	/*����*/
	explicit WorkThread(QObject* parent = nullptr);

	/*����*/
	~WorkThread();

	/*��ȡAardvark*/
	AardvarkMgr& getAardvark();

	/*���ø�����*/
	void setParent(QObject* parent);

	/*������¼���˿�*/
	void setAardvarkPort(const int& port);

	/*���õ�Դͨ��*/
	void setPowerChannel(const int& channel);

	/*���ü̵����˿�*/
	void setRelayPort(const int& port);

	/*�������к�*/
	void setAardvarkSn(const quint32& sn);

	/*��ʼ��*/
	bool initInstance();

	/*��ȡ����*/
	const QString& getLastError();

	/************************************************************************/
	/* ��������                                                             */
	/************************************************************************/

	/*�ı�FLASH��¼״̬*/
	bool changeFlashStatus();

	/*��ʼ��������¼*/
	bool initNetworkBurn();

	/*���ƹ���*/
	bool controlPower(bool powerSwitch);

	/*����У��bin�ļ�*/
	bool saveBinFile(const QString& name, const char* data, const ulong& size);

	/************************************************************************/
	/* ��¼˳��                                                             */
	/************************************************************************/

	/*׼����¼*/
	bool prepareBurn();

	/*��ʼ����ַ*/
	bool initAddress(bool mask = false);

	/*��ȡ����*/
	bool readCoordinate();

	/*дBIN�ļ�*/
	bool alterBinFile();

	/*д������*/
	bool writeBinFile();

	/*������Դ*/
	bool restartPower();

	/*У������*/
	bool checkData();

	/*������־*/
	bool saveLog(bool success);

	/*�ȴ�ͬ��*/
	bool waitSync(bool success);

	/************************************************************************/
	/* ��ȡʱ������                                                         */
	/************************************************************************/

	/*��ȡ��ǰʱ��*/
	const QString getTime(bool fileFormat = true);
	
	/*��ȡ��ǰ����*/
	const QString getDate();
	
	/*��ȡ��ǰ����ʱ��*/
	const QString getDateTime(bool fileFormat = true);

	/************************************************************************/
	/* ����GUI�߳�                                                           */
	/************************************************************************/

	/*���½�����*/
	void updateProgress(const int& progress);

	/*���µ�ǰ״̬*/
	void updateCurrentStatus(const QString& status);

	/*������¼״̬*/
	void updateBurnStatus(const burnStatus_t& status, const QString& err = QString());

	/*���������*/
	void updateGroupTitle(const QString& title);

	/*���ö�ʱ������*/
	void setBurnTimerRun(bool go);

	/*������¼˳��*/
	void setBurnSequence(const burnSequence_t& sequence);

	const QString getAardvarkError();
	/*�ź�[ͬ��]*/
signals:
	void updateProgressSignal(const int& progress);

	void updateCurrentStatusSiganl(const QString& status);

	void updateBurnStatusSiganl(const burnStatus_t& status, const QString& err);

	void updateGroupTitleSignal(const QString& title);

	void setBurnTimerRunSignal(bool run);
	/*��*/
public slots:
	void progressTimerSlot();

	void getBurnTimerTimeSlot(const int& data);
protected:
	/*��дrun*/
	virtual void run();

	/*���ô���*/
	void setLastError(const QString& err);
private:
	JsonTool* m_jsonTool = nullptr;

	/*�ļ�����*/
	FileConfig* m_fileConfig = nullptr;

	/*�豸����*/
	DeviceConfig* m_deviceConfig = nullptr;

	/*Ӳ������*/
	HardwareConfig* m_hardwareConfig = nullptr;

	/*��ֵ����*/
	ThresholdConfig* m_thresholdConfig = nullptr;

	/*��Դͨ��*/
	int m_powerChannel = 0;

	/*�̵���IO��*/
	int m_relayPort = 0;

	/*��¼��IO��*/
	int m_aardvarkPort = 0;

	/*���ڷ���ThreadHandler��*/
	ThreadHandler* m_threadHandler = nullptr;

	/*��¼˳��*/
	burnSequence_t m_testSequence = BurnSequence::BS_NONE;

	/*��¼������*/
	AardvarkMgr m_aardvark;

	/*������Ϣ*/
	QString m_lastError = "No Error";

	/*��������*/
	uchar m_coordData[8];

	/*���½�������ʱ��*/
	QTimer* m_progressTimer = nullptr;

	/*���½���������*/
	int m_progressData[3];

	/*��¼��ʱ��ʱ��*/
	int m_burnTimerTime = 0;

	/*�Ƿ�ʼ����*/
	bool m_startWork = false;

	/*��¼�����к�*/
	quint32 m_aardvarkSn = 0;

	/*�Ƿ��������µ��*/
	bool m_restartPower = false;

	/*A12������¼*/
	IspTool* m_ispTool = nullptr;
};