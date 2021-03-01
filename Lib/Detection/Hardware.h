#pragma once

//���ݾɵ�MsgNode�ṹ��
//����������Ŀ�б���,δ����_MsgNode.xxxx
//��Ҫ�������Ŀ��#define USE_NEW_MSG_NODE
#ifndef USE_NEW_MSG_NODE
#define USE_NEW_MSG_NODE
#endif //!USE_NEW_MSG_NODE

#include "MainDlg.h"

/************************************************************************/
/* Ӳ��                                                                 */
/************************************************************************/

namespace Hwd {
	//����
	class GAC : public Dt::Hardware {
		Q_OBJECT
	public:
		explicit GAC(QObject* parent = nullptr) {};

		~GAC() {};

		bool writeSn();

		bool writeOldSn();

		bool writeDate();
	protected:
		virtual void run() = 0;
	private:
	};

	//����
	class BAIC : public Dt::Hardware {
		Q_OBJECT
	public:
		explicit BAIC(QObject* parent = nullptr) {};

		~BAIC() {};
	protected:
		virtual void run() = 0;
	private:

	};

	//���ͼ�
	class CHJAutoMotive : public Dt::Hardware {
		Q_OBJECT
	public:
		explicit CHJAutoMotive(QObject* parent = nullptr) {};

		~CHJAutoMotive() {};
	protected:
		virtual void run() = 0;
	private:
	};
}
