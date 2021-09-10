#pragma once

/*
* @Detection.h���ɵ���include���ͷ�ļ�,���򽫻ᱨ��
* @notice1,�����Ҫд����,��#include <Function.h>
* @notice2,�����ҪдӲ��,��#include <Hardware.h>
*/

#include "Types.h"

#pragma warning(disable:4838)
#pragma execution_character_set("utf-8")

//��������
extern QString g_code;

/*
* @Cc,�ɼ��������ռ�����
*/
namespace Cc {

	/*
	* @Mv800Proc,MV800�ɼ����ص�����
	*/
	void WINAPI Mv800Proc(const uchar* head, const uchar* bits, LPVOID param);

	/*
	* @Mil,MOR�ɼ���������
	*/
	class Mil;

	/*
	* @CaptureCard,ͨ�òɼ���������
	*/
	class CaptureCard;
}

/*
* @Nt,���紫�������ռ�����
*/
namespace Nt {
	/*
	* @DvrClient,DVR�ͻ���������
	*/
	class DvrClient;

	/*
	* @SfrServer,SFR�����������
	*/
	class SfrServer;
}

/*Detection*/
namespace Dt {
	/************************************************************************/
	/* Base Class                                                           */
	/************************************************************************/

	class Base : public QThread
	{
		Q_OBJECT
	public:
		/*����*/
		explicit Base(QObject* parent = nullptr);

		/*������*/
		virtual ~Base();

		/*��ȡ����*/
		const QString& getLastError();

		/*���ò���˳��*/
		void setTestSequence(int testSequence);

		/*���ü������*/
		void setDetectionType(BaseTypes::DetectionType type);

		/*��ȡ�������*/
		static BaseTypes::DetectionType getDetectionType();
		
		/*��ȡ�������*/
		static QString getDetectionName();

		/*����SOC��ʱ*/
		void setSocDelay(ulong delay);

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*
		* ��ʼ������̨����
		* @notice,���ô˺���ǰ,���ɽ����κ����,
		* �����ض�������ʧ��,���޷�������ӡ����.
		*/
		bool initConsoleWindow();

		/*�˳�����̨����*/
		bool exitConsoleWindow();

		/*���豸*/
		virtual bool openDevice();

		/*�ر��豸*/
		virtual bool closeDevice();

		/*׼������,����1*/
		virtual bool prepareTest(ulong delay = START_DELAY);

		/*
		 ׼������,����2
		 @param1,����״̬����ID
		 @param2,������ʱ
		 @notice,�������Ҫ�ȴ�ECU��ȫ������param3,4����
		 @param3,��ȫ������ֵ
		 @param4,��ȫ��������
		 @return,bool
		 */
		virtual bool prepareTest(int id, ulong timeout, int value = 0, MsgProc msgProc = nullptr);

		/*��������*/
		virtual bool finishTest(bool success);

		/*������־*/
		virtual bool saveLog(bool success);

		/*������*/
		virtual bool checkCurrent();

		/*
		* ��⾲̬����
		* @param1,�Ƿ��ACC
		* @notice,�����������������ߺͻ���setAcc��Ϊtrue,
		* @param2,�Ƿ�����Ϊ16V��ѹ
		* @return,bool
		*/
		virtual bool checkStaticCurrent(bool setAcc = false, bool set16Vol = true);

		/*����ѹ*/
		virtual bool checkVoltage();

		/*���DTC*/
		virtual bool clearDtc();

		/*
		* ���汾��
		* @param1,��ȡDID֮���ʱ����
		* @param2,���ʧ��,�ظ���ȡ���ٴ�
		* @return,bool
		*/
		virtual bool checkVersion(ulong delay = 200, int tryTimes = 3);

		/*���DTC*/
		virtual bool checkDtc();

		/*д�����к�*/
		bool writeSn(const std::function<bool()>& lambda);

		/*������к�*/
		bool checkSn(const std::function<bool()>& lambda);

		/*д������*/
		bool writeDate(const std::function<bool()>& lambda);

		/*�������*/
		bool checkDate(const std::function<bool()>& lambda);

		/*д������*/
		bool writeSet(const std::function<bool()>& lambda);

		/*
		* @setFnc,���ú���
		* @notice,�˺�������,����һЩû���ڿ���еĺ���,
		* ������뵽lambda��ִ��
		*/
		bool setFnc(const std::function<bool()>& lambda);

		/*���CAN��־*/
		void outputCanLog(bool enable = true);

		/*����CAN��־*/
		void saveCanLog(bool enable);

		/*����CAN��־��*/
		void setCanLogName(const QString& modelName, const QString& code);

		/*ˢ��CAN��־������*/
		void flushCanLogBuffer();

		/*���CAN������*/
		void clearCanBuffer();

		/*���ٽ���CAN��Ϣ*/
		int quickRecvCanMsg(MsgNode* msgNode, int maxSize, int ms);

		/*�Զ�����CAN��Ϣ,[����1]*/
		bool autoProcessCanMsg(int id, int value, MsgProc msgProc, ulong timeout = 10000U);

		/*�Զ�����CAN��Ϣ��չ,[����2]*/
		bool autoProcessCanMsgEx(IdList idList, ValList valList, MsgProc msgProc, ulong timeout = 10000U);

		/*����Can������[����1]*/
		bool setCanProcessFnc(const char* name, const CanMsg& msg, const CanProcInfo& procInfo);

		/*����Can������[����2]*/
		bool setCanProcessFnc(const char* name, const CanMsg& msg, int id, int value, CanProc proc);

		/*����Can������,��չ��[����1]*/
		bool setCanProcessFncEx(const char* name, CanList list, const CanProcInfo& procInfo);

		/*����Can������,��չ��[����2]*/
		bool setCanProcessFncEx(const char* name, CanList list, int id, int value, CanProc proc);

		/*����UDS������*/
		bool setUdsProcessFnc(const char* name, DidList list, int value, int size, UdsProc proc, ulong timeout = 10000U);

		/*����UDS������,��չ��*/
		bool setUdsProcessFncEx(const char* name, DidList list, ValList valList, int size, UdsProcEx procEx, ulong timeout = 10000U);

		/*
		 *�Զ�����,���ڴ�����,����ռ�ÿռ������
		 *@param1,·���б�
		 *@param2,��׺���б�
		 *@param3,�����»���һ��
		 *@return,void
		*/
		void autoRecycle(const QStringList& path,
			const QStringList& suffixName = { ".mp4",".jpg",".png",".bmp",".net",".run",".can" },
			int interval = 1);

		/*�����Զ�����*/
		void enableRecycle(bool enable) { m_autoRecycle = enable; };

		/*���û��պ�׺��*/
		void setRecycleSuffixName(const QStringList& suffixName) { m_recycleSuffixName = suffixName; }

		/*���û��ռ����*/
		void setRecycleIntervalMonth(int interval) { m_recycleIntervalMonth = interval; }

		/************************************************************************/
		/* Get Local Function                                                   */
		/************************************************************************/
		static CanTransfer* getCanTransfer();

		static CanSender* getCanSender();

		static CItechSCPIMgr* getPowerDevice();

		static CMRDO16KNMgr* getRelayDevice();

		static CVoltageTestMgr* getVoltageDevice();

		static StaticCurrentMgr* getCurrentDevice();

		/************************************************************************/
		/* UDS                                                                  */
		/************************************************************************/

		/*���÷��ʵȼ�*/
		void setAccessLevel(int udsLevel);

		/*������ϻỰ*/
		void setDiagnosticSession(int udsSession);

		/*��ԭ���ʵȼ�*/
		void restoreAccessLevel();

		/*��ԭ��ϻỰ*/
		void restoreDiagnosticSession();

		/*���밲ȫ����*/
		bool enterSecurityAccess(uchar session = SL_EXTENDED, uchar access = SAL_LEVEL2, int repeat = 3);

		/*ͨ��DID������*/
		bool readDataByDid(uchar did0, uchar did1, uchar* data, int* size);

		/*ͨ��DIDд����[����1]*/
		bool writeDataByDid(uchar did0, uchar did1, const uchar* data, int size);

		/*ͨ��DIDд����[����2]*/
		bool writeDataByDid(uchar did0, uchar did1, const std::initializer_list<uchar>& data);

		/*ͨ��DIDд����,����ȫ,��չ�汾[����1]*/
		bool writeDataByDidEx(const uchar* routine, uchar did0, uchar did1, const uchar* data, int size);

		/*ͨ��DIDд����,��չ�汾[����2]*/
		bool writeDataByDidEx(const std::initializer_list<uchar>& routine, uchar did0, uchar did1, const uchar* data, int size);

		/*ͨ��DIDȷ������*/
		bool confirmDataByDid(uchar did0, uchar did1, const uchar* data, int size);

		/*��ȫͨ��DIDд����*/
		bool safeWriteDataByDid(uchar did0, uchar did1, const uchar* data, int size);

		/*��ȡUDS���մ���*/
		QString getUdsLastError() const;

		/************************************************************************/
		/* Log                                                                  */
		/************************************************************************/

		/*��ʼ�������־*/
		void initDetectionLog();

		/*���ü����־*/
		void setDetectionLog(BaseTypes::DetectionLog log = BaseTypes::DL_ALL, const std::function<void(const int&)>& fnc = nullptr);

		/*������־�ļ�*/
		QString createLogFile(bool success);

		/*д����־*/
		bool writeLog(bool success);

		/************************************************************************/
		/* Thread control                                                       */
		/************************************************************************/

		/*�߳���ͣ*/
		void threadPause();

		/*�߳��Ƿ���ͣ*/
		bool threadIsPause();

		/*�̼߳���*/
		void threadContinue();

		/*�߳��˳�*/
		void threadQuit();

		/************************************************************************/
		/* GUI                                                                  */
		/************************************************************************/
		/*����ɨ��Ի���*/
		bool setScanCodeDlg(bool show = true);

		/*���ý����Ի���*/
		void setUnlockDlg(bool show = true);

		/*��Ϣ�Ի���,ֻ���������߳���ʹ��*/
		void setMessageBox(const QString& title, const QString& text);

		/*��Ϣ�Ի���,��չ��*/
		void setMessageBoxEx(const QString& title, const QString& text, const QPoint& point = QPoint(0, 0));

		/*ѯ�ʶԻ���,ֻ���������߳���ʹ��*/
		bool setQuestionBox(const QString& title, const QString& text);

		/*ѯ�ʶԻ���,��չ��*/
		bool setQuestionBoxEx(const QString& title, const QString& text, const QPoint& point = QPoint(0, 0));

		/*���ò��Խ��*/
		void setTestResult(BaseTypes::TestResult testResult);

		/*���õ�ǰ״̬*/
		void setCurrentStatus(const QString& status, bool systemStatus = false);

		/*�б�ؼ�������һ��Ԫ��*/
		void addListItem(const QString& item, bool logItem = true);

		/*�б�ؼ�������һ��Ԫ��,��չ��*/
		void addListItemEx(const QString& item);

		/*����б�ؼ�*/
		void clearListItem();

		/*�������ضԻ����*/
		bool setDownloadDlg(BaseTypes::DownloadInfo* info);

		bool callPythonFnc();

		/*
		* �ȴ�����
		* @param1,������ʱ
		* @return,bool
		*/
		virtual bool waitStartup(ulong delay);

		/*
		* @checkPing,����Ƿ�Pingͨ
		* @notice,�����ϵͳ�й�,���ܻ᲻׼ȷ
		* @param1,IP��ַ
		* @param2,����
		* @return,bool
		*/
		bool checkPing(const QString& address, int times);

		/*
		* @checkPing,����Ƿ�Pingͨ
		* @param1,IP��ַ
		* @param2,�˿�
		* @param3,��ʱʱ��
		* @return,bool
		*/
		bool checkPing(const QString& address, int port, int timeout);
	protected:
		/*�߳��Ƿ��˳�*/
		bool m_quit = false;

		/*�Ƿ�����*/
		bool m_connect = false;

		/*�ȴ�SOC������ʱ*/
		ulong m_socDelay = 3000U;

		/*����˳��*/
		int m_testSequence = TS_NO;

		/*����ʱ��*/
		size_t m_elapsedTime = 0;

		/*ͳ�Ʋ�Ʒ*/
		size_t m_total = 1;

		/*�������*/
		static BaseTypes::DetectionType m_detectionType;

		/*JSON������*/
		JsonTool* m_jsonTool = nullptr;

		/*Ĭ������*/
		DefConfig* m_defConfig = nullptr;

		/*Ӳ��������� */
		HwdConfig* m_hwdConfig = nullptr;

		/*UDS�������*/
		UdsConfig* m_udsConfig = nullptr;

		/*��Դ��*/
		static CItechSCPIMgr m_power;

		/*16·�̵�����*/
		static CMRDO16KNMgr m_relay;

		/*��ѹ����*/
		static CVoltageTestMgr m_voltage;

		/*������*/
		static StaticCurrentMgr m_current;

		/*CAN���ӹ���*/
		static CanTransfer* m_canTransfer;

		/*UDS����*/
		UdsTransfer* m_udsTransfer = nullptr;

		/*CAN������*/
		static CanSender* m_canSender;

		/*�������*/
		QString m_lastError = "No Error";

		/*��־����*/
		QList<QString> m_logList;

		/*���ѵ���*/
		float m_rouseCurrent = 0.0f;

		/*�����㷨*/
		CanMatrix m_matrix;

		void* m_core = nullptr;
	protected:
		/*���ô�����Ϣ,����1*/
		void setLastError(const QString& error);

		/*���ô�����Ϣ,����2*/
		void setLastError(const QString& error, bool addItem, bool msgBox = false);

		/*UDS����ת��*/
		bool udsEncodeConvert(VersonConfig* config);

	private:
		/*UDS�ȼ�*/
		int m_udsLevel = SAL_LEVEL1;

		/*UDS�Ự*/
		int m_udsSession = 0x03;

		/*�����Զ�����*/
		bool m_autoRecycle = true;

		/*���պ�׺��*/
		QStringList m_recycleSuffixName = {};

		/*���ռ����*/
		int m_recycleIntervalMonth = -1;

	signals:
		/*����ͼ���ź�*/
		void updateImageSignal(const QImage& image);

		/*������Ϣ�Ի����ź�*/
		void setMessageBoxSignal(const QString& title, const QString& text);

		/*������Ϣ�Ի�����չ���ź�*/
		void setMessageBoxExSignal(const QString& title, const QString& text, const QPoint& point);

		/*����ѯ�ʶԻ����ź�*/
		void setQuestionBoxSignal(const QString& title, const QString& text, bool* result);

		/*����ѯ�ʶԻ�����չ���ź�*/
		void setQuestionBoxExSignal(const QString& title, const QString& text, bool* result, const QPoint& point);

		/*���ò��Խ���ź�*/
		void setTestResultSignal(BaseTypes::TestResult result);

		/*���õ�ǰ״̬�ź�*/
		void setCurrentStatusSignal(const QString& status, bool append);

		/*����ɨ��Ի����ź�*/
		void setScanCodeDlgSignal(bool show);

		/*���ý����Ի����ź�*/
		void setUnlockDlgSignal(bool show);

		/*����һ���б���Ŀ�ź�*/
		void addListItemSignal(const QString& item, bool logItem);

		/*����б�ؼ��ź�*/
		void clearListItemSignal();

		/*�������ضԻ����ź�*/
		void setDownloadDlgSignal(BaseTypes::DownloadInfo* info);
	};

	/************************************************************************/
	/* Hardware Class                                                       */
	/************************************************************************/
	class Hardware : public Base {
		Q_OBJECT
	public:
		explicit Hardware(QObject* parent = nullptr);

		~Hardware();

	protected:
		virtual void run() override = 0;

	private:

	};

	/************************************************************************/
	/* Function Class                                                       */
	/************************************************************************/

	class Function : public Base {
		Q_OBJECT
	public:
		/*����*/
		explicit Function(QObject* parent = nullptr);

		/*����*/
		~Function();

		/*��Ԫ*/
		friend void WINAPI Cc::Mv800Proc(const uchar* head, const uchar* bits, LPVOID param);

		/*��Ԫ*/
		friend class Cc::Mil;

		/*��Ԫ*/
		friend class Cc::CaptureCard;

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*���豸*/
		virtual bool openDevice();

		/*�ر��豸*/
		virtual bool closeDevice();

		/*
		 *���CAN��������,[����1]
		 *@param1,���ѱ���
		 *@param2,������ʱ
		 *@param3,����״̬����ID
		 *@param4,���ѳɹ���ֵ
		 *@param5,���Ĵ�����
		 *@return,bool
		*/
		virtual bool checkCanRouseSleep(const MsgNode& msg, ulong delay, int id, int value = 0, MsgProc msgProc = nullptr);

		/*
		* ���CAN��������,[����2]
		* @param1,����״̬����ID
		* @param2,���ѳɹ���ֵ
		* @param3,���Ĵ�����
		* @return,bool
		*/
		virtual bool checkCanRouseSleep(int id, int value = 0, MsgProc msgProc = nullptr);

		/************************************************************************/
		/* ͼ�����                                                              */
		/************************************************************************/

		/*���òɼ�������*/
		void setCaptureCardAttribute();

		/*��ʼ�ɼ����ɼ�����*/
		void startCapture();

		/*ֹͣ�ɼ����ɼ�����*/
		void stopCapture();

		/*�򿪲ɼ���*/
		bool openCaptureCard();

		/*�رղɼ���*/
		bool closeCaptureCard();

		/*ѭ��ץͼ,Ч�����*/
		bool cycleCapture();

		/*�������ͼ��*/
		bool saveAnalyzeImage(const QString& name, const IplImage* image, const CvSize& size = CvSize());

		/*��ͼ���ϻ�����*/
		inline void drawRectOnImage(IplImage* image);

		/*��ͼ���ϻ�����*/
		inline void drawRectOnImage(cv::Mat& mat);

		/*��ͼ���ϼ�����*/
		bool checkRectOnImage(IplImage* cvImage, const RectConfig& rectConfig, QString& colorData);

		/*���þ�������*/
		void setRectType(FcTypes::RectType rectType = FcTypes::RT_SMALL);

		/*��ȡ��������*/
		FcTypes::RectType getRectType() const;

		/*��ԭ��������*/
		void restoreRectType();

		/*����ͼ��*/
		void updateImage(const QImage& image);

		/*��ʾͼ��*/
		void showImage(const IplImage* image, const QString& name = "image");

		/************************************************************************/
		/* Get                                                                  */
		/************************************************************************/
		inline Cc::Mil* getMil() { return m_mil; };

		inline CMV800Mgr* getMv800() { return &m_mv800; }

		inline Cc::CaptureCard* getCaptureCard() { return m_cap; }

		bool getCardConnectStatus();

		inline const int& getMilChannelId() { return m_cardConfig.channelId; }

		inline const int& getMv800ChannelId() { return m_cardConfig.channelId; }

		inline const int& getCaptureCardId() { return m_cardConfig.channelId; }

		const FcTypes::CardConfig* getCaptureCardConfig();

		/*���òɼ�״̬*/
		void setCaptureStatus(bool capture);

		/*��ȡ�ɼ�״̬*/
		bool getCaptureStatus();
	protected:
		/*������д�߳�*/
		virtual void run() override = 0;

		/*Ŀ�����ͼ��*/
		IplImage* m_cvAnalyze = nullptr;

	private:
		/*����*/
		FcTypes::RectType m_rectType = FcTypes::RT_NO;

		/*�ؼ���ʾͼ��*/
		IplImage* m_cvPainting = nullptr;

		/*MIL�ɼ���*/
		Cc::Mil* m_mil = nullptr;

		/*MV800�ɼ���*/
		CMV800Mgr m_mv800;

		/*�ɼ���ͨ����*/
		Cc::CaptureCard* m_cap = nullptr;

		/*�ɼ����ṹ��*/
		FcTypes::CardConfig m_cardConfig;

		/*ץͼ*/
		bool m_capture = false;
	};

	/************************************************************************/
	/* AVM Class                                                            */
	/************************************************************************/
	class Avm : public Function {
		Q_OBJECT
	public:
		/*����*/
		explicit Avm(QObject* parent = nullptr);

		/*����*/
		~Avm();

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*
		* ����led��
		* @param1,����
		* @return,bool
		*/
		void setLedLight(bool _switch);

		/*
		* ͨ����������AVM
		* @param1,�ߵ�ƽ��ʱ
		* @param2,AVM��������ID
		* @param3,AVM�����ɹ���ֵ
		* @param4,����AVM��������
		* @return,bool
		*/
		virtual bool triggerAvmByKey(ulong delay = 300, int id = 0, int value = 0, MsgProc proc = nullptr);

		/*
		* ͨ�����Ĵ���AVM
		* @param1,����AVM�ı���
		* @param2,AVM��������ID
		* @param3,AVM�����ɹ���ֵ
		* @param4,����AVM��������
		* @return,bool
		*/
		virtual bool triggerAvmByMsg(const CanMsg& msg, int id = 0, int value = 0, MsgProc proc = nullptr);

		/*�����Ƶ������ʹ���κ�*/
		virtual bool checkVideoUseNot();

		/*
		* �����Ƶ����ʹ�ñ���[��չ��]
		* @param1,����ȫ������
		* @param2,����ȫ���ɹ�����
		* @param3,����ȫ���ɹ���ֵ
		* @param4,����ȫ���ɹ���������
		* @return,bool
		*/
		virtual bool checkVideoUseMsg(const CanMsg& msg, int id, int value, MsgProc proc);

		/*
		* �����Ƶ����ʹ�ð���
		* @param1,����״̬����ID
		* @param2,����ȫ���ɹ���ֵ
		* @param3,����ȫ�����ĺ���
		* @param4,�ߵ�ƽ��ʱ
		* @param5,�����ɹ���ʱ
		* @return,bool
		*/
		virtual bool checkVideoUseKey(int id, int req, MsgProc proc, ulong hDelay = 300, ulong sDelay = 3000);

		/*�����Ƶ����ʹ���˹�
		* @return,bool
		*/
		virtual bool checkVideoUsePerson();

		/*
		* ��鵥��ͼ��ʹ�ñ���
		* @param1,��������
		* @param2,��������
		* @param3,�ɹ�����
		* @param4,�ɹ���ֵ
		* @param5,������
		* @param6,��ʱ
		* @return,bool
		*/
		bool checkSingleImageUseMsg(FcTypes::RectType type, const CanMsg& msg,
			int id = 0, int value = 0, MsgProc proc = 0, ulong timeout = 10000U);

		/*
		* ���AVMǰ����ͼʹ�ñ���
		* @notice,[F]����ǰ,[R]�����,
		* Ĭ��ȫ����ǰ,���к���д��һ��,��֮һ��.
		* @param1,ǰ�󾰱����б�
		* @param2,������ʱ
		* @param3,����ID
		* @param4,�����б�
		* @param5,lambda
		* @return,bool
		*/
		virtual bool checkFRViewUseMsg(CanList msgList, int id, ValList valList, MsgProc proc);

		/*
		* ��ⰴ����ѹ
		* @param1,����ȫ���ɹ�����ID
		* @param2,����ȫ���ɹ���ֵ
		* @param3,�������ȫ���ı���
		* @param4,�ߵ�ƽ��ʱ
		* @param5,�ɹ�֮����ʱ
		* @notice,��ʱȡ���ڸߵ�ƽ��ѹ�Ƿ�׼ȷ
		* @return,bool
		*/
		virtual bool checkFRViewUseKey(int id, int value, MsgProc proc, ulong hDelay = 300U, ulong sDelay = 3000U);

		/*
		* ��ⰴ����ѹ
		* @param1,�ߵ�ƽ��ʱ
		* @param1,�ɹ�֮����ʱ
		* @notice,��ʱȡ���ڸߵ�ƽ��ѹ�Ƿ�׼ȷ
		* @param2,����ȫ���ɹ�����ID
		* @param3,����ȫ���ɹ���ֵ
		* @param4,�������ȫ���ı���
		* @return,bool
		*/
		virtual bool checkKeyVoltage(ulong hDelay = 300U, ulong sDelay = 3000U,
			int id = 0, int value = 0, MsgProc proc = nullptr);

	protected:
		/*������д�߳�*/
		virtual void run() override = 0;
	private:
	};

	/************************************************************************/
	/* DVR Class                                                            */
	/************************************************************************/
	class Dvr : public Function {
		Q_OBJECT
	public:
		/*����*/
		explicit Dvr(QObject* parent = nullptr);

		/*������*/
		~Dvr();

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*��ʼ����*/
		virtual bool prepareTest(ulong delay = 30000U);

		/*��������*/
		virtual bool finishTest(bool success);

		/*����ϵͳ״̬����*/
		void setSysStatusMsg(DvrTypes::SysStatusMsg msg);

		/*����SD��״̬*/
		void setSdCardStatus(DvrTypes::SdCardStatus status);

		/*����ϵͳ״̬*/
		void setSystemStatus(DvrTypes::SystemStatus status);

		/*������������,[����ʱ��ͬ��,�������������...]*/
		virtual bool setOtherAction() { return true; };

		/*��ȡ����״̬
		* @param1,ϵͳ״̬����
		* @param2,����ȡ��״ֵ̬
		* @return,bool
		*/
		template<class T>bool getAllStatus(T& status);

		/*�Զ�����״̬,[����1]
		* @param1,ϵͳ״̬����
		* @param2,����ʱ
		* @return,bool
		*/
		template<class T>bool autoProcessStatus(ulong timeout = 30000U);

		/*�Զ�����״̬,[����2]
		* @param1,ϵͳ״̬����
		* @param2,ϵͳ��ȷ״ֵ̬
		* @param3,����ʱ
		* @return,bool
		*/
		template<class T>bool autoProcessStatus(T value, ulong timeout = 30000U);

		/*���DVR[����1]
		* @param1,RTSPЭ���ַ
		* @param2,�Ƿ�ʹ��WIFI
		* @param3,�Ƿ�ʹ�òɼ���
		* @param4,�Ƿ�������Ƶ
		* @return,bool
		*/
		bool checkDvr(const QString& rtspUrl, bool useWifi = true, bool useCard = false, bool downloadVideo = true);

		/*���DVR[����2]
		* @param1,�Ƿ�ʹ��WIFI
		* @param2,�Ƿ�ʹ�òɼ���
		* @param3,�Ƿ�������Ƶ
		* @return,bool
		*/
		bool checkDvr(bool useWifi = true, bool useCard = false, bool downloadVideo = true);

		/*���������͵ƹ�*/
		bool setSoundLight(bool enable);

		/*�����͵ƹ��Ƿ���*/
		bool getSoundLigth();

		/*��������*/
		bool setSound(bool enable);

		/*���õƹ�*/
		bool setLight(bool enable);

		/*����vlcý�岥�ž��*/
		void setVlcMediaHwnd(HWND vlcHwnd);

		/*ʹ��vlc�����rtsp�����ļ�*/
		bool vlcRtspStart(const QString& url);

		/*ֹͣrtsp����*/
		bool vlcRtspStop();

		/*��ȡ�ļ��б�,����1*/
		bool getFileUrl(QString& url, DvrTypes::FilePath filePath);

		/*���ؽ���¼���ļ�,����1*/
		bool downloadFile(const QString& url, const QString& dirName, bool isVideo = true);

		/*���ؽ���¼���ļ�,����2*/
		bool downloadFile(const QString& url, DvrTypes::FileType types);

		/*���������ļ�Ŀ¼*/
		void setDownloadFileDir(DvrTypes::FileType types, const QString& dirName);

		/*���DVR����*/
		bool checkRayAxis(const QString& url, const QString& dirName);

		/*��ȡDVR�����*/
		bool checkSfr(const QString& url, const QString& dirName);

		/*
		* ����������ʹ�ñ���
		* @param1,CAN�����б�
		* @param2,���ձ���ID
		* @param3,��ֵ
		* @param4,lambda
		* @return,bool
		*/
		bool checkRayAxisSfrUseMsg(CanList list, int id, int req, MsgProc proc);

		/*
		* ����������ʹ������
		* @return,bool
		*/
		bool checkRayAxisSfrUseNet();

		/*
		* ����������
		* @return,bool
		*/
		bool checkRayAxisSfr();

		/*��ʽ��SD��*/
		virtual bool formatSdCard(bool pauseRecord = true);

		/*ж��SD��*/
		virtual bool umountSdCard();

		/*����WIFI����*/
		virtual bool changeWifiPassword();

		/*���õ�ַ�˿�*/
		void setAddressPort(const QString& address, const ushort& port);

		/*��������*/
		bool networkPhotoGraph();

		Misc::UpdateSfr* getUpdateSfr();

		Nt::SfrServer* getSfrServer();

		/*����ѯ�ʶԻ���*/
		int setPlayQuestionBox(const QString& title, const QString& text, const QPoint& point = QPoint(0, 0));
	signals:
		void setPlayQuestionBoxSignal(const QString&, const QString&, int* result, const QPoint& point);
	protected:
		/*������д�߳�*/
		virtual void run() override = 0;

		/*ʹ��WIFI�ı�����д�˺���,ΪcheckDvr��̬ʹ��*/
		virtual bool getWifiInfo(bool rawData = false, bool showLog = true);

		/*д��������־*/
		bool writeNetLog(const char* name, const char* data, size_t size);
	protected:
		/*WIFI*/
		WifiMgr m_wifiMgr;

		/*WIFI��Ϣ�ṹ��*/
		WIFIInfo m_wifiInfo = { 0 };
	private:
		/*�����͵��Ƿ��*/
		bool m_soundLight = false;

		/*VLC*/
		HWND m_vlcHwnd = nullptr;

		libvlc_instance_t* m_vlcInstance = nullptr;

		libvlc_media_t* m_vlcMedia = nullptr;

		libvlc_media_player_t* m_vlcMediaPlayer = nullptr;

		Nt::DvrClient* m_dvrClient = nullptr;

		Nt::SfrServer* m_sfrServer = nullptr;

		/*��Ƭ·��*/
		QString m_photoPath = "PHODownload";

		/*��Ƶ·��*/
		QString m_videoPath = "EVTDownload";

		/*IP��ַ*/
		QString m_address = "10.0.0.10";

		/*�˿�*/
		ushort m_port = 2000;

		/*ϵͳ״̬����*/
		int m_sysStatusMsg = DvrTypes::SSM_BAIC_C62X;

		/*SD��״̬*/
		DvrTypes::SdCardStatus m_sdCardStatus = DvrTypes::SCS_NORMAL;

		/*ϵͳ״̬*/
		DvrTypes::SystemStatus m_systemStatus = DvrTypes::SS_PAUSE_RECORD;

		Misc::UpdateSfr m_updateSfr;

		/*HASH��ṹ������ģ���ж�*/
		struct HashCode {
			/*ϵͳ״̬*/
			size_t systemStatus;

			/*WIFI״̬*/
			size_t wifiStatus;

			/*��̫��״̬*/
			size_t ethernetStatus;

			/*SD��״̬*/
			size_t sdCardStatus;
		}m_hashCode;

		/*DVR�ļ��б�,���������ͨѶЭ��*/
		struct FileList {
			size_t listCount;
			struct FileInfo {
				ushort index;

				uchar path;

				uchar type;

				uchar suffix;

				uchar reserved[3];

				size_t size;

				size_t date;
			}fileInfo[100];
		};
	};

	/************************************************************************/
	/* TAP Class                                                            */
	/************************************************************************/
	class Tap : public Function {
		Q_OBJECT
	public:
		explicit Tap(QObject* parent = nullptr);

		~Tap();

		virtual bool initInstance();

		virtual bool openDevice();

		virtual bool closeDevice();

		bool checkUSBByJson(const QString& url = "http://172.19.1.2:20001/info");
	protected:
		virtual void run() override = 0;
	private:
		/*���ڹ���*/
		SerialPort* m_serialPort = nullptr;

		/*TAP������Ļ����*/
		void screenUartHandler(const QString& port, const QByteArray& bytes);
	};

	/*
	* Module Class
	*/
	class Module : public Base {
		Q_OBJECT
	public:
		explicit Module(QObject* parent = nullptr);

		~Module();

		virtual bool initInstance();

		bool printLabel(const std::function<bool(void)>& fnc);
	protected:
		virtual void run() override = 0;

		QString getPrinterError();

		TSCPrinterMgr m_printer;
	private:
	};

	template<class T> inline bool Dvr::getAllStatus(T& status)
	{
		bool result = false, success = false;
		do
		{
			const size_t& statusCode = typeid(status).hash_code();
			MsgNode msg[512] = { 0 };
			size_t&& startTime = GetTickCount();
			clearCanBuffer();
			for (;;)
			{
				int size = quickRecvCanMsg(msg, 512, 100);
				for (int i = 0; i < size; i++)
				{
					if (msg[i].id == m_sysStatusMsg)
					{
						if (statusCode == m_hashCode.systemStatus)
						{
							status = static_cast<T>(msg[i].data[0] & 0x07);
							switch (status)
							{
							case DvrTypes::SS_INITIALIZING:setCurrentStatus("ϵͳ��ʼ����", true); break;
							case DvrTypes::SS_GENERAL_RECORD:setCurrentStatus("����¼��", true); break;
							case DvrTypes::SS_PAUSE_RECORD:setCurrentStatus("��ͣ¼��", true); break;
							case DvrTypes::SS_HARDWARE_KEY:setCurrentStatus("����¼�� ����", true); break;
							case DvrTypes::SS_CRASH_KEY:setCurrentStatus("����¼�� ��ײ", true); break;
							case DvrTypes::SS_UPDATE_MODE:setCurrentStatus("����ģʽ", true); break;
							case DvrTypes::SS_ERROR:setCurrentStatus("ϵͳ����", true); break;
							default:setCurrentStatus("δ֪ϵͳ״̬"); break;
							}
						}
						else if (statusCode == m_hashCode.wifiStatus)
						{
							if (m_sysStatusMsg == DvrTypes::SSM_BAIC_C62X)
							{
								status = static_cast<T>((msg[i].data[0] >> 4) & 0x07);
							}
							else if (m_sysStatusMsg == DvrTypes::SSM_CHJ_M01)
							{
								status = static_cast<T>((msg[i].data[1] >> 0) & 0x07);
							}

							switch (status)
							{
							case DvrTypes::WS_CLOSE:setCurrentStatus("WIFI�ѹر�", true); break;
							case DvrTypes::WS_INIT:setCurrentStatus("WIFI���ڳ�ʼ��", true); break;
							case DvrTypes::WS_NORMAL:setCurrentStatus("WIFI����", true); break;
							case DvrTypes::WS_CONNECT:setCurrentStatus("WIFI��������", true); break;
							case DvrTypes::WS_ERROR:setCurrentStatus("WIFI����", true); break;
							default:setCurrentStatus("δ֪WIFI״̬", true); break;
							}

							if (status == DvrTypes::WS_NORMAL)
							{
								if (!getWifiInfo())
								{
									addListItem(getLastError());
									break;
								}
								success = m_wifiMgr.connect(m_wifiInfo);
								addListItem(Q_SPRINTF("����WIFI %s", OK_NG(success)));
								if (!success)
								{
									addListItem(G_TO_Q_STR(m_wifiMgr.getLastError()));
									break;
								}
								addListItem("�������ӷ����,�ù��̴�Լ��Ҫ1~20��,�����ĵȴ�...");
								success = m_dvrClient->connect(20);
								status = static_cast<T>(success ? DvrTypes::WS_CONNECTED : DvrTypes::WS_ERROR);
								m_dvrClient->disconnect();
								addListItem(Q_SPRINTF("���ӷ���� %s", OK_NG(success)));
								if (success && m_sysStatusMsg == DvrTypes::SSM_CHJ_M01)
								{
									addListItem("�ȴ�ϵͳ�ȶ���,��Լ��Ҫ5��,�����ĵȴ�...");
									msleep(5000);
								}
							}
						}
						else if (statusCode == m_hashCode.ethernetStatus)
						{
							addListItem("�������ӷ����,�ù��̴�Լ��Ҫ1~20��,�����ĵȴ�...");
							success = m_dvrClient->connect(20);
							status = static_cast<T>(success ? DvrTypes::ES_CONNECT : DvrTypes::ES_ERROR);
							m_dvrClient->disconnect();
							setCurrentStatus(Q_SPRINTF("��̫��%s����", success ? "��" : "δ"), true);
							addListItem(Q_SPRINTF("���ӷ���� %s", OK_NG(success)));
						}
						else if (statusCode == m_hashCode.sdCardStatus)
						{
							if (m_sysStatusMsg == DvrTypes::SSM_BAIC_C62X)
							{
								status = static_cast<T>(msg[i].data[1] & 0x07);
							}
							else if (m_sysStatusMsg == DvrTypes::SSM_CHJ_M01)
							{
								status = static_cast<T>((msg[i].data[0] >> 3) & 0x07);
							}

							switch (status)
							{
							case DvrTypes::SCS_NORMAL:setCurrentStatus("SD������", true); break;
							case DvrTypes::SCS_NO_SD:setCurrentStatus("�����SD��", true); break;
							case DvrTypes::SCS_ERROR:setCurrentStatus("SD������", true); break;
							case DvrTypes::SCS_NOT_FORMAT:setCurrentStatus("SD��δ��ʽ��", true); break;
							case DvrTypes::SCS_INSUFFICIENT:setCurrentStatus("SD���ռ䲻��", true); break;
							case DvrTypes::SCS_SPEED_LOW:setCurrentStatus("SD���ٶȵ�", true); break;
							case DvrTypes::SCS_USING:setCurrentStatus("SD������ʹ����", true); break;
							default:setCurrentStatus("δ֪SD��״̬", true); break;
							}

							if (status == DvrTypes::SCS_USING)
							{
								status = static_cast<T>(DvrTypes::SCS_NORMAL);
							}
						}
						success = true;
						break;
					}
				}

				if (success || GetTickCount() - startTime > 2000)
				{
					break;
				}
			}

			if (!success)
			{
				break;
			}
			result = true;
		} while (false);
		return result;
	}

	template<class T> inline bool Dvr::autoProcessStatus(ulong timeout)
	{
		T status;
		const size_t& statusCode = typeid(status).hash_code();
		bool result = false, success = false;
		do
		{
			size_t&& startTime = GetTickCount();
			while (true)
			{

				if (!getAllStatus<T>(status))
				{
					setLastError("δ�յ����Ļ���������,�����������־");
					break;
				}

				DEBUG_INFO_EX("״̬ %d", (int)status);
				if (statusCode == m_hashCode.systemStatus)
				{
					if (status == static_cast<T>(DvrTypes::SS_GENERAL_RECORD))
					{
						success = true;
						break;
					}
				}
				else if (statusCode == m_hashCode.wifiStatus)
				{
					if (status == static_cast<T>(DvrTypes::/*WS_NORMAL*/WS_CONNECTED))
					{
						success = true;
						break;
					}
				}
				else if (statusCode == m_hashCode.ethernetStatus)
				{
					if (status == static_cast<T>(DvrTypes::ES_CONNECT))
					{
						success = true;
						break;
					}
				}
				else if (statusCode == m_hashCode.sdCardStatus)
				{
					if (status == static_cast<T>(DvrTypes::SCS_NORMAL))
					{
						success = true;
						break;
					}
				}

				if (success || GetTickCount() - startTime > timeout)
				{
					setLastError("��ȡ״̬����������,���������������־");
					break;
				}
			}

			if (!success)
			{
				break;
			}
			result = true;
		} while (false);
		return result;
	}

	template<class T> inline bool Dvr::autoProcessStatus(T value, ulong timeout)
	{
		T status;
		const size_t& statusCode = typeid(status).hash_code();
		bool result = false, success = false;
		do
		{
			size_t&& startTime = GetTickCount();
			while (true)
			{

				if (!getAllStatus<T>(status))
				{
					setLastError("δ�յ����Ļ���������,�����������־");
					break;
				}

				DEBUG_INFO_EX("״̬ %d,��ֵ %d", (int)status, (int)value);
				if (statusCode == m_hashCode.systemStatus)
				{
					if (status == value)
					{
						success = true;
						break;
					}
				}
				else if (statusCode == m_hashCode.wifiStatus)
				{
					if (status == value)
					{
						success = true;
						break;
					}
				}
				else if (statusCode == m_hashCode.ethernetStatus)
				{
					if (status == value)
					{
						success = true;
						break;
					}
				}
				else if (statusCode == m_hashCode.sdCardStatus)
				{
					if (status == value)
					{
						success = true;
						break;
					}
				}

				if (success || GetTickCount() - startTime > timeout)
				{
					setLastError("��ȡ״̬����������,���������������־");
					break;
				}
			}

			if (!success)
			{
				break;
			}
			result = true;
		} while (false);
		return result;
	}
}

namespace Cc {
	/*MIL�ɼ����߳�*/
	class Mil : public QThread {
		Q_OBJECT
	public:
		/*����*/
		explicit Mil(QObject* parent = nullptr);

		/*����*/
		~Mil();

		/*��MIL�豸����*/
		bool open(const QString& name, int channel);

		/*�ر�MIL�豸����*/
		void close();

		/*�Ƿ��*/
		bool isOpen();

		/*��ʼ�ɼ�*/
		void startCapture();

		/*�����ɼ�*/
		void stopCapture();

		/*��ȡ������Ϣ*/
		const QString& getLastError();
	protected:
		/*��дrun*/
		virtual void run();

		/*���ô���*/
		void setLastError(const QString& error);
	private:
		/*���߳�ָ��*/
		Dt::Function* m_function = nullptr;

		/*MIL����*/
		MIL_ID MilApplication = M_NULL, MilSystem = M_NULL, MilDisplay = M_NULL;
		MIL_ID MilDigitizer = M_NULL, MilImage = M_NULL, MilImage0 = M_NULL, MilImage2D = M_NULL;

		/*���������Ϣ*/
		QString m_lastError = "No Error";

		bool m_capture = false;

		bool m_quit = false;

		bool m_open = false;

		int m_channel[2] = { M_CH0,M_CH1 };
	};

	class CaptureCard : public QObject {
		Q_OBJECT
	public:
		CaptureCard(QObject* parent);

		~CaptureCard();

		const QString& getLastError();

		bool openDevice(int id, int count);

		bool closeDevice();

		bool isOpen();

		bool startCapture();

		bool stopCapture();

		void setFPS(int fps);
	public slots:
		void getImageSlot();
	protected:
		void setLastError(const QString& error);
	private:
		VideoCapture m_video;

		Mat m_mat;

		QTimer m_timer;

		int m_fps = 40;

		QString m_lastError = "No Error";

		Dt::Function* m_function = nullptr;

		bool m_open = false;

		double m_scalew = 1.0f;

		double m_scaleh = 1.0f;
	};
}

/*Network transmission*/
namespace Nt {

	/************************************************************************/
	/* SFR�����,������SFR APP����ͨѶ,�˴����������                       */
	/************************************************************************/
	class SfrServer {
	public:
		SfrServer();

		~SfrServer();

		bool startListen(const ushort& port = 2000);

		bool getSfr(const char* filePath, float& sfr);

		int send(const char* buffer, int len);

		int recv(char* buffer, int len);

		void closeListen();

		const QString& getLastError();

		static void sfrProcThread(void* arg);
	protected:
		void setLastError(const QString& error);
	private:
		SOCKET m_socket = INVALID_SOCKET;

		SOCKET m_client = INVALID_SOCKET;

		sockaddr_in m_sockAddr = { 0 };

		QString m_lastError = "No error";

		bool m_quit = false;
	};

	/************************************************************************/
	/* DVR�ͻ���,������DVR�����ͨѶ                                        */
	/************************************************************************/
	class DvrClient {
	public:
		DvrClient();

		DvrClient(const QString& address, ushort port);

		~DvrClient();

		void setAddressPort(const QString& address, const ushort& port);

		bool connect(int count = 10);

		bool connect(const QString& address, ushort port, int count = 10);

		void disconnect();

		int send(const char* buffer, int len);

		int recv(char* buffer, int len);

		bool sendFrameData(const char* buffer, int len, uchar cmd, uchar sub);

		bool recvFrameData(char* buffer, int* const len);

		bool sendFrameDataEx(const std::initializer_list<char>& buffer, uchar cmd, uchar sub);

		bool sendFrameDataEx(const char* buffer, int len, uchar cmd, uchar sub);

		bool recvFrameDataEx(char* buffer, int* const len, uchar cmd, uchar sub);

		const size_t crc32Algorithm(uchar const* memoryAddr, size_t memoryLen, size_t oldCrc32);

		const char* getAddress();

		const ushort& getPort();

		const QString& getLastError();
	protected:
		void setLastError(const QString& error);
	private:
		QString m_lastError = "No error";

		SOCKET m_socket = INVALID_SOCKET;

		SOCKADDR_IN m_sockAddr = { 0 };

		char m_address[32] = { 0 };

		ushort m_port = 2000;

		bool m_connected = false;

		bool m_disconnected = false;
	};
}

