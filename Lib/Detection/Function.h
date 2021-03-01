#pragma once

//���ݾɵ�MsgNode�ṹ��
//����������Ŀ�б���,δ����_MsgNode.xxxx
//��Ҫ�������Ŀ��#define USE_NEW_MSG_NODE
#ifndef USE_NEW_MSG_NODE
#define USE_NEW_MSG_NODE
#endif //!USE_NEW_MSG_NODE

#include "MainDlg.h"

/************************************************************************/
/* ����                                                                 */
/************************************************************************/

namespace Fnc {
	//����
	class GAC : public Dt::Avm {
		Q_OBJECT
	public:
		explicit GAC(QObject* parent = nullptr) {};

		~GAC() {};

		/*���ݾɳ���*/
		bool writeSn();

		bool checkSn();

		bool checkOldSn();

		bool writeSet(const uchar& value);
	protected:
		virtual void run() = 0;
	private:
		bool generateSn(uchar* data, int* const size = nullptr);
	};

	//����
	class BAIC : public Dt::Dvr {
		Q_OBJECT
	public:
		explicit BAIC(QObject* parent = nullptr);

		~BAIC();

		bool setOtherAction();

		bool checkSn();

		bool checkRecord();

	protected:
		virtual void run() = 0;
	private:

	};

	//���ͼ�
	class CHJAutoMotive : public Dt::Dvr {
		Q_OBJECT
	public:
		explicit CHJAutoMotive(QObject* parent = nullptr);

		~CHJAutoMotive();

		bool setOtherAction();

		bool checkSn();

		bool checkMaxCurrent();

		bool checkRecord(const ulong& timeout = 10000U);

		bool writeDate();

		static void getCurrentThread(void* args);
	protected:
		virtual void run() = 0;

		bool getWifiInfo(bool rawData = false, bool showLog = true);
	private:
		bool m_startThread = false;

		bool m_startGetCur = false;

		QVector<float> m_vector;
	};
}
