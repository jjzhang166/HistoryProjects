#pragma once
/*
													�������С�

															�׾���

											�����ͷҹ�Ϳͣ���Ҷݶ����ɪɪ��
											����������ڴ����پ������޹��ҡ�
											���ɻ��ҽ��𣬱�ʱãã�����¡�

											����ˮ������������������Ͳ�����
											Ѱ�����ʵ���˭��������ͣ����١�
											�ƴ�������������ƻص��ؿ��硣
											ǧ����ʼ�������̱����ð����档
											ת�Ღ����������δ�����������顣
											������������˼������ƽ������־��
											��ü������������˵�����������¡�
											��£����Ĩ��������Ϊ���Ѻ����ۡ�
											���������缱�꣬С��������˽�
											�������д��ӵ�������С�������̡�
											���ݺ�ﻨ�׻�������Ȫ�������ѡ�
											��Ȫ��ɬ��������������ͨ����Ъ��
											�����ĳ��������ʱ����ʤ������
											��ƿէ��ˮ���ţ�����ͻ����ǹ����
											�����ղ����Ļ�������һ�����Ѳ���
											�������������ԣ�Ψ���������°ס�

											�����Ų������У��������������ݡ�
											���Ա��Ǿ���Ů������Ϻ�����ס��
											ʮ��ѧ�����óɣ������̷���һ����
											���������Ʋŷ���ױ��ÿ������ʡ�
											������������ͷ��һ����篲�֪����
											��ͷ���������飬Ѫɫ��ȹ�����ۡ�
											���껶Ц�����꣬���´�����жȡ�
											���ߴӾ���������ĺȥ������ɫ�ʡ�
											��ǰ���䰰��ϡ���ϴ�������˸���
											������������룬ǰ�¸������ȥ��
											ȥ�������ؿմ����ƴ�������ˮ����
											ҹ����������£�����ױ������ɡ�

											����������̾Ϣ�����Ŵ���������
											ͬ�����������ˣ����α�����ʶ��
											�Ҵ�ȥ��ǵ۾����ؾ��Բ�����ǡ�
											�����Ƨ�����֣����겻��˿������
											ס���Խ��ص�ʪ����«������լ����
											��䵩ĺ�ź���ž���ѪԳ������
											������������ҹ������ȡ�ƻ����㡣
											����ɽ�����ѣ�Ż�Ƴ�����Ϊ����
											��ҹ�ž�������������ֶ�������
											Ī�Ǹ�����һ����Ϊ�����������С�

											���Ҵ�����������ȴ��������ת����
											���಻����ǰ�����������Ž�������
											��������˭��ࣿ����˾������ʪ��
*/

/************************************************************************/
/* ��C++����ΪӲ�� ����[AVM DVR TAP]�����		*/
/* ������		  :Qt													*/
/* VS_QT���		  :V2.3.3(�˰汾��Ҫȥ����,���ѡ��ģ��,û��ȷ����ť��ѡ�س�����)*/
/* ����			  :���̲�												*/
/* ��ʹ�þ�̬�⿪����:¬����												*/
/* ��ܿ�����		  :����													*/
/************************************************************************/

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
//#include <QtNetwork/QNetworkAccessManager>
//
//#include <QtNetwork/QNetworkRequest>
//
//#include <QtNetwork/QNetworkReply>

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

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <imgproc\imgproc_c.h>

#pragma comment(lib, "opencv_core231.lib")
#pragma comment(lib, "opencv_highgui231.lib")
#pragma comment(lib, "opencv_imgproc231.lib")
using namespace cv;

#include <StaticCurrentMgr/StaticCurrentMgr.h>
#pragma comment(lib,"StaticCurrentMgr.lib")

#include <Mil.h>
#pragma comment(lib, "mil.lib")
#include <MV800Mgr/MV800Mgr.h>
#pragma comment(lib, "MV800Mgr.lib")

#include "QMessageBoxEx.h"

#include "QLabelEx.h"

#include "CanMatrix.h"

#include "JsonTool.h"

#include "RayAxis.h"

#include "CanSender.h"

#include "SerialPortTool.h"

#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "version.lib")

#include <vlc/vlc.h>
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")
#pragma comment(lib, "urlmon.lib")

/************************************************************************/
/* Define                                                               */
/************************************************************************/
#define S_TO_Q_STR(X) QString::fromStdString(X)

#define WS_TO_Q_STR(X) QString::fromStdWString(X)

#define Q_TO_C_STR(X) X.toStdString().c_str()

#define Q_TO_WC_STR(X) X.toStdWString().c_str()

#define G_TO_Q_STR(X) QString::fromLocal8Bit(X)

#define G_TO_C_STR(X) Q_TO_C_STR(G_TO_Q_STR(X))

#define OK_NG(X) X ? "OK" : "NG"

#define Q_SPRINTF(format,...) QString().sprintf(format,__VA_ARGS__)

#define OUTPUT_DEBUG_INFO(format,...) \
static ulong count = 0;\
if (Dt::Base::getOutputRunLog())\
qDebug() << QString("%1 %2 %3 %4 %5").arg(QString::number(++count),4,'0').arg(Misc::getCurrentTime(),\
__FUNCTION__,QString::number(__LINE__),Q_SPRINTF(format,__VA_ARGS__))

#define WRITE_LOG(format,...)\
Dt::Base::m_logList.push_back(Q_SPRINTF(format,__VA_ARGS__).replace(" ",","))

#define RUN_BREAK(success,error) \
if ((success))\
{\
	Dt::Base::m_lastError = error;\
	break;\
}

#define CYCLE_OUTPUT(DELAY,FNC)\
QTimer* timer = new QTimer;\
QObject::connect(timer,&QTimer::timeout,FNC);\
timer->start(DELAY);

#define FUNC_NAME(X) #X

#define BUFF_SIZE 0x1000

#define NO_THROW_NEW new(std::nothrow)

#define GET_DETECTION_DIR(NAME) NAME == "����" ?"FcLog":"HwLog"

#define AUTO_RESIZE(X) X->resize(QApplication::desktop()->screenGeometry().width() / 2 + 100,\
QApplication::desktop()->screenGeometry().height() / 2 + 100)

typedef const std::function<bool(const int&, const MsgNode&)>& MsgProc;

typedef std::initializer_list<MsgNode>&& MsgList;

typedef const std::initializer_list<int>& ReqList;

/*��������ص�����*/
typedef bool (*LaunchProc)(void*);

typedef bool (*LaunchProcEx)(void*, const int&, MsgProc);

/*������ص�����*/
typedef bool (*RequestProc)(void*, const int&);

typedef bool (*RequestProcEx)(void*, const int&, MsgProc);

#define TS_SCAN_CODE 0x1024
#define TS_NO 0x2048

namespace BaseTypes {
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
	enum RectType { RT_FRONT_BIG, RT_REAR_BIG, RT_SMALL, RT_NO };
}

namespace AvmTypes {

}

namespace DvrTypes {
	/*DVRϵͳ״̬*/
	enum SystemStatus { SS_INITIALIZING, SS_GENERAL_RECORD, SS_PAUSE_RECORD, SS_HARDWARE_KEY, SS_CRASH_KEY, SS_UPDATE_MODE, SS_ERROR, };

	/*DVR WIFI״̬*/
	enum WifiStatus { WS_CLOSE, WS_INIT, WS_NORMAL, WS_CONNECT, WS_ERROR, };

	/*DVR��̫��״̬*/
	enum EthernetStatus { ES_CONNECT, ES_ERROR, };

	/*DVR SD��״̬*/
	enum SdCardStatus { SCS_NORMAL, SCS_NO_SD, SCS_ERROR, SCS_NOT_FORMAT, SCS_INSUFFICIENT, SCS_RESERVED, };

	/*DVR�ļ�����*/
	enum FilePath { FP_NOR, FP_EVT, FP_PHO, FP_D1, FP_ALL, };

	/*��ʽ��sd��*/
	enum FormatSdCard { FSC_BY_NETWORK, FSC_BY_CAN, FSC_BY_UDS, };

	enum NetCmd { NC_CONNECT = 0x00, NC_FAST_CONTROL = 0x02,NC_FILE_CTRL = 0x10, NC_CONFIG_SET = 0x12 };

	enum NetSub { NS_HEART = 0x10, NS_FAST_CYCLE_RECORD = 0x00, NS_GET_FILE_LIST = 0x02, NS_FORMAT_SD_CARD = 0x20 };

	/*�����㷨��Կ*/
	static const size_t crc32Table[256] = { 
	};
}

