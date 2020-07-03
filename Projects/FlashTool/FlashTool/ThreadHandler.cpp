#include "ThreadHandler.h"

/************************************************************************/
/* ThreadHandler                                                        */
/************************************************************************/

/*�̵߳ȴ�*/
bool g_threadWait = false;

/*���ڱ��������б�*/
QStringList g_codeList;

/*���뱸���б�*/
static QStringList g_codeListBackup;

/*����������*/
static int g_restartCounter = 0;

/*�ظ������б�*/
extern QStringList g_repeatCodeList;

/************************************************************************/
/* public                                                               */
/************************************************************************/
ThreadHandler::ThreadHandler(QObject* parent)
	: QThread(parent)
{
#ifdef QT_DEBUG
	QTimer* timer = new QTimer;
	connect(timer, &QTimer::timeout, this, [&]() {m_jsonTool->printFileConfig(); });
	timer->start(5000);
#endif
}

ThreadHandler::~ThreadHandler()
{
	threadQuit();
	deleteWorkThread();
	if (isRunning())
	{
		wait(5000);
	}
}

void ThreadHandler::enableDebugMode(bool enable)
{
	m_debugMode = enable;
}

bool ThreadHandler::getDebugMode()
{
	return m_debugMode;
}

bool ThreadHandler::initInstance()
{
	bool result = false;
	do
	{
		m_jsonTool = JsonTool::getInstance();

		m_deviceConfig = m_jsonTool->getParsedDeviceConfig();
		
		m_hardwareConfig = m_jsonTool->getParsedHardwareConfig();

		m_thresholdConfig = m_jsonTool->getParsedThresholdConfig();

		m_relayConfig = m_jsonTool->getParsedRelayConfig();
		
		m_fileConfig = m_jsonTool->getParsedFileConfig();

		if (!getAardvarkPortAndSn().size())
		{
			/*û���豸����*/
		}
		else
		{
			if (!createWorkThread())
			{
				break;
			}
		}
		this->start();
		result = true;
	} while (false);
	return result;
}

bool ThreadHandler::openDevice()
{
	bool result = false;
	do
	{
		if (!openAardvarkDevice())
		{
			break;
		}

#ifdef QT_DEBUG
		m_debugMode = false;
#endif

		if (m_debugMode)
		{
			result = true;
			break;
		}

		if (m_deviceConfig->openPower.toInt())
		{
			bool success = true;

			if (m_deviceConfig->powerType == IT6302)
			{
				for (int i = 0; i < m_hardwareConfig->powerChannel; i++)
				{
					if (!m_power6302.Open(i + 1, m_hardwareConfig->powerPort, m_hardwareConfig->powerBaud, m_thresholdConfig->powerVoltage))
					{
						success = false;
						setLastError("��6302��Դʧ��");
						break;
					}

					if (m_deviceConfig->currentLimit.toInt())
					{
						if (!m_power6302.SetCurr(m_thresholdConfig->powerCurrent))
						{
							success = false;
							setLastError("6302��Դ��������ʧ��");
							break;
						}
					}
				}
			}
			else if (m_deviceConfig->powerType == IT6832)
			{
				if (!m_power6832.Open(m_hardwareConfig->powerPort, m_hardwareConfig->powerBaud, m_thresholdConfig->powerVoltage))
				{
					setLastError("��6832��Դʧ��");
					break;
				}

				if (m_deviceConfig->currentLimit.toInt())
				{
					if (!m_power6832.SetCurr(m_thresholdConfig->powerCurrent))
					{
						setLastError("6832��Դ��������ʧ��");
						break;
					}
				}
			}
			else
			{
				setLastError("��Ч�ĵ�Դ����");
				break;
			}

			if (!success)
			{
				break;
			}
			else
			{
				if (m_deviceConfig->powerType == IT6302)
				{
					m_power6302.Output(true);
				}
				else if (m_deviceConfig->powerType == IT6832)
				{
					m_power6832.Output(true);
				}
			}
		}

		if (m_deviceConfig->openRelay.toInt())
		{
			if (!m_relay.Open(m_hardwareConfig->relayPort, m_hardwareConfig->relayBaud))
			{
				setLastError("�򿪼̵���ʧ��");
				break;
			}
		}
		result = true;
	} while (false);
	return result;
}

bool ThreadHandler::closeDevice()
{
	bool result = false;
	do
	{
		closeAardvarkDevice();

		if (m_debugMode)
		{
			result = true;
			break;
		}

		if (m_deviceConfig->openPower.toInt())
		{
			if (m_deviceConfig->powerType == IT6302)
			{
				m_power6302.Output(false);
				m_power6302.Close();
			}
			else if (m_deviceConfig->powerType == IT6832)
			{
				m_power6832.Output(false);
				m_power6832.Close();
			}
		}

		if (m_deviceConfig->openRelay.toInt())
		{
			m_relay.SetAllIO(false);
			m_relay.Close();
		}
		result = true;
	} while (false);
	return result;
}

void ThreadHandler::setChannelCount(const int& count)
{
	m_channelCount = count;
}

void ThreadHandler::threadQuit()
{
	if (m_connect)
	{
		m_connect = false;
	}
	m_quit = true;
}

bool ThreadHandler::createWorkThread()
{
	bool result = false;
	do
	{
		m_workThread = new(std::nothrow) WorkThread[m_aardvarkCount];
		if (!m_workThread)
		{
			setLastError(QString("%1 m_workThread�����ڴ�ʧ��,\n������Ӧ�ó���.").arg(__FUNCTION__), true);
			break;
		}

		for (int i = 0, j = 0; i < m_aardvarkCount; i++)
		{
			m_workThread[i].setParent(reinterpret_cast<QObject*>(this));
			m_workThread[i].setAardvarkSn(m_aardvarkPortAndSn[i]);
			m_workThread[i].setPowerChannel(i + 1);
			m_workThread[i].setAardvarkPort(i);
			m_workThread[i].initInstance();

			for (; j < m_jsonTool->getRelayConfigCount(); j++)
			{
				if (m_relayConfig[j].enable)
				{
					m_workThread[i].setRelayPort(m_relayConfig[j].port);
					j++;
					break;
				}
			}
		}
		result = true;
	} while (false);
	return result;
}

void ThreadHandler::deleteWorkThread()
{
	if (m_workThread)
	{
		quitWorkThread();
		delete[] m_workThread;
		m_workThread = nullptr;
	}
}

const WorkThread* ThreadHandler::getWorkThread() const
{
	return m_workThread;
}

void ThreadHandler::startWorkThread()
{
	/*if (m_jsonTool->getParsedBinFileConfig().burnMode == BM_NET_AXS340)
	{
		m_channelCount = 1;
	}*/
	for (int i = 0; i < m_channelCount; i++)
	{
		m_childQuit = false;
		m_workThread[i].setBurnSequence(BurnSequence::BS_PREP_BURN);
		m_workThread[i].start();
		m_childWork = true;
		msleep(10);
	}
}

void ThreadHandler::quitWorkThread()
{
	m_childQuit = true;
}

const QMap<quint8, quint32>& ThreadHandler::getAardvarkPortAndSn()
{
	do
	{
		quint16 portArray[MAX_DEVICE_COUNT] = {};
		quint32 serialArray[MAX_DEVICE_COUNT] = {};
		m_aardvarkCount = aa_find_devices_ext(MAX_DEVICE_COUNT, portArray, MAX_DEVICE_COUNT, serialArray);
		if (m_aardvarkCount <= 0)
		{
			setLastError("δ�ҵ�Aardvark��¼���豸,�������豸.");
			break;
		}

		for (int i = 0; i < m_aardvarkCount; i++)
		{
			m_aardvarkPortAndSn.insert(portArray[i], serialArray[i]);
		}
	} while (false);
	return m_aardvarkPortAndSn;
}

const int ThreadHandler::getAardvarkCount()
{
	return m_aardvarkCount;
}

bool ThreadHandler::loadAardvarkDevice()
{
	bool result = false;
	do
	{
		if (m_aardvarkCount)
		{
			deleteWorkThread();
		}

		if (m_aardvarkPortAndSn.size())
		{
			m_aardvarkPortAndSn.clear();
		}

		if (!getAardvarkPortAndSn().size())
		{
			/*û���豸����*/
		}
		else
		{
			if (!createWorkThread())
			{
				break;
			}
		}
		result = true;
	} while (false);
	return result;
}

const QString& ThreadHandler::getLastError()
{
	return m_lastError;
}

bool ThreadHandler::scanCodeDlg(const int& number)
{
	emit scanCodeDlgSignal(number);
	g_threadWait = true;
	while (g_threadWait) { msleep(100); };
	return m_scDlgShow;
}

void ThreadHandler::setRunSwitch(bool on)
{
	m_scDlgShow = on;
	m_connect = on;
	if (!on)
	{
		g_threadWait = false;
	}
}

void ThreadHandler::setMessageBox(const QString& title, const QString& text)
{
	emit setMessageBoxSignal(title, text);
}

bool ThreadHandler::isWorkThreadRunning()
{
	return m_childWork;
}

/************************************************************************/
/* protected                                                            */
/************************************************************************/
void ThreadHandler::run()
{
	bool scan = true;
	while (!m_quit)
	{
		if (m_connect)
		{
			scan = true;

			if (m_childWork)
			{
				continue;
			}

			for (int i = 0; i < m_channelCount; i++)
			{
				if (g_codeList.count() >= m_channelCount)
				{
					break;
				}

				if (!scanCodeDlg(i + 1))
				{
					scan = false;
					break;
				}
			}

			if (m_deviceConfig->codeRepeat.toInt())
			{
				g_repeatCodeList.clear();
			}

			if (!scan)
			{
				g_codeList.clear();
				continue;
			}
			g_codeListBackup = g_codeList;
			g_restartCounter = g_codeList.count();
			startWorkThread();
		}
		msleep(100);
	}
	quit();
}

void ThreadHandler::setLastError(const QString& err, bool msgBox)
{
#ifdef QT_DEBUG
	qDebug() << err << endl;
#endif
	if (msgBox)
	{
		setMessageBox("��ʾ", err);
	}
	m_lastError = err;
}

bool ThreadHandler::openAardvarkDevice()
{
	bool result = true;
	do
	{
		for (int i = 0; i < m_channelCount; i++)
		{
			if (!m_workThread[i].getAardvark().OpenDevice(i, m_fileConfig->deviceSpeed, m_fileConfig->deviceTimeout))
			{
				result = false;
				setLastError(QString("����¼��ʧ��,����������"), true);
				break;
			}
		}
	} while (false);
	return result;
}

bool ThreadHandler::closeAardvarkDevice()
{
	bool result = true;
	do
	{
		for (int i = 0; i < m_channelCount; i++)
		{
			if (!m_workThread[i].getAardvark().CloseDevice())
			{
				result = false;
			}
		}
	} while (false);
	return result;
}

/************************************************************************/
/* WorkThread                                                           */
/************************************************************************/

WorkThread::WorkThread(QObject* parent)
	:QThread(parent)
{
	qRegisterMetaType<burnStatus_t>("burnStatus_t");
}

WorkThread::~WorkThread()
{
	if (isRunning())
	{
		wait(10000);
	}

	SAFE_DELETE(m_progressTimer);
	m_threadHandler = nullptr;
}

AardvarkMgr& WorkThread::getAardvark()
{
	return m_aardvark;
}

void WorkThread::setParent(QObject* parent)
{
	m_threadHandler = reinterpret_cast<ThreadHandler*>(parent);
}

void WorkThread::setAardvarkPort(const int& port)
{
	m_aardvarkPort = port;
}

void WorkThread::setPowerChannel(const int& channel)
{
	m_powerChannel = channel;
}

void WorkThread::setRelayPort(const int& port)
{
	m_relayPort = port;
}

void WorkThread::setAardvarkSn(const quint32& sn)
{
	m_aardvarkSn = sn;
}

bool WorkThread::initInstance()
{
	bool result = false;
	do
	{
		m_jsonTool = JsonTool::getInstance();
		
		m_deviceConfig = m_jsonTool->getParsedDeviceConfig();

		m_hardwareConfig = m_jsonTool->getParsedHardwareConfig();

		m_thresholdConfig = m_jsonTool->getParsedThresholdConfig();

		m_fileConfig = m_jsonTool->getParsedFileConfig();

#ifdef QT_DEBUG
		m_aardvark.EnableDebugInfo(true);
#endif

		m_progressTimer = new(std::nothrow) QTimer;
		if (!m_progressTimer)
		{
			setLastError("m_progressTimer�����ڴ�ʧ��");
			break;
		}
		connect(m_progressTimer, &QTimer::timeout, this, &WorkThread::progressTimerSlot);
		m_progressTimer->start(200);
		result = true;
	} while (false);
	return result;
}

const QString& WorkThread::getLastError()
{
	return m_lastError;
}

bool WorkThread::changeFlashStatus()
{
	
	//����016����ͷ����FFFFдF1���л�Ϊ��״̬����FFFFдF0���л�Ϊд״̬
	
	bool result = false;
	quint8 output[3] = { 0xFF,0xFF,0xF1 };
	quint8 inputData[8] = { 0 };
	do
	{
		if (m_fileConfig->burnMode == BurnMode::BM_ATC_016_SET)
		{
			if (!m_aardvark.IICAddrWrite(0xFFFF, 2, &output[2], 1, false, m_fileConfig->dataSlave))
			{
				break;
			}
		}
		msleep(100);
		result = true;
	} while (false);
	return result;
}

bool WorkThread::initNetworkBurn()
{
	bool result = false;
	const int ipCount = 9;
	int index = 0;
	char ipArray[][32] = { "10.0.0.10","10.0.0.11","10.0.0.12","10.0.0.13","192.168.1.10",
		"192.168.1.11","192.168.1.12","192.168.1.13","192.168.1.5" };
	do
	{
		m_ispTool = IspTool::getInstance();
		if (!m_ispTool)
		{
			setLastError("m_ispTool�����ڴ�ʧ��");
			break;
		}
		if (!m_ispTool->initNetwork(ipArray[index], 50001))
		{
			setLastError("��ʼ������ʧ��");
			break;
		}

		if (ERROR_ENOERR == m_ispTool->canBeCommunication())
		{
			result = true;
			break;
		}

		if (index >= ipCount)
		{
			setLastError("û���ҵ�IP��ַ");
			break;
		}
		index++;
	} while (true);
	return result;
}

bool WorkThread::controlPower(bool powerSwitch)
{
	bool result = false;
	do
	{
		if (m_threadHandler->m_debugMode)
		{
			result = true;
			break;
		}

		if (m_deviceConfig->openPower.toInt() && m_deviceConfig->openRelay.toInt())
		{
			goto relay;
		}
		else if (m_deviceConfig->openPower.toInt())
		{
			m_threadHandler->m_mutex.lock();
			if (m_deviceConfig->powerType == IT6302)
			{
				if (!m_threadHandler->m_power6302.SetChannel(m_powerChannel))
				{
					break;
				}

				if (!m_threadHandler->m_power6302.SetVol(powerSwitch ? m_thresholdConfig->powerVoltage : 0.0f))
				{
					break;
				}
			}
			else if (m_deviceConfig->powerType == IT6832)
			{
				if (!m_threadHandler->m_power6832.SetVol(powerSwitch ? m_thresholdConfig->powerVoltage : 0.0f))
				{
					break;
				}
			}
			msleep(m_hardwareConfig->powerDelay);
			m_threadHandler->m_mutex.unlock();
		}
		else if (m_deviceConfig->openRelay.toInt())
		{
		relay:
			m_threadHandler->m_mutex.lock();
			if (!m_threadHandler->m_relay.SetOneIO(m_relayPort, powerSwitch))
			{
				break;
			}
			msleep(m_hardwareConfig->relayDelay);
			m_threadHandler->m_mutex.unlock();
		}
		result = true;
	} while (false);

	if (!result)
	{
		m_threadHandler->m_mutex.unlock();
		setLastError("��⵽�����豸�������⣬���鴮�������Ƿ��ɶ�");
	}
	return result;
}

bool WorkThread::saveBinFile(const QString& name, const char* data, const ulong& size)
{
	bool result = false;
	do 
	{
		QFile file(name);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError(QString("��%1�ļ�ʧ��,%1").arg(name, file.errorString()));
			break;
		}

		if (file.write(data, size) != size)
		{
			setLastError(QString("д��%1�ļ�У��ʧ��").arg(name));
			break;
		}

		file.close();
		result = true;
	} while (false);
	return result;
}

bool WorkThread::prepareBurn()
{
	updateCurrentStatus("׼����¼");
	updateBurnStatus(BurnStatus::BS_WR);
	bool result = false;
	do
	{
		m_restartPower = false;
		setBurnTimerRun(true);
		memset(m_progressData, 0x00, sizeof(m_progressData));
		memset(m_coordData, 0x00, sizeof(m_coordData));
		
		if (m_fileConfig->speedMode)
		{
			m_aardvark.SetReadWriteDelay(m_fileConfig->libReadDelay, m_fileConfig->libWriteDelay);
		}

		m_aardvark.Set019Jump0xFF(m_deviceConfig->jump0xff.toInt());

		if (m_fileConfig->checkOutage)
		{
			if (!controlPower(true))
			{
				break;
			}
		}
		msleep(m_fileConfig->rebootDelay);
		m_progressData[0] = 5;
		result = true;
	} while (false);
	return result;
}

bool WorkThread::initAddress(bool mask)
{
	if (!mask)
	{
		updateCurrentStatus("��ʼ����ַ");
	}

	bool result = false;
	do
	{
		bool success = true, networkBurn = true;
		CameraType cameraType;
		switch (m_fileConfig->burnMode)
		{
		case BurnMode::BM_ATC_016_SET:
			cameraType = CT_ATC_016_SET;
			break;
		case BurnMode::BM_CTC_016_SET:
			cameraType = CT_CTC_016_SET;
			break;
		case BurnMode::BM_CTC_019_SET:
			cameraType = CT_CTC_019_SET;
			break;
		case BurnMode::BM_EEP_AXS340:
			cameraType = CT_EEP_ASX340;
			break;
		case BurnMode::BM_FLASH_AXS340:
			cameraType = CT_FLASH_ASX340;
			break;
		case BurnMode::BM_NET_AXS340:
			networkBurn = false;
			break;
		case BurnMode::BM_CTC_CHANGAN_IMS:
			cameraType = CT_CTC_CHANGAN_IMS;
			break;
		case BurnMode::BM_EEP_GEELY_BX11:
			cameraType = CT_EEP_GEELY_BX11;
			break;
		case BurnMode::BM_CTC_EP30TAP_DMS:
			cameraType = CT_CTC_EP30TAP_DMS;
			break;
		case BurnMode::BM_ATC_BYD_OV7958:
			cameraType = CT_ATC_BYD_OV7958;
			break;
		default:
			success = false;
			break;
		}

		if (!success)
		{
			setLastError(QString("��¼ģʽΪ%1,δ֪ģʽ").arg(m_fileConfig->burnMode));
			break;
		}

		if (m_fileConfig->burnMode == BurnMode::BM_NET_AXS340)
		{
			if (!networkBurn)
			{
				setLastError("��������¼�ݲ�֧��");
				break;
			}
		}
		else
		{
			if (!m_aardvark.Init(cameraType))
			{
				msleep(1000);
				if (!m_aardvark.Init(cameraType))
				{
					setLastError("��ʼ����ַʧ��," + getAardvarkError());
					break;
				}
			}
		}

		if (!mask)
		{
			m_progressData[0] = 10;
		}
		result = true;
	} while (false);
	return result;
}

bool WorkThread::readCoordinate()
{
	updateCurrentStatus("��ȡ����");
	bool result = false, success = true;
	do
	{
		if (m_fileConfig->burnMode == BM_ATC_016_SET)
		{
			if (!changeFlashStatus())
			{
				setLastError("�ı�״̬ʧ��," + getAardvarkError());
				break;
			}

			if (!m_aardvark.IICAddrRead(0x0120, 2, m_coordData, 8, m_fileConfig->dataSlave))
			{
				setLastError("��ȡ����ʧ��," + getAardvarkError());
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_CTC_016_SET)
		{
			if (!m_aardvark.CmdUnlock(m_fileConfig->dataSlave))
			{
				setLastError("�����н���ʧ��," + getAardvarkError());
				break;
			}

			if (!m_aardvark.IICCmdReadEEP(2, 0x0120, 8, m_coordData, m_fileConfig->dataSlave))
			{
				setLastError("��ȡ����ʧ��," + getAardvarkError());
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_CTC_019_SET || m_fileConfig->burnMode == BM_CTC_CHANGAN_IMS)
		{
			if (!m_aardvark.CmdUnlock(m_fileConfig->dataSlave))
			{
				setLastError("�����н���ʧ��," + getAardvarkError());
				break;
			}

			if (!m_aardvark.IICCmdReadFlash(4, 0x07e000, 8, m_coordData, m_fileConfig->dataSlave))
			{
				setLastError("��ȡ����ʧ��," + getAardvarkError());
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_EEP_GEELY_BX11)
		{
			//uchar data[] = { 0xe0,0x02,0x1e,0xfb,0x01,0x02 };
			//m_aardvark.ASX340WriteFlash(0x5000, 6, data);

			if (!m_aardvark.ASX340ReadFlash(0x5000, 6, m_coordData))
			{
				setLastError("��ȡ����ʧ��");
				break;
			}
		}
		m_progressData[0] = 15;
		result = true;
	} while (false);
	return result;
}

bool WorkThread::alterBinFile()
{
	updateCurrentStatus("�޸�BIN�ļ�");
	bool result = false;
	do
	{
		quint8* buffer = (quint8*)m_fileConfig->data.data();
		int bufferSize = m_fileConfig->data.size();
		if (m_fileConfig->burnMode == BM_CTC_019_SET)
		{
			quint16 xShiftH = m_coordData[HOR] + (TEMP_MODE ? 0 : (quint16)(m_coordData[HOR + 1] << 8));
			quint16 yShiftV = m_coordData[VOR] + (TEMP_MODE ? 0 : (quint16)(m_coordData[VOR + 1] << 8));
			if (1)
			{
				u32 ShiftSrcB0 = *((u32*)(&buffer[0x18]));
				*((u16*)(&buffer[0x18])) = xShiftH;
				*((u16*)(&buffer[0x1A])) = yShiftV;
				u32 ShiftDstB0 = *((u32*)(&buffer[0x18]));
				u32 CheckSumB0 = *((u32*)(&buffer[0xFFC]));
				*((u32*)(&buffer[0xFFC])) = CheckSumB0 - ShiftSrcB0 + ShiftDstB0;
			}
			if (1)
			{
				u32 ShiftSrcB1 = *((u32*)(&buffer[0x10018]));
				*((u16*)(&buffer[0x10018])) = xShiftH;
				*((u16*)(&buffer[0x1001A])) = yShiftV;
				u32 ShiftDstB1 = *((u32*)(&buffer[0x10018]));
				u32 CheckSumB1 = *((u32*)(&buffer[0x10FFC]));
				*((u32*)(&buffer[0x10FFC])) = CheckSumB1 - ShiftSrcB1 + ShiftDstB1;
			}
		}
		else if (m_fileConfig->burnMode == BM_CTC_CHANGAN_IMS)
		{
			quint16 xShiftH = m_coordData[HOR] + (TEMP_MODE ? 0 : (quint16)(m_coordData[HOR + 1] << 8));
			quint16 yShiftV = m_coordData[VOR] + (TEMP_MODE ? 0 : (quint16)(m_coordData[VOR + 1] << 8));
			if (0)
			{
				u32 ShiftSrcB0 = *((u32*)(&buffer[0x18]));
				*((u16*)(&buffer[0x18])) = xShiftH;
				*((u16*)(&buffer[0x1A])) = yShiftV;
				u32 ShiftDstB0 = *((u32*)(&buffer[0x18]));
				u32 CheckSumB0 = *((u32*)(&buffer[0xFFC]));
				//*((u32*)(&buffer[0xFFC])) = CheckSumB0 - ShiftSrcB0 + ShiftDstB0;
			}
			if (0)
			{
				u32 ShiftSrcB1 = *((u32*)(&buffer[0x10018]));
				*((u16*)(&buffer[0x10018])) = xShiftH;
				*((u16*)(&buffer[0x1001A])) = yShiftV;
				u32 ShiftDstB1 = *((u32*)(&buffer[0x10018]));
				u32 CheckSumB1 = *((u32*)(&buffer[0x10FFC]));
				//*((u32*)(&buffer[0x10FFC])) = CheckSumB1 - ShiftSrcB1 + ShiftDstB1;
			}
		}
		else if (m_fileConfig->burnMode == BM_ATC_016_SET || m_fileConfig->burnMode == BM_CTC_016_SET)
		{
			memcpy(&buffer[0x120], m_coordData, 8);
			//m_coordData[4] = 0xff - m_coordData[4];
			//m_coordData[5] = 0xff - m_coordData[5];
			memcpy(&buffer[0x9ec], &m_coordData[4], 2);
		}
		else if (m_fileConfig->burnMode == BM_EEP_GEELY_BX11)
		{
			if (0)
			{
				/*�ڽ���У��һ�ι�������,���⹩Ӧ�̳���*/
				ushort head = *(ushort*)&m_coordData[0];
				ushort axis = *(ushort*)&m_coordData[4];
				ushort sum = *(ushort*)&m_coordData[2];

				if ((0xFFFF - (head + axis)) != sum)
				{
					setLastError("��Ӧ��д�������У�����");
					break;
				}

				int checkCount = 0;
				ushort xAxis = 0xA8C8, yAxis = 0xA9C8;
				int xRecord = 0, yRecord = 0, checksumRecord = 0;
				for (int i = 0; i < bufferSize; i++)
				{
					/*��ȡx��y��checksum����*/
					if ((*(ushort*)&buffer[i] == xAxis) && (*(ushort*)(&buffer[i] + 8) == yAxis))
					{
						checksumRecord = i - 2;
						xRecord = i + 7;
						yRecord = i + 8 + 7;
						checkCount++;
					}
				}

				if (!checkCount)
				{
					setLastError("û���ҵ���������,�����Ƿ�Ϊ�����ļ�");
					break;
				}

				/*����У�����*/
				if (checkCount > 1)
				{
					setLastError("�����Դ���,BX11д����,�����ظ�");
					break;
				}

				/*Ĭ�Ϲ���ֵ*/
				uchar xDefaultAxis = buffer[xRecord];
				uchar yDefaultAxis = buffer[yRecord];

				/*��������ֵ*/
				uchar xValue = m_coordData[4], yValue = m_coordData[5];

				/*�¹���ֵ*/
				uchar xNewAxis = xValue + xDefaultAxis;
				uchar yNewAxis = yValue + yDefaultAxis;

				/*���Ǿɹ���ֵ*/
				buffer[xRecord] = xNewAxis;
				buffer[yRecord] = yNewAxis;

				/*�����µ�checksum*/
				ushort defaultChecksum = 0xEC9C;
				ushort newChecksum = defaultChecksum - xNewAxis - yNewAxis;
				*(ushort*)&buffer[checksumRecord] = newChecksum;
			}
		}
		result = true;
		m_progressData[0] = 20;
	} while (false);
	return result;
}

bool WorkThread::writeBinFile()
{
	updateCurrentStatus("д��BIN�ļ�");
	updateBurnStatus(BurnStatus::BS_WR);
	bool result = false;
	do
	{
		quint32 bufferSize = m_fileConfig->data.size();
		quint8* bufferData = (quint8*)m_fileConfig->data.data();

		if (m_fileConfig->burnMode == BM_ATC_016_SET)
		{
			if (!m_aardvark.AddrWriteBinFile(2, 0x0000, bufferSize, bufferData, 8, m_fileConfig->dataSlave, m_fileConfig->appWriteDelay, &m_progressData[1]))
			{
				setLastError("д������ʧ��," + getAardvarkError());
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_CTC_016_SET)
		{
			if (!m_aardvark.CmdWriteBinFile(2, 0x0000, bufferSize, bufferData, 8, m_fileConfig->dataSlave, m_fileConfig->appWriteDelay, &m_progressData[1]))
			{
				setLastError("д������ʧ��," + getAardvarkError());
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_CTC_019_SET || m_fileConfig->burnMode == BM_CTC_CHANGAN_IMS || m_fileConfig->burnMode == BM_CTC_EP30TAP_DMS)
		{
			if (!m_aardvark.CmdWriteBinFile(4, 0x00000000, bufferSize, bufferData, 8, m_fileConfig->dataSlave, m_fileConfig->appWriteDelay, &m_progressData[1]))
			{
				setLastError("д������ʧ��," + getAardvarkError());
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_EEP_AXS340 || m_fileConfig->burnMode == BM_FLASH_AXS340 || m_fileConfig->burnMode == BM_EEP_GEELY_BX11)
		{
			if (!m_aardvark.ASX340WriteBinFile(4, 0, bufferSize, bufferData, &m_progressData[1]))
			{
				setLastError("д������ʧ��," + getAardvarkError());
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_NET_AXS340)
		{
			if (m_ispTool->writeFlashDeviceNvm(bufferData, bufferSize, 2, 0, &m_progressData[1]) != ERROR_ENOERR)
			{
				setLastError("д������ʧ��");
				break;
			}
		}
		else if (m_fileConfig->burnMode == BM_ATC_BYD_OV7958)
		{
			if (!m_aardvark.OV7958SetProtect(false, m_fileConfig->dataSlave))
			{
				setLastError("�Ᵽ��ʧ��");
				break;
			}

			if (!m_aardvark.OV7958WriteBinFile(bufferData, bufferSize, m_fileConfig->dataSlave, &m_progressData[1]))
			{
				setLastError("д������ʧ��");
				break;
			}

			if (!m_aardvark.OV7958SetProtect(true, m_fileConfig->dataSlave))
			{
				setLastError("д����ʧ��");
				break;
			}
		}
		else
		{
			setLastError("��¼ģʽ����");
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool WorkThread::restartPower()
{
	updateCurrentStatus("�ȴ�ͬ������");
	bool result = false;
	do
	{
		/*��д�����,��ͣ1��,�ڽ��жϵ�,ȷ���豸���ȶ�״̬*/
		msleep(1000);
		m_restartPower = true;
		m_threadHandler->m_mutex.lock();
		g_restartCounter = g_restartCounter - 1;
		m_threadHandler->m_mutex.unlock();

		if (!m_fileConfig->checkOutage || m_threadHandler->m_debugMode)
		{
			result = true;
			break;
		}

		while (g_restartCounter > 0) { msleep(100); }

		updateCurrentStatus("�����ϵ�");
		if (!controlPower(false))
		{
			break;
		}

		/*��ͣ2��,��֤�豸�����ܹ�*/
		msleep(2000);
		
		if (!controlPower(true))
		{
			break;
		}
		msleep(m_fileConfig->rebootDelay);
		if (!initAddress(true))
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool WorkThread::checkData()
{
	bool result = false;
	updateCurrentStatus("У������");
	updateBurnStatus(BurnStatus::BS_RD);
	do
	{
		quint32 bufferSize = m_fileConfig->data.size();
		quint8* bufferData = (quint8*)m_fileConfig->data.data();

		quint8* checkBuffer = new quint8[bufferSize + 1024 * 1024];
		if (!checkBuffer)
		{
			setLastError("У�黺���ڴ����ʧ��");
			break;
		}
		memset(checkBuffer, 0x0, bufferSize + 1024 * 1024);
		do
		{
			if (m_fileConfig->burnMode == BM_CTC_019_SET
				|| m_fileConfig->burnMode == BM_CTC_CHANGAN_IMS
				|| m_fileConfig->burnMode == BM_CTC_EP30TAP_DMS)
			{
				memcpy(&checkBuffer[0x7e000], &bufferData[0x7e000], 0x1000);

				if (!m_aardvark.CmdReadBinFile(4, 0x00000000, bufferSize, checkBuffer, 128, m_fileConfig->checkSlave, m_fileConfig->appReadDelay, &m_progressData[2]))
				{
					setLastError("��ȡ����ʧ��," + getAardvarkError());
					break;
				}
			}
			else if (m_fileConfig->burnMode == BM_ATC_016_SET)
			{
				if (!m_aardvark.AddrReadBinFile(2, 0x0000, bufferSize, checkBuffer, 8, m_fileConfig->checkSlave, m_fileConfig->appReadDelay, &m_progressData[2]))
				{
					setLastError("��ȡ����ʧ��," + getAardvarkError());
					break;
				}
			}
			else if (m_fileConfig->burnMode == BM_CTC_016_SET)
			{
				if (!m_aardvark.CmdReadBinFile(2, 0x0000, bufferSize, checkBuffer, 8, m_fileConfig->checkSlave, m_fileConfig->appReadDelay, &m_progressData[2]))
				{
					setLastError("��ȡ����ʧ��," + getAardvarkError());
					break;
				}
			}
			else if (m_fileConfig->burnMode == BM_EEP_AXS340 
				|| m_fileConfig->burnMode == BM_FLASH_AXS340
				|| m_fileConfig->burnMode == BM_EEP_GEELY_BX11)
			{
				if (!m_aardvark.ASX340ReadBinFile(4, 0, bufferSize, checkBuffer, &m_progressData[2]))
				{
					setLastError("��ȡ����ʧ��," + getAardvarkError());
					break;
				}
			}
			else if (m_fileConfig->burnMode == BurnMode::BM_NET_AXS340)
			{
				if (m_ispTool->readFlashDeviceNvm(&checkBuffer, bufferSize, 0, &m_progressData[2]) != ERROR_ENOERR)
				{
					setLastError("������¼��E2Pʧ��");
					break;
				}
			}
			else if (m_fileConfig->burnMode == BurnMode::BM_ATC_BYD_OV7958)
			{
				if (!m_aardvark.OV7958ReadBinFile(checkBuffer, bufferSize, m_fileConfig->checkSlave, &m_progressData[2]))
				{
					setLastError("��ȡ����ʧ��," + getAardvarkError());
					break;
				}
			}

			if (memcmp(bufferData, checkBuffer, bufferSize))
			{
				setLastError("У������ʧ��");
				break;
			}
			result = true;
		} while (false);
		saveBinFile("У������.bin", (const char*)checkBuffer, bufferSize);
		delete[] checkBuffer;
		checkBuffer = nullptr;
	} while (false);
	return result;
}

bool WorkThread::saveLog(bool success)
{
	updateCurrentStatus("������־");
	setBurnTimerRun(false);
	if (success)
	{
		updateProgress(100);
	}

	bool result = false;
	do
	{
		QDir dir;
		QString filePath = QString("./Log/%1/%2/").arg(success ? "NOR" : "ERR", getDate());
		if (!dir.exists(filePath))
		{
			if (!dir.mkpath(filePath))
			{
				setLastError("����������־Ŀ¼�ļ���ʧ��");
				break;
			}
		}

		QString code = g_codeListBackup.value(m_aardvarkPort);
		if (code.isEmpty())
		{
			code = "δ֪����";
		}

		QString fileName = QString("%1%2_%3_%4.csv").arg(filePath, m_fileConfig->nodeName, code, getTime());

		QFile file(fileName);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError("������־�ļ�ʧ��");
			break;
		}

		QTextStream stream(&file);
		stream << QString(" ,������,%1,\n,��¼���,%2,\n").arg(code, OK_NG(success));
		stream << QString("����ԭ��,%1").arg(success ? "��" : m_lastError) << endl;
		stream << QString("��¼ģʽ,%1").arg(m_fileConfig->burnMode) << endl;
		stream << QString("��¼ʱ��,%1").arg(m_burnTimerTime) << endl;
		stream << QString("��¼��SN,%1").arg(m_aardvarkSn) << endl;

		file.close();
		result = true;
	} while (false);
	return result;
}

bool WorkThread::waitSync(bool success)
{
	updateCurrentStatus("�ȴ�ͬ��");
	bool result = false;

	do
	{
		updateBurnStatus(success ? BurnStatus::BS_OK : BurnStatus::BS_NG, m_lastError);
		memset(m_progressData, 0x00, sizeof(m_progressData));
		if (!m_restartPower)
		{
			m_threadHandler->m_mutex.lock();
			g_restartCounter = g_restartCounter - 1;
			m_threadHandler->m_mutex.unlock();
		}

		int lastComplete = 0;
		m_threadHandler->m_mutex.lock();
		g_codeList.pop_back();
		lastComplete = g_codeList.count();
		m_threadHandler->m_mutex.unlock();

		while (g_codeList.count()) { msleep(100); }
		if (!lastComplete)
		{
			m_threadHandler->m_childWork = false;
		}

		updateCurrentStatus(success ? "��¼�ɹ�" : "��¼ʧ��");

		if (m_fileConfig->checkOutage)
		{
			if (!controlPower(false))
			{
				break;
			}
		}
		result = true;
	} while (false);
	return result;
}

const QString WorkThread::getTime(bool fileFormat)
{
	QString time = QTime::currentTime()
		.toString("hh:mm:ss.zzz");
	if (fileFormat)
	{
		return time.remove(":").remove(".");
	}
	return time;
}

const QString WorkThread::getDate()
{
	QString date = QDate::currentDate()
		.toString("yyyy-MM-dd");
	return date;
}

const QString WorkThread::getDateTime(bool fileFormat)
{
	QString dateTime = QDateTime::currentDateTime()
		.toString("yyyy-MM-dd hh:mm:ss.zzz");
	if (fileFormat)
	{
		return dateTime.replace(":", "-").replace(".", "-");
	}
	return dateTime;
}

void WorkThread::updateProgress(const int& progress)
{
	emit updateProgressSignal(progress);
}

void WorkThread::updateCurrentStatus(const QString& status)
{
	emit updateCurrentStatusSiganl(status);
}

void WorkThread::updateBurnStatus(const burnStatus_t& status, const QString& err)
{
	emit updateBurnStatusSiganl(status, err);
}

void WorkThread::updateGroupTitle(const QString& title)
{
	emit updateGroupTitleSignal(title);
}

void WorkThread::setBurnTimerRun(bool go)
{
	m_startWork = go;
	emit setBurnTimerRunSignal(go);
}

void WorkThread::setBurnSequence(const burnSequence_t& sequence)
{
	m_testSequence = sequence;
}

const QString WorkThread::getAardvarkError()
{
	return From8Bit(m_aardvark.GetLastError());
}

void WorkThread::progressTimerSlot()
{
	if (m_startWork)
	{
		updateProgress(m_progressData[0] + m_progressData[1] * 40 / 100 + m_progressData[2] * 40 / 100);
	}
}

void WorkThread::getBurnTimerTimeSlot(const int& data)
{
	m_burnTimerTime = data;
}

void WorkThread::run()
{
	bool success = false;
	while (!m_threadHandler->m_childQuit)
	{
		if (m_threadHandler->m_connect)
		{
			switch (m_testSequence)
			{
			case BurnSequence::BS_PREP_BURN:
				success = false;
				if (!prepareBurn())
				{
					m_testSequence = BurnSequence::BS_SAVE_LOG;
					break;
				}
				m_testSequence = BurnSequence::BS_INIT_ADDR;
				break;
			case BurnSequence::BS_INIT_ADDR:
				if (!initAddress())
				{
					m_testSequence = BurnSequence::BS_SAVE_LOG;
					break;
				}
				m_testSequence = BurnSequence::BS_READ_COOR;
				break;
			case BurnSequence::BS_READ_COOR:
				if (!readCoordinate())
				{
					m_testSequence = BurnSequence::BS_SAVE_LOG;
					break;
				}
				m_testSequence = BurnSequence::BS_ALTER_BIN_FILE;
				break;
			case BurnSequence::BS_ALTER_BIN_FILE:
				if (!alterBinFile())
				{
					m_testSequence = BurnSequence::BS_SAVE_LOG;
					break;
				}
				m_testSequence = BurnSequence::BS_WRITE_BIN_FILE;
				break;
			case BurnSequence::BS_WRITE_BIN_FILE:
				if (!writeBinFile())
				{
					m_testSequence = BurnSequence::BS_SAVE_LOG;
					break;
				}
				m_testSequence = BurnSequence::BS_RESTART_POWER;
			case BurnSequence::BS_RESTART_POWER:
				if (!restartPower())
				{
					m_testSequence = BurnSequence::BS_SAVE_LOG;
					break;
				}
				m_testSequence = BurnSequence::BS_CHECK_DATA;
				break;
			case BurnSequence::BS_CHECK_DATA:
				if (!checkData())
				{
					m_testSequence = BurnSequence::BS_SAVE_LOG;
					break;
				}
				success = true;
				m_testSequence = BurnSequence::BS_SAVE_LOG;
				break;
			case BurnSequence::BS_SAVE_LOG:
				if (!saveLog(success))
				{
					updateGroupTitle(m_lastError);
				}
				m_testSequence = BurnSequence::BS_WAIT_SYNC;
				break;
			case BurnSequence::BS_WAIT_SYNC:
				if (!waitSync(success))
				{
					updateGroupTitle(m_lastError);
				}
				m_testSequence = BurnSequence::BS_NONE;
				break;
			default:
				break;
			}
		}
		msleep(100);
	}
	quit();
}

void WorkThread::setLastError(const QString& err)
{
#ifdef QT_DEBUG
	qDebug() << err << endl;
#endif
	m_lastError = err;
}