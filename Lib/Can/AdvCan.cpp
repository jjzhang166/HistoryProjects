#include "AdvCan.h"
#include "AdvCAN/AdvCANIO.cpp"

AdvCan::AdvCan()
{

}

AdvCan::~AdvCan()
{
    clearBuffer();
    close();
}

bool AdvCan::open(int baudrate, int extFrame, int device, int port)
{
	bool result = false;
    do 
    {
		close();

        wchar_t name[64] = {0};
		swprintf(name, L"can%d", port);

		m_handle = acCanOpen(name, false);

        if (INVALID_HANDLE_VALUE == m_handle)
        {
			setLastError("��CAN��ʧ��");
			acCanClose(m_handle);
			break;
		}

		if (SUCCESS != acEnterResetMode(m_handle))
		{
			setLastError("����CAN��ʧ��");
			acCanClose(m_handle);
			break;
		}

		if (SUCCESS != acSetBaud(m_handle, baudrate))
		{
			setLastError("����CAN��������ʧ��");
			acCanClose(m_handle);
			break;
		}

		if (SUCCESS != acSetTimeOut(m_handle, 300, 300))
		{
			setLastError("����CAN����ʱʧ��");
			acCanClose(m_handle);
			break;
		}

		m_extFrame = extFrame;

		ULONG ulCode = 0;
		ULONG ulMask = 0;
		if (0 == m_extFrame)
		{
			ulMask = 0x001fffff;
			ulCode = m_recvID << 21;
		}
		else
		{
			ulMask = 0;
			ulCode = m_recvID << 3;
		}

		if (m_filter)
		{
			if (SUCCESS != acSetAcceptanceFilter(m_handle, ulCode, ulMask))
			{
				setLastError("����CAN��������ʧ��");
				acCanClose(m_handle);
				break;
			}
		}

		if (SUCCESS != acEnterWorkMode(m_handle))
		{
			setLastError("����CAN������ģʽʧ��");
			acCanClose(m_handle);
			break;
		}

		if (!clearBuffer())
		{
			acCanClose(m_handle);
			break;
		}

		m_open = true;
		result = true;
	} while (false);
	return result;
}

bool AdvCan::close()
{
    bool result = false;
    do 
    {
		m_open = false;

        if (SUCCESS != acCanClose(m_handle))
        {
			setLastError("�ر�CAN��ʧ��");
            break;
        }
        result = true;
    } while (false);
    return result;
}

bool AdvCan::clearBuffer()
{
    bool result = false;
    do 
    {
		m_message.clear();
        result = true;
	} while (false);
    return result;
}

bool AdvCan::send(const MsgNode *msg)
{
    bool result = false;
	OVERLAPPED ov = { 0 };
	ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    do 
    {
        if (!m_open)
        {
			setLastError("CAN��δ��");
            break;
        }

		canmsg_t can = { 0 };
        can.id = msg->id;
        can.length = getDlc(msg);
        can.cob = 0;
		memcpy(can.data, msg->data, getDlc(msg));
		if (m_extFrame)
		{
            can.flags = MSG_EXT;
        }

        ULONG writeCount = 0;
        if(SUCCESS != acCanWrite(m_handle, &can, 1, &writeCount, &ov))
        {
            if (m_debug)
            {
				OutputDebugStringW(L"����ʧ��\n");
			}

			saveLog("F", msg, 1);
            break;
        }

        const_cast<MsgNode*>(msg)->timeStamp = (float)m_timer.getEndTime();

		outputMsg("S", msg);

		saveLog("S", msg, 1);

        result = true;
    } while (false);
    CloseHandle(ov.hEvent);
    return result;
}

bool AdvCan::multiSend(const MsgNode* msg, int count)
{
    return false;
}

int AdvCan::receiveProtected(MsgNode* msg, int size, int ms)
{
	int count = 0;
	OVERLAPPED ov = { 0 };
	ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    do
    {
		canmsg_t can[512] = { 0 };
		Sleep(10);
		ULONG ulErrorCode = 0;
		if (acClearCommError(m_handle, &ulErrorCode) != SUCCESS)
		{
			setLastError("���CAN������ʧ��");
			break;
		}

		ULONG read = 0;
		if (SUCCESS != acCanRead(m_handle, can, 512, (ULONG*)&read, &ov))
		{
			setLastError("CAN�����ձ���ʧ��");
			break;
		}
		else
		{
			for (ULONG i = 0; i < read; i++)
			{
				msg[count].id = can[i].id;
				memcpy(msg[count].data, can[i].data, can[i].length);
				msg[count].timeStamp = (float)m_timer.getEndTime();
				if (++count >= size)
				{
					break;
				}
			}
			outputMsg("R", msg, count);
			saveLog("R", msg, count);
		}
		acClearRxFifo(m_handle);
	} while (false);
	CloseHandle(ov.hEvent);
	return count;
}


