#include "Hardware.h"

bool Hwd::GAC::writeSn()
{
	//VD331A5K0020B2202123
	//SE305A20002050220999
	setCurrentStatus("д�����к�");
	bool result = false, convert = false;
	do
	{
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

		RUN_BREAK(!writeDataByDid(0xf1, 0x8c, sendSize, sendData), "д�����к�ʧ��," + getUdsLastError());
		result = true;
	} while (false);
	WRITE_LOG("%s д�����к�", OK_NG(result));
	addListItemEx(Q_SPRINTF("д�����к� %s", OK_NG(result)));
	return result;
}
