#pragma once
#include "Types.h"
#pragma warning(disable:4838)
#pragma execution_character_set("utf-8")

/************************************************************************/
/* namespace declare                                                    */
/************************************************************************/
namespace Cc {
	void WINAPI Mv800Proc(const uchar* head, const uchar* bits, LPVOID param);
}

namespace Nt {
	class DvrClient;

	class SfrServer;
}

namespace Misc {
	namespace Var {
		static QString appendName;
	}

	/*Cvͼ��תQtͼ��*/
	bool cvImageToQtImage(IplImage* cv, QImage* qt);

	/*ͨ��URL��ȡ�ļ���*/
	const QString getFileNameByUrl(const QString& url);

	/*ͨ��·����ȡ�ļ���*/
	const QString getFileNameByPath(const QString& path);

	/*��ȡ��ǰ�ļ���*/
	const QString getCurrentFileName();

	/*��ȡ��ǰĿ¼*/
	const QString getCurrentDir();

	/*����·��*/
	bool makePath(const QString& path);

	/*��ȡAPP�汾��*/
	const QString getAppVersion();
	
	/*����APP������*/
	void setAppAppendName(const QString& name);

	/*��ȡAPP������*/
	const QString getAppAppendName();

	/*ͨ���汾��������APP*/
	bool renameAppByVersion(QWidget* widget);

	/*����Ӧ�ó���*/
	bool startApp(const QString& name, const int& show);

	/*����Ӧ�ó���*/
	bool finishApp(const QString& name);

	/*��ȡ��ǰʱ��*/
	const QString getCurrentTime(bool fileFormat = false);

	/*��ȡ��ǰ����*/
	const QString getCurrentDate(bool fileFormat = false);

	/*��ȡ��ǰʱ������*/
	const QString getCurrentDateTime(bool fileFormat = false);

	/*ͨ��·����ȡ�ļ��б�*/
	void getFileListByPath(const QString& path, QStringList& fileList);

	/*ͨ����׺����ȡ�ļ��б�*/
	const QStringList getFileListBySuffixName(const QString& path, const QStringList& suffix);

	class ThemeFactory {
	public:
		/*����*/
		inline ThemeFactory() {}

		/*����*/
		inline ~ThemeFactory() {}

		/*��ȡ�����б�*/
		inline static const QStringList getThemeList()
		{
			return QStyleFactory::keys();
		}

		/*���ѡ��һ������*/
		inline static void randomTheme()
		{
			setTheme(getThemeList().value(qrand() % getThemeList().size()));
		}

		/*��������*/
		inline static void setTheme(const QString& theme = QString("Fusion"))
		{
			qApp->setStyle(QStyleFactory::create(theme));
			QPalette palette;
			palette.setColor(QPalette::Window, QColor(53, 53, 53));
			palette.setColor(QPalette::WindowText, Qt::white);
			palette.setColor(QPalette::Base, QColor(15, 15, 15));
			palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
			palette.setColor(QPalette::ToolTipBase, Qt::white);
			palette.setColor(QPalette::ToolTipText, Qt::white);
			palette.setColor(QPalette::Text, Qt::white);
			palette.setColor(QPalette::Button, QColor(53, 53, 53));
			palette.setColor(QPalette::ButtonText, Qt::white);
			palette.setColor(QPalette::BrightText, Qt::red);
			palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
			palette.setColor(QPalette::HighlightedText, Qt::black);
			qApp->setPalette(palette);
		}

		/*���ñ߿�ΪԲ��*/
		inline static void setBorderRadius(QWidget* widget)
		{
			QBitmap bmp(widget->size());
			bmp.fill();
			QPainter p(&bmp);
			p.setPen(Qt::NoPen);
			p.setBrush(Qt::black);
			p.drawRoundedRect(bmp.rect(), 20, 20);
			widget->setMask(bmp);
		}
	};
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
		Base(QObject* parent = nullptr);

		/*������*/
		virtual ~Base();

		/*��ȡ����*/
		const QString& getLastError();

		/*���ò���˳��*/
		void setTestSequence(const int& testSequence);

		/*���ü������*/
		void setDetectionType(const BaseTypes::DetectionType& type);

		/*��ȡ�������*/
		const BaseTypes::DetectionType& getDetectionType();

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*��ʼ������̨����*/
		bool initConsoleWindow();

		/*�˳�����̨����*/
		void exitConsoleWindow();

		/*���豸*/
		virtual bool openDevice();

		/*�ر��豸*/
		virtual bool closeDevice();

		/*׼������,����1*/
		virtual bool prepareTest(LaunchProc launchProc = nullptr, void* args = nullptr);

		/*׼������,����2*/
		virtual bool prepareTest(LaunchProcEx lauProcEx, void* args, const int& request = 0, MsgProc msgProc = nullptr);

		/*
		 ׼������,����3
		 @param1,����״̬����ID
		 @param2,������ʱ
		 @notice,�������Ҫ�ȴ�ECU��ȫ������param3,4����
		 @param3,��ȫ��������
		 @param4,��ȫ��������
		 @return,bool
		 */
		virtual bool prepareTest(const int& id, const ulong& delay = 20000U, const int& req = 0, MsgProc msgProc = nullptr);

		/*����������ʱ*/
		void setStartDelay(const size_t& delay);

		/*��������*/
		virtual bool finishTest(bool success);

		/*������־*/
		virtual bool saveLog(bool success);
		
		/*������*/
		virtual bool checkCurrent();

		/*��⾲̬����*/
		virtual bool checkStaticCurrent(const ulong& delay = 6000U);

		/*����ѹ*/
		virtual bool checkVoltage();

		/*���DTC*/
		virtual bool clearDtc();

		/*���汾��*/
		virtual bool checkVersion();

		/*���DTC*/
		virtual bool checkDtc();
		
		/*���CAN��־*/
		void outputCanLog(bool enable = true);

		/*����CAN��־*/
		void saveCanLog(bool enable = true);

		/*����CAN��־��*/
		void setCanLogName(const QString& detectionName, const QString& modelName, const QString& code);

		/*ˢ��CAN��־������*/
		void flushCanLogBuffer();

		/*���CAN���ջ�����*/
		void clearCanRecvBuffer();

		/*���ٽ���CAN��Ϣ*/
		const int quickRecvCanMsg(MsgNode* msgNode, const int& maxSize, const int& ms);

		/*�Զ�����CAN��Ϣ*/
		bool autoProcessCanMsg(const int& id, const int& request, MsgProc msgProc, const ulong& delay = 10000U);

		/*�Զ�ģ��CAN����*/
		bool autoTemplateCanFnc(const char* name, const int& id, const int& req, MsgProc proc, MsgList msg = {}, const ulong& delay = 0);

		/************************************************************************/
		/* Get Local Function                                                   */
		/************************************************************************/
		static IConnMgr* getCanConnect();

		static CanSender* getCanSender();

		const float& getCanRouseCur() const;

		CItechSCPIMgr& getPower();

		static bool getOutputRunLog() { return m_outputRunLog; };

		/************************************************************************/
		/* UDS                                                                  */
		/************************************************************************/

		/*���÷��ʵȼ�*/
		void setAccessLevel(const int& udsLevel);

		/*������ϻỰ*/
		void setDiagnosticSession(const int& udsSession);

		/*��ԭ���ʵȼ�*/
		void restoreAccessLevel();

		/*��ԭ��ϻỰ*/
		void restoreDiagnosticSession();

		/*���밲ȫ����*/
		bool enterSecurityAccess(const uchar& session = 0x03, const uchar& access = 0x01);

		/*ͨ��DID������*/
		bool readDataByDid(const uchar& did0, const uchar& did1, int* size, uchar* data);

		/*ͨ��DIDд����*/
		bool writeDataByDid(const uchar& did0, const uchar& did1, const int& size, const uchar* data);

		/*ͨ��DIDд����,��չ�汾*/
		bool writeDataByDidEx(const uchar* routine, const uchar& did0, const uchar& did1, const int& size, const uchar* data);

		/*ͨ��DIDȷ������*/
		bool confirmDataByDid(const uchar& did0, const uchar& did1, const int& size, const uchar* data);

		/*��ȡUDS���մ���*/
		const QString getUdsLastError();

		/************************************************************************/
		/* Log                                                                  */
		/************************************************************************/

		/*��ʼ�������־*/
		void initDetectionLog();

		void setDetectionLog(const BaseTypes::DetectionLog& log = BaseTypes::DL_ALL, const std::function<void(const int&)>& fnc = nullptr);

		/*������־�ļ�*/
		const QString createLogFile(bool success);

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
		void setScanCodeDlg(bool show = true);

		/*���ý����Ի���*/
		void setUnlockDlg(bool show = true);

		/*��Ϣ�Ի���,ֻ���������߳���ʹ��*/
		void setMessageBox(const QString& title, const QString& text);

		/*��Ϣ�Ի���,��չ��*/
		void setMessageBoxEx(const QString& title, const QString& text, const QPoint& point = QPoint(0, 0));

		/*ѯ�ʶԻ���,ֻ���������߳���ʹ��*/
		bool setQuestionBox(const QString& title, const QString& text, bool auth = false);

		/*ѯ�ʶԻ���,��չ��*/
		bool setQuestionBoxEx(const QString& title, const QString& text, const QPoint& point = QPoint(0, 0));

		/*���ò��Խ��*/
		void setTestResult(const BaseTypes::TestResult& testResult);

		/*���õ�ǰ״̬*/
		void setCurrentStatus(const QString& status, bool systemStatus = false);

		/*�б�ؼ�������һ��Ԫ��*/
		void addListItem(const QString& item, bool logItem = true);

		/*�б�ؼ�������һ��Ԫ��,��չ��*/
		void addListItemEx(const QString& item);

		/*����б�ؼ�*/
		void clearListItem();

		/*��֤�ź�*/
		bool setAuthDlg(const int& flag = 0);
	public:
		/*�������*/
		static QString m_lastError;

		/*��־����*/
		static QList<QString> m_logList;
	protected:
		/*�߳��Ƿ��˳�*/
		bool m_quit = false;

		/*�Ƿ�����*/
		bool m_connect = false;

		/*����˳��*/
		int m_testSequence = TS_NO;

		/*����ʱ��*/
		size_t m_elapsedTime = 0;

		/*ͳ�Ʋ�Ʒ*/
		size_t m_total = 1;

		/*�������*/
		BaseTypes::DetectionType m_detectionType = BaseTypes::DT_AVM;

		/*JSON������*/
		JsonTool* m_jsonTool = nullptr;

		/*Ĭ������*/
		DefConfig* m_defConfig = nullptr;

		/*Ӳ��������� */
		HwdConfig* m_hwdConfig = nullptr;

		/*UDS�������*/
		UdsConfig* m_udsConfig = nullptr;

		/*��Դ��*/
		CItechSCPIMgr m_power;

		/*16·�̵�����*/
		CMRDO16KNMgr m_relay;

		/*��ѹ����*/
		CVoltageTestMgr m_voltage;

		/*CAN���ӹ���*/
		CConnFactory m_canConnFactory;

		/*CAN���ӹ���*/
		static IConnMgr* m_canConnMgr;

		/*UDS����*/
		CUdsFactory m_udsFactory;

		/*UDS�������*/
		IUdsApplyMgr* m_udsApplyMgr = nullptr;

		/*CAN������*/
		static CanSender m_canSender;

	protected:
		/*���ô�����Ϣ,����1*/
		void setLastError(const QString& error);

		/*���ô�����Ϣ,����2*/
		void setLastError(const QString& error, bool addItem, bool msgBox = false);

		/*UDS����ת��*/
		bool udsEncodeConvert(VersonConfig* config);

		/*����Ҹ�*/
		static const char* getAdvice();
	private:
		/*UDS�ȼ�*/
		int m_udsLevel = SAL_LEVEL1;

		/*UDS�Ự*/
		int m_udsSession = 0x03;

		/*������ʱ*/
		size_t m_startDelay = 15000;

		/*���������־*/
		static bool m_outputRunLog;
	signals:
		/*����ͼ���ź�*/
		void updateImageSignal(const QImage& image);

		/*������Ϣ�Ի����ź�*/
		void setMessageBoxSignal(const QString& title, const QString& text);

		/*������Ϣ�Ի�����չ���ź�*/
		void setMessageBoxExSignal(const QString& title, const QString& text, const QPoint& point);

		/*����ѯ�ʶԻ����ź�*/
		void setQuestionBoxSignal(const QString& title, const QString& text, bool* result, bool auth);

		/*����ѯ�ʶԻ�����չ���ź�*/
		void setQuestionBoxExSignal(const QString& title, const QString& text, bool* result, const QPoint& point);

		/*���ò��Խ���ź�*/
		void setTestResultSignal(const BaseTypes::TestResult& result);

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

		/*��֤�ź�*/
		void setAuthDlgSignal(bool* result, const int& flag);
	};

	/************************************************************************/
	/* Hardware Class                                                       */
	/************************************************************************/
	class Hardware :public Base {
		Q_OBJECT
	public:
		Hardware(QObject* parent = nullptr);

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
		Function(QObject* parent = nullptr);

		/*����*/
		~Function();

		/*��Ԫ*/
		friend void WINAPI Cc::Mv800Proc(const uchar* head, const uchar* bits, LPVOID param);

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*���豸*/
		virtual bool openDevice();

		/*�ر��豸*/
		virtual bool closeDevice();

		/*���CAN��������,����1[�ѷ���]*/
		virtual bool checkCanRouseSleep(const MsgNode& msg, const ulong& delay, LaunchProc launchProc, void* args);

		/*���CAN��������,����2[�ѷ���]*/
		virtual bool checkCanRouseSleep(const MsgNode& msg, const ulong& delay, LaunchProcEx lauProcEx, void* args, const int& request = 0, MsgProc msgProc = nullptr);
		
		/*
		 *���CAN��������,����3
		 *@param1,���ѱ���
		 *@param2,������ʱ
		 *@param3,����״̬����ID
		 *@param4,���ѳɹ�����
		 *@param5,lambda
		 *@return,bool
		*/
		virtual bool checkCanRouseSleep(const MsgNode& msg, const ulong& delay, const int& id, const int& req = 0, MsgProc msgProc = nullptr);
		
		/************************************************************************/
		/* ͼ�����                                                              */
		/************************************************************************/

		/*���òɼ�������*/
		void setCaptureCardAttribute();

		/*��ʼ�ɼ����ɼ�����*/
		bool startCaptureCard();

		/*ֹͣ�ɼ����ɼ�����*/
		bool endCaptureCard();

		/*ѭ��ץͼ,Ч�����*/
		bool cycleCapture();

		/*�������ͼ��*/
		bool saveAnalyzeImage(const QString& name, const IplImage* image, const CvSize& size);

		/*��ͼ���ϻ�����*/
		inline void drawRectOnImage();

		/*��ͼ���ϼ�����*/
		bool checkRectOnImage(IplImage* cvImage, const rectConfig_t& rectConfig, QString& colorData);

		/*���þ�������*/
		void setRectType(const FcTypes::RectType& rectType = FcTypes::RT_SMALL);

		/*��ȡ��������*/
		const FcTypes::RectType& getRectType();

		/*��ԭ��������*/
		void restoreRectType();

		/*����ͼ��*/
		void updateImage(const QImage& image);

		/*��ʾͼ��*/
		void showImage(const IplImage* image, const QString& name = "image");

		/*����CANID*/
		void setCanId(const int& id);

		/*����Mil�ɼ���ͨ��ID*/
		void setMilChannelId(const int& id);

		/*����Mv800�ɼ���ͨ��ID*/
		void setMv800ChannelId(const int& id);

		/************************************************************************/
		/* Get                                                                  */
		/************************************************************************/
		inline MilCC* getMil() { return &m_mil; };

		inline CMV800Mgr* getMv800() { return &m_mv800; }

		inline bool isCapture() { return m_capture; };
		
		inline const int& getMilChannelId() { return m_milChannelId; }

		inline const int& getMv800ChannelId() { return m_mv800ChannelId; }
		
		inline const FcTypes::CardConfig& getCardConfig() { return m_cardConfig; };
	public slots:
		void getChannelImageSlot(const int& channelID, const IplImage* image);
	protected:
		/*������д�߳�*/
		virtual void run() override = 0;

		/*Ŀ�����ͼ��*/
		IplImage* m_cvAnalyze = nullptr;

		/*
		 *�Զ�����,���ڴ���������������Ƶ����Ƭ,����ռ�õ�����
		 *@param1,·���б�
		 *@param2,��׺���б�
		 *@param3,�����»���һ��
		 *@return,void
		*/
		void autoRecycle(const QStringList& path,
			const QStringList& suffixName = { ".mp4",".jpg",".png",".bmp" },
			const int& interval = 3);

		/*�����Զ�����*/
		void enableRecycle(bool enable) { m_autoRecycle = enable; };

		/*���û��պ�׺��*/
		void setRecycleSuffixName(const QStringList& suffixName) { m_recycleSuffixName = suffixName; }

		/*���û��ռ����*/
		void setRecycleIntervalMonth(const int& interval) { m_recycleIntervalMonth = interval; }
	private:
		/*����*/
		FcTypes::RectType m_rectType = FcTypes::RT_NO;

		/*�ؼ���ʾͼ��*/
		IplImage* m_cvPainting = nullptr;

		/*MIL�ɼ���*/
		MilCC m_mil;

		/*MV800�ɼ���*/
		CMV800Mgr m_mv800;

		/*�ɼ����ṹ��*/
		FcTypes::CardConfig m_cardConfig;

		/*ץͼ*/
		bool m_capture = false;

		/*CANID*/
		int m_canId = 0;

		/*�����Զ�����*/
		bool m_autoRecycle = true;

		/*���պ�׺��*/
		QStringList m_recycleSuffixName = {};

		/*���ռ����*/
		int m_recycleIntervalMonth = -1;

		/*MIL�ɼ���ͨ��ID*/
		int m_milChannelId = 0;

		/*MV800�ɼ���ͨ��ID,AV2��0,AV1��1*/
		int m_mv800ChannelId = 1;
	};

	/************************************************************************/
	/* AVM Class                                                            */
	/************************************************************************/
	class Avm : public Function {
		Q_OBJECT
	public:
		/*����*/
		Avm(QObject* parent = nullptr);

		/*����*/
		~Avm();

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*ͨ����������AVM*/
		void tiggerAVMByKey();

		/*����led��*/
		void setLedLight(bool _switch);

		/*ʹ�ñ��ļ��AVM*/
		bool checkAVMUseMsg(const MsgNode& msg, const ulong& delay, const int& id, const int& req, MsgProc msgProc);

		/*���AVMʹ�ð���,����1[�ѷ���]*/
		bool checkAVMUseKey(LaunchProc launchProc, RequestProc requestProc, void* args, const int& request, const ulong& delay = 0U);

		/*���AVMʹ�ð���,����2[�ѷ���]*/
		bool checkAVMUseKey(LaunchProcEx launchProcEx, void* args, ReqList lauList, MsgProc lauFnc, RequestProcEx requestProcEx,
			ReqList reqList, MsgProc reqFnc, const ulong& delay = 0U);

		/*���AVMʹ�ð���,����3
		 *@param1,����״̬����ID
		 *@param2,�����ɹ�����
		 *@param3,lambda
		 *@param4,����޷���ȡ�����ɹ�״̬,��δ���д��ʱ,����0
		 *@param5,����ȫ������
		 *@param6,lambda
		 *@return,bool
		*/
		bool checkAVMUseKey(const int& id, const int& req0, MsgProc msgProc0, const ulong& delay, const int& req1, MsgProc msgProc1);

		/*���AVMǰ����ͼ,����1[�ѷ���]*/
		bool checkAVMFRView(MsgList msgList, const ulong& msgDelay, RequestProc requestProc = nullptr, void* args = nullptr, const int& request = 0);

		/*���AVMǰ����ͼ,����2[�ѷ���]*/
		bool checkAVMFRView(MsgList msgList, const ulong& msgDelay, RequestProcEx reqProcEx, void* args, ReqList reqList, MsgProc reqFnc);

		/*���AVMǰ����ͼ,����3
		 *@notice,[F]����ǰ,[R]�����
		 *@param1,ǰ�󾰱����б�
		 *@param2,������ʱ
		 *@param3,����ID
		 *@param4,�����б�
		 *@param5,lambda
		 *@return,bool
		 */
		bool checkAVMFRView(MsgList msgList, const ulong& msgDelay, const int& id, ReqList reqList, MsgProc msgProc);
	protected:
		/*������д�߳�*/
		virtual void run() override = 0;
	private:
	};

	/************************************************************************/
	/* DVR Class                                                            */
	/************************************************************************/
	class Dvr :public Function {
		Q_OBJECT
	public:
		/*����*/
		Dvr(QObject* parent = nullptr);

		/*������*/
		~Dvr();

		/*��ʼ��ʵ��*/
		virtual bool initInstance();

		/*��ʼ����*/
		virtual bool prepareTest(LaunchProc launchProc = nullptr, void* args = nullptr);

		/*��������*/
		virtual bool finishTest(bool success);

		/*��ʼʱ��ͬ��*/
		virtual void startTimeSync();

		/*ֹͣʱ��ͬ��*/
		virtual void stopTimeSync();

		/*��ȡDVR����״̬*/
		template<class T>bool getAllStatus(T& status);

		/*�Զ�����DVR״̬*/
		template<class T>bool autoProcessStatus();

		/*���������͵ƹ�*/
		bool setSoundLight(bool enable);

		/*�����͵ƹ��Ƿ���*/
		bool getSoundLigth();

		/*����vlcý�岥�ž��*/
		void setVlcMediaHwnd(HWND vlcHwnd);

		/*ʹ��vlc�����rtsp�����ļ�*/
		bool vlcRtspStart(const QString& url);

		/*ֹͣrtsp����*/
		bool vlcRtspStop();

		/*DVR����ͨѶЭ���㷨*/
		const size_t crc32Algorithm(uchar const* memoryAddr, const size_t& memoryLen, const size_t& oldCrc32);

		/*�˺�������Ľ������ȷ,��Ҫʹ��*/
		bool crcVerify(const uchar* data, const size_t& length, const size_t& oldCrc32);

		/*��ȡ�ļ��б�,����1*/
		bool getFileUrl(QString& url, const DvrTypes::FilePath& filePath, const QString& address, const ushort& port);

		/*��ȡ�ļ��б�,����2*/
		bool getFileUrl(QString& url, const DvrTypes::FilePath& filePath);

		/*���ؽ���¼���ļ�,����1*/
		bool downloadFile(const QString& url, const QString& dirName, bool isVideo = true);

		/*���ؽ���¼���ļ�,����2*/
		bool downloadFile(const QString& url, const DvrTypes::FileType& types);

		/*���������ļ�Ŀ¼*/
		void setDownloadFileDir(const DvrTypes::FileType& types, const QString& dirName);

		/*���DVR����*/
		bool checkRayAxis(const QString& url, const QString& dirName);

		/*��ȡDVR�����*/
		bool checkSfr(const QString& url, const QString& dirName);

		/*
		 *�����ἰͼ�������
		 *@param1,���ձ���
		 *@param2,������ʱ
		 *@param3,���ͱ�������
		 *@param4,���Ĵ���
		 *@param5,���ձ���ID
		 *@param6,������
		 *@param7,lambda
		 *@return,bool
		*/
		bool checkRayAxisSfr(const MsgNode& msg, const int& delay, const SendType& st, const int& count, const int& id, const int& req, MsgProc proc);

		/*��ʽ��DVR SD��*/
		bool formatSdCard(const DvrTypes::FormatSdCard& flag);

		/*���õ�ַ�˿�*/
		void setAddressPort(const QString& address, const ushort& port);
	protected:
		/*������д�߳�*/
		virtual void run() override = 0;

		/*д��������־*/
		bool writeNetLog(const char* name, const char* data, const size_t& size);
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
	private:
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
		Tap(QObject* parent = nullptr);

		~Tap();

		virtual bool initInstance();

		virtual bool openDevice();

		virtual bool closeDevice();

		bool checkUSBByJson(const QString& url = "http://172.19.1.2:20001/info");
	protected:
		virtual void run() override = 0;
	private:
		/*���ڹ���*/
		SerialPortTool* m_serialPortTool = nullptr;

		/*TAP������Ļ����*/
		void screenUartHandler(const QString& port,const QByteArray& bytes);
	};

	/************************************************************************/
	/* Module Class                                                         */
	/************************************************************************/
	class Module : public Dvr {
		Q_OBJECT
	public:
		Module(QObject* parent = nullptr);

		~Module();
	protected:

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
			m_canConnMgr->ClearRecBuffer();
			for (;;)
			{
				int size = m_canConnMgr->QuickReceive(msg, 512, 100);
				for (int i = 0; i < size; i++)
				{
					if (msg[i].id == 0x5A0)
					{
						if (statusCode == m_hashCode.systemStatus)
						{
							status = static_cast<T>(msg[i].ucData[0] & 0x07);
							switch (status)
							{
							case DvrTypes::SS_INITIALIZING:setCurrentStatus("ϵͳ��ʼ����", true); break;
							case DvrTypes::SS_GENERAL_RECORD:setCurrentStatus("����¼��", true); break;
							case DvrTypes::SS_PAUSE_RECORD:setCurrentStatus("��ͣ¼��[ȱ��SD��]", true); break;
							case DvrTypes::SS_HARDWARE_KEY:setCurrentStatus("����¼�� ����", true); break;
							case DvrTypes::SS_CRASH_KEY:setCurrentStatus("����¼�� ��ײ", true); break;
							case DvrTypes::SS_UPDATE_MODE:setCurrentStatus("����ģʽ", true); break;
							case DvrTypes::SS_ERROR:setCurrentStatus("ϵͳ����", true); break;
							default:break;
							}
						}
						else if (statusCode == m_hashCode.wifiStatus)
						{
							status = static_cast<T>((msg[i].ucData[0] >> 4) & 0x07);
							switch (status)
							{
							case DvrTypes::WS_CLOSE:setCurrentStatus("WIFI�ѹر�", true); break;
							case DvrTypes::WS_INIT:setCurrentStatus("WIFI���ڳ�ʼ��", true); break;
							case DvrTypes::WS_NORMAL:setCurrentStatus("WIFI����", true); break;
							case DvrTypes::WS_CONNECT:setCurrentStatus("WIFI��������", true); break;
							case DvrTypes::WS_ERROR:setCurrentStatus("WIFI����", true); break;
							default:break;
							}
						}
						else if (statusCode == m_hashCode.ethernetStatus)
						{
							if (m_dvrClient->connectServer("10.0.0.10", 2000, 30))
							{
								setCurrentStatus("��̫��������", true);
								status = static_cast<T>(DvrTypes::ES_CONNECT);
							}
							else
							{
								setCurrentStatus("��̫��δ����", true);
								status = static_cast<T>(DvrTypes::ES_ERROR);
							}
							m_dvrClient->closeConnect();
						}
						else if (statusCode == m_hashCode.sdCardStatus)
						{
							status = static_cast<T>(msg[i].ucData[1] & 0x07);
							switch (status)
							{
							case DvrTypes::SCS_NORMAL:setCurrentStatus("SD������", true); break;
							case DvrTypes::SCS_NO_SD:setCurrentStatus("�����SD��", true); break;
							case DvrTypes::SCS_ERROR:setCurrentStatus("SD������", true); break;
							default:break;
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

	template<class T> inline bool Dvr::autoProcessStatus()
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
					setLastError("δ�յ�CAN����,���鹩��");
					break;
				}

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
					if (status == static_cast<T>(DvrTypes::WS_NORMAL))
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

				if (success || GetTickCount() - startTime >= 20000)
				{
					setLastError("δ��ȡ���κ�״̬,��ȷ�ϲ�ƷCAN�����Ƿ���ȷ");
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

/*Network transmission*/
namespace Nt {
	/************************************************************************/
	/* SFR�����,������SFR APP����ͨѶ,�˴����������                       */
	/************************************************************************/
	class SfrServer {
	public:
		SOCKET m_socket;

		SOCKET m_client;

		sockaddr_in m_sockAddr;

		QString m_lastError = "No error";

		bool m_quit = false;
	protected:
		void setLastError(const QString& err);
	public:
		SfrServer();

		~SfrServer();

		bool startListen(const ushort& port = 2000);

		bool getSfr(const QString& filePath, float& sfr);

		int send(SOCKET socket, char* buffer, int len);

		int recv(SOCKET socket, char* buffer, int len);

		void closeServer();

		const QString& getLastError();
	};

	/************************************************************************/
	/* DVR�ͻ���,������DVR�����ͨѶ                                        */
	/************************************************************************/
	class DvrClient {
	private:
		QString m_lastError = "No error";

		SOCKET m_socket;

		SOCKADDR_IN m_sockAddr;

		char m_ipAddr[32] = {};

		ushort m_port;

		bool m_init = false;

		bool m_close = false;
	protected:
		void setLastError(const QString& err);
	public:
		DvrClient();

		~DvrClient();

		bool connectServer(const char* ip, const ushort& port,const int& count = 10);

		int send(char* buffer, int len);

		int recv(char* buffer, const uchar& cmd, const uchar& sub);

		const char* getIpAddr();

		const ushort& getPort();

		void closeConnect();

		const QString& getLastError();
	};
}

