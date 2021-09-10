#include "ZlgCan.h"

#include "ZlgCan/ControlCAN.h"
#pragma comment(lib, "ControlCAN.lib")

ZlgCan::ZlgCan()
{
	InitializeCriticalSection(&m_cs);

    m_accCode = 0;
    m_accMask = 0xffff;
}

ZlgCan::~ZlgCan()
{
    close();
    DeleteCriticalSection(&m_cs);
}

bool ZlgCan::initDevice(void* config)
{
    bool result = false;
    do 
    {
        if(VCI_OpenDevice(VCI_USBCAN2, m_device, m_port) != STATUS_OK)
        {
            setLastError("��CAN��ʧ��");
            break;
        }

        if(VCI_InitCAN(VCI_USBCAN2, m_device, m_port, (PVCI_INIT_CONFIG)config) != STATUS_OK)
        {
			setLastError("��ʼ��CAN��ʧ��");
            break;
        }

        if(VCI_StartCAN(VCI_USBCAN2, m_device, m_port) != STATUS_OK)
        {
			setLastError("����CAN��ʧ��");
            break;
        }

        if(VCI_ClearBuffer(VCI_USBCAN2, m_device, m_port) != STATUS_OK)
        {
			setLastError("���CAN������ʧ��");
            break;
        }

        result = true;
    } while (false);
    return result;
}

bool ZlgCan::open(int baudrate, int extframe, int device, int port)
{
    bool result = false;
    do 
    {
        m_baudrate = baudrate;
        m_extFrame = extframe;
        m_device = device;
        m_port = port;

		VCI_INIT_CONFIG config = { 0 };
        config.AccCode = m_accCode;
        config.AccMask = m_accMask;
        config.Filter = 0;
        config.Mode = 0;

        if (m_filter)
        {
            config.Filter = 1;
            if (0 == m_extFrame)
            {
                config.AccMask = 0x001fffff;
                config.AccCode = m_recvID << 21;
            }
            else
            {
                config.AccMask = 0;
                config.AccCode = m_recvID << 3;
            }
        }

        switch (baudrate)
        {
        case 100:
            config.Timing0 = 0x04;
            config.Timing1 = 0x1c;
            break;

        case 125:
            config.Timing0 = 0x03;
            config.Timing1 = 0x1c;
            break;

        case 250:
            config.Timing0 = 0x01;
            config.Timing1 = 0x1c;
            break;

        case 500:
            config.Timing0 = 0x00;
            config.Timing1 = 0x1c;
            break;

        case 1000:
            config.Timing0 = 0x00;
            config.Timing1 = 0x14;
            break;

        default:
			config.Timing0 = 0x00;
			config.Timing1 = 0x1c;
            break;
        }

        if(!initDevice((void*)&config))
        {
			VCI_CloseDevice(VCI_USBCAN2, m_device);
            break;
        }

        clearBuffer();
        result = true;
        m_open = true;
    } while (false);
    return result;
}

bool ZlgCan::close()
{
    stopReceiveThread();

    if(m_open)
    {
        m_open = false;
        VCI_CloseDevice(VCI_USBCAN2, m_device);
    }
	flushLogFile();
    return true;
}

bool ZlgCan::clearBuffer()
{
	VCI_ClearBuffer(VCI_USBCAN2, m_device, m_port);
    m_message.clear();
    return true;
}

bool ZlgCan::send(const MsgNode* msg)
{
    bool result = false;
    do 
    {
        if (!m_open)
        {
            setLastError("CAN��δ��");
            break;
        }

		VCI_CAN_OBJ obj = { 0 };
		obj.ID = msg->id;
		obj.DataLen = getDlc(msg);
		obj.ExternFlag = m_extFrame;
		memcpy(obj.Data, msg->data, getDlc(msg));

		EnterCriticalSection(&m_cs);
		int value = VCI_Transmit(VCI_USBCAN2, m_device, m_port, &obj, 1);
		LeaveCriticalSection(&m_cs);

		if (STATUS_OK != value)
		{
			if (m_debug)
			{
				OutputDebugStringW(L"����ʧ��\n");
			}

			saveLog("F", msg, 1);
            setLastError("���ͱ���ʧ��");
			break;
		}

        const_cast<MsgNode*>(msg)->timeStamp = (float)m_timer.getEndTime();

        outputMsg("S", msg);

		saveLog("S", msg, 1);

        result = true;
	} while (false);
	return result;
}

bool ZlgCan::multiSend(const MsgNode* msg, int count)
{
	bool result = false;
	do
	{
		if (!m_open)
		{
            setLastError("CAN��δ��");
			break;
		}

		VCI_CAN_OBJ obj[100] = { 0 };
        int _count = min(100, count);
        for (int i = 0; i < _count; i++)
        {
            obj[i].ID = msg[i].id;
            obj[i].DataLen = msg[i].dlc;
            obj[i].ExternFlag = msg[i].extFrame;
            memcpy(obj[i].Data, msg[i].data, msg[i].dlc);
        }

        EnterCriticalSection(&m_cs);
		int value = VCI_Transmit(VCI_USBCAN2, m_device, m_port, obj, _count);
        LeaveCriticalSection(&m_cs);

        if(_count != value)
        {
            if (m_debug)
            {
				OutputDebugStringW(L"����ʧ��\n");
			}
			saveLog("F", msg, _count);
            setLastError("���ձ���ʧ��");
            break;
        }

		for (int i = 0; i < _count; i++)
		{
			const_cast<MsgNode*>(msg + i)->timeStamp = (float)m_timer.getEndTime();
        }

        outputMsg("S", msg, _count);
		
        saveLog("S", msg, _count);

        result = true;
    } while (false);
	return result;
}

int ZlgCan::receiveProtected(MsgNode* msg, int size, int ms)
{
    int count = 0;
    do 
    {
        if (!m_open)
        {
			setLastError("CAN��δ��");
			break;
		}

		//VCI_ClearBuffer(VCI_USBCAN2, m_device, m_port);
  //      Sleep(100);
		VCI_CAN_OBJ obj[MAX_FRAME_COUNT] = { 0 };
        int value = VCI_Receive(VCI_USBCAN2, m_device, m_port, obj, MAX_FRAME_COUNT, ms);
        if(value <= 0)
        {
			setLastError("CAN�����ձ���ʧ��");
            break;
        }
        else
        {
            for(int i = 0; i < value; i++)
            {
                msg[count].id = obj[i].ID;
                msg[count].extFrame = (bool)obj[i].ExternFlag;
                msg[count].remFrame = (bool)obj[i].RemoteFlag;
                msg[count].dlc = obj[i].DataLen;
				memcpy(msg[count].data, obj[i].Data, obj[i].DataLen);

                msg[count].timeStamp = (float)m_timer.getEndTime();

				if (++count >= size)
                {
                    break;
                }
            }
			outputMsg("R", msg, count);
			saveLog("R", msg, count);
        }
    } while (false);
	return count;
}


