#include "Hardware.h"

/************************************************************************/
/* ���� Class                                                           */
/************************************************************************/
bool Hwd::GAC::writeSn()
{
	//VD331A5K0020B2202123
	//SE305A20002050220999
	setCurrentStatus("д�����к�");
	bool result = false, convert = false;
	do
	{
		RUN_BREAK(g_code.isEmpty(), "����Ϊ��");
		int&& seriesNumber = g_code.right(3).toInt();
		QString lineNumber = g_code.right(5).left(2);
		int&& year = g_code.right(10).left(2).toInt();
		int&& month = g_code.right(8).left(1).toInt(&convert, 16);
		RUN_BREAK(!convert || month > 12, "�������·ݲ����Ϲ���");

		int&& day = g_code.right(7).left(2).toInt();
		int sendSize = 16;
		uchar sendData[32] = { 0 };
		sendData[0] = seriesNumber >> 16;
		sendData[1] = seriesNumber >> 8;
		sendData[2] = seriesNumber;
		sendData[3] = Q_TO_C_STR(lineNumber)[0];
		sendData[4] = Q_TO_C_STR(lineNumber)[1];
		sendData[5] = year;
		sendData[6] = month;
		sendData[7] = day;

		RUN_BREAK(!writeDataByDid(0xf1, 0x8c, sendData, sendSize), "д�����к�ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д�����к�", OK_NG(result));
	addListItemEx(Q_SPRINTF("д�����к� %s", OK_NG(result)));
	return result;
}

bool Hwd::GAC::writeOldSn()
{
	setCurrentStatus("д�����к�");
	bool result = false;
	do
	{
		RUN_BREAK(g_code.isEmpty(), "����Ϊ��");

		RUN_BREAK(!writeDataByDid(0x0e, 0x01, (uchar*)Q_TO_C_STR(g_code), g_code.size()),
			"д�����к�ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д�����к�", OK_NG(result));
	addListItemEx(Q_SPRINTF("д�����к� %s", OK_NG(result)));
	return result;
}

bool Hwd::GAC::writeDate()
{
	setCurrentStatus("д����������");
	bool result = false;
	do
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		uchar data[4] = { 0 };
		data[0] = time.wYear / 100;
		data[1] = time.wYear % 100;
		data[2] = time.wMonth;
		data[3] = time.wDay;
		RUN_BREAK(!writeDataByDid(0xf1, 0x8b, data, 4), "д����������ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д����������", OK_NG(result));
	addListItemEx(Q_SPRINTF("д���������� %s", OK_NG(result)));
	return result;
}

bool Hwd::SGMW::writeSn()
{
	setCurrentStatus("д�����к�");
	bool result = false;
	do
	{
		RUN_BREAK(g_code.isEmpty(), "����Ϊ��");

		QString line = g_code.right(10).left(2);
		QString sn = g_code.right(4);

		uchar data[12] = { 0 };
		data[0] = Q_TO_C_STR(line)[0];
		data[1] = Q_TO_C_STR(line)[1];
		memcpy(&data[2], Q_TO_C_STR(sn), Q_TO_C_LEN(sn));

		RUN_BREAK(!writeDataByDid(0xf1, 0x8c, data, 7), "д�����к�ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д�����к�", OK_NG(result));
	addListItemEx(Q_SPRINTF("д�����к� %s", OK_NG(result)));
	return result;
}

bool Hwd::SGMW::writeDate()
{
	setCurrentStatus("д����������");
	bool result = false;
	do
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		uchar data[4] = { 0 };
		data[0] = DEC_TO_HEX(time.wYear / 100);
		data[1] = DEC_TO_HEX(time.wYear % 100);
		data[2] = DEC_TO_HEX(time.wMonth);
		data[3] = DEC_TO_HEX(time.wDay);
		RUN_BREAK(!writeDataByDid(0xf1, 0x8b, data, 4), "д����������ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д����������", OK_NG(result));
	addListItemEx(Q_SPRINTF("д���������� %s", OK_NG(result)));
	return result;
}
