#pragma once

/*
* Qt5.7.0 VS2019 ����汾,2.4.3(rev.7)
*/

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

#include <QStandardPaths>

#include <QTcpSocket>

#include <VoltageTestMgr/VoltageTestMgr.h>
#pragma comment(lib, "VoltageTestMgr.lib")

#include <MR-DO16-KNMgr/MR-DO16-KNMgr.h>
#pragma comment(lib, "MR-DO16-KNMgr.lib")

#include <ItechSCPIMgr/ItechSCPIMgr.h>
#pragma comment(lib, "ItechSCPIMgr.lib")

#include <Can/Can.h>
#pragma comment(lib, "Can.lib")

#include <Uds/Uds.h>
#pragma comment(lib, "Uds.lib")

#include <StaticCurrentMgr/StaticCurrentMgr.h>
#pragma comment(lib, "StaticCurrentMgr.lib")

#include <MV800Mgr/MV800Mgr.h>
#pragma comment(lib, "MV800Mgr.lib")

#include <WifiMgr/WifiMgr.h>
#pragma comment(lib, "WifiMgr.lib")

#include <TSCPrinterMgr/TSCPrinterMgr.h>
#pragma comment(lib, "TSCPrinterMgr.lib")

#include "QMessageBoxEx.h"

#include "QLabelEx.h"

#include "JsonTool.h"

#include "RayAxis.h"

#include "SerialPort.h"

#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
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
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <wincon.h>
#include <WinInet.h>
#pragma comment(lib,"wininet.lib")

#include <Urlmon.h>
#pragma comment(lib,"Urlmon.lib")

#include <shellapi.h>
#pragma comment(lib,"Shell32.lib")

#define CALL_PYTHON_LIB 0

#if CALL_PYTHON_LIB
#include "../DetectionExt/Lib/Python34/include/Python.h"
#endif

#define MY_KITS_PATH "C:\\Windows\\MyKits\\"

#define OVERRIDE_INIT_INSTANCE virtual bool initInstance

/************************************************************************/
/* Define                                                               */
/************************************************************************/
#define S_TO_Q_STR(X) QString::fromStdString(X)

#define WS_TO_Q_STR(X) QString::fromStdWString(X)

#define WC_TO_Q_STR QString::fromWCharArray

#define Q_TO_C_STR(X) X.toStdString().c_str()

#define Q_TO_C_LEN(X) X.toStdString().length()

#define Q_TO_WC_STR(X) X.toStdWString().c_str()

#define Q_TO_WC_LEN(X) X.toStdWString().length()

#define G_TO_Q_STR(X) QString::fromLocal8Bit(X)

#define G_TO_C_STR(X) Q_TO_C_STR(G_TO_Q_STR(X))

#define N_TO_Q_STR QString::number

#define SU_FA(X) X ? "�ɹ�":"ʧ��"

#define OK_NG(X) X ? "OK" : "NG"

#define Q_SPRINTF(format,...) QString().sprintf(format,##__VA_ARGS__)

#define DEC_TO_HEX(num) Q_SPRINTF("%d",num).toInt(nullptr,16)

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

/*
* @GET_FNC_NAME,��ȡ������
* @param1,����
* @return,const char*
*/
#define GET_FNC_NAME(X) #X

/*
* @BUFF_SIZE,��������С
* @return,int
*/
#define BUFF_SIZE 0x1000

/*
* @NO_THROW_NEW,���״�new
* @return,void*
*/
#define NO_THROW_NEW new(std::nothrow)

/*
* @GET_LOG_DIR,��ȡ��־Ŀ¼
* @return,const char*
*/
#define GET_LOG_DIR() ((Dt::Base::getDetectionType() == BaseTypes::DT_HARDWARE) ? "HwLog" : "FcLog")

/*
* @GET_TYPE_NAME,��ȡ��������
* @return,QString
*/
#define GET_TYPE_NAME() GET_JSON()->getDeviceConfigValue("��������")

/*
* @START_DELAY,������ʱ
* @return,ulong
*/
#define START_DELAY (ulong)GET_JSON()->getParsedThresholdConfig().startDelay

/*
* @TYPE_NAME_IS,�жϻ������Ƿ�ΪNAME
* @param1,�Աȵ�����
* @return,bool
*/
#define TYPE_NAME_IS(NAME) (GET_TYPE_NAME() == NAME)

#define AUTO_RESIZE(X) X->resize(QApplication::desktop()->screenGeometry().width() / 2 + 100,\
QApplication::desktop()->screenGeometry().height() / 2 + 100)

#define FVAL val

#define FMSG msg

#define FSIZE size

#define FDATA data

#define MSG_PROC_FNC(...) [__VA_ARGS__](int FVAL,const MsgNode& FMSG)mutable->bool

#define CAN_PROC_FNC(...) [__VA_ARGS__](int FVAL,const MsgNode& FMSG)mutable->bool

#define UDS_PROC_FNC(...) [__VA_ARGS__](int FVAL,int FSIZE,const uchar* FDATA)mutable->bool

#define UDS_PROC_FNC_EX(...) [__VA_ARGS__](ValList FREQL,int FSIZE,const uchar* FDATA)mutable->bool

#define TEST_PASS []()->bool{ return true; }

typedef const std::initializer_list<int>& IdList;

typedef const std::initializer_list<int>& ValList;

typedef const std::initializer_list<CanMsg>& CanList;

typedef const std::initializer_list<MsgNode>& MsgList;

typedef const std::initializer_list<uchar>& DidList;

typedef const std::function<bool(int, const MsgNode&)>& MsgProc;

typedef const std::function<bool(int, int, const uchar* data)>& UdsProc;

typedef const std::function<bool(ValList, int, const uchar* data)>& UdsProcEx;

typedef MsgProc CanProc;

struct MsgProcInfo {
	int id;
	int val;
	MsgProc proc;
};

struct CanProcInfo {
	int id;
	int val;
	CanProc proc;
};

struct MsgProcInfoEx {
	IdList idList;
	ValList valList;
	MsgProc proc;
};

struct CanProcInfoEx {
	IdList idList;
	ValList valList;
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
	TS_CHECK_DATE,
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
	TS_CHECK_NETWORK,
	TS_CHECK_PING,
	TS_PRINT_LABEL,
	TS_CHECK_OTHER0,
	TS_CHECK_OTHER1,
	TS_CHECK_OTHER2,
	TS_CHECK_OTHER3,
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
	if ((CURR == TS_CHECK_SN || CURR == TS_WRITE_SN) && !GET_JSON()->getSkipItem(SI_SN))\
	{\
		\
	}\
	else if ((CURR == TS_CHECK_DATE || CURR == TS_WRITE_DATE) && !GET_JSON()->getSkipItem(SI_DATE))\
	{\
		\
	}\
	else\
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
	enum DetectionType { DT_AVM, DT_DVR, DT_HARDWARE, DT_TAP, DT_MODULE };

	/*���Խ��*/
	enum TestResult { TR_NO, TR_OK, TR_NG, TR_TS };

	/*�����־*/
	enum DetectionLog { DL_CUR, DL_RES, DL_VOL, DL_VER, DL_DTC, DL_ALL };
}

namespace HwTypes {

}

namespace FcTypes {
	/*��������*/
	enum RectType 
	{ 
		RT_FRONT_BIG,
		RT_REAR_BIG,
		RT_LEFT_BIG,
		RT_RIGHT_BIG,
		RT_SMALL,
		RT_NO
	};

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
		SSM_BAIC_C62X = 0x5A0,
		SSM_CHJ_M01 = 0x514,
	};

	/*���Ž��*/
	enum PlayResult {
		PR_OK,
		PR_NG,
		PR_REPLAY
	};

	/*��������*/
	enum NetworkTypes { NT_WIFI, NT_ETHERNET };

	/*DVRϵͳ״̬*/
	enum SystemStatus { SS_INITIALIZING, SS_GENERAL_RECORD, SS_PAUSE_RECORD,
		SS_HARDWARE_KEY, SS_CRASH_KEY, SS_UPDATE_MODE, SS_ERROR, };

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
	enum NetCmd 
	{ 
		//��������
		NC_CONNECTION,

		//�û��������
		NC_UI_CONTROL,

		//���ٿ���
		NC_FAST_CONTROL,

		//�ļ�����
		NC_FILE_CONTROL = 0x10,

		//��ȡϵͳ����
		NC_CONFIG_GET,

		//����ϵͳ����
		NC_CONFIG_SET,

		//��������
		NC_OTA_CONTROL,

		//������Ӧ,�����ѹ��
		NC_OTA_RESPOND,
	};

	/*��������2*/
	enum NetSub 
	{ 
		//NC_CONNECTION
		NS_HANDSHAKE = 0x00,
		NS_PAIRING_REQS = 0x01,
		NS_PAIRING_CHECK = 0x02,
		NS_HEART = 0x10,
		NS_AP_CONNECT_REQS = 0x20,

		//NC_UI_CONTROL
		NS_UI_REQ_PREVIEW = 0x00,
		NS_UI_REQ_FILES = 0x01,
		NS_UI_REQ_CONFIG = 0x02,
		NS_UI_REQ_PLAYBACK = 0x03,

		//NC_FAST_CONTROL
		NS_FAST_CYCLE_RECORD = 0x00,
		NS_FAST_EMERGE = 0x10,
		NS_FAST_PHOTOGRAPHY = 0x11,

		//NC_FILE_CONTROL
		NS_GET_SDCARD_STATUS = 0x00,
		NS_GET_FILE_LIST = 0x02,
		NS_REQ_FILE_DELETE = 0x21,
		NS_REQ_FILE_SAVING = 0x22,

		//NC_CONFIG_GET
		NS_GET_RECORD_CFG = 0x00,
		NS_GET_WIFI_CFG = 0x01,
		NS_GET_VERSION = 0x20,

		//NC_CONFIG_SET
		NS_SET_RESOLUTION = 0x00,
		NS_SET_DURATION = 0x01,
		NS_SET_AUDIO_RECORD = 0x02,
		NS_SET_OSD = 0x04,
		NS_SET_WIFI_CFG = 0x05,
		NS_SDCARD_FORMATTING = 0x20,
		NS_FACTORY_RESET = 0x21,
		NS_SDCARD_UMOUNT = 0x22,

		//NC_OTA_CONTROL
		NS_FILE_DOWNLOAD = 0x00,

		//NC_OTA_RESPOND
		NS_DECOMPRESS_RES = 0x00,
	};

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
		0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,
		0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,
		0xE7B82D07,0x90BF1D91,0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,
		0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,
		0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,0x3B6E20C8,0x4C69105E,0xD56041E4,
		0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,
		0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,0x26D930AC,
		0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,
		0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,
		0xB6662D3D,0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,
		0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,
		0x086D3D2D,0x91646C97,0xE6635C01,0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,
		0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,
		0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,0x4DB26158,0x3AB551CE,
		0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,
		0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
		0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,
		0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,
		0xB7BD5C3B,0xC0BA6CAD,0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,
		0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,
		0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,0xF00F9344,0x8708A3D2,0x1E01F268,
		0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,
		0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,0xD6D6A3E8,
		0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,
		0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,
		0x4669BE79,0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,
		0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,
		0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,
		0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,
		0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,0x86D3D2D4,0xF1D4E242,
		0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,
		0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
		0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,
		0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,
		0x47B2CF7F,0x30B5FFE9,0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,
		0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,
		0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D
	};
}

/*
* @Misc,��������ռ��а���һЩ���õĺ���
*/
namespace Misc {

	//���������ռ�
	namespace Var {
		static QString appendName;
	}

	//���߼�·���Ƿ����
	bool isExistKitsPath();

	//д�����д���
	bool writeRunError(const QString& error);

	/*Cvͼ��תQtͼ��[����1]*/
	bool cvImageToQtImage(IplImage* cv, QImage* qt);

	/*Cvͼ��תQtͼ��[����2]*/
	bool cvImageToQtImage(const Mat& mat, QImage& qt);

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

	/*��ȡAPP�汾��*/
	const QString getAppVersion();

	/*����APP������*/
	void setAppAppendName(const QString& name);

	/*��ȡAPP������*/
	const QString getAppAppendName();

	/*ͨ���汾��������APP*/
	bool renameAppByVersion(QWidget* widget);

	/*����Ӧ�ó���*/
	bool startApp(const QString& name, int show, bool absolutely = false);

	/*����Ӧ�ó���*/
	bool finishApp(const QString& name);

	/*��ȡ����ʱ��,WIN API*/
	const SYSTEMTIME getLocalTime();

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

	//������ݷ�ʽ
	bool createShortcut();

	/*
	* @ping,���������Ƿ�ͨ
	* @param1,IP��ַ
	* @param2,����
	* @return,bool
	*/
	bool ping(const QString& address, int times);

	/*
	* @ipLive,IP�Ƿ���
	* @param1,IP��ַ
	* @param2,�˿�
	* @param3,��ʱ
	* @return,bool
	*/
	bool ipLive(const QString& address, int port, int timeout);

	/*
	* @execute,ִ��
	* @param1,cmd����
	* @param2,����[��ѡ]
	* @return,bool
	*/
	bool execute(const QString& cmd, const QStringList& arguments = {});

	/*
	* @setIpAddress,����IP��ַ
	* @param1,������
	* @param2,IP��ַ
	* @param3,��������
	* @param4,����
	*/
	bool setIpAddress(int netCardId, QString ip, QString mask, QString gateway);

	/*
	* @ThemeFactory,���⹤����
	*/
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

	/*
	* @CharSet,����������UNICODEת����ASCII�ַ���
	*/
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

	/*
	* @UpdateSfr,���ڸ���SFR[Ŀǰ��û��ʵ��]
	*/
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
