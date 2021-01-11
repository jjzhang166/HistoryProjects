#include "Function.h"

Fnc::BAIC::BAIC(QObject* parent)
{
	setSysStatusMsg(DvrTypes::SSM_BAIC);
	setSdCardStatus(DvrTypes::SCS_NORMAL);
	setSystemStatus(DvrTypes::SS_GENERAL_RECORD);
	setAddressPort("10.0.0.10", 2000);
}

Fnc::BAIC::~BAIC()
{

}

bool Fnc::BAIC::setOtherAction()
{
	MsgNode msg = { 0 };
	msg.id = 0x511;
	msg.dlc = 8;
	SYSTEMTIME time;
	GetLocalTime(&time);

	/*����ʱ��ͬ��*/
	msg.data[0] = time.wSecond << 2;
	msg.data[1] = (0x1f & time.wHour) >> 3;
	msg.data[2] = (0x07 & time.wHour) << 5;
	msg.data[1] |= (0x3f & time.wMinute) << 2;
	msg.data[2] |= (0x1f & time.wDay) >> 3;
	msg.data[3] = (0x07 & time.wDay) << 5;
	msg.data[3] |= (0x1f & (time.wYear - 2014)) >> 4;
	msg.data[4] = (0x0f & (time.wYear - 2014)) << 4;
	msg.data[3] |= (0x0f & time.wMonth) << 1;
	m_canSender.AddMsg(msg, 1000);
	msleep(1000);
	return true;
}

Fnc::CHJAutoMotive::CHJAutoMotive(QObject* parent)
{
	setSysStatusMsg(DvrTypes::SSM_CHJ);
	setAddressPort("192.168.42.1", 2000); 
}

Fnc::CHJAutoMotive::~CHJAutoMotive()
{

}

bool Fnc::CHJAutoMotive::setOtherAction()
{
	bool result = false;
	do
	{
		m_canSender.AddMsg({ 0x440,8,{0} }, 100, ST_Event, 10);
		msleep(1000);
		m_canSender.AddMsg(SEND_PROC_FNC() {
			time_t tm = time(NULL) * 1000;
			uchar temp[6] = { 0 };
			memcpy(temp, &tm, 6);
			FMSG.id = 0x179;
			FMSG.dlc = 8;
			for (int i = 1; i <= 6; ++i)
				FMSG.data[i] = temp[6 - i];
			FMSG.data[7] = 0x21;
		}, 10);
		m_startGetCur = true;
		m_vector.clear();
		RUN_BREAK(0 >= (!m_startThread ? _beginthread(getCurrentThread, NULL, this) : true), "����getCurrentThreadʧ��");
		result = true;
	} while (false);
	return result;
}

bool Fnc::CHJAutoMotive::checkSn()
{
	setCurrentStatus("������к�");
	bool result = false;
	do
	{
		int size = 0;
		uchar readData[32] = { 0 }, cmpData[32] = { 0 };
		RUN_BREAK(!readDataByDid(0xf1, 0x8c, &size, readData), "��ȡ���к�ʧ��," + getUdsLastError());
		RUN_BREAK(size != 16, "���кų��ȴ���");
		sprintf((char*)cmpData, "M01 %s", Q_TO_C_STR(g_code.right(10)));
		RUN_BREAK(memcmp(readData, cmpData, 16), "���кŶԱ�ʧ��");
		result = true;
	} while (false);
	WRITE_LOG("%s ������к�", OK_NG(result));
	addListItemEx(Q_SPRINTF("������к� %s", OK_NG(result)));
	return result;
}

bool Fnc::CHJAutoMotive::checkMaxCurrent()
{
	setCurrentStatus("���������");
	bool result = false, success = false;
	float current = 0.0f, minRange = 0.0f, maxRange = 0.0f;
	do
	{
		RUN_BREAK(m_vector.empty(), "��ȡ�����б�Ϊ��");
		qSort(m_vector.begin(), m_vector.end());
		current = m_vector.back();
		minRange = m_defConfig->range.maxCurrent0;
		maxRange = m_defConfig->range.maxCurrent1;
		success = (current >= minRange && current <= maxRange);
		addListItem(Q_SPRINTF("���������  %.2fA  %.2fA~%.2fA  %s", current, minRange, maxRange, OK_NG(success)));
		RUN_BREAK(!success, "���������ʧ��");
		result = true;
	} while (false);
	m_startGetCur = false;
	WRITE_LOG("%s ��������� %.2f %.2f %.2f", OK_NG(result), current, minRange, maxRange);
	addListItem(Q_SPRINTF("��������� %s", OK_NG(result)), false);
	return result;
}

bool Fnc::CHJAutoMotive::checkRecord(const ulong& timeout)
{
	setCurrentStatus("������¼��");
	bool result = false, success = false;
	do
	{
		addListItem("������¼����,�����ĵȴ�...");
		m_canSender.AddMsg({ 0x39a,8,{0} }, 100, ST_Event, 3);
		msleep(300);
		m_canSender.AddMsg({ 0x39a,8,{0,0,0,0,2} }, 100, ST_Event, 3);
		msleep(300);
		RUN_BREAK(!setSoundLight(true), "������͵ƹ�ʧ��");
		success = autoProcessStatus(DvrTypes::SS_HARDWARE_KEY, timeout);
		addListItem(Q_SPRINTF("��������¼�� %s", OK_NG(success)));
		m_canSender.AddMsg({ 0x39a,8,{0} }, 100, ST_Event, 3);
		msleep(300);
		RUN_BREAK(!success, "��������¼��ʧ��");
		result = true;
	} while (false);
	m_canSender.DeleteOneMsg(0x39a);
	WRITE_LOG("%s ������¼��", OK_NG(success));
	addListItemEx(Q_SPRINTF("������¼�� %s", OK_NG(success)));
	return result;
}

bool Fnc::CHJAutoMotive::writeDate()
{
	setCurrentStatus("д����������");
	bool result = false;
	do
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		uchar data[32] = { 0 };
		data[0] = time.wYear % 100;
		data[1] = time.wMonth;
		data[2] = time.wDay;

		RUN_BREAK(!writeDataByDidEx({ 0x01, 0xFD, 0x00,0x00,0x00 }, 0xf1, 0x8b, 3, data), "д����������ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д����������", OK_NG(result));
	addListItemEx(Q_SPRINTF("д���������� %s", OK_NG(result)));
	return result;
}

void Fnc::CHJAutoMotive::getCurrentThread(void* args)
{
	DEBUG_INFO_EX("��ȡ�����߳�ID:%lu������", GetCurrentThreadId());
	Fnc::CHJAutoMotive* self = static_cast<Fnc::CHJAutoMotive*>(args);
	self->m_startThread = true;
	float current = 0.0f;
	while (!self->m_quit)
	{
		if (self->m_connect && self->m_startGetCur)
		{
			if (!self->m_power.GetCurrent(&current))
			{
				self->setLastError("getCurrentThread��ȡ����ʧ��");
				break;
			}
			self->m_vector.push_back(current);
		}
		msleep(100);
	}
	self->m_startThread = false;
	DEBUG_INFO_EX("��ȡ�����߳�ID:%lu���˳�", GetCurrentThreadId());
}

bool Fnc::CHJAutoMotive::getWifiInfo(bool rawData, bool showLog)
{
	bool result = false, success = false;
	do
	{
		memset(&m_wifiInfo, 0x00, sizeof(WIFIInfo));
		if (showLog) addListItem("���ڻ�ȡWIFI��Ϣ...");
		const int account = 0x515, password = 0x516;
		success = autoProcessCanMsgEx({ account ,password }, { 0 ,0 },
			MSG_PROC_FNC(&)
		{
			if (FMSG.id == account)
			{
				if (rawData)
				{
					memcpy(m_wifiInfo.account, FMSG.data, 8);
				}
				else
				{
					sprintf(m_wifiInfo.account, "M01_DVR_%02X%02X%02X%02X%02X%02X",
						FMSG.data[0], FMSG.data[1], FMSG.data[2],
						FMSG.data[3], FMSG.data[4], FMSG.data[5]);
				}
			}
			else
			{
				sprintf(m_wifiInfo.password, "%c%c%c%c%c%c%c%c",
					FMSG.data[0], FMSG.data[1], FMSG.data[2],
					FMSG.data[3], FMSG.data[4], FMSG.data[5],
					FMSG.data[6], FMSG.data[7]);
			}
			return true;
		}
		);
		strcpy(m_wifiInfo.mode, MODE_MANUAL);
		strcpy(m_wifiInfo.auth, AUTH_WPA2PSK);
		strcpy(m_wifiInfo.encrypt, ENCR_AES);
		if (showLog)
		{
			addListItem(Q_SPRINTF("��ȡWIFI��Ϣ %s", OK_NG(success)));
			addListItem(Q_SPRINTF("WIFI����:%s WIFI����:%s", m_wifiInfo.account, m_wifiInfo.password));
			addListItem(Q_SPRINTF("����ģʽ:%s ��֤��ʽ:%s ���ܷ�ʽ:%s", m_wifiInfo.mode, m_wifiInfo.auth, m_wifiInfo.encrypt));
		}

		if (!success)
			break;
		result = true;
	}
	while (false);
	return result;
}

bool Fnc::GAC::writeSn()
{
	//VD331A5K0020B2202123
	//SE305A20002050220999
	setCurrentStatus("д�����к�");
	bool result = false, convert = false;
	do
	{
		uchar data[32] = { 0 };
		int size = 0;
		RUN_BREAK(!generateSn(data, &size), "����Ϊ��");
		RUN_BREAK(!writeDataByDid(0xf1, 0x8c, size, data), "д�����к�ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д�����к�", OK_NG(result));
	addListItemEx(Q_SPRINTF("д�����к� %s", OK_NG(result)));
	return result;
}

bool Fnc::GAC::checkSn()
{
	setCurrentStatus("������к�");
	bool result = false, convert = false;
	do
	{
		uchar readData[32] = { 0 };
		int readSize = 0;
		RUN_BREAK(!readDataByDid(0xf1, 0x8c, &readSize, readData), "��ȡ���к�ʧ��," + getUdsLastError());
		RUN_BREAK(readSize != 16, "��ȡ���кų��Ȳ�����16");

		uchar cmpData[32] = { 0 };
		RUN_BREAK(!generateSn(cmpData), "����Ϊ��");
		RUN_BREAK(memcmp(readData, cmpData, 8), "���кŶԱ�ʧ��");
		result = true;
	} while (false);
	WRITE_LOG("%s ������к�", OK_NG(result));
	addListItemEx(Q_SPRINTF("������к� %s", OK_NG(result)));
	return result;
}

bool Fnc::GAC::generateSn(uchar* data, int* const size)
{
	bool result = false, convert = false;
	do
	{
		if (g_code.isEmpty())
			break;

		if (size)
			*size = 16;

		int&& seriesNumber = g_code.right(3).toInt();
		QString lineNumber = g_code.right(5).left(2);
		int&& year = g_code.right(10).left(2).toInt();
		int&& month = g_code.right(8).left(1).toInt(&convert, 16);
		RUN_BREAK(!convert || month > 12, "�������·ݲ����Ϲ���");

		int&& day = g_code.right(7).left(2).toInt();
		data[0] = seriesNumber >> 16;
		data[1] = seriesNumber >> 8;
		data[2] = seriesNumber;
		data[3] = Q_TO_C_STR(lineNumber)[0];
		data[4] = Q_TO_C_STR(lineNumber)[1];
		data[5] = year;
		data[6] = month;
		data[7] = day;
		result = true;
	} while (false);
	return result;
}
