#pragma once

/************************************************************************/
/* Include                                                              */
/************************************************************************/
#include <QStyleFactory>

#include <QApplication>

#include <QBitmap>

#include <QPainter>

#include <QThread>

#include <QDesktopWidget>

#include <QThread>

#include <QDateTime>

#include <QImage>

#include <QDebug>

#include <QProcess>

#include <QTimer>

#include <VoltageTestMgr/VoltageTestMgr.h>
#pragma comment(lib, "VoltageTestMgr.lib")

#include <MR-DO16-KNMgr/MR-DO16-KNMgr.h>
#pragma comment(lib, "MR-DO16-KNMgr.lib")

#include <UdsProtocolMgr/UdsFactory.h>
#pragma comment(lib, "UdsProtocolMgr.lib")

#include <ItechSCPIMgr/ItechSCPIMgr.h>
#pragma comment(lib, "ItechSCPIMgr.lib")

#include <CanMgr/ConnFactory.h>
#pragma comment(lib, "CanMgr.lib")

#include <StaticCurrentMgr/StaticCurrentMgr.h>
#pragma comment(lib, "StaticCurrentMgr.lib")

#include <MV800Mgr/MV800Mgr.h>
#pragma comment(lib, "MV800Mgr.lib")

#include <WifiMgr/WifiMgr.h>
#pragma comment(lib, "WifiMgr.lib")

#include "QMessageBoxEx.h"

#include "QLabelEx.h"

#include "JsonTool.h"

#include "RayAxis.h"

#include "CanMatrix.hpp"

#include "CanSender.h"

#include "SerialPortTool.h"

#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "urlmon.lib")

#include "../DetectionExt/Lib/OpenCv2.3.1/include/opencv/cv.h"
#include "../DetectionExt/Lib/OpenCv2.3.1/include/opencv/cxcore.h"
#include "../DetectionExt/Lib/OpenCv2.3.1/include/opencv/highgui.h"
#include "../DetectionExt/Lib/OpenCv2.3.1/include/opencv2/imgproc/imgproc_c.h"
#include "../DetectionExt/Lib/MIL/include/Mil.h"

#pragma comment(lib, "../DetectionExt/Lib/OpenCv2.3.1/lib/opencv_core231.lib")
#pragma comment(lib, "../DetectionExt/Lib/OpenCv2.3.1/lib/opencv_highgui231.lib")
#pragma comment(lib, "../DetectionExt/Lib/OpenCv2.3.1/lib/opencv_imgproc231.lib")
#pragma comment(lib, "../DetectionExt/Lib/MIL/lib/mil.lib")

using namespace cv;

#include "../DetectionExt/Lib/vlc-2.2.4/include/vlc.h"
#pragma comment(lib, "../DetectionExt/Lib/vlc-2.2.4/lib/libvlc.lib")
#pragma comment(lib, "../DetectionExt/Lib/vlc-2.2.4/lib/libvlccore.lib")

#include <process.h>
#include <wincon.h>
#include <WinInet.h>
#pragma comment(lib,"wininet.lib")


#define CALL_PYTHON_LIB 0

#if CALL_PYTHON_LIB
#include "../DetectionExt/Lib/Python34/include/Python.h"
#endif

#define MY_KITS_PATH "C:\\Windows\\MyKits\\"

/************************************************************************/
/* Define                                                               */
/************************************************************************/
#define S_TO_Q_STR(X) QString::fromStdString(X)

#define WS_TO_Q_STR(X) QString::fromStdWString(X)

#define WC_TO_Q_STR QString::fromWCharArray

#define Q_TO_C_STR(X) X.toStdString().c_str()

#define Q_TO_WC_STR(X) X.toStdWString().c_str()

#define G_TO_Q_STR(X) QString::fromLocal8Bit(X)

#define G_TO_C_STR(X) Q_TO_C_STR(G_TO_Q_STR(X))

#define N_TO_Q_STR QString::number

#define SU_FA(X) X ? "�ɹ�":"ʧ��"

#define OK_NG(X) X ? "OK" : "NG"

#define Q_SPRINTF(format,...) QString().sprintf(format,##__VA_ARGS__)

extern int* g_debugInfo;

static ulong __logCount = 0;

#define DEBUG_INFO()\
if (g_debugInfo && *g_debugInfo)\
	qDebug().noquote() << QString("%1 %2 %3 %4").arg(QString::number(++__logCount), 4, '0').arg(Misc::getCurrentTime(), \
	__FUNCTION__, QString::number(__LINE__))

#define DEBUG_INFO_EX(format,...) \
if (g_debugInfo && *g_debugInfo)\
	qDebug().noquote() << QString("%1 %2 %3 %4 %5").arg(QString::number(++__logCount), 4, '0').arg(Misc::getCurrentTime(), \
	__FUNCTION__, QString::number(__LINE__), Q_SPRINTF(format, ##__VA_ARGS__))

#define WRITE_LOG(format,...)\
m_logList.push_back(Q_SPRINTF(format,##__VA_ARGS__).replace(" ",","))

#define RUN_BREAK(success,error) \
if ((success))\
{\
	setLastError(error);\
	break;\
}

#define CYCLE_PROC_FNC(FNC,DELAY)\
{\
QTimer* timer = NO_THROW_NEW QTimer;\
if (timer)\
{\
QObject::connect(timer,&QTimer::timeout,FNC);\
timer->start(DELAY);\
}\
}

#define GET_FNC_NAME(X) #X

#define BUFF_SIZE 0x1000

#define NO_THROW_NEW new(std::nothrow)

#define GET_DT_DIR() ((Dt::Base::getDetectionType() == BaseTypes::DT_HARDWARE) ? "HwLog" : "FcLog")

#define GET_DT_TYPE() ((Dt::Base::getDetectionType() == BaseTypes::DT_HARDWARE) ? "Ӳ��" : "����")

#define AUTO_RESIZE(X) X->resize(QApplication::desktop()->screenGeometry().width() / 2 + 100,\
QApplication::desktop()->screenGeometry().height() / 2 + 100)

#define FREQ req

#define FMSG msg

#define FSIZE size

#define FDATA data

#define MSG_PROC_FNC(...) [__VA_ARGS__](const int& FREQ,const MsgNode& FMSG)mutable->bool

#define CAN_PROC_FNC(...) [__VA_ARGS__](const int& FREQ,const MsgNode& FMSG)mutable->bool

#define UDS_PROC_FNC(...) [__VA_ARGS__](const int& FREQ,const int& FSIZE,const uchar* FDATA)mutable->bool

#define TEST_PASS []()->bool{ return true; }

typedef const std::function<bool(const int&, const MsgNode&)>& MsgProc;

typedef const std::function<bool(const int&, const int&, const uchar* data)>& UdsProc;

typedef const std::initializer_list<int>& IdList;

typedef const std::initializer_list<int>& ReqList;

typedef const std::initializer_list<CanMsg>& CanList;

typedef const std::initializer_list<MsgNode>& MsgList;

typedef const std::initializer_list<uchar>& DidList;

typedef MsgProc CanProc;

struct MsgProcInfo {
	int id;
	int req;
	MsgProc proc;
};

struct CanProcInfo {
	int id;
	int req;
	CanProc proc;
};

struct MsgProcInfoEx {
	IdList idList;
	ReqList reqList;
	MsgProc proc;
};

struct CanProcInfoEx {
	IdList idList;
	ReqList reqList;
	CanProc proc;
};

enum TestSequence {
	TS_SCAN_CODE = 0x1000,
	TS_PREP_TEST,
	TS_CHECK_STATIC,
	TS_CHECK_ROUSE,
	TS_CHECK_VOLTAGE,
	TS_CHECK_CURRENT,
	TS_CHECK_VERSION,
	TS_WRITE_SN,
	TS_CHECK_SN,
	TS_WRITE_DATE,
	TS_SAVE_LOG,
	TS_CHECK_VIDEO,
	TS_CHECK_AVM,
	TS_TRIGGER_AVM,
	TS_CHECK_DVR,
	TS_CHECK_FRVIEW,
	TS_CHECK_LRVIEW,
	TS_CHECK_ALLVIEW,
	TS_CLEAR_DTC,
	TS_CHECK_DTC,
	TS_FORMAT_CARD,
	TS_UMOUNT_CARD,
	TS_CHECK_CARD,
	TS_POPUP_CARD,
	TS_CHECK_RECORD,
	TS_CHECK_RAYAXIS_SFR,
	TS_CHECK_RAYAXIS,
	TS_CHECK_SFR,
	TS_CHECK_PLAYBACK,
	TS_CHECK_UART,
	TS_CHECK_KEYVOL,
	TS_CHECK_LDW,
	TS_CHANGE_PWD,
	TS_CHECK_MINCUR,
	TS_CHECK_MAXCUR,
	TS_RESET_FACTORY,
	TS_WRITE_SET,
	TS_WAIT_STARTUP,
	TS_CAN_PROC0,
	TS_CAN_PROC1,
	TS_CAN_PROC2,
	TS_CAN_PROC3,
	TS_CAN_PROC4,
	TS_CAN_PROC5,
	TS_CAN_PROC6,
	TS_CAN_PROC7,
	TS_CAN_PROC8,
	TS_CAN_PROC9,
	TS_UDS_PROC0,
	TS_UDS_PROC1,
	TS_UDS_PROC2,
	TS_UDS_PROC3,
	TS_UDS_PROC4,
	TS_UDS_PROC5,
	TS_UDS_PROC6,
	TS_UDS_PROC7,
	TS_UDS_PROC8,
	TS_UDS_PROC9,
	TS_NO
};

#define TS_SUCCESS success

#define GO_SAVE_LOG()\
success = false;\
m_testSequence = TS_SAVE_LOG;\
break

#define GO_NEXT_TEST(NEXT)\
success = true;\
m_testSequence = NEXT;\
break

template<class T> static void _assertProcCall(const T& arg)
{
	(static_cast<const std::function<void()>&>(arg))();
}

template<class T, class ...args> static void _assertProcCall(const T& fnc, args... arg)
{ 
	_assertProcCall(arg...);
}

#define ASSERT_PROC_FNC(...) [__VA_ARGS__]()mutable->void

/*
* @ASSERT_TEST
* @param1,��ǰ��������
* @param2,��ǰ���Ժ���
* @param3,�¸���������
* @param4,���Ӻ���,ASSERT_PROC_FNC(){},��ѡ
*/
#define ASSERT_TEST(CURR,FNC,NEXT,...)\
case CURR:\
{\
	if (!FNC)\
	{\
		if (CURR == TS_SAVE_LOG)\
		{\
			setMessageBox("������־ʧ��",getLastError());\
		}\
		else\
		{\
			GO_SAVE_LOG();\
		}\
	}\
	_assertProcCall(ASSERT_PROC_FNC(){},__VA_ARGS__);\
	GO_NEXT_TEST(NEXT);\
}

#define TEST_THREAD(FNC)\
bool success = true;\
while(!m_quit)\
{\
	if (m_connect)\
	{\
		switch(m_testSequence)\
		{\
			FNC\
			default:break;\
		}\
	}\
	msleep(100);\
}

#define TEST_THREAD_EX(FNC)\
bool success = true;\
while(!m_quit)\
{\
	if (m_connect)\
	{\
		switch(m_testSequence)\
		{\
			ASSERT_TEST(TS_SCAN_CODE, setScanCodeDlg(), TS_PREP_TEST);\
			FNC\
			ASSERT_TEST(TS_SAVE_LOG, saveLog(TS_SUCCESS), TS_SCAN_CODE);\
			default:break;\
		}\
	}\
	msleep(100);\
}

namespace BaseTypes {
	/*
	* ������Ϣ
	* @param1,�Ի������
	* @param2,URL����
	* @param3,����·��
	* @param4,�ļ���С
	* @param5,����ʱ��
	* @param6,ƽ���ٶ�
	* @param7,��������
	* @param8,���
	*/
	struct DownloadInfo {
		QString title;
		QString url;
		QString path;
		float size;
		ulong time;
		float speed;
		QString error;
		bool result;
	};

	/*�������*/
	enum DetectionType { DT_AVM, DT_DVR, DT_HARDWARE, DT_TAP };

	/*���Խ��*/
	enum TestResult { TR_NO, TR_OK, TR_NG, TR_TS };

	/*�����־*/
	enum DetectionLog { DL_CUR, DL_RES, DL_VOL, DL_VER, DL_DTC, DL_ALL };
}

namespace HwTypes {

}

namespace FcTypes {
	/*��������*/
	enum RectType { RT_FRONT_BIG, RT_REAR_BIG, RT_LEFT_BIG, RT_RIGHT_BIG, RT_SMALL, RT_NO };

	/*�ɼ����ṹ��*/
	struct CardConfig {
		/*�ɼ�������*/
		QString name;

		/*�ɼ���ͨ����*/
		int channelCount;

		/*�ɼ���ͨ����*/
		int channelId;

		/*ͼ����*/
		int width;

		/*ͼ��߶�*/
		int height;

		/*ͼ���ܴ�С*/
		int size;
	};
}

namespace AvmTypes {

}

namespace DvrTypes {
	/*ϵͳ״̬����*/
	enum SysStatusMsg {
		SSM_BAIC = 0x5A0,
		SSM_CHJ = 0x514,
	};

	enum PlayResult {
		PR_OK,
		PR_NG,
		PR_REPLAY
	};

	/*��������*/
	enum NetworkTypes { NT_WIFI, NT_ETHERNET };

	/*DVRϵͳ״̬*/
	enum SystemStatus { SS_INITIALIZING, SS_GENERAL_RECORD, SS_PAUSE_RECORD, SS_HARDWARE_KEY, SS_CRASH_KEY, SS_UPDATE_MODE, SS_ERROR, };

	/*WIFI״̬*/
	enum WifiStatus { WS_CLOSE, WS_INIT, WS_NORMAL, WS_CONNECT, WS_ERROR, WS_CONNECTED };

	/*��̫��״̬*/
	enum EthernetStatus { ES_CONNECT, ES_ERROR, };

	/*SD��״̬*/
	enum SdCardStatus { SCS_NORMAL, SCS_NO_SD, SCS_ERROR, SCS_NOT_FORMAT, SCS_INSUFFICIENT, SCS_SPEED_LOW, SCS_USING, SCS_RESERVED, };

	/*�ļ�·��*/
	enum FilePath { FP_NOR, FP_EVT, FP_PHO, FP_D1, FP_ALL, };

	/*�ļ�����*/
	enum FileType { FT_PHOTO, FT_VIDEO };

	/*��ʽ��SD��*/
	enum FormatSdCard { FSC_BY_NETWORK, FSC_BY_CAN, FSC_BY_UDS, };

	/*��������1*/
	enum NetCmd { NC_CONNECT = 0x00, NC_FAST_CONTROL = 0x02,NC_FILE_CTRL = 0x10, NC_CONFIG_SET = 0x12 };

	/*��������2*/
	enum NetSub { NS_HEART = 0x10, NS_FAST_CYCLE_RECORD = 0x00, NS_GET_FILE_LIST = 0x02, NS_FORMAT_SD_CARD = 0x20 };

	/*�������*/
	enum NetErr {
		NE_OK = 0x00000000U,
		NE_PARA_CONST,
		NE_PARA_RANGE,
		NE_PARA_VALUE = 0x00000004U,
		NE_OVERTIME = 0x00000008U,
		NE_BUSY = 0x00000010U,
		NE_NOT_INIT = 0x00000020U,
		NE_NOT_SUPPORT = 0x00000040U,
		NE_BUFF_EMPTY = 0x00000080U,
		NE_BUFF_FULL = 0x00000100U,
		NE_HW_PER = 0x00000200U,
		NE_HW_IC = 0x00000400U,
		NE_ACCESS = 0x00000800U,
		NE_CHECK = 0x00001000U,
		NE_BUS_OFF = 0x00002000U,
		NE_ABORT = 0x00004000U,
		NE_OVERFLOW = 0x00008000U,
		NE_UNKNOW = 0x80000000U,
	};

	/*�����㷨��Կ*/
	static const size_t crc32Table[256] = { 
	};
}

namespace Misc {
	namespace Var {
		static QString appendName;
	}

	bool isExistKitsPath();

	bool writeRunError(const QString& error);

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
	const QString _getAppVersion();

	const QString getAppVersion();

	/*����APP������*/
	void setAppAppendName(const QString& name);

	/*��ȡAPP������*/
	const QString getAppAppendName();

	/*ͨ���汾��������APP*/
	bool renameAppByVersion(QWidget* widget);

	/*����Ӧ�ó���*/
	bool startApp(const QString& name, const int& show, bool absolutely = false);

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

	/*UNICODEת���ַ���*/
	const char* wideCharToMultiByte(const wchar_t* wide);

	/*QStringת���ַ���*/
	const char* qstringToMultiByte(const QString& str);

	bool saveBitmapToFile(HBITMAP hBitmap, const QString& fileName);

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

	class CharSet {
	public:
		inline CharSet() {}

		inline CharSet(const QString& str) { qstringToMultiByte(str); }

		inline CharSet(const wchar_t* str) { wideCharToMultiByte(str); }

		inline ~CharSet() { for (auto& x : m_list) SAFE_DELETE_A(x); }

		inline operator const char* () { return m_list.back(); }

		inline const char* getData() { return m_list.back(); }

		const char* wideCharToMultiByte(const wchar_t* wide)
		{
			do
			{
				int size = ::WideCharToMultiByte(CP_OEMCP, 0, wide, -1, NULL, 0, NULL, FALSE);
				if (size <= 0) break;
				char* buffer = NO_THROW_NEW char[size];
				m_list.append(buffer);
				if (!buffer) break;
				memset(buffer, 0x00, size);
				if (::WideCharToMultiByte(CP_OEMCP, 0, wide, -1, buffer, size, NULL, FALSE) <= 0) break;
			} while (false);
			return m_list.back();
		}

		const char* qstringToMultiByte(const QString& str)
		{
			do
			{
				wchar_t* buffer = NO_THROW_NEW wchar_t[(str.length() + 1) * 2];
				if (!buffer) break;
				memset(buffer, 0x00, (str.length() + 1) * 2);
				int size = str.toWCharArray(buffer);
				buffer[size] = '\0';
				wideCharToMultiByte(buffer);
				SAFE_DELETE_A(buffer);
			} while (false);
			return m_list.back();
		}
	private:
		QList<char*> m_list;
	};

	class UpdateSfr :public QThread {
		Q_OBJECT
	public:
		UpdateSfr(QObject* parent = nullptr) { }

		~UpdateSfr() { m_quit = true; if (this->isRunning()) { wait(1000); } }

		void startUpdate() { /*this->start();*/ m_start = true; }

		void stopUpdate() { m_start = false; }

		void setInterval(const int& interval) { m_interval = interval; }
	protected:
		void run()
		{
			DEBUG_INFO_EX("����SFR�߳�%lu������", (ulong)QThread::currentThreadId());
			while (!m_quit)
			{
				if (m_start)
					emit updateSfrSignal();
				msleep(m_interval);
			}
			DEBUG_INFO_EX("����SFR�߳�%lu���˳�", (ulong)QThread::currentThreadId());
		}
	private:
		bool m_start = false;

		bool m_quit = false;

		int m_interval = 100;
	signals:
		void updateSfrSignal();
	};
}
