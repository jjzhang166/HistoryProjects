#include "Detection.h"

/*�����߳̿���ȫ�ֱ���*/
bool g_threadWait = false;

/*������Ϣȫ�ֱ���*/
int* g_debugInfo = nullptr;

/*����ȫ�ֱ���*/
QString g_code = "";

CanTransfer* Dt::Base::m_canTransfer = nullptr;

CanSender Dt::Base::m_canSender = CanSender();

CItechSCPIMgr Dt::Base::m_power = CItechSCPIMgr();

CMRDO16KNMgr Dt::Base::m_relay = CMRDO16KNMgr();

CVoltageTestMgr Dt::Base::m_voltage = CVoltageTestMgr();

StaticCurrentMgr Dt::Base::m_current = StaticCurrentMgr();

BaseTypes::DetectionType Dt::Base::m_detectionType = BaseTypes::DT_AVM;

/************************************************************************/
/* Dt::Base realize                                                         */
/************************************************************************/

Dt::Base::Base(QObject* parent)
{
	qRegisterMetaType<BaseTypes::TestResult>("BaseTypes::TestResult");
	qRegisterMetaType<BaseTypes::DownloadInfo*>("BaseTypes::DownloadInfo*");
	qRegisterMetaType<bool*>("bool*");
}

Dt::Base::~Base()
{
	UdsProtocolMgr::freeUdsTransfer(m_udsTransfer);

	CanMgr::freeCanTransfer(m_canTransfer);
	
	autoRecycle({ GET_DT_DIR() });

	exitConsoleWindow();

	JsonTool::deleteInstance();
	
	threadQuit();

	QApplication::exit(0);
}

const QString& Dt::Base::getLastError()
{
	return m_lastError;
}

void Dt::Base::setTestSequence(const int& testSequence)
{
	m_testSequence = testSequence;
}

void Dt::Base::setDetectionType(const BaseTypes::DetectionType& type)
{
	m_detectionType = type;
}

void Dt::Base::setSocDelay(const ulong& delay)
{
	m_socDelay = delay;
}

const BaseTypes::DetectionType& Dt::Base::getDetectionType()
{
	return m_detectionType;
}

bool Dt::Base::initInstance()
{
	bool result = false;
	do
	{
		m_jsonTool = JsonTool::getInstance();

		RUN_BREAK(!m_jsonTool, "m_jsonTool�����ڴ�ʧ��");

		RUN_BREAK(!m_jsonTool->initInstance(), m_jsonTool->getLastError());

		m_defConfig = m_jsonTool->getParsedDefConfig();

		m_hwdConfig = m_jsonTool->getParsedHwdConfig();

		m_udsConfig = m_jsonTool->getParsedUdsConfig();

		g_debugInfo = &m_defConfig->enable.outputRunLog;

		m_canTransfer = CanMgr::allocCanTransfer(m_defConfig->device.canName.toLatin1());
		RUN_BREAK(!m_canTransfer, "CANͨ�ų�ʼ��ʧ��");

#ifdef QT_DEBUG
		m_canTransfer->EnableDebugInfo(true);
#endif

		if (!initConsoleWindow())
		{
			setLastError(getLastError(), false, true);
		}

		m_udsTransfer = UdsProtocolMgr::allocUdsTransfer(m_defConfig->device.udsName.toLatin1(),
			m_canTransfer);
		RUN_BREAK(!m_udsTransfer, "UDSͨ��Э���ʼ��ʧ��");

		RUN_BREAK(!m_canSender.Init(m_canTransfer), "CanSender��ʼ��ʧ��");

		result = true;
	} while (false);
	return result;
}

bool Dt::Base::initConsoleWindow()
{
	bool result = false;
	do
	{
		if (g_debugInfo && !*g_debugInfo)
		{
			result = true;
			break;
		}

		RUN_BREAK(!AllocConsole(), "�������̨ʧ��");
		SetConsoleTitleW(Q_TO_WC_STR(Q_SPRINTF("�����[%s]���Կ���̨", LIB_VERSION)));
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		if (output != INVALID_HANDLE_VALUE)
		{
			SetConsoleTextAttribute(output, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		}
		RUN_BREAK(!freopen("CONOUT$", "w", stderr), "�ض��������stderrʧ��");

		DEBUG_INFO() << "��ʼ������̨�ɹ�";
		DEBUG_INFO() << "�ض���stderr���ɹ�";
		RUN_BREAK(!freopen("CONOUT$", "w", stdout), "�ض��������stdoutʧ��");

		DEBUG_INFO() << "�ض���stdout���ɹ�";
		DEBUG_INFO() << "���Ի���:" << m_defConfig->device.modelName;
		DEBUG_INFO() << "UDSЭ��:" << m_defConfig->device.udsName;
		DEBUG_INFO() << "�ɼ���:" << m_defConfig->device.cardName;
		DEBUG_INFO() << "�ɼ���ͨ��:" << m_defConfig->device.cardChannelId << "[û�л�������ϸȷ�ϴ˴�]";

		for (int i = 0; i < m_jsonTool->getErrorList().size(); i++)
		{
			if (!i)
			{
				DEBUG_INFO() << "Json�����ļ����������Դ���:";
			}
			DEBUG_INFO() << m_jsonTool->getErrorList()[i];
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Base::exitConsoleWindow()
{
	return (g_debugInfo && !*g_debugInfo) ? true : FreeConsole() == TRUE;
}

bool Dt::Base::openDevice()
{
	bool result = false;
	do
	{
		m_connect = true;
		if (!m_canTransfer->Connect(500, 0))
		{
			setLastError("����CAN��ʧ��", false, true);
		}

		auto& hardware = m_defConfig->hardware;
		if (!m_power.Open(hardware.powerPort, hardware.powerBaud, hardware.powerVoltage, hardware.powerCurrent))
		{
			setLastError("�򿪵�Դʧ��", false, true);
		}

		if (!m_relay.Open(hardware.relayPort, hardware.relayBaud))
		{
			setLastError("�򿪼̵���ʧ��", false, true);
		}

		if (!m_voltage.Open(hardware.voltagePort, hardware.voltageBaud))
		{
			setLastError("�򿪵�ѹ��ʧ��", false, true);
		}

		if (!m_current.open(hardware.staticPort, hardware.staticBaud))
		{
			setLastError("�򿪵�����ʧ��", false, true);
		}

		result = true;
	} while (false);
	return result;
}

bool Dt::Base::closeDevice()
{
	bool result = false;
	do
	{
		setScanCodeDlg(m_connect = false);

		if (!m_canTransfer->DisConnect())
		{
			setLastError("CAN�Ͽ�����ʧ��", false, true);
		}

		if (!m_power.Output(false))
		{
			setLastError("�رյ�Դʧ��", false, true);
		}

		if (!m_power.Close())
		{
			setLastError("�رյ�Դʧ��", false, true);
		}

		if (!m_relay.Close())
		{
			setLastError("�رռ̵���ʧ��", false, true);
		}

		if (!m_voltage.Close())
		{
			setLastError("�رյ�ѹ��ʧ��", false, true);
		}

		if (!m_current.close())
		{
			setLastError("�رյ�����ʧ��", false, true);
		}

		result = true;
	} while (false);
	return result;
}

bool Dt::Base::prepareTest(const ulong& delay)
{
	setCurrentStatus("׼������");
	setTestResult(BaseTypes::TestResult::TR_TS);
	bool result = false, success = true;
	do
	{
		m_lastError = "No Error";

		saveCanLog(m_defConfig->enable.saveCanLog);

		setCanLogName(m_defConfig->device.modelName, g_code);

		clearListItem();

		m_elapsedTime = GetTickCount();

		addListItem(Q_SPRINTF("��%u���Ʒ��ʼ����", m_total), false);

		initDetectionLog();

		addListItem("�ȴ�ϵͳ����,�����ĵȴ�...");

		RUN_BREAK(!m_power.Output(true), "��Դ�ϵ�ʧ��,��������");

		msleep(300);

		m_canSender.Start();

		if (m_detectionType == BaseTypes::DT_AVM)
		{
			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.acc, true), "��ACCʧ��");
			msleep(300);

			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.gnd, true), "��GNDʧ��");
			msleep(300);

			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.pinboard, true), "��ת�Ӱ�ʧ��");
			msleep(300);

			if (m_defConfig->enable.signalLight)
			{
				RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.white, true), "�򿪰�ɫ�źŵ�ʧ��");
				msleep(300);

				RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.red, false), "�رպ�ɫ�źŵ�ʧ��");
				msleep(300);

				RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.green, false), "�ر���ɫ�źŵ�ʧ��");
				msleep(300);
			}
		}

		if (m_detectionType != BaseTypes::DT_DVR)
		{
			size_t&& startTime = GetTickCount();
			msleep(delay);
			setCurrentStatus(success ? "״̬����" : "״̬�쳣", true);
			addListItem(Q_SPRINTF("ϵͳ����%s��ʱ %.2f��", success ? "�ɹ�" : "ʧ��", static_cast<float>(GetTickCount() - startTime) / 1000));
			addListItem(Q_SPRINTF("ϵͳ���� %s", OK_NG(success)), false);
		}
		else
		{
			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.acc, true), "�̵����պ�ʧ��,��������");
			msleep(300);
		}

		RUN_BREAK(!success, "��ʼ��ϵͳ�쳣");

		result = true;
	} while (false);
	return result;
}

bool Dt::Base::prepareTest(const int& id, const ulong& timeout, const int& req, MsgProc msgProc)
{
	setCurrentStatus("׼������");
	setTestResult(BaseTypes::TestResult::TR_TS);
	bool result = false, success = true;
	do
	{
		m_lastError = "No Error";

		saveCanLog(m_defConfig->enable.saveCanLog);

		setCanLogName(m_defConfig->device.modelName, g_code);

		clearListItem();

		m_elapsedTime = GetTickCount();

		addListItem(Q_SPRINTF("��%u���Ʒ��ʼ����", m_total), false);

		initDetectionLog();

		addListItem("�ȴ�ϵͳ����,�����ĵȴ�...");

		RUN_BREAK(!m_power.Output(true), "��Դ�ϵ�ʧ��,��������");

		msleep(300);

		m_canSender.Start();

		if (m_detectionType == BaseTypes::DT_AVM)
		{
			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.acc, true), "��ACCʧ��");
			msleep(300);

			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.gnd, true), "��GNDʧ��");
			msleep(300);

			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.pinboard, true), "��ת�Ӱ�ʧ��");
			msleep(300);

			if (m_defConfig->enable.signalLight)
			{
				RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.white, true), "�򿪰�ɫ�źŵ�ʧ��");
				msleep(300);

				RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.red, false), "�رպ�ɫ�źŵ�ʧ��");
				msleep(300);

				RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.green, false), "�ر���ɫ�źŵ�ʧ��");
				msleep(300);
			}
		}

		if (m_detectionType != BaseTypes::DT_DVR)
		{
			size_t&& startTime = GetTickCount();
			success = autoProcessCanMsg(id, req, msgProc, timeout);
			setCurrentStatus(success ? "״̬����" : "״̬�쳣", true);
			addListItem(Q_SPRINTF("ϵͳ����%s��ʱ %.2f��", success ? "�ɹ�" : "ʧ��", static_cast<float>(GetTickCount() - startTime) / 1000));
			addListItem(Q_SPRINTF("ϵͳ���� %s", OK_NG(success)), false);
		}
		else
		{
			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.acc, true), "��ACCʧ��");
			msleep(300);
		}

		RUN_BREAK(!success, "��ʼ��ϵͳ�쳣," + getLastError());

		result = true;
	} while (false);
	return result;
}

bool Dt::Base::finishTest(bool success)
{
	bool result = false;
	do
	{
		if (success)
		{
			++m_total;
		}

		flushCanLogBuffer();

		addListItem(Q_SPRINTF("������ʱ %.2f��", (float)(GetTickCount() - m_elapsedTime) / 1000), false);

		setTestResult(success ? BaseTypes::TestResult::TR_OK : BaseTypes::TestResult::TR_NG);

		m_canSender.Stop();

		m_canSender.DeleteAllMsgs();

		RUN_BREAK(!m_power.Output(false), "��Դ����ʧ��,��������");

		msleep(300);

		if (m_detectionType == BaseTypes::DT_AVM)
		{
			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.acc, false), "�ر�ACCʧ��");
			msleep(300);

			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.gnd, false), "�ر�GNDʧ��");
			msleep(300);

			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.pinboard, false), "�ر�ת�Ӱ�ʧ��");
			msleep(300);

			if (m_defConfig->enable.signalLight)
			{
				RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.white, false), "�رհ�ɫ�źŵ�ʧ��");
				msleep(300);

				RUN_BREAK(!(success ? m_relay.SetOneIO(m_defConfig->relay.green, true) :
					m_relay.KeySimulate(m_defConfig->relay.red, 3000)), 
					Q_SPRINTF("�ر�%s�źŵ�ʧ��",success ? "��ɫ":"��ɫ"));
			}
		}
		else if (m_detectionType == BaseTypes::DT_DVR)
		{
			RUN_BREAK(!m_relay.SetOneIO(m_defConfig->relay.acc, false), "�ر�ACCʧ��");
			msleep(300);
		}
		else if (m_detectionType == BaseTypes::DT_HARDWARE)
		{
			RUN_BREAK(!m_relay.SetAllIO(false), "�̵����Ͽ�ʧ��,��������");
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Base::saveLog(bool success)
{
	bool result = false;
	do
	{
		if (success ? true : setQuestionBox("��ʾ", getLastError() + "\n\n���NG�Ƿ�Ҫ������־?"))
		{
			if (!writeLog(success))
			{
				break;
			}
		}

		if (!finishTest(success))
		{
			break;
		}

		if (!success && m_defConfig->enable.unlockDlg)
		{
			setUnlockDlg();
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Base::checkCurrent()
{
	setCurrentStatus("������");
	bool result = false, success = true,deviceFail = false;
	do
	{
		CurrentConfig* info = m_hwdConfig->current;
		for (int i = 0; i < m_jsonTool->getCurrentConfigCount(); i++)
		{
			float voltage = 0.0f;
			RUN_BREAK(deviceFail = !m_power.GetVoltage(&voltage), "��ȡ��ѹʧ��");

			if (fabs(voltage - info[i].voltage) > 0.1)
			{
				RUN_BREAK(deviceFail = !m_power.SetVol(info[i].voltage), "���õ�ѹʧ��");
				msleep(3000);
			}

			RUN_BREAK(deviceFail = !m_power.GetCurrent(&info[i].read), "��ȡ����ʧ��");

			(info[i].read >= info[i].low) && (info[i].read <= info[i].high) ? info[i].result = true : info[i].result = success = false;

			addListItem(Q_SPRINTF("%s  %.3f  %s", info[i].name, info[i].read, OK_NG(info[i].result)));

			WRITE_LOG("%s,%s,%.3f,%.3f,%.3f", OK_NG(info[i].result), info[i].name, info[i].read, info[i].high, info[i].low);
		}
		
		RUN_BREAK(deviceFail, getLastError());

		RUN_BREAK(!success, "������ʧ��");

		result = true;
	} while (false);
	m_power.SetVol(m_defConfig->hardware.powerVoltage);
	addListItem(Q_SPRINTF("������ %s", OK_NG(result)), false);
	return result;
}

bool Dt::Base::checkStaticCurrent(bool setAcc, bool set16Vol)
{
	setCurrentStatus("��⾲̬����");
	bool result = false, success = false;
	do
	{
		addListItem("��⾲̬������Ҫһ��ʱ���Լ30��,�����ĵȴ�...");
		float current = 0.0f;
		RUN_BREAK(!m_power.GetCurrent(&current), "��ȡ��������ʧ��");

		RUN_BREAK(current < 0.1, "ϵͳδ�ϵ�");

		auto& relay = m_defConfig->relay;
		RUN_BREAK(!m_relay.SetOneIO(relay.acc, false), "�̵����ر�ACCʧ��");
		msleep(300);

		ulong startTime = GetTickCount();
		while (true)
		{
			if (m_power.GetCurrent(&current))
			{
				if (current < 0.01)
				{
					success = true;
					break;
				}
			}

			if (GetTickCount() - startTime >= 15000)
			{
				break;
			}
			msleep(300);
		}

		RUN_BREAK(!success, "ϵͳ���߳�ʱ");
		msleep(500);

		RUN_BREAK(!m_relay.SetOneIO(relay.staticCur, true), "�̵�����̬������˿ڴ�ʧ��");
		msleep(300);

		RUN_BREAK(!m_relay.SetOneIO(relay.gnd, false), "�̵���GND�˿ڹر�ʧ��");
		msleep(300);

		StaticConfig& info = m_hwdConfig->staticCurrent;

		HardwareConfig& hardware = m_defConfig->hardware;

		//StaticCurrentMgr staticCurrent;

		//RUN_BREAK(!staticCurrent.open(hardware.staticPort, hardware.staticBaud), G_TO_Q_STR(staticCurrent.getLastError()));

		//RUN_BREAK(!staticCurrent.getStaticCurrent(info.read), G_TO_Q_STR(staticCurrent.getLastError()));

		//staticCurrent.close();

		startTime = GetTickCount();
		success = false;
		QVector<float> currentV;
		int rigth = 0;
		while (true)
		{
			RUN_BREAK(!m_current.getStaticCurrent(info.read), G_TO_Q_STR(m_current.getLastError()));

			currentV.push_back(info.read);

			DEBUG_INFO_EX("���� %.2f %.2f %d", info.read, abs(currentV.back() - currentV.at(currentV.size() - 2)), rigth);

			if (currentV.size() > 2 && abs(currentV.back() - currentV.at(currentV.size() - 2)) <= 1.000f)
			{
				rigth++;
				if (rigth >= 10)
				{
					success = true;
					break;
				}
			}

			RUN_BREAK(GetTickCount() - startTime > 30000, "��⾲̬������ʱ,����δ�ȶ�");

			msleep(200);
		}

		if (!success)
		{
			break;
		}

		info.result = ((info.read >= info.low) && (info.read < info.high));

		addListItem(Q_SPRINTF("��̬����  %.3f  %s", info.read, OK_NG(info.result)));

		WRITE_LOG("%s,��̬����,%.3f,%.3f,%.3f", OK_NG(info.result), info.read, info.high, info.low);

		RUN_BREAK(!info.result, "��⾲̬����ʧ��");

		if (set16Vol)
		{
			RUN_BREAK(!m_power.SetVol(16.0f), "��Դ����16V��ѹʧ��");
		}

		RUN_BREAK(!m_relay.SetOneIO(relay.gnd, true), "�̵���GND�˿ڴ�ʧ��");
		msleep(300);

		RUN_BREAK(!m_relay.SetOneIO(relay.staticCur, false), "�̵�����̬������˿ڹر�ʧ��");
		msleep(300);

		if (setAcc)
		{
			RUN_BREAK(!m_relay.SetOneIO(relay.acc, true), "�̵���ACC�˿ڴ�ʧ��");
		}
		result = true;
	} while (false);
	addListItem(Q_SPRINTF("��⾲̬���� %s", OK_NG(result)), false);
	if (result && setAcc) waitStartup(START_DELAY);
	return result;
}

bool Dt::Base::checkVoltage()
{
	setCurrentStatus("����ѹ");
	bool result = false, success = true, deviceFail = false;
	do
	{
		VoltageConfig* info = m_hwdConfig->voltage;
		for (int i = 0; i < m_jsonTool->getVoltageConfigCount(); i++)
		{
			RUN_BREAK(deviceFail = !m_relay.SetOneIO(info[i].relay, true), "�򿪼̵���ʧ��");
			msleep(1200);

			RUN_BREAK(deviceFail = !m_voltage.ReadVol(&info[i].read), "��ѹ���ȡʧ��");

			(info[i].read >= info[i].low) && (info[i].read <= info[i].high) ? info[i].result = true : info[i].result = success = false;

			addListItem(Q_SPRINTF("%s  %.3f  %s", info[i].name, info[i].read, OK_NG(info[i].result)));

			WRITE_LOG("%s,%s,%.3f,%.3f,%.3f", OK_NG(info[i].result), info[i].name, info[i].read, info[i].high, info[i].low);

			RUN_BREAK(deviceFail = !m_relay.SetOneIO(info[i].relay, false), "�رռ̵���ʧ��");
			msleep(300);
		}
		
		RUN_BREAK(deviceFail, getLastError());

		RUN_BREAK(!success, "����ѹʧ��");

		result = true;
	} while (false);
	addListItem(Q_SPRINTF("����ѹ %s", OK_NG(result)), false);
	return result;
}

bool Dt::Base::clearDtc()
{
	setCurrentStatus("���DTC");
	bool result = false;
	do
	{
		RUN_BREAK(!m_udsTransfer->ClearDiagnosticInformation(), "���DTCʧ��");
		result = true;
	} while (false);
	addListItemEx(Q_SPRINTF("���DTC %s", OK_NG(result)));
	WRITE_LOG("%s ���DTC", OK_NG(result));
	return result;
}

bool Dt::Base::checkVersion(const ulong& delay,const int& tryTimes)
{
	setCurrentStatus("���汾��");
tryAngin:
	bool result = false, success = true;
	int rereadCount = tryTimes + 1;
	do
	{
		clearCanRecvBuffer();
		QList<int> modify;
		auto info = m_udsConfig->ver;
		for (int i = 0; i < m_jsonTool->getVerConfigCount(); i++)
		{
		reread:
			if (!m_udsTransfer->ReadDataByIdentifier(info[i].did[0], info[i].did[1], &info[i].size, (uchar*)info[i].read))
			{
				strcpy(info[i].read, "��ȡʧ��");
				success = info[i].result = false;
				addListItem(Q_SPRINTF("0x%02X%02X  %s  %s  %s",
					info[i].did[0], info[i].did[1],
					info[i].name, info[i].read, OK_NG(info[i].result)));
				continue;
			}

			DEBUG_INFO_EX("0x%02X%02X  %d  %s", info[i].did[0], info[i].did[1], info[i].size, info[i].read);

			udsEncodeConvert(&info[i]);

			if (strncmp(info[i].setup, info[i].read, strlen(info[i].setup)))
			{
				if (rereadCount++ <= tryTimes)
				{
					addListItem(Q_SPRINTF("���¶�ȡ[%s],��%d��", info[i].name, rereadCount));
					msleep(delay);
					goto reread;
				}
				else
				{
					info[i].result = false;
					success = false;
					modify.push_back(i);
				}
			}
			else
			{
				info[i].result = true;
			}
			addListItem(Q_SPRINTF("0x%02X%02X  %s  %s  %s",
				info[i].did[0], info[i].did[1],
				info[i].name, info[i].read, OK_NG(info[i].result)));
			msleep(delay);
		}

		/*�������,������Զ�����*/
		if (!success && m_jsonTool->getUserPrivileges())
		{
			if (setQuestionBox("������ʾ", "���汾���ݲ�ƥ��,\n�Ƿ��Զ��޸�Ϊ��ȷ����?"))
			{
				bool complete = true;
				for (int i = 0; i < modify.size(); i++)
				{
					if (!m_jsonTool->setVerConfigValue(info[modify[i]].name, "ֵ", info[modify[i]].read))
						complete = false;
				}

				if (complete && m_jsonTool->initInstance(true))
				{
					setDetectionLog(BaseTypes::DL_VER);
					addListItem("���Զ�����,���¼��汾��");
					goto tryAngin;
				}
				else
				{
					setMessageBox("����", QString("�Զ��޸�Ϊ��ȷ����ʧ��,\n%1,���ֶ��޸�").arg(m_jsonTool->getLastError()));
				}
			}
		}

		/*д��������־*/
		setDetectionLog(BaseTypes::DL_VER, [&](const int& i)->void {WRITE_LOG("%s %s %s", OK_NG(info[i].result), info[i].name, info[i].read); });

		RUN_BREAK(!success, "���汾��ʧ��");
		result = true;
	} while (false);
	addListItem(Q_SPRINTF("���汾�� %s", OK_NG(result)), false);
	return result;
}

bool Dt::Base::checkDtc()
{
	setCurrentStatus("���DTC");
	bool again = false;
tryAgain:
	bool result = false, success = true;
	QList<int> modify;
	do
	{
		clearCanRecvBuffer();
		int count = 0;
		uchar dtcInfo[512] = { 0 };
		RUN_BREAK(!m_udsTransfer->SafeReadDTCInformation(02, 0xff, &count, dtcInfo), "��ȡDTCʧ��");

		auto config = m_udsConfig->dtc;
		for (int i = 0; i < count / 4; i++)
		{
			for (int j = 0; j < m_jsonTool->getDtcConfigCount(); j++)
			{
				if (config[j].ignore)
				{
					continue;
				}

				if ((dtcInfo[i * 4 + 0] == config[j].dtc[0])
					&& (dtcInfo[i * 4 + 1] == config[j].dtc[1])
					&& (dtcInfo[i * 4 + 2] == config[j].dtc[2]))
				{
					modify.push_back(j);
					config[j].dtc[3] = dtcInfo[i * 4 + 3];
					config[j].exist = true;
					success = false;
					addListItem(Q_SPRINTF("%s  ����  %d", config[j].name, config[j].dtc[3]));
				}
			}
		}

		/*�������DTC*/
		if (!success && !again)
		{
			RUN_BREAK(!m_udsTransfer->ClearDiagnosticInformation(), "���DTCʧ��");
			setDetectionLog(BaseTypes::DL_DTC);
			addListItem("���DTC�ɹ�,���¼��DTC");
			again = true;
			goto tryAgain;
		}

		/*��������DTC,�ڶ������²��Ի�û��ͨ��,������Զ�����*/
		if (!success && again)
		{
			/*������ʾ�ò�Ʒ���ڹ���*/
			addListItem("��ע��,�ò�Ʒ�������DTC,���²���NG,���ܴ��ڹ���");

			/*�����Զ�����,��Ҫ��֤*/
			if (m_jsonTool->getUserPrivileges())
			{
				if (!setQuestionBox("������ʾ", "���DTC�����쳣,\n�Ƿ��Զ����Դ�����Ŀ?"))
				{
					break;
				}

				bool complete = true;
				for (int i = 0; i < modify.size(); i++)
				{
					if (!m_jsonTool->setDtcConfigValue(config[modify[i]].name, "����", "1"))
						complete = false;
				}

				if (complete && m_jsonTool->initInstance(true))
				{
					setDetectionLog(BaseTypes::DL_DTC);
					addListItem("���Զ�����,���¼��DTC");
					goto tryAgain;
				}
				else
				{
					setMessageBox("����", QString("�Զ��޸�Ϊ��ȷ����ʧ��,\n%1,���ֶ��޸�").arg(m_jsonTool->getLastError()));
				}
			}
		}
		RUN_BREAK(!success, "���DTCʧ��,�ò�Ʒ���ܴ��ڹ���,\n����ϵ����Ա.");
		result = true;
	} while (false);

	/*д������DTC��־*/
	setDetectionLog(BaseTypes::DL_DTC, [&](const int& j)->void {WRITE_LOG("%s ���� %d", m_udsConfig->dtc[j].name, m_udsConfig->dtc[j].dtc[3]); });
	addListItem(Q_SPRINTF("���DTC %s", OK_NG(result)), false);
	return result;
}

void Dt::Base::outputCanLog(bool enable)
{
	m_canTransfer->EnableDebugInfo(enable);
}

void Dt::Base::saveCanLog(bool enable)
{
	m_canTransfer->EnableSaveLog(enable);
}

void Dt::Base::setCanLogName(const QString& modelName, const QString& code)
{
	auto nameBytes = modelName.toLocal8Bit();
	auto codeBytes = code.toLocal8Bit();
	m_canTransfer->SetDetectionData(GET_DT_DIR(), nameBytes.data(), codeBytes.data());
}

void Dt::Base::flushCanLogBuffer()
{
	m_canTransfer->NewLogFile();
}

void Dt::Base::clearCanRecvBuffer()
{
	m_canTransfer->ClearRecBuffer();
}

const int Dt::Base::quickRecvCanMsg(MsgNode* msgNode, const int& maxSize, const int& ms)
{
	return m_canTransfer->QuickReceive(msgNode, maxSize, ms);
}

bool Dt::Base::autoProcessCanMsg(const int& id, const int& request, MsgProc msgProc, const ulong& timeout)
{
	bool result = false, success = false, deviceFail = false;
	do
	{
		MsgNode msgNode[512] = { 0 };
		clearCanRecvBuffer();
		size_t&& startTime = GetTickCount();
		while (true)
		{
			const int&& size = quickRecvCanMsg(msgNode, 512, 100);
			for (int i = 0; i < size; i++)
			{
				if (msgNode[i].id == id)
				{
					if (msgProc == nullptr)
					{
						msleep(m_socDelay);
						m_power.GetCurrent(&m_rouseCurrent);

						if (m_rouseCurrent < 0.1f)
						{
							deviceFail = true;
							break;
						}

						if (m_rouseCurrent >= m_defConfig->threshold.canRouse)
						{
							success = true;
							break;
						}
					}
					else
					{
						if (msgProc(request, msgNode[i]))
						{
							success = true;
							break;
						}
					}
				}
			}

			RUN_BREAK(deviceFail, "��Դδ�ϵ�");

			if (success) break;

			RUN_BREAK(GetTickCount() - startTime > timeout, "CAN���Ĵ���ʧ��");
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Base::autoProcessCanMsgEx(IdList idList, ReqList reqList, MsgProc msgProc, const ulong& timeout)
{
	bool result = false, success = false;
	do
	{
		if (idList.size() != reqList.size())
		{
			setLastError("ID�б��������б��С��һ��");
			break;
		}

		QList<int> cmpList;
		MsgNode msgNode[512] = { 0 };
		clearCanRecvBuffer();
		size_t&& startTime = GetTickCount();
		while (true)
		{
			const int&& size = quickRecvCanMsg(msgNode, 512, 100);
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < idList.size(); j++)
				{
					if (msgNode[i].id == idList.begin()[j])
					{
						if (msgProc(reqList.begin()[j], msgNode[i]))
						{
							if (cmpList.size())
							{
								for (auto& cmp : cmpList)
									if (cmp != idList.begin()[j])
										cmpList.append(idList.begin()[j]);
							}
							else
							{
								cmpList.append(idList.begin()[j]);
							}
							break;
						}
					}
				}

				if (cmpList.size() == idList.size())
				{
					success = true;
					break;
				}
			}

			if (success) break;

			RUN_BREAK(GetTickCount() - startTime > timeout, "CAN���Ĵ���ʧ��");
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Base::setCanProcessFnc(const char* name, const CanMsg& msg, const CanProcInfo& procInfo)
{
	return setCanProcessFncEx(name, { msg }, procInfo);
}

bool Dt::Base::setCanProcessFnc(const char* name, const CanMsg& msg, const int& id, const int& req, CanProc proc)
{
	return setCanProcessFnc(name, msg, { id,req,proc });
}

bool Dt::Base::setCanProcessFncEx(const char* name, CanList list, const CanProcInfo& procInfo)
{
	setCurrentStatus(name);
	bool result = false;
	do 
	{
		addListItem(Q_SPRINTF("���ڽ���%s,�����ĵȴ�...", name));
		for (auto& x : list)
		{
			m_canSender.AddMsg(x);
			msleep(x.type == ST_Event ? x.delay * x.count : x.delay);
		}

		if (!autoProcessCanMsg(procInfo.id, procInfo.req, procInfo.proc))
		{
			break;
		}
		result = true;
	} while (false);

	for (auto& x : list)
	{
		if (x.type == ST_Period)
		{
			m_canSender.DeleteOneMsg(x.msg.id);
		}
		else if (x.type == ST_PE)
		{
			//ֻ�����ڴ���򵥵�PE
			CanMsg value = x;
			value.count = 10;
			value.delay = x.delay / 20;
			value.type = ST_Event;
			memset(value.msg.data, 0x00, 8);
			m_canSender.AddMsg(value);
			msleep(value.count * value.delay);
		}
	}
	WRITE_LOG("%s %s", OK_NG(result), name);
	addListItemEx(Q_SPRINTF("%s %s", name, OK_NG(result)));
	return result;
}

bool Dt::Base::setCanProcessFncEx(const char* name, CanList list, const int& id, const int& req, CanProc proc)
{
	return setCanProcessFncEx(name, list, { id,req,proc });
}

bool Dt::Base::setUdsProcessFnc(const char* name, DidList list, const int& req, const int& size, UdsProc proc, const ulong& timeout)
{
	setCurrentStatus(name);
	bool result = false, success = false;
	do
	{
		RUN_BREAK(list.size() != 2, "list��С����Ϊ2");

		ulong&& startTime = GetTickCount();
		int _size = 0;
		uchar _data[BUFF_SIZE] = { 0 };
		while (true)
		{
			clearCanRecvBuffer();
			memset(_data, 0, BUFF_SIZE);
			RUN_BREAK(!readDataByDid(list.begin()[0], list.begin()[1], &_size, _data), getUdsLastError());

			if (g_debugInfo && *g_debugInfo)
			{
				DEBUG_INFO_EX("%s ��ȡ��С %d", name, size);
				for (int i = 0; i < size; i++)
					printf("0x%02X ", _data[i]);
				printf("\n");
			}

			if (size)
			{
				RUN_BREAK(_size != size, "���ݳ��Ȳ�ƥ��");
			}

			if (proc(req, _size, _data))
			{
				success = true;
				break;
			}

			RUN_BREAK(GetTickCount() - startTime > timeout, "UDS�������ݳ�ʱ,����������");
			msleep(100);
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	addListItemEx(Q_SPRINTF("%s %s", name, OK_NG(result)));
	WRITE_LOG("%s %s", OK_NG(result), name);
	return result;
}

bool Dt::Base::setUdsProcessFncEx(const char* name, DidList list, ReqList req, const int& size, UdsProcEx procEx, const ulong& timeout)
{
	setCurrentStatus(name);
	bool result = false, success = false;
	do
	{
		RUN_BREAK(list.size() != 2, "list��С����Ϊ2");

		ulong&& startTime = GetTickCount();
		int _size = 0;
		uchar _data[BUFF_SIZE] = { 0 };
		while (true)
		{
			clearCanRecvBuffer();
			memset(_data, 0, BUFF_SIZE);
			RUN_BREAK(!readDataByDid(list.begin()[0], list.begin()[1], &_size, _data), getUdsLastError());

			if (g_debugInfo && *g_debugInfo)
			{
				DEBUG_INFO_EX("%s ��ȡ��С %d", name, size);
				for (int i = 0; i < size; i++)
					printf("0x%02X ", _data[i]);
				printf("\n");
			}

			if (size)
			{
				RUN_BREAK(_size != size, "���ݳ��Ȳ�ƥ��");
			}

			if (procEx(req, _size, _data))
			{
				success = true;
				break;
			}

			RUN_BREAK(GetTickCount() - startTime > timeout, "UDS�������ݳ�ʱ,����������");
			msleep(100);
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	addListItemEx(Q_SPRINTF("%s %s", name, OK_NG(result)));
	WRITE_LOG("%s %s", OK_NG(result), name);
	return result;
}

void Dt::Base::autoRecycle(const QStringList& path, const QStringList& suffixName, const int& interval)
{
	do
	{
		if (!m_autoRecycle)
		{
			break;
		}

		auto&& currentDate = QDate::currentDate();
		for (int i = 0; i < path.size(); i++)
		{
			auto&& fileList = Misc::getFileListBySuffixName(path[i],
				m_recycleSuffixName.isEmpty() ? suffixName : m_recycleSuffixName);
			for (auto& x : fileList)
			{
				QFileInfo fi(x);
				auto& date = fi.created().date();
				int newYear = currentDate.year() - date.year();
				int current = (currentDate.month() + newYear * 12) - date.month();
				bool recycle = current >= ((m_recycleIntervalMonth == -1) ? interval : m_recycleIntervalMonth);
				DEBUG_INFO_EX("�Զ�����%s,%s���:%d����,��ǰ:%d����,�ļ���:%s", SU_FA(recycle),
					recycle ? "" : "����������,", interval, current, Q_TO_C_STR(x));
				if (recycle)
				{
					QFile::remove(x);
				}
			}
		}
	} while (false);
	return;
}

CanTransfer* Dt::Base::getCanConnect()
{
	return m_canTransfer;
}

CanSender* Dt::Base::getCanSender()
{
	return &m_canSender;
}

CItechSCPIMgr* Dt::Base::getPowerDevice()
{
	return &m_power;
}

CMRDO16KNMgr* Dt::Base::getRelayDevice()
{
	return &m_relay;
}

CVoltageTestMgr* Dt::Base::getVoltageDevice()
{
	return &m_voltage;
}

StaticCurrentMgr* Dt::Base::getCurrentDevice()
{
	return &m_current;
}

void Dt::Base::setAccessLevel(const int& udsLevel)
{
	m_udsLevel = udsLevel;
}

void Dt::Base::setDiagnosticSession(const int& udsSession)
{
	m_udsSession = udsSession;
}

void Dt::Base::restoreAccessLevel()
{
	m_udsLevel = SAL_LEVEL1;
}

void Dt::Base::restoreDiagnosticSession()
{
	m_udsSession = 0x03;
}

bool Dt::Base::enterSecurityAccess(const uchar& session, const uchar& access)
{
	bool result = false;
	do
	{
		RUN_BREAK(!m_udsTransfer->SafeDiagnosticSessionControl(session), "������չģʽʧ��," + getUdsLastError());

		RUN_BREAK(!m_udsTransfer->SafeSecurityAccess(access), "��ȫ����ʧ��," + getUdsLastError());

		result = true;
	} while (false);
	return result;
}

bool Dt::Base::readDataByDid(const uchar& did0, const uchar& did1, int* size, uchar* data)
{
	return m_udsTransfer->ReadDataByIdentifier(did0, did1, size, data);
}

bool Dt::Base::writeDataByDid(const uchar& did0, const uchar& did1, const int& size, const uchar* data)
{
	bool result = false;
	do
	{
		if (!m_udsTransfer->DiagnosticSessionControl(m_udsSession))
		{
			break;
		}

		if (!m_udsTransfer->SecurityAccess(m_udsLevel))
		{
			break;
		}

		if (!m_udsTransfer->WriteDataByIdentifier(did0, did1, size, data))
		{
			break;
		}

		if (!confirmDataByDid(did0, did1, size, data))
		{
			break;
		}
		result = true;
	} while (false);

	if (!result)
	{
		setLastError(getUdsLastError());
	}
	return result;
}

bool Dt::Base::writeDataByDidEx(const uchar* routine, const uchar& did0, const uchar& did1, const int& size, const uchar* data)
{
	bool result = false;
	do
	{
		if (!m_udsTransfer->DiagnosticSessionControl(m_udsSession))
		{
			break;
		}

		if (!m_udsTransfer->SecurityAccess(m_udsLevel))
		{
			break;
		}

		//EP30TAP 1 1
		if (!m_udsTransfer->RoutineControl(routine[0], routine[1], routine[2], routine[3], (uchar*)&routine[4], 0, 0))
		{
			break;
		}

		if (!m_udsTransfer->WriteDataByIdentifier(did0, did1, size, data))
		{
			break;
		}

		if (!confirmDataByDid(did0, did1, size, data))
		{
			break;
		}
		result = true;
	} while (false);

	if (!result)
	{
		setLastError(getUdsLastError());
	}
	return result;
}

bool Dt::Base::writeDataByDidEx(const std::initializer_list<uchar>& routine, const uchar& did0, const uchar& did1, const int& size, const uchar* data)
{
	bool result = false;
	do 
	{
		RUN_BREAK(routine.size() < 5, "���̿��Ʊ���>=5���ֽ�");

		if (!writeDataByDidEx(routine.begin(), did0, did1, size, data))
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Base::confirmDataByDid(const uchar& did0, const uchar& did1, const int& size, const uchar* data)
{
	bool result = false;
	do
	{
		int recvSize = 0;
		uchar buffer[256] = { 0 };

		RUN_BREAK(!readDataByDid(did0, did1, &recvSize, buffer), getUdsLastError());

		RUN_BREAK(size != recvSize, "��ȡ���ȶԱ�ʧ��");

		RUN_BREAK(memcmp(data, buffer, size), "�Ա�����ʧ��");
		result = true;
	} while (false);
	return result;
}

const QString Dt::Base::getUdsLastError()
{
	return G_TO_Q_STR(m_udsTransfer->GetLastError());
}

void Dt::Base::initDetectionLog()
{
	m_logList.clear();

	for (size_t i = 0; i < m_jsonTool->getCurrentConfigCount(); i++)
	{
		m_hwdConfig->current[i].read = 0.0f;
		m_hwdConfig->current[i].result = false;
	}

	for (size_t i = 0; i < m_jsonTool->getResConfigCount(); i++)
	{
		m_hwdConfig->res[i].read = 0.0f;
		m_hwdConfig->res[i].result = false;
	}

	for (size_t i = 0; i < m_jsonTool->getVoltageConfigCount(); i++)
	{
		m_hwdConfig->voltage[i].read = 0.0f;
		m_hwdConfig->voltage[i].result = false;
	}

	for (size_t i = 0; i < m_jsonTool->getVerConfigCount(); i++)
	{
		m_udsConfig->ver[i].result = false;
		memset(m_udsConfig->ver[i].read, 0x00, sizeof(m_udsConfig->ver[i].read));
	}

	for (size_t i = 0; i < m_jsonTool->getDtcConfigCount(); i++)
	{
		m_udsConfig->dtc[i].exist = false;
	}
}

void Dt::Base::setDetectionLog(const BaseTypes::DetectionLog& log, const std::function<void(const int&)>& fnc)
{
	if (log == BaseTypes::DL_ALL)
	{
		m_logList.clear();
	}

	if (log == BaseTypes::DL_CUR || log == BaseTypes::DL_ALL)
	{
		for (size_t i = 0; i < m_jsonTool->getCurrentConfigCount(); i++)
		{
			if (!fnc)
			{
				m_hwdConfig->current[i].read = 0.0f;
				m_hwdConfig->current[i].result = false;
			}
			else
			{
				fnc(i);
			}
		}
	}

	if (log == BaseTypes::DL_RES || log == BaseTypes::DL_ALL)
	{
		for (size_t i = 0; i < m_jsonTool->getResConfigCount(); i++)
		{
			if (!fnc)
			{
				m_hwdConfig->res[i].read = 0.0f;
				m_hwdConfig->res[i].result = false;
			}
			else
			{
				fnc(i);
			}
		}
	}
	
	if (log == BaseTypes::DL_VOL || log == BaseTypes::DL_ALL)
	{
		for (size_t i = 0; i < m_jsonTool->getVoltageConfigCount(); i++)
		{
			if (!fnc)
			{
				m_hwdConfig->voltage[i].read = 0.0f;
				m_hwdConfig->voltage[i].result = false;
			}
			else
			{
				fnc(i);
			}
		}
	}
	
	if (log == BaseTypes::DL_VER || log == BaseTypes::DL_ALL)
	{
		for (size_t i = 0; i < m_jsonTool->getVerConfigCount(); i++)
		{
			if (!fnc)
			{
				m_udsConfig->ver[i].result = false;
				memset(m_udsConfig->ver[i].read, 0x00, HWD_BUF);
			}
			else
			{
				fnc(i);
			}
		}
	}
	
	if (log == BaseTypes::DL_DTC || log == BaseTypes::DL_ALL)
	{
		for (size_t i = 0; i < m_jsonTool->getDtcConfigCount(); i++)
		{
			if (!fnc)
			{
				m_udsConfig->dtc[i].exist = false;
			}
			else
			{
				fnc(i);
			}
		}
	}
}

const QString Dt::Base::createLogFile(bool success)
{
	QString result = "";
	do
	{
		QDir dir;
		auto& device = m_defConfig->device;
		QString logDirName(GET_DT_DIR());
		/*log/error/20200228/����_����_ʱ����.csv*/
		QString filePath = QString("./%1/%2/%3/").arg(logDirName, success ? "NOR" : "ERR", Misc::getCurrentDate(true));
		if (!dir.exists(filePath))
		{
			RUN_BREAK(!dir.mkpath(filePath), "������־·��ʧ��");
		}
		result = filePath.append(QString("%1_%2_%3.csv").arg(device.modelName, g_code.isEmpty() ? "δ֪����" : g_code, Misc::getCurrentTime(true)));
	} while (false);
	return result;
}

bool Dt::Base::writeLog(bool success)
{
	bool result = false;
	do
	{
		QFile file(createLogFile(success));
		RUN_BREAK(!file.open(QFile::WriteOnly), "������־�ļ�ʧ��," + file.errorString());

		QTextStream stream(&file);
		stream << Q_SPRINTF(" ,������,%s,\n,�����,%s,\n", Q_TO_C_STR(g_code), OK_NG(success));

		for (int i = 0; i < m_logList.size(); i++)
			stream << m_logList[i] << endl;

		file.close();
		result = true;
	} while (false);
	return result;
}

void Dt::Base::threadPause()
{
	g_threadWait = true;
	while (g_threadWait) { msleep(100); }
}

bool Dt::Base::threadIsPause()
{
	return g_threadWait;
}

void Dt::Base::threadContinue()
{
	g_threadWait = false;
}

void Dt::Base::threadQuit()
{
	m_connect = false;
	m_quit = true;

	if (threadIsPause())
	{
		closeDevice();
		threadContinue();
	}

	if (isRunning())
	{
		wait(5000);
	}
	quit();
}

bool Dt::Base::setScanCodeDlg(bool show)
{
	emit setScanCodeDlgSignal(show);
	show ? threadPause() : threadContinue();
	return true;
}

void Dt::Base::setUnlockDlg(bool show)
{
	emit setUnlockDlgSignal(show);
	threadPause();
}

void Dt::Base::setMessageBox(const QString& title, const QString& text)
{
	emit setMessageBoxSignal(title, text);
	threadPause();
}

void Dt::Base::setMessageBoxEx(const QString& title, const QString& text, const QPoint& point)
{
	emit setMessageBoxExSignal(title, text, point);
	threadPause();
}

bool Dt::Base::setQuestionBox(const QString& title, const QString& text)
{
	bool result = false;
	emit setQuestionBoxSignal(title, text, &result);
	threadPause();
	return result;
}

bool Dt::Base::setQuestionBoxEx(const QString& title, const QString& text, const QPoint& point)
{
	bool result = false;
	emit setQuestionBoxExSignal(title, text, &result, point);
	threadPause();
	return result;
}

void Dt::Base::setTestResult(const BaseTypes::TestResult& testResult)
{
	emit setTestResultSignal(testResult);
}

void Dt::Base::setCurrentStatus(const QString& status, bool systemStatus)
{
	emit setCurrentStatusSignal(status, systemStatus);
}

void Dt::Base::addListItem(const QString& item, bool logItem)
{
	emit addListItemSignal(QString("%1 %2").arg(Misc::getCurrentTime(), item), logItem);
}

void Dt::Base::addListItemEx(const QString& item)
{
	addListItem(item, false); addListItem(item, true);
}

void Dt::Base::clearListItem()
{
	emit clearListItemSignal();
}

bool Dt::Base::setDownloadDlg(BaseTypes::DownloadInfo* info)
{
	emit setDownloadDlgSignal(info);
	threadPause();
	return info->result;
}

bool Dt::Base::callPythonFnc()
{
	bool result = false;
#if CALL_PYTHON_LIB
	do
	{
		Py_Initialize();
		if (!Py_IsInitialized())
		{
			setLastError("��ʼ��Pythonʧ��");
			break;
		}

		auto pyModule = PyImport_ImportModule("module");
		RUN_BREAK(!pyModule, "����module.pyʧ��");
		auto pyFnc = PyObject_GetAttrString(pyModule, "writeSN");
		RUN_BREAK(!pyFnc, "δ�ҵ�writeSN����");
		auto pyArgs = Py_BuildValue("si", Q_TO_C_STR(g_code), g_code.length());
		auto pyRet = PyObject_CallObject(pyFnc, pyArgs);
		Py_Finalize();
	} while (false);
#endif
	return result;
}

bool Dt::Base::waitStartup(const ulong& delay)
{
	addListItem(Q_SPRINTF("�ȴ�ϵͳ�ȶ���,��Լ��Ҫ%u��,�����ĵȴ�...", delay / 1000));
	msleep(delay);
	return true;
}

bool Dt::Base::checkPing(const char* address, const int& times)
{
	setCurrentStatus("���Ping");
	bool result = false, success = false;
	do
	{
		addListItem(Q_SPRINTF("����Ping %s,�����ĵȴ�...", address));
		success = Misc::ping(address, times);
		addListItem(Q_SPRINTF("Ping %s %s", address, OK_NG(success)));
		RUN_BREAK(!success, Q_SPRINTF("Ping %sʧ��", address));
		result = true;
	} while (false);
	addListItemEx(Q_SPRINTF("���Ping %s", OK_NG(result)));
	WRITE_LOG("%s ���Ping", OK_NG(result));
	return result;
}

void Dt::Base::setLastError(const QString& error)
{
	DEBUG_INFO() << error;
	Misc::writeRunError(error);
	m_lastError = error;
}

void Dt::Base::setLastError(const QString& error, bool addItem, bool msgBox)
{
	DEBUG_INFO() << error;
	Misc::writeRunError(error);
	m_lastError = error;
	if (addItem)
	{
		addListItem(m_lastError);
	}

	if (msgBox)
	{
		QMessageBoxEx::warning(static_cast<QWidget*>(nullptr), "����", error);
	}
}

bool Dt::Base::udsEncodeConvert(VersonConfig* config)
{
	bool bRet = false;
	do
	{
		std::string strDataType = config->encode;
		if (strDataType.find("ASCII") != std::string::npos)
		{

		}
		else if (strDataType.find("U08") != std::string::npos)
		{
			for (int i = 8; i < 4 + 8; i++)
			{
				config->read[i] += ('1' - 1);
			}
		}
		else if (strDataType.find("ASCR4") != std::string::npos)
		{
			for (int i = config->size - 4; i < config->size; i++)
			{
				config->read[i] += ('1' - 1);
			}
		}
		else if (strDataType.find("USN") != std::string::npos)
		{
			if (config->size > sizeof(ULONGLONG))
			{
				DEBUG_INFO_EX("0x%x 0x%x %s,USN���ݱ����ֽڿռ䲻��", config->did[0],
					config->did[1], config->name);
				break;
			}

			ULONGLONG value = 0;
			char temp[sizeof(value)] = { 0 };
			for (int i = 0; i < config->size; i++)
				temp[i] = config->read[config->size - i - 1];
			memcpy(&value, temp, config->size);
			strcpy(config->read, Q_TO_C_STR(QString::number(value)));
		}
		else if (strDataType.find("BCD") != std::string::npos)
		{
			QStringList value;
			for (int i = 0; i < config->size; i++)
				value.append(Q_SPRINTF("%02x", config->read[i]));
			strcpy(config->read, Q_TO_C_STR(value.join("")));
		}
		else if (strDataType.find("BIN") != std::string::npos)
		{
			for (int i = 0; i < config->size; i++)
			{
				config->read[i] += ('1' - 1);
			}
		}
		else if (strDataType.find("INT") != std::string::npos)
		{
			UCHAR ucTemp = config->read[0];
			config->read[0] = config->read[3];
			config->read[3] = ucTemp;

			ucTemp = config->read[1];
			config->read[1] = config->read[2];
			config->read[2] = ucTemp;

			int iTemp = *(int*)&config->read[0];
			sprintf(config->read, "%d", iTemp);
		}
		else if (strDataType.find("ASCBCD44") != std::string::npos)
		{
			char szEnd[64] = { 0 };
			strncpy(szEnd, &config->read[7], config->size - 7);
			char szMid[64] = { 0 };
			for (int i = 3; i < 4 + 3; i++)
			{
				char temp[8] = { 0 };
				sprintf(temp, "%02x", config->read[i]);
				sprintf(&szMid[strlen(szMid)], "%s", temp);
			}
			sprintf(&config->read[3], "%s%s", szMid, szEnd);
		}
		else
		{
			break;
		}

		//���ͷ��β�Ŀո�
		std::string strVer = config->read;
		strVer.erase(0, strVer.find_first_not_of(" "));
		strVer.erase(strVer.find_last_not_of(" ") + 1);
		strcpy(config->read, strVer.c_str());
		bRet = true;
	} while (false);
	return bRet;
}

/************************************************************************/
/* Dt::Hardware realize                                                     */
/************************************************************************/
Dt::Hardware::Hardware(QObject* parent)
{
	m_detectionType = BaseTypes::DetectionType::DT_HARDWARE;
}

Dt::Hardware::~Hardware()
{

}

/************************************************************************/
/* Dt::Function realize                                                 */
/************************************************************************/

Dt::Function::Function(QObject* parent)
{

}

Dt::Function::~Function()
{
	if (m_cvAnalyze)
	{
		cvReleaseImage(&m_cvAnalyze);
	}

	if (m_cvPainting)
	{
		cvReleaseImageHeader(&m_cvPainting);
	}

	if (m_cardConfig.name == MV800_CC)
	{
		m_mv800.DeinitCard();
	}
	else if (m_cardConfig.name == MOR_CC)
	{
		SAFE_DELETE(m_mil);
	}
	else
	{
		SAFE_DELETE(m_captureCard);
	}
}

bool Dt::Function::initInstance()
{
	bool result = false;
	do
	{
		if (!Dt::Base::initInstance())
		{
			break;
		}

		setCaptureCardAttribute();

		m_cvAnalyze = cvCreateImage(cvSize(m_cardConfig.width, m_cardConfig.height), 8, 3);
		RUN_BREAK(!m_cvAnalyze, "m_cvAnalyze�����ڴ�ʧ��");

		m_cvPainting = cvCreateImageHeader(cvSize(m_cardConfig.width, m_cardConfig.height), 8, 3);
		RUN_BREAK(!m_cvPainting, "m_cvPainting�����ڴ�ʧ��");

		if (m_cardConfig.name == MV800_CC)
		{
			RUN_BREAK(!m_mv800.InitCard(1), "��ʼ��MV800_CC�ɼ���ʧ��,����˵����Ƿ�װӲ��������");
		}
		else if (m_cardConfig.name == MOR_CC)
		{
			m_mil = NO_THROW_NEW Cc::Mil(this);
			RUN_BREAK(!m_mil, "Cc::Mil�����ڴ�ʧ��");
		}
		else
		{
			m_captureCard = NO_THROW_NEW Cc::CaptureCard(this);
			RUN_BREAK(!m_captureCard, "CaptureCard�����ڴ�ʧ��");
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Function::openDevice()
{
	bool result = false;
	do 
	{
		if (!Dt::Base::openDevice())
		{
			break;
		}

		if (!openCaptureCard())
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Function::closeDevice()
{
	bool result = false;
	do 
	{
		if (!Dt::Base::closeDevice())
		{
			break;
		}

		closeCaptureCard();
		result = true;
	} while (false);
	return result;
}

bool Dt::Function::checkCanRouseSleep(const MsgNode& msg, const ulong& delay, const int& id, const int& req, MsgProc msgProc)
{
	bool result = false, success = false;
	do
	{
		setCurrentStatus("���CAN����");
		m_canSender.AddMsg(msg, delay);
		m_canSender.Start();
		success = autoProcessCanMsg(id, req, msgProc);
		m_canSender.DeleteOneMsg(msg);
		WRITE_LOG("%s CAN���� %.3fA", OK_NG(success), m_rouseCurrent);
		addListItem(Q_SPRINTF("CAN���� %s %.3fA", OK_NG(success), m_rouseCurrent));
		addListItem(Q_SPRINTF("CAN���� %s", OK_NG(success)), false);
		RUN_BREAK(!success, "CAN����ʧ��");

		success = false;
		setCurrentStatus("���CAN����");
		size_t&& startTime = GetTickCount();
		float current = 0.0f;
		while (true)
		{
			m_power.GetCurrent(&current);

			if (current <= m_defConfig->threshold.canSleep)
			{
				success = true;
				break;
			}

			if (success || GetTickCount() - startTime >= 20000)
			{
				break;
			}
			msleep(300);
		}

		WRITE_LOG("%s CAN���� %.3fA", OK_NG(success), current);
		addListItem(Q_SPRINTF("CAN���� %s %.3fA", OK_NG(success), current));
		addListItem(Q_SPRINTF("CAN���� %s", OK_NG(success)), false);
		RUN_BREAK(!success, "CAN����ʧ��");
		m_relay.SetOneIO(m_defConfig->relay.acc, true);
		msleep(300);
		result = true;
	} while (false);
	return result;
}

bool Dt::Function::checkCanRouseSleep(const int& id, const ulong& delay, const int& req, MsgProc msgProc)
{
	bool result = false;
	do
	{
		MsgNode msg = { 0x100,8,{0} };
		if (!checkCanRouseSleep(msg, 100, id, req, msgProc))
		{
			break;
		}

		if (delay)
		{
			msleep(delay);
		}
		result = true;
	} while (false);
	return result;
}

void Dt::Function::setCaptureCardAttribute()
{
	m_cardConfig.name = m_defConfig->device.cardName;
	m_cardConfig.channelCount = m_defConfig->device.cardChannelCount.toInt();
	m_cardConfig.channelId = m_defConfig->device.cardChannelId.toInt();
	if (m_cardConfig.name == MOR_CC)
	{
		m_cardConfig.width = 640;
		m_cardConfig.height = 480;
	}
	else
	{
		m_cardConfig.width = 720;
		m_cardConfig.height = 480;
	}
	m_cardConfig.size = m_cardConfig.width * m_cardConfig.height * 3;
}

void Dt::Function::startCaptureCard()
{
	if (m_cardConfig.name == MV800_CC)
		m_mv800.StartCapture();
	else if (m_cardConfig.name == MOR_CC)
		m_mil->startCapture();
	else
		m_captureCard->startCapture();
}

void Dt::Function::endCaptureCard()
{
	if (m_cardConfig.name == MV800_CC)
		m_mv800.EndCapture();
	else if (m_cardConfig.name == MOR_CC)
		m_mil->endCapture();
	else
		m_captureCard->stopCapture();
}

bool Dt::Function::openCaptureCard()
{
	bool result = true;
	do 
	{
		if (m_cardConfig.name == MV800_CC)
		{
			if (m_cardConfig.channelCount == 1)
			{
				if (m_mv800.IsConnected())
				{
					break;
				}

				if (!m_mv800.Connect(NULL, NULL, m_cardConfig.width, m_cardConfig.height, 
					Cc::Mv800Proc, m_cardConfig.channelId, this))
				{
					setLastError(QString("��MV800_CC�ɼ���ͨ��%1ʧ��,%2").arg(m_cardConfig.channelId)
						.arg(G_TO_Q_STR(m_mv800.GetLastError())), false, true);
				}
			}
			else
			{
				for (int i = 0; i < m_cardConfig.channelCount; ++i)
				{
					if (m_mv800.IsConnected())
					{
						break;
					}

					if (!m_mv800.Connect(NULL, NULL, m_cardConfig.width, m_cardConfig.height, 
						Cc::Mv800Proc, i, this))
					{
						setLastError(QString("��MV800_CC�ɼ���ͨ��%1ʧ��,%2").arg(i)
							.arg(G_TO_Q_STR(m_mv800.GetLastError())), false, true);
					}
				}
			}
		}
		else if (m_cardConfig.name == MOR_CC)
		{
			QString&& dcfFile = QString("Config/DcfFile_%1/ntsc.dcf").arg(DCF_VERSION);
			if (!QFileInfo(dcfFile).exists())
			{
				if (!m_jsonTool->writeDcfFile(dcfFile))
				{
					setLastError("MOR_CCд��Ĭ��ntsc.dcf�ļ�ʧ��," + m_jsonTool->getLastError(), false, true);
				}
			}

			if (m_mil->isOpen())
			{
				break;
			}
			m_mil->open(dcfFile, m_cardConfig.channelId) ? m_mil->startCapture() : setLastError("��MOR_CC�ɼ���ʧ��," + m_mil->getLastError(), false, true);
		}
		else
		{
			RUN_BREAK(!m_captureCard->openDevice(m_cardConfig.channelId, m_cardConfig.channelCount),
				m_captureCard->getLastError());
			m_captureCard->startCapture();
		}
	} while (false);
	return result;
}

bool Dt::Function::closeCaptureCard()
{
	if (getCaptureCardConnect())
	{
		if (m_cardConfig.name == MV800_CC)
			m_mv800.Disconnect();
		else if (m_cardConfig.name == MOR_CC)
			m_mil->close();
		else
			m_captureCard->closeDevice();
	}
	return true;
}

bool Dt::Function::cycleCapture()
{
	bool result = false, success = true;
	do
	{
		m_capture = true;
		int count = 0;
		while (m_capture)
		{
			msleep(100);
			count++;
			if (count > 50)
			{
				success = false;
				break;
			}
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Function::saveAnalyzeImage(const QString& name, const IplImage* image, const CvSize& size)
{
	bool result = false;
	do
	{
		if (!m_defConfig->image.saveLog)
		{
			result = true;
			break;
		}

		const QString path = QString("%1\\FcLog\\Images").arg(Misc::getCurrentDir());
		QDir dir;
		if (!dir.exists(path))
		{
			RUN_BREAK(!dir.mkpath(path), QString("����Ŀ¼:%1ʧ��").arg(path));
		}

		const QString& fileName = QString("%1\\%2.bmp").arg(path, name);
		Misc::CharSet&& convert = Misc::CharSet(fileName);
		RUN_BREAK(!convert, "������ͼ�����,�ַ���ת��ʧ��");
		if (!size.height || !size.width)
		{
			cvSaveImage(convert, image);
		}
		else
		{
			IplImage* newImage = cvCreateImage(size, image->depth, image->nChannels);
			cvCopy(image, newImage);
			showImage(newImage, name);
			cvSaveImage(convert, newImage);
		}
		result = true;
	} while (false);
	return result;
}

inline void Dt::Function::drawRectOnImage(IplImage* image)
{
	if (m_rectType == FcTypes::RT_NO)
	{
		return;
	}

	if (m_defConfig->image.showBig)
	{
		auto rect = m_defConfig->image.bigRect;
		const int i = static_cast<int>(m_rectType);
		cvRectangleR(image, cvRect(rect[i].startX, rect[i].startY, rect[i].width, rect[i].height), CV_RGB(255, 0, 0), 2);
	}

	if (m_defConfig->image.showSmall)
	{
		auto rect = m_defConfig->image.smallRect;
		for (int i = 0; i < SMALL_RECT_; i++)
		{
			cvRectangleR(image, cvRect(rect[i].startX, rect[i].startY, rect[i].width, rect[i].height), CV_RGB(0, 255, 0), 2);
		}
	}
	return;
}

inline void Dt::Function::drawRectOnImage(cv::Mat& mat)
{
	if (m_rectType == FcTypes::RT_NO)
	{
		return;
	}

	if (m_defConfig->image.showBig)
	{
		auto rect = m_defConfig->image.bigRect;
		const int i = static_cast<int>(m_rectType);
		rectangle(mat, cvRect(rect[i].startX, rect[i].startY, rect[i].width, rect[i].height), CV_RGB(255, 0, 0), 2);
	}

	if (m_defConfig->image.showSmall)
	{
		auto rect = m_defConfig->image.smallRect;
		for (int i = 0; i < SMALL_RECT_; i++)
		{
			rectangle(mat, cvRect(rect[i].startX, rect[i].startY, rect[i].width, rect[i].height), CV_RGB(0, 255, 0), 2);
		}
	}
	return;
}

bool Dt::Function::checkRectOnImage(IplImage* cvImage, const rectConfig_t& rectConfig, QString& colorData)
{
	bool result = false, success = false, syntaxError = false;
	do
	{
		if (m_defConfig->image.saveLog)
		{
			QString fileName = "NoName";
			switch (getRectType())
			{
			case FcTypes::RT_FRONT_BIG:fileName = "all&Front"; break;
			case FcTypes::RT_REAR_BIG:fileName = "all&Rear"; break;
			case FcTypes::RT_LEFT_BIG:fileName = "all&Left"; break;
			case FcTypes::RT_RIGHT_BIG:fileName = "all&Right"; break;
			case FcTypes::RT_SMALL:fileName = "all&Small"; break;
			default:break;
			}
			saveAnalyzeImage(fileName, cvImage);
		}

		cvSetImageROI(cvImage, cvRect(rectConfig.startX, rectConfig.startY, rectConfig.width, rectConfig.height));

		showImage(cvImage, "ROI");

		std::vector<int> vec;

		/*����RGB�ж�*/
		if (m_defConfig->image.ignoreRgb)
		{
			Mat matHsv;
			cvtColor(cvarrToMat(cvImage), matHsv, COLOR_BGR2HSV);

			size_t size = matHsv.cols * matHsv.rows * matHsv.elemSize();
			size_t red = 0, green = 0, blue = 0;
			size_t count = 0;

			/*��ROIƽ��ֵ*/
			for (size_t i = 0; i < size; i += matHsv.elemSize())
			{
				blue += matHsv.data[i];
				green += matHsv.data[i + 1];
				red += matHsv.data[i + 2];
				count++;
			}

			red /= count;
			green /= count;
			blue /= count;

			vec.push_back(blue);
			vec.push_back(green);
			vec.push_back(red);

			char color[32] = { 0 };

			if ((vec[0] >= 0 && vec[0] <= 180) && (vec[1] >= 0 && vec[1] <= 255) && (vec[2] >= 0 && vec[2] <= 46))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 0 && vec[0] <= 180) && (vec[1] >= 0 && vec[1] <= 43) && (vec[2] >= 46 && vec[2] <= 220))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 0 && vec[0] <= 180) && (vec[1] >= 0 && vec[1] <= 35) && (vec[2] >= 221 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else if (((vec[0] >= 0 && vec[0] <= 10) || (vec[0] >= 156 && vec[0] <= 180)) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 11 && vec[0] <= 25) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 26 && vec[0] <= 34) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 35 && vec[0] <= 77) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 78 && vec[0] <= 99) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 100 && vec[0] <= 124) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else if ((vec[0] >= 125 && vec[0] <= 155) && (vec[1] >= 43 && vec[1] <= 255) && (vec[2] >= 46 && vec[2] <= 255))
			{
				strcpy(color, "��ɫ");
			}
			else
			{
				strcpy(color, "δ֪��ɫ");
			}

			QString imageColor = rectConfig.color;

			/*ȥ��imageColor�﷨�����пո�*/

			imageColor.remove(QRegExp("\\s"));

			QStringList configColorList = imageColor.mid(2).split(",", QString::SkipEmptyParts);
			if (imageColor.contains("!="))
			{
				for (auto& x : configColorList)
				{
					if (x != color)
					{
						success = true;
					}
					else
					{
						success = false;
						break;
					}
				}
			}
			else if (imageColor.contains("=="))
			{
				for (auto& x : configColorList)
				{
					if (x == color)
					{
						success = true;
					}
					else
					{
						success = false;
						break;
					}
				}
			}
			else
			{
				syntaxError = true;
			}
			colorData.sprintf("ɫ��ģ��RGB[%03d,%03d,%03d]  �����﷨[%s%s]  ����������ɫ[%s]  %s", 
				vec[2], vec[1], vec[0], Q_TO_C_STR(rectConfig.color), syntaxError ? ",�﷨����" : "", color, OK_NG(success));
		}
		else
		{
			/*RGBʹ�þ�ֵ�㷨*/
			CvScalar mean, stddev;
			cvAvgSdv(cvImage, &mean, &stddev);

			for (int i = 0; i < 3; i++)
			{
				vec.push_back(mean.val[i]);
			}

			int limit = 0, upper = 0;
			int rgb[3] = { rectConfig.red,rectConfig.green,rectConfig.blue };
			for (int i = 0; i < 3; i++)
			{
				limit = rgb[i] - rectConfig.deviation;
				upper = rgb[i] + rectConfig.deviation;
				if (!((vec[abs(i - 2)] >= limit) && (vec[abs(i - 2)] <= upper)))
				{
					success = false;
				}
			}
			colorData.sprintf("����RGB[%03d,%03d,%03d]  ʵ��RGB[%03d,%03d,%03d]  �������[%03d]  %s",
				rectConfig.red, rectConfig.green, rectConfig.blue,
				vec[2], vec[1], vec[0], rectConfig.deviation, OK_NG(success));
		}


		if (m_defConfig->image.saveLog)
		{
			auto& image = m_defConfig->image;
			if (m_rectType == FcTypes::RT_SMALL)
			{
				QStringList nameList = { "frontSmall","rearSmall","leftSmall","rightSmall" };
				for (int i = 0; i < SMALL_RECT_; i++)
				{
					if (!memcmp(&image.smallRect[i], &rectConfig, sizeof(RectConfig)))
					{
						saveAnalyzeImage(nameList.at(i), cvImage, cvSize(rectConfig.width, rectConfig.height));
						break;
					}
				}
			}
			else
			{
				QStringList bigName = { "frontBig","rearBig","leftBig","rightBig" };
				for (int i = 0; i < BIG_RECT_; i++)
				{
					if (!memcmp(&image.bigRect[i], &rectConfig, sizeof(RectConfig)))
					{
						saveAnalyzeImage(bigName.at(i), cvImage, cvSize(rectConfig.width, rectConfig.height));
						break;
					}
				}
			}
		}
		cvResetImageROI(cvImage);

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

void Dt::Function::setRectType(const FcTypes::RectType& rectType)
{
	m_rectType = rectType;
}

const FcTypes::RectType& Dt::Function::getRectType()
{
	return m_rectType;
}

void Dt::Function::restoreRectType()
{
	m_rectType = FcTypes::RT_NO;
}

void Dt::Function::updateImage(const QImage& image)
{
	emit updateImageSignal(image);
}

void Dt::Function::showImage(const IplImage* image, const QString& name)
{
#ifdef QT_DEBUG
	cvNamedWindow(Q_TO_C_STR(name), 0);
	cvShowImage(Q_TO_C_STR(name), image);
	waitKey();
#endif
}

bool Dt::Function::getCaptureCardConnect()
{
	bool result = false;
	if (m_cardConfig.name == MV800_CC)
		result = m_mv800.IsConnected();
	else if (m_cardConfig.name == MOR_CC)
		result = m_mil->isOpen();
	else
		result = m_captureCard->isOpen();
	return result;
}

void Dt::Function::setCaptureImage(bool capture)
{
	m_capture = capture;
}

bool Dt::Function::getCaptureImage()
{
	return m_capture;
}

/************************************************************************/
/* Dt::Avm realize                                                          */
/************************************************************************/
Dt::Avm::Avm(QObject* parent)
{
	m_detectionType = BaseTypes::DetectionType::DT_AVM;
}

Dt::Avm::~Avm()
{
}

bool Dt::Avm::initInstance()
{
	bool result = false;
	do
	{
		if (!Dt::Function::initInstance())
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

void Dt::Avm::setLedLight(bool _switch)
{
	m_relay.SetOneIO(m_defConfig->relay.led, _switch);
	msleep(300);
}

bool Dt::Avm::triggerAvmByKey(const ulong& delay, const int& id, const int& req, MsgProc proc)
{
	bool result = false;
	do 
	{
		RUN_BREAK(!m_relay.KeySimulate(m_defConfig->relay.key, delay), "�����պ�ʧ��");
		if (id && req && proc)
		{
			RUN_BREAK(!autoProcessCanMsg(id, req, proc), "��������AVMʧ��");
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Avm::triggerAvmByMsg(const CanMsg& msg, const int& id, const int& req, MsgProc proc)
{
	bool result = false;
	do 
	{
		m_canSender.AddMsg(msg);
		msleep(msg.type == ST_Event ? msg.delay * msg.count : msg.delay);
		if (id && req && proc)
		{
			RUN_BREAK(!autoProcessCanMsg(id, req, proc), "���Ĵ���AVMʧ��");
		}
		result = true;
	} while (false);
	if (msg.type != ST_Event)
	{
		m_canSender.DeleteOneMsg(msg.msg.id);
	}
	return result;
}

bool Dt::Avm::checkVideoUseNot()
{
	setCurrentStatus("�����Ƶ����");
	bool result = false, success = true;
	do 
	{
		setRectType(FcTypes::RT_SMALL);

		msleep(2000);

		RUN_BREAK(!cycleCapture(), "ץͼʧ��");

		QStringList viewName = { "ǰ","��","��","��" };
		QString colorData;
		for (int i = 0; i < SMALL_RECT_; i++)
		{
			if (!checkRectOnImage(m_cvAnalyze, m_defConfig->image.smallRect[i], colorData))
			{
				success = false;
			}
			addListItem(QString("%1����ͷСͼ,%2").arg(viewName.at(i), colorData));
		}
		RUN_BREAK(!success, "�����Ƶ����ʧ��");
		result = true;
	} while (false);
	restoreRectType();
	WRITE_LOG("%s �����Ƶ����", OK_NG(result));
	addListItem(Q_SPRINTF("�����Ƶ���� %s", OK_NG(result)), false);
	return result;
}

bool Dt::Avm::checkVideoUseMsg(const CanMsg& msg, const int& id, const int& req0, MsgProc msgProc0)
{
	setCurrentStatus("�����Ƶ����");
	bool result = false, success = true;
	do 
	{
		m_canSender.AddMsg(msg);
		m_canSender.Start();

		RUN_BREAK(!autoProcessCanMsg(id, req0, msgProc0, 10000), "����ȫ��ʧ��");

		setRectType(FcTypes::RT_SMALL);

		msleep(2000);

		RUN_BREAK(!cycleCapture(), "ץͼʧ��");

		QStringList viewName = { "ǰ","��","��","��" };
		QString colorData;
		for (int i = 0; i < SMALL_RECT_; i++)
		{
			if (!checkRectOnImage(m_cvAnalyze, m_defConfig->image.smallRect[i], colorData))
			{
				success = false;
			}
			addListItem(QString("%1����ͷСͼ,%2").arg(viewName.at(i), colorData));
		}
		RUN_BREAK(!success, "�����Ƶ����ʧ��");
		result = true;
	} while (false);
	m_canSender.DeleteOneMsg(msg.msg.id);
	restoreRectType();
	WRITE_LOG("%s �����Ƶ����", OK_NG(result));
	addListItem(Q_SPRINTF("�����Ƶ���� %s", OK_NG(result)), false);
	return result;
}

bool Dt::Avm::checkVideoUseKey(const int& id, const int& req, MsgProc msgProc, const ulong& hDelay,
	const ulong& sDelay)
{
	setCurrentStatus("�����Ƶ����");
	bool result = false, success = true;
	do
	{
		setRectType(FcTypes::RT_SMALL);

		if (!checkKeyVoltage(hDelay, sDelay, id, req, msgProc))
		{
			break;
		}

		RUN_BREAK(!cycleCapture(), "ץͼʧ��");

		QStringList viewName = { "ǰ","��","��","��" };
		QString colorData;
		for (int i = 0; i < SMALL_RECT_; i++)
		{
			if (!checkRectOnImage(m_cvAnalyze, m_defConfig->image.smallRect[i], colorData))
			{
				success = false;
			}
			addListItem(QString("%1����ͷСͼ,%2").arg(viewName.at(i), colorData));
		}

		RUN_BREAK(!success, "�����Ƶ����ʧ��");
		result = true;
	} while (false);
	restoreRectType();
	WRITE_LOG("%s �����Ƶ����", OK_NG(result));
	addListItem(Q_SPRINTF("�����Ƶ���� %s", OK_NG(result)), false);
	return result;
}

bool Dt::Avm::checkVideoUsePerson()
{
	setCurrentStatus("�����Ƶ����");
	bool result = false, success = false;
	do 
	{
		success = setQuestionBoxEx("��ʾ", "��Ƶ�Ƿ����?");
		RUN_BREAK(!success, "�����Ƶ����ʧ��");
		result = true;
	} while (false);
	WRITE_LOG("%s �����Ƶ����", OK_NG(result));
	addListItemEx(Q_SPRINTF("�����Ƶ���� %s", OK_NG(result)));
	return result;
}

bool Dt::Avm::checkSingleImageUseMsg(const FcTypes::RectType& type, const CanMsg& msg,
	const int& id, const int& req, MsgProc proc, const ulong& timeout)
{
	setCurrentStatus("��ⵥ��ͼ��");
	bool result = false, success = true;
	do
	{
		RUN_BREAK(type == FcTypes::RT_SMALL
			|| type == FcTypes::RT_NO,
			"��֧�ִ���ο���");

		m_canSender.AddMsg(msg);
		msleep(msg.delay);

		if (proc && !autoProcessCanMsg(id, req, proc, timeout))
		{
			break;
		}

		setRectType(type);

		msleep(2000);

		RUN_BREAK(!cycleCapture(), "ץͼʧ��");

		QStringList viewName = { "ǰ","��","��","��" };
		QString colorData;
		int index = static_cast<int>(type);
		if (!checkRectOnImage(m_cvAnalyze, m_defConfig->image.smallRect[index], colorData))
		{
			success = false;
		}
		addListItem(QString("%1����ͷ��ͼ,%2").arg(viewName.at(index), colorData));
		RUN_BREAK(!success, "��ⵥ��ͼ��ʧ��");
		result = true;
	} while (false);
	restoreRectType();
	m_canSender.DeleteOneMsg(msg);
	WRITE_LOG("%s ��ⵥ��ͼ��", OK_NG(result));
	addListItem(Q_SPRINTF("��ⵥ��ͼ�� %s", OK_NG(result)), false);
	return result;
}

bool Dt::Avm::checkFRViewUseMsg(CanList msgList, const int& id, ReqList reqList, MsgProc proc)
{
	setCurrentStatus("���ǰ����ͼ");
	bool result = false, success = true;
	do
	{
		RUN_BREAK(msgList.size() != 2, "���ǰ����ͼmsgList.size()!=2");

		QStringList viewName = { "ǰ","��","��","��" };
		QString colorData;
		int subscript = -1;
		for (int i = 0; i < msgList.size(); i++)
		{
			subscript = abs(i - 1);

			setRectType(static_cast<FcTypes::RectType>(subscript));

			m_canSender.AddMsg(msgList.begin()[subscript]);

			m_canSender.Start();

			if (!autoProcessCanMsg(id, reqList.begin()[subscript], proc))
			{
				success = false;
				setLastError(Q_SPRINTF("����%s����ͼʧ��", viewName.at(subscript)));
				break;
			}

			msleep(1000);

			if (!cycleCapture())
			{
				setLastError("ץͼʧ��");
				success = false;
				break;
			}

			if (!checkRectOnImage(m_cvAnalyze, m_defConfig->image.bigRect[subscript], colorData))
			{
				success = false;
				setLastError("���ǰ����ͼʧ��");
			}
			m_canSender.DeleteOneMsg(msgList.begin()[subscript]);
			addListItem(QString("%1����ͷ��ͼ,%2").arg(viewName[subscript], colorData));
		}

		if (!success)
		{
			break;
		}
		msleep(1000);
		result = true;
	} while (false);
	restoreRectType();
	WRITE_LOG("%s ���ǰ����ͼ", OK_NG(result));
	addListItem(Q_SPRINTF("���ǰ����ͼ %s", OK_NG(result)), false);
	return result;
}

bool Dt::Avm::checkFRViewUseKey(const int& id, const int& req, MsgProc proc, const ulong& hDelay, const ulong& sDelay)
{
	setCurrentStatus("���ǰ����ͼ");
	bool result = false, success = true;
	do
	{
		QStringList viewName = { "ǰ","��","��","��" };
		QString colorData;
		//�ȼ�����ͼ�ڼ��ǰ��ͼ
		for (int i = 1; i >= 0; i--)
		{
			if (i == 0)
			{
				if (!checkKeyVoltage(hDelay, sDelay, id, req, proc))
				{
					success = false;
					break;
				}
			}

			setRectType(static_cast<FcTypes::RectType>(i));
			msleep(500);

			if (!cycleCapture())
			{
				setLastError("ץͼʧ��");
				success = false;
				break;
			}

			if (!checkRectOnImage(m_cvAnalyze, m_defConfig->image.bigRect[i], colorData))
			{
				success = false;
				setLastError("���ǰ����ͼʧ��");
			}
			addListItem(QString("%1����ͷ��ͼ,%2").arg(viewName[i], colorData));
		}

		if (!success)
		{
			break;
		}
		msleep(1000);
		result = true;
	} while (false);
	restoreRectType();
	WRITE_LOG("%s ���ǰ����ͼ", OK_NG(result));
	addListItem(Q_SPRINTF("���ǰ����ͼ %s", OK_NG(result)), false);
	return result;
}

bool Dt::Avm::checkKeyVoltage(const ulong& hDelay, const ulong& sDelay, const int& id, const int& req, MsgProc proc)
{
	setCurrentStatus("��ⰴ����ѹ");
	bool result = false, success = true;
	do
	{
		KeyVolConfig& keyVol = m_hwdConfig->keyVol;
		RUN_BREAK(!m_voltage.ReadVol(&keyVol.lRead), "��ȡ��ѹ��ʧ��");

		(keyVol.lRead >= keyVol.lLLimit) && (keyVol.lRead <= keyVol.lULimit) ? keyVol.lResult = true : keyVol.lResult = success = false;

		addListItem(Q_SPRINTF("�����͵�ƽ  %.3f  %s", keyVol.lRead, OK_NG(keyVol.lResult)));
		WRITE_LOG("%s �����͵�ƽ %.3f", OK_NG(keyVol.lResult), keyVol.lRead);

		if (!triggerAvmByKey(hDelay, id, req, proc))
		{
			break;
		}

		msleep(sDelay);

		RUN_BREAK(!m_voltage.ReadVol(&keyVol.hRead), "��ȡ��ѹ��ʧ��");

		(keyVol.hRead >= keyVol.hLLimit) && (keyVol.hRead <= keyVol.hULimit) ? keyVol.hResult = true : keyVol.hResult = success = false;

		addListItem(Q_SPRINTF("�����ߵ�ƽ  %.3f  %s", keyVol.hRead, OK_NG(keyVol.hResult)));
		WRITE_LOG("%s �����ߵ�ƽ %.3f", OK_NG(keyVol.hResult), keyVol.hRead);

		RUN_BREAK(!success, "��ⰴ����ѹʧ��");
		result = true;
	} while (false);
	WRITE_LOG("%s ��ⰴ����ѹ", OK_NG(result));
	addListItemEx(Q_SPRINTF("��ⰴ����ѹ %s", OK_NG(result)));
	return result;
}

/************************************************************************/
/* Dt::Dvr realize                                                          */
/************************************************************************/
Dt::Dvr::Dvr(QObject* parent)
{
	m_detectionType = BaseTypes::DetectionType::DT_DVR;
}

Dt::Dvr::~Dvr()
{
	Misc::finishApp("win32_demo.exe");

	SAFE_DELETE(m_dvrClient);

	SAFE_DELETE(m_sfrServer);

	autoRecycle({ m_videoPath,m_photoPath });
}

bool Dt::Dvr::initInstance()
{
	bool result = false;
	do
	{
		if (!Dt::Function::initInstance())
		{
			break;
		}

		m_hashCode.systemStatus = typeid(DvrTypes::SystemStatus).hash_code();

		m_hashCode.wifiStatus = typeid(DvrTypes::WifiStatus).hash_code();

		m_hashCode.ethernetStatus = typeid(DvrTypes::EthernetStatus).hash_code();

		m_hashCode.sdCardStatus = typeid(DvrTypes::SdCardStatus).hash_code();

		m_dvrClient = NO_THROW_NEW Nt::DvrClient(m_address, m_port);
		RUN_BREAK(!m_dvrClient, "Nt::DvrClient�����ڴ�ʧ��");

		m_sfrServer = NO_THROW_NEW Nt::SfrServer;
		RUN_BREAK(!m_sfrServer, "Nt::SfrServer�����ڴ�ʧ��");

		RUN_BREAK(!m_sfrServer->startListen(), m_sfrServer->getLastError());

		Misc::finishApp("win32_demo.exe");

		QString appName = "App\\sfr_client\\bin\\win32_demo.exe";
		if (Misc::isExistKitsPath())
		{
			RUN_BREAK(!Misc::startApp(MY_KITS_PATH + appName, SW_NORMAL, true),
				QString("����%1Ӧ�ó���ʧ��").arg(appName));
		}
		else
		{
			RUN_BREAK(!Misc::startApp(appName, SW_NORMAL), QString("����%1Ӧ�ó���ʧ��").arg(appName));
		}

		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::prepareTest(const ulong& delay)
{
	bool result = false, success = false;
	do
	{
		if (!Dt::Base::prepareTest(delay))
		{
			break;
		}

		RUN_BREAK(!setOtherAction(), "������������ʧ��," + getLastError());
		
		addListItem("���ڼ��ϵͳ״̬,�����ĵȴ�...");
		size_t&& startTime = GetTickCount();
		success = autoProcessStatus(m_systemStatus, delay);
		addListItem(Q_SPRINTF("���ϵͳ״̬%s,��ʱ:%.3f��", OK_NG(success), float(GetTickCount() - startTime) / 1000.000f));
		RUN_BREAK(!success, "ϵͳ��ʼ��ʧ��," + getLastError());

		success = autoProcessStatus(m_sdCardStatus, delay);
		addListItem(Q_SPRINTF("SD��״̬ %s", OK_NG(success)));
		RUN_BREAK(!success, "SD����ʼ��ʧ��," + getLastError());

		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::finishTest(bool success)
{
	bool result = false;
	do
	{
		if (m_wifiMgr.connected() && !m_wifiMgr.disconnect())
		{
			addListItem(G_TO_Q_STR(m_wifiMgr.getLastError()));
		}

		if (!Dt::Base::finishTest(success))
		{
			break;
		}

		if (getSoundLigth())
		{
			RUN_BREAK(!setSoundLight(false), "�̵����Ͽ�ʧ��,��������");
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::getWifiInfo(bool rawData, bool showLog)
{
	setLastError("����δ��д�麯��bool Dt::Dvr::getWifiInfo(bool,bool)");
	return false;
}

void Dt::Dvr::setSysStatusMsg(const DvrTypes::SysStatusMsg& msg)
{
	m_sysStatusMsg = (int)msg;
}

void Dt::Dvr::setSdCardStatus(const DvrTypes::SdCardStatus& status)
{
	m_sdCardStatus = status;
}

void Dt::Dvr::setSystemStatus(const DvrTypes::SystemStatus& status)
{
	m_systemStatus = status;
}

bool Dt::Dvr::checkDvr(const QString& rtspUrl, bool useWifi, bool useCard, bool downloadVideo)
{
	setCurrentStatus("���DVR");
	bool result = false, record = false, success = false;
	do
	{
		if (useCard)
		{
			addListItem("���ɼ�������");
			success = setQuestionBoxEx("��ʾ", "�ɼ��������Ƿ�ɹ�?", QPoint(80, 0));
			endCaptureCard();
			addListItem(Q_SPRINTF("���ɼ������� %s", OK_NG(success)));
			WRITE_LOG("%s �ɼ�������", OK_NG(success));
			RUN_BREAK(!success, "���ɼ�������ʧ��");
		}

		addListItem("���ڼ������״̬,�����ĵȴ�...");
		ulong&& startTime = GetTickCount();
		success = useWifi ? autoProcessStatus<DvrTypes::WifiStatus>() :
			autoProcessStatus<DvrTypes::EthernetStatus>();
		addListItem(Q_SPRINTF("�������״̬%s,��ʱ:%.3f��", success ? "����" : "�쳣",
			float(GetTickCount() - startTime) / 1000.00f));
		RUN_BREAK(!success, "����״̬�쳣");

		addListItem("����������");
		int repalyCount = 1;
	replay1:
		success = vlcRtspStart(rtspUrl);
		RUN_BREAK(!success, "RTSPЭ�����ʧ��");

		//success = setQuestionBoxEx("��ʾ", "��������Ƿ�ɹ�?", QPoint(50, 0));
		int play = setPlayQuestionBox("��ʾ", "��������Ƿ�ɹ�?", QPoint(130, 0));
		if (play == DvrTypes::PR_OK) success = true;
		else if (play == DvrTypes::PR_NG) success = false;
		else
		{
			vlcRtspStop();
			addListItem(Q_SPRINTF("��������ز���%d��", repalyCount));
			if (repalyCount++ >= 5) success = false; else goto replay1;
		}
		vlcRtspStop();
		addListItem(Q_SPRINTF("���������� %s", OK_NG(success)));
		WRITE_LOG("%s �������", OK_NG(success));
		RUN_BREAK(!success, "�������ʧ��");

		if (getSoundLigth())
		{
			RUN_BREAK(!setSoundLight(false), "�ر�����͵ƹ�ʧ��");
		}

		addListItem("��ȡ����¼���ļ�·��");
		QString url;
		success = getFileUrl(url, DvrTypes::FP_EVT);
		addListItem("��ȡ����¼���ļ�·��:" + success ? url : "��Ч·��");
		RUN_BREAK(!success, "��ȡ����¼���ļ�·��ʧ��");

		if (downloadVideo)
		{
			addListItem("���ؽ���¼���ļ�,��Լ��Ҫ10~30��,��ȴ�...");
			//success = downloadFile(url, "EVTDownload");
			success = downloadFile(url, DvrTypes::FileType::FT_VIDEO);
			addListItem(Q_SPRINTF("���ؽ���¼���ļ� %s", OK_NG(success)));
			RUN_BREAK(!success, "���ؽ���¼���ļ�ʧ��," + getLastError());
		}

		msleep(1000);

		addListItem("���Ž���¼����Ƶ��...");
		repalyCount = 1;
	replay2:
		success = vlcRtspStart(url);
		RUN_BREAK(!success, "���Ž���¼����Ƶʧ��");
		//success = setQuestionBoxEx("��ʾ", "����¼���Ƿ�ط�?", QPoint(80, 0));
		play = setPlayQuestionBox("��ʾ", "����¼����Ƶ�Ƿ�ط�?", QPoint(130, 0));
		if (play == DvrTypes::PR_OK) success = true;
		else if (play == DvrTypes::PR_NG) success = false;
		else
		{
			vlcRtspStop();
			addListItem(Q_SPRINTF("����¼����Ƶ�ز���%d��", repalyCount));
			if (repalyCount++ >= 5) success = false; else goto replay2;
		}
		addListItem(Q_SPRINTF("����¼����Ƶ�ط� %s", OK_NG(success)));
		vlcRtspStop();
		WRITE_LOG("%s ����¼��", OK_NG(success));
		RUN_BREAK(!success, "����¼����Ƶ�ط�ʧ��");
		result = true;
	} while (false);
	addListItem(Q_SPRINTF("���DVR %s", OK_NG(result)), false);
	return result;
}

bool Dt::Dvr::checkDvr(bool useWifi, bool useCard, bool downloadVideo)
{
	return checkDvr(QString("rtsp://%1/stream2").arg(m_address), useWifi, useCard, downloadVideo);
}

bool Dt::Dvr::setSoundLight(bool enable)
{
	bool result = false;
	do
	{
		if (!m_relay.SetOneIO(m_defConfig->relay.led, enable))
		{
			break;
		}
		msleep(300);

		if (!m_relay.SetOneIO(m_defConfig->relay.sound, true))
		{
			break;
		}
		msleep(150);

		if (!m_relay.SetOneIO(m_defConfig->relay.sound, false))
		{
			break;
		}
		msleep(300);
		m_soundLight = enable;
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::getSoundLigth()
{
	return m_soundLight;
}

bool Dt::Dvr::setSound(bool enable)
{
	bool result = false;
	do 
	{
		if (!m_relay.SetOneIO(m_defConfig->relay.sound, true))
		{
			break;
		}
		msleep(150);

		if (!m_relay.SetOneIO(m_defConfig->relay.sound, false))
		{
			break;
		}
		msleep(300);

		m_soundLight = enable;
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::setLight(bool enable)
{
	bool result = false;
	do 
	{
		if (!m_relay.SetOneIO(m_defConfig->relay.led, enable))
		{
			break;
		}
		msleep(300);

		result = true;
	} while (false);
	return result;
}

void Dt::Dvr::setVlcMediaHwnd(HWND vlcHwnd)
{
	m_vlcHwnd = vlcHwnd;
}

bool Dt::Dvr::vlcRtspStart(const QString& url)
{
	bool result = false;
	do
	{
		RUN_BREAK(url.isEmpty(), "RTSPЭ���ַΪ��");

		const char* const vlcArgs[] = {
			//"--rtsp-frame-buffer-size=1000000",
			"--ipv4",
			"--no-prefer-system-codecs",
			"--rtsp-caching=300",
			"--network-caching=500",
			"--rtsp-tcp"
		};

		if (!m_vlcInstance)
		{
			m_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs);
			RUN_BREAK(!m_vlcInstance, "����vlcʵ��ʧ��,��ȷ���ļ����Ƿ����\n[lua ,plugins ,libvlc.dll ,libvlccore.dll]");
		}

		if (!m_vlcMedia)
		{
			m_vlcMedia = libvlc_media_new_location(m_vlcInstance, url.toStdString().c_str());
			RUN_BREAK(!m_vlcMedia, "����vlcý��ʧ��");
		}

		if (!m_vlcMediaPlayer)
		{
			m_vlcMediaPlayer = libvlc_media_player_new_from_media(m_vlcMedia);
			RUN_BREAK(!m_vlcMediaPlayer, "����vlcý�岥����ʧ��");
		}

		RUN_BREAK(!m_vlcHwnd, "�����setVlcMediaHwnd���ò��ſؼ����");

		libvlc_media_player_set_hwnd(m_vlcMediaPlayer, m_vlcHwnd);

		RUN_BREAK(libvlc_media_player_play(m_vlcMediaPlayer) == -1, "VLCý�岥����������Ƶʧ��");

		m_updateSfr.startUpdate();
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::vlcRtspStop()
{
	bool result = false;
	do
	{
		if (m_vlcInstance)
		{
			libvlc_release(m_vlcInstance);
			m_vlcInstance = nullptr;
		}

		if (m_vlcMedia)
		{
			libvlc_media_release(m_vlcMedia);
			m_vlcMedia = nullptr;
		}

		if (m_vlcMediaPlayer)
		{
			libvlc_media_player_stop(m_vlcMediaPlayer);
			libvlc_media_player_release(m_vlcMediaPlayer);
			m_vlcMediaPlayer = nullptr;
		}
		m_updateSfr.stopUpdate();
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::getFileUrl(QString& url, const DvrTypes::FilePath& filePath)
{
	bool result = false;
	do
	{
		const char* dirName = (filePath == DvrTypes::FP_PHO ? "��Ƭ" : "��Ƶ");
		msleep(1000);
		char recvData[BUFF_SIZE] = { 0 };
		int recvLen = 0, tryAgainCount = 0;
		RUN_BREAK(!m_dvrClient->connect(), m_dvrClient->getLastError());

	tryAgain:
		DEBUG_INFO_EX("���ͻ�ȡ%s�ļ��б���", dirName);
		RUN_BREAK(!m_dvrClient->sendFrameDataEx({ (char)filePath, (char)filePath, 0x01, 0x01 },
			DvrTypes::NC_FILE_CONTROL, DvrTypes::NS_GET_FILE_LIST), m_dvrClient->getLastError());

		memset(recvData, 0x00, BUFF_SIZE);
		RUN_BREAK(!m_dvrClient->recvFrameDataEx(recvData, &recvLen, DvrTypes::NC_FILE_CONTROL,
			DvrTypes::NS_GET_FILE_LIST), m_dvrClient->getLastError());


		FileList dvrFileList = { 0 };
		memcpy(&dvrFileList.listCount, &recvData[2], sizeof(size_t));

		dvrFileList.listCount = dvrFileList.listCount > 100 ? 100 : dvrFileList.listCount;

		DEBUG_INFO_EX("%s�ļ��б�����:%lu", dirName, dvrFileList.listCount);

		/*����û�л�ȡ�����,�����ٴλ�ȡ,ԭ�������??δ֪*/
		if (dvrFileList.listCount == 0)
		{
			tryAgainCount++;
			if (tryAgainCount >= 6)
			{
				writeNetLog(filePath == DvrTypes::FP_EVT ? "getEvtUrl" : "getPhoUrl", recvData, recvLen);
				setLastError(Q_SPRINTF("%s�ļ��б�Ϊ��,��������%d��,\n��ȷ��SD�����Ƿ�����ļ�", dirName, tryAgainCount));
				break;
			}
			goto tryAgain;
		}

		const char* pointer = &recvData[6];
		const char* dvrPath[] = { "NOR/", "EVT/", "PHO/" };
		const char* dvrType[] = { "NOR_", "EVT_", "PHO_", "_D1_" };
		const char* dvrSuffix[] = { ".mp4", ".jpg" };

		int maxDate = 0, flag = 0;

		for (int i = 0; i < dvrFileList.listCount; i++)
		{
			memcpy(&dvrFileList.fileInfo[i].index, pointer, 2);
			pointer += 2;
			memcpy(&dvrFileList.fileInfo[i].path, pointer, 1);
			pointer++;
			memcpy(&dvrFileList.fileInfo[i].type, pointer, 1);
			pointer++;
			memcpy(&dvrFileList.fileInfo[i].suffix, pointer, 1);
			pointer += 4;
			memcpy(&dvrFileList.fileInfo[i].size, pointer, 4);
			pointer += 4;
			memcpy(&dvrFileList.fileInfo[i].date, pointer, 4);
			pointer += 4;

			if (dvrFileList.fileInfo[i].date >= maxDate)
			{
				maxDate = dvrFileList.fileInfo[i].date;
				flag = i;
			}
		}

		int pathId = dvrFileList.fileInfo[flag].path;
		int typeId = dvrFileList.fileInfo[flag].type;

		RUN_BREAK((pathId < 0 || pathId > 2) || (typeId < 0 || typeId > 3),
			"��ȡDVR�ļ��б����ݰ��쳣,\n�������������Ƿ��в���");

		url.sprintf("http://%s:%d/%s%s", Q_TO_C_STR(m_address), 8080, dvrPath[pathId], dvrType[typeId]);
		/*�˴�Ҫ��ȥʱ��*/
		time_t dvrSecond = dvrFileList.fileInfo[flag].date - 8 * 60 * 60;

		/*ͨ��localtime������ת��Ϊ �� �� �� ʱ �� ��*/
		struct tm* dvrDate = localtime(&dvrSecond);
		RUN_BREAK(!dvrDate, "localtime����һ��nullptr�쳣");

		url.append(Q_SPRINTF("%04d%02d%02d_%02d%02d%02d_%05d",
			dvrDate->tm_year + 1900,
			dvrDate->tm_mon + 1,
			dvrDate->tm_mday,
			dvrDate->tm_hour,
			dvrDate->tm_min,
			dvrDate->tm_sec,
			dvrFileList.fileInfo[flag].index));
		url.append(dvrSuffix[dvrFileList.fileInfo[flag].suffix]);
		result = true;
	} while (false);
	m_dvrClient->disconnect();
	return result;
}

bool Dt::Dvr::downloadFile(const QString& url, const QString& dirName, bool isVideo)
{
	bool result = false, success = true;
	do
	{
		RUN_BREAK(isVideo ? (dirName != m_videoPath) : (dirName != m_photoPath),
			"Ŀ¼��ƥ��,�޷������Զ����ջ���,\n��ʹ��bool downloadFile(const QString&,const DvrTypes::FileType&)����");

		QString path = QString("%1/%2/%3").arg(Misc::getCurrentDir(), dirName, Misc::getCurrentDate());
		if (!QDir(path).exists())
		{
			QDir dir;
			dir.mkpath(path);
		}

		QString destFile = path + url.mid(url.lastIndexOf("/"));
		DeleteUrlCacheEntryW(Q_TO_WC_STR(url));
		ulong startDownloadTime = GetTickCount();
		HRESULT downloadResult = URLDownloadToFileW(NULL, Q_TO_WC_STR(url), Q_TO_WC_STR(destFile), NULL, NULL);
		float endDownloadTime = (GetTickCount() - startDownloadTime) / 1000.0f;
		RUN_BREAK(downloadResult != S_OK, "URLDownloadToFile�����ļ�ʧ��");

		struct _stat64i32 stat = { 0 };
		_stat64i32(Q_TO_C_STR(destFile), &stat);
		float fileSize = stat.st_size / 1024.0f / 1024.0f;
		float networkSpeed = fileSize / endDownloadTime;
		QString downloadInfo = Q_SPRINTF("�ļ���С:%.2fMB,������ʱ:%.2f��,ƽ���ٶ�:%.2fM/��", fileSize, endDownloadTime, networkSpeed);
		/*��Ƶ������Ҫ�����ٴ���*/
		if (isVideo)
		{
			auto& range = m_defConfig->range;
			success = (networkSpeed >= range.minNetworkSpeed && networkSpeed <= range.maxNetworkSpeed);
			downloadInfo.append(Q_SPRINTF(",���ٷ�Χ:%.2fM~%.2fM %s", range.minNetworkSpeed, range.maxNetworkSpeed, OK_NG(success)));
			WRITE_LOG("%s ���� %.2f", OK_NG(success), networkSpeed);
		}
		addListItem(downloadInfo);
		RUN_BREAK(!success, "������ֵ���ڷ�Χ֮��");
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::downloadFile(const QString& url, const DvrTypes::FileType& types)
{
	//return downloadFile(url, isVideo ? m_videoPath : m_photoPath, isVideo);
	bool result = false, success = true;
	do
	{
		int tryAgainCount = 1;
	tryAgain:
		bool isVideo = (types == DvrTypes::FT_VIDEO);
		BaseTypes::DownloadInfo info;
		info.title.sprintf("����DVR%s", isVideo ? "����¼����Ƶ" : "��Ƭ");
		info.url = url;
		info.path = QString("%1\\%2").arg(isVideo ? m_videoPath : m_photoPath).arg(Misc::getCurrentDate());
		//RUN_BREAK(!setDownloadDlg(&info), info.error);
		if (!setDownloadDlg(&info)/* && info.error == "Connection closed"*/)
		{
			addListItem(Q_SPRINTF("�������Ӳ��ȶ�����ʧ��%s,���Ե�%d��", Q_TO_C_STR(info.error), tryAgainCount));
			RUN_BREAK(tryAgainCount++ >= 3, info.error);
			msleep(100);
			goto tryAgain;
		}

		info.speed /= 1024;
		QString downloadInfo = Q_SPRINTF("�ļ���С:%.2fMB,������ʱ:%.2f��,ƽ���ٶ�:%.2fM/��", info.size, float(info.time) / 1000.00f, info.speed);
		/*��Ƶ������Ҫ�����ٴ���*/
		if (isVideo)
		{
			auto& range = m_defConfig->range;
			success = (info.speed >= range.minNetworkSpeed && info.speed <= range.maxNetworkSpeed);
			downloadInfo.append(Q_SPRINTF(",���ٷ�Χ:%.2fM~%.2fM %s", range.minNetworkSpeed, range.maxNetworkSpeed, OK_NG(success)));
			WRITE_LOG("%s ���� %.2f", OK_NG(success), info.speed);
		}
		addListItem(downloadInfo);
		RUN_BREAK(!success, "������ֵ���ڷ�Χ֮��");
		result = true;
	} while (false);
	return result;
}

void Dt::Dvr::setDownloadFileDir(const DvrTypes::FileType& types, const QString& dirName)
{
	(types == DvrTypes::FT_VIDEO) ? m_videoPath = dirName : m_photoPath = dirName;
}

bool Dt::Dvr::checkRayAxis(const QString& url, const QString& dirName)
{
	setCurrentStatus("������");
	bool result = false;
	do
	{
		QString localPath = QString("%1/%2/%3/%4").arg(Misc::getCurrentDir(), dirName, Misc::getCurrentDate(), Misc::getFileNameByUrl(url));
		DEBUG_INFO() << "��Ƭ·�� " << localPath << endl;

		Misc::CharSet&& asciiPath = Misc::CharSet(localPath);
		RUN_BREAK(!(const char*)asciiPath, "localPath�ַ���ת��ʧ��");
		IplImage* grayImage = cvLoadImage(asciiPath, CV_LOAD_IMAGE_GRAYSCALE);
		RUN_BREAK(!grayImage, Q_SPRINTF("��Ч��·��:\n%s", asciiPath.getData()));
		grayBuffer_t grayBuffer = { 0 };
		grayBuffer.buffer = (uchar*)grayImage->imageData;
		grayBuffer.height = grayImage->height;
		grayBuffer.width = grayImage->width;

		threshold_t threshold = { 0 };
		threshold.xAxis = 100;
		threshold.yAxis = 100;

		axisStandard_t axisStandard = { 0 };
		axisStandard.height = 40;
		axisStandard.width = 120;
		axisStandard.x = 100;
		axisStandard.y = 100;

		cross_t cross = { 0 };
		cross = calculateCross(&grayBuffer, &threshold, &axisStandard);

		cvReleaseImage(&grayImage);
		RUN_BREAK(cross.iResult != 0, "�������ʧ��");

		auto& range = m_defConfig->range;

		bool tsX = false, tsY = false, tsA = false;
		tsX = (cross.x >= range.minRayAxisX && cross.x <= range.maxRayAxisX) ? true : result = false;
		addListItem(Q_SPRINTF("����X:%.2f,��Χ:%.2f~%.2f %s", cross.x, range.minRayAxisX, range.maxRayAxisX, tsX ? "OK" : "NG"));
		WRITE_LOG("%s ����X %.2f", OK_NG(tsX), cross.x);

		tsY = (cross.y >= range.minRayAxisY && cross.y <= range.maxRayAxisY) ? true : result = false;
		addListItem(Q_SPRINTF("����Y:%.2f,��Χ:%.2f~%.2f %s", cross.y, range.minRayAxisY, range.maxRayAxisY, tsY ? "OK" : "NG"));
		WRITE_LOG("%s ����Y %.2f", OK_NG(tsY), cross.y);

		tsA = (cross.angle >= range.minRayAxisA && cross.angle <= range.maxRayAxisA) ? true : result = false;
		addListItem(Q_SPRINTF("����Ƕ�:%.2f,��Χ:%.2f~%.2f %s", cross.angle, range.minRayAxisA, range.maxRayAxisA, tsA ? "OK" : "NG"));
		WRITE_LOG("%s ����Ƕ� %.2f", OK_NG(tsA), cross.angle);

		QString errorInfo;
		if (!tsX) errorInfo.append("����X,");
		if (!tsY) errorInfo.append("����Y,");
		if (!tsA) errorInfo.append("����Ƕ�,");
		RUN_BREAK(!tsX || !tsY || !tsA, errorInfo.append("��ֵ�����趨��Χ֮��"));
		result = true;
	} while (false);
	addListItem(Q_SPRINTF("������ %s", OK_NG(result)), false);
	return result;
}

bool Dt::Dvr::checkSfr(const QString& url, const QString& dirName)
{
	setCurrentStatus("�������");
	bool result = false;
	do
	{
		QString localPath = QString("%1/%2/%3/%4").arg(Misc::getCurrentDir(), dirName, Misc::getCurrentDate(), Misc::getFileNameByUrl(url));
		DEBUG_INFO() << "��Ƭ·�� " << localPath << endl;

		Misc::CharSet&& asciiPath = Misc::CharSet(localPath);
		RUN_BREAK(!(const char*)asciiPath, "localPath�ַ���ת��ʧ��");

		IplImage* source = cvLoadImage(asciiPath);
		RUN_BREAK(!source, Q_SPRINTF("��Ч��·��:\n%s", asciiPath.getData()));

		QString destFile = localPath;
		destFile.replace(".jpg", ".bmp");
		DEBUG_INFO() << "ת����Ƭ·�� " << destFile << endl;

		asciiPath.qstringToMultiByte(destFile);
		RUN_BREAK(!(const char*)asciiPath, "destFile�ַ���ת��ʧ��");
		cvSaveImage(asciiPath, source);
		cvReleaseImage(&source);

		float value = 0.0f;
		RUN_BREAK(!m_sfrServer->getSfr(asciiPath, value), m_sfrServer->getLastError());
		auto& range = m_defConfig->range;
		bool success = ((value >= range.minSfr) && (value <= range.maxSfr));
		addListItem(Q_SPRINTF("ͼ������:%.2f,��Χ:%.2f~%.2f %s", value, range.minSfr, range.maxSfr, OK_NG(success)));
		WRITE_LOG("%s ����� %.2f", OK_NG(success), value);
		RUN_BREAK(!success, "�������ֵ���ڷ�Χ֮��");

		result = true;
	} while (false);
	addListItem(Q_SPRINTF("������� %s", OK_NG(result)), false);
	return result;
}

bool Dt::Dvr::checkRayAxisSfrUseMsg(CanList list, const int& id, const int& req, MsgProc proc)
{
	bool result = false;
	do
	{
		if (!setCanProcessFncEx("��������", list, id, req, proc) ||
			!checkRayAxisSfr())
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::checkRayAxisSfrUseNet()
{
	bool result = false;
	do 
	{
		if (!networkPhotoGraph() || !checkRayAxisSfr())
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::checkRayAxisSfr()
{
	bool result = false, success = false;
	do 
	{
		addListItem("��ȡ��Ƭ�ļ�·��");
		QString url;
		success = getFileUrl(url, DvrTypes::FP_PHO);
		addListItem(QString("��ȡ��Ƭ�ļ�·��:%1").arg(success ? url : "��Ч��·��"));
		RUN_BREAK(!success, "��ȡ��Ƭ�ļ�·��ʧ��,\n" + getLastError());

		addListItem("����DVR��Ƭ,�����ĵȴ�...");
		//success = downloadFile(url, "PHODownload", false);
		success = downloadFile(url, DvrTypes::FileType::FT_PHOTO);
		addListItem(Q_SPRINTF("����DVR��Ƭ %s", OK_NG(success)));
		RUN_BREAK(!success, "����DVR��Ƭʧ��,\n" + getLastError());

		addListItem("������");
		success = checkRayAxis(url, "PHODownload");
		addListItem(Q_SPRINTF("������ %s", OK_NG(success)));
		RUN_BREAK(!success, "������ʧ��,\n" + getLastError());

		addListItem("�������");
		success = checkSfr(url, "PHODownload");
		addListItem(Q_SPRINTF("������� %s", OK_NG(success)));
		RUN_BREAK(!success, "�������ʧ��,\n" + getLastError());
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::formatSdCard(bool pauseRecord)
{
	setCurrentStatus("��ʽ��SD��");
	bool result = false;
	int tryAgainCount = 0;
tryAgain:
	do
	{
		msleep(1000);
		char data[BUFF_SIZE] = { 0 };
		int len = 0;
		RUN_BREAK(!m_dvrClient->connect(), m_dvrClient->getLastError());
		if (pauseRecord)
		{
			DEBUG_INFO() << "������ͣѭ��¼�Ʊ���";
			addListItem("������ͣѭ��¼�Ʊ���");
			RUN_BREAK(!m_dvrClient->sendFrameDataEx({ 0x00 }, 0x02, 0x00), m_dvrClient->getLastError());
			RUN_BREAK(!m_dvrClient->recvFrameDataEx(data, &len, DvrTypes::NC_FAST_CONTROL,
				DvrTypes::NS_FAST_CYCLE_RECORD), m_dvrClient->getLastError());
			addListItem(Q_SPRINTF("��ͣѭ��¼�� %s", OK_NG(*(uint*)&data[2] == 0)));
			//writeNetLog("pauseRecord", data, len, *(int*)&data[2] == 0);
			RUN_BREAK(*(uint*)&data[2], Q_SPRINTF("��ͣѭ��¼��ʧ��,�����������:0x%X", *(uint*)&data[2]));
			msleep(1000);
			memset(data, 0x00, BUFF_SIZE);
		}
		DEBUG_INFO() << "���͸�ʽ��SD������";
		addListItem("���͸�ʽ��SD������");
		RUN_BREAK(!m_dvrClient->sendFrameDataEx({ }, 0x12, 0x20), m_dvrClient->getLastError());
		RUN_BREAK(!m_dvrClient->recvFrameDataEx(data, &len, 0x12, 0x20), m_dvrClient->getLastError());
		//writeNetLog("formatSDCard", data, len, *(int*)&data[2] == 0);
		if (m_sysStatusMsg != DvrTypes::SSM_CHJ_M01)
		{	//M01��ʽ��SD��һֱʧ��,��Ʒ˵������Ƿ�ɹ�
			RUN_BREAK(*(uint*)&data[2], Q_SPRINTF("��ʽ��SD��ʧ��,�����������:0x%X", *(uint*)&data[2]));
		}
		result = true;
	} while (false);
	m_dvrClient->disconnect();
	if (!result && ++tryAgainCount <= 3 && m_sysStatusMsg != DvrTypes::SSM_CHJ_M01)
	{
		addListItem(Q_SPRINTF("����ʧ��,���Ե�%d��", tryAgainCount));
		msleep(1000);
		goto tryAgain;
	}
	addListItemEx(Q_SPRINTF("��ʽ��SD�� %s", OK_NG(result)));
	WRITE_LOG("%s ��ʽ��SD��", OK_NG(result));
	return result;
}

bool Dt::Dvr::umountSdCard()
{
	setCurrentStatus("ж��SD��");
	bool result = false;
	do
	{
		msleep(1000);
		char data[BUFF_SIZE] = { 0 };
		int len = 0;
		DEBUG_INFO() << "����ж��SD������";
		RUN_BREAK(!m_dvrClient->connect(), m_dvrClient->getLastError());
		RUN_BREAK(!m_dvrClient->sendFrameDataEx({}, 0x12, 0x22), m_dvrClient->getLastError());
		RUN_BREAK(!m_dvrClient->recvFrameDataEx(data, &len, 0x12, 0x22), m_dvrClient->getLastError());
		RUN_BREAK(*(uint*)&data[2], Q_SPRINTF("ж��SD��ʧ��,�����������:0x%X", *(uint*)&data[2]));
		result = true;
	} while (false);
	m_dvrClient->disconnect();
	addListItemEx(Q_SPRINTF("ж��SD�� %s", OK_NG(result)));
	WRITE_LOG("%s ж��SD��", OK_NG(result));
	return result;
}

bool Dt::Dvr::changeWifiPassword()
{
	setCurrentStatus("�޸�WIFI����");
	bool result = false, success = false;
	QString newPassword;
	do
	{
		QString oldPassword = m_wifiInfo.password;

		if (!getWifiInfo(true, false))
		{
			break;
		}

		char word[62] =
		{
			'0','1','2','3','4','5','6','7','8','9',
			'q','w','e','r','t','y','u','i','o','p',
			'a','s','d','f','g','h','j','k','l',
			'z','x','c','v','b','n','m',
			'Q','W','E','R','T','Y','U','I','O','P',
			'A','S','D','F','G','H','J','K','L',
			'Z','X','C','V','B','N','M'
		};

		srand((uint)time(NULL));
		for (int i = 0; i < 8; i++)
		{
			newPassword.append(word[rand() % 62]);
		}

		DEBUG_INFO() << "�����޸�WIFI���뱨��";
		addListItem("��ʼ�����޸�WIFI���뱨��");
		char data[256] = { 0 };
		int len = 0;
		memcpy(&data[00], m_wifiInfo.account, 8);
		memcpy(&data[50], Q_TO_C_STR(newPassword), 8);
		RUN_BREAK(!m_dvrClient->connect(), m_dvrClient->getLastError());
		RUN_BREAK(!m_dvrClient->sendFrameDataEx(data, 100, 0x12, 0x05), m_dvrClient->getLastError());
		memset(data, 0xff, sizeof(data));
		RUN_BREAK(!m_dvrClient->recvFrameDataEx(data, &len, 0x12, 0x05), m_dvrClient->getLastError());
		RUN_BREAK(*(uint*)&data[2], "�޸�WIFI����ʧ��");
		addListItem("����У��WIFI����,�����ĵȴ�...");
		size_t&& starTime = GetTickCount();
		while (true)
		{
			if (!getWifiInfo(false, false))
			{
				break;
			}

			DEBUG_INFO_EX("WIFI����������: %s", m_wifiInfo.password);
			if (newPassword == m_wifiInfo.password)
			{
				success = true;
				break;
			}
			RUN_BREAK(GetTickCount() - starTime > 20000, "У��WIFI���볬ʱ");
			msleep(100);
		}

		addListItem("WIFI���������: " + oldPassword);
		addListItem("WIFI���������: " + newPassword);
		addListItem(Q_SPRINTF("WIFI����������: %s", m_wifiInfo.password));
		addListItem(Q_SPRINTF("У��WIFI���� %s", OK_NG(success)));
		RUN_BREAK(!success, "У��WIFI����ʧ��");
		result = true;
	} while (false);
	m_dvrClient->disconnect();
	WRITE_LOG("%s �޸�WIFI���� %s", OK_NG(result), Q_TO_C_STR(newPassword));
	addListItemEx(Q_SPRINTF("�޸�WIFI���� %s", OK_NG(result)));
	return result;
}

void Dt::Dvr::setAddressPort(const QString& address, const ushort& port)
{
	m_address = address;
	m_port = port;
}

bool Dt::Dvr::writeNetLog(const char* name, const char* data, const size_t& size)
{
	bool result = false;
	do
	{
		QString path = QString("./FcLog/NET/%1/").arg(Misc::getCurrentDate(true));
		Misc::makePath(path);

		QString fileName(name);
		fileName.insert(0, Misc::getCurrentTime(true));
		path.append(fileName).append(".net");

		QFile file(path);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError("д��������־�ļ�ʧ��," + file.errorString());
			break;
		}

		char buffer[0x10] = { 0 };
		sprintf(buffer, "%s\n", name);
		file.write(buffer, strlen(buffer));
		for (size_t i = 0; i < size; i++)
		{
			memset(buffer, 0x00, sizeof(buffer));
			sprintf(buffer, "0x%02X\t", (uchar)data[i]);
			file.write(buffer, strlen(buffer));
			if ((i % 10 == 0) && i != 0)
			{
				file.write("\r\n", strlen("\r\n"));
			}
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

bool Dt::Dvr::networkPhotoGraph()
{
	setCurrentStatus("��������");
	bool result = false;
	do
	{
		addListItem("���ڽ�����������,�����ĵȴ�...");
		int len = 0;
		char data[32] = { 0 };
		RUN_BREAK(!m_dvrClient->connect(), m_dvrClient->getLastError());
		RUN_BREAK(!m_dvrClient->sendFrameDataEx(nullptr, 0, DvrTypes::NC_FAST_CONTROL,
			DvrTypes::NS_FAST_PHOTOGRAPHY), m_dvrClient->getLastError());
		RUN_BREAK(!m_dvrClient->recvFrameDataEx(data, &len, DvrTypes::NC_FAST_CONTROL,
			DvrTypes::NS_FAST_PHOTOGRAPHY), m_dvrClient->getLastError());
		RUN_BREAK(*(uint*)&data[2], Q_SPRINTF("��������ʧ��,�������0x%X", *(uint*)&data[2]));
		result = true;
	} while (false);
	m_dvrClient->disconnect();
	addListItemEx(Q_SPRINTF("�������� %s", OK_NG(result)));
	WRITE_LOG("%s ��������", OK_NG(result));
	return result;
}

Misc::UpdateSfr* Dt::Dvr::getUpdateSfr()
{
	return &m_updateSfr;
}

Nt::SfrServer* Dt::Dvr::getSfrServer()
{
	return m_sfrServer;
}

const int Dt::Dvr::setPlayQuestionBox(const QString& title, const QString& text, const QPoint& point)
{
	int result = DvrTypes::PR_NG;
	emit setPlayQuestionBoxSignal(title, text, &result, point);
	threadPause();
	return result;
}

void WINAPI Cc::Mv800Proc(const uchar* head, const uchar* bits, LPVOID param)
{
	do 
	{
		static QImage image;
		Dt::Function* function = reinterpret_cast<Dt::Function*>((static_cast<VideoSteamParam*>(param))->pArgs);
		if (!function)
		{
			break;
		}

		if (*static_cast<VideoSteamParam*>(param)->piChannelID != function->m_cardConfig.channelId)
		{
			break;
		}

		if (function->m_connect)
		{
			function->m_cvPainting->imageData = (char*)bits;
			if (function->m_capture)
			{
				memcpy(function->m_cvAnalyze->imageData, bits, function->m_cardConfig.size);
				cvFlip(function->m_cvAnalyze, function->m_cvAnalyze, 0);
				function->m_capture = false;
			}

			/*��������ͼתΪ������ͼ*/
			cvFlip(function->m_cvPainting, function->m_cvPainting, 0);

			function->drawRectOnImage(function->m_cvPainting);

			if (Misc::cvImageToQtImage(function->m_cvPainting, &image))
			{
				function->updateImage(image);
			}
		}
	} while (false);
	return;
}

/************************************************************************/
/* Nt::DvrClient realize                                                */
/************************************************************************/
Nt::DvrClient::DvrClient()
{

}

Nt::DvrClient::DvrClient(const QString& address, const ushort& port)
{
	setAddressPort(address, port);
}

Nt::DvrClient::~DvrClient()
{
	if (!m_disconnected)
	{
		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
		}
		WSACleanup();
	}
}

void Nt::DvrClient::setAddressPort(const QString& address, const ushort& port)
{
	strcpy(m_address, Q_TO_C_STR(address));
	m_port = port;
}

bool Nt::DvrClient::connect(const int& count)
{
	return connect(m_address, m_port, count);
}

bool Nt::DvrClient::connect(const QString& address, const ushort& port, const int& count)
{
	bool result = false, success = false;
	do
	{
		if (m_connected)
		{
			result = true;
			break;
		}

		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		RUN_BREAK(WSAStartup(sockVersion, &wsaData) != 0, Q_SPRINTF("WSAStartup��ʼ��ʧ��,�������:%d", WSAGetLastError()));

		for (size_t i = 0; i < count; i++)
		{
			m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
			RUN_BREAK(m_socket == INVALID_SOCKET, Q_SPRINTF("�׽��ֳ�ʼ��ʧ��,�������:%d", WSAGetLastError()));

			int timeout = 3000;
			setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
			setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

			memset(&m_sockAddr, 0x00, sizeof(sockaddr_in));
			m_sockAddr.sin_addr.S_un.S_addr = inet_addr(Q_TO_C_STR(address));
			m_sockAddr.sin_family = AF_INET;
			m_sockAddr.sin_port = htons(port);

			timeval tv = { 0 };
			fd_set set = { 0 };
			ulong argp = 1;
			ioctlsocket(m_socket, FIONBIO, &argp);
			int error = -1;
			int length = sizeof(int);
			if (::connect(m_socket, (const sockaddr*)&m_sockAddr, sizeof(m_sockAddr)) == SOCKET_ERROR)
			{
				tv.tv_sec = 1;
				tv.tv_usec = 0;
				FD_ZERO(&set);
				FD_SET(m_socket, &set);

				if (select(m_socket + 1, NULL, &set, NULL, &tv) > 0)
				{
					getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&error, &length);
					if (error == 0)
					{
						DEBUG_INFO_EX("���ӳɹ�,ʣ�����Ӵ���%d��", count - i - 1);
						success = true;
						break;
					}
					else
					{
						closesocket(m_socket);
						success = false;
					}
				}
				else
				{
					DEBUG_INFO_EX("����ʧ��,������ѯ����,ʣ�����Ӵ���%d��", count - i - 1);
					closesocket(m_socket);
					success = false;
				}
			}
			else
			{
				success = true;
				break;
			}
		}

		ulong argp = 0;
		ioctlsocket(m_socket, FIONBIO, &argp);

		if (!success)
		{
			setLastError("���ӷ�������ʱ");
			WSACleanup();
			break;
		}
		m_connected = result = true;
		m_disconnected = false;
	} while (false);
	return result;
}

void Nt::DvrClient::disconnect()
{
	if (!m_connected)
		return;

	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	WSACleanup();
	m_connected = false;
	m_disconnected = true;
}

int Nt::DvrClient::send(const char* buffer, const int& len)
{
	int result = 0, count = len;
	while (count > 0)
	{
		result = ::send(m_socket, buffer, count, 0);
		if (result == SOCKET_ERROR)
		{
			setLastError(Q_SPRINTF("����ʧ��,�׽��ִ���,�������:%d", WSAGetLastError()));
			return -1;
		}

		if (result == 0)
		{
			setLastError(Q_SPRINTF("����ʧ��,��ʧ���ݰ�,�������:%d", WSAGetLastError()));
			return len - count;
		}

		buffer += result;
		count -= result;
	}
	return len;
}

bool Nt::DvrClient::sendFrameData(const char* buffer, const int& len, const uchar& cmd, const uchar& sub)
{
	uchar data[BUFF_SIZE] = { 0 };
	data[0] = 0xEE;
	data[1] = 0xAA;
	int sendLen = 2 + len;
	memcpy(&data[2], &sendLen, sizeof(int));
	data[6] = cmd;
	data[7] = sub;
	if (buffer != nullptr && len != 0)
		memcpy(&data[8], buffer, len);
	size_t&& crc32 = crc32Algorithm(&data[2], 4 + 2 + len, 0);
	for (int i = 0; i < 4; i++)
	{
		data[8 + len + i] = (crc32 >> i * 8) & 0xff;
	}
	return send((const char*)data, len + 12) == (len + 12);
}

bool Nt::DvrClient::sendFrameDataEx(const std::initializer_list<char>& buffer, const uchar& cmd, const uchar& sub)
{
	return sendFrameData(buffer.size() ? buffer.begin() : nullptr, buffer.size(), cmd, sub);
}

bool Nt::DvrClient::sendFrameDataEx(const char* buffer, const int& len, const uchar& cmd, const uchar& sub)
{
	return sendFrameData(buffer, len, cmd, sub);
}

int Nt::DvrClient::recv(char* buffer, const int& len)
{
	int total = len;
	ulong&& startTime = GetTickCount();
	while (total > 0)
	{
		int count = ::recv(m_socket, buffer, 1, 0);
		if (count == SOCKET_ERROR)
		{
			setLastError(Q_SPRINTF("����ʧ��,�׽��ִ���,�������:%d", WSAGetLastError()));
			return -1;
		}

		if (count == 0)
		{
			setLastError(Q_SPRINTF("����ʧ��,��ʧ���ݰ�,�������:%d", WSAGetLastError()));
			return len - total;
		}

		buffer += count;
		total -= count;

		if (GetTickCount() - startTime > 6000)
		{
			return len - total;
		}
	}
	return len;
}

bool Nt::DvrClient::recvFrameData(char* buffer, int* const recvLen)
{
	bool success = false;
	uchar data[BUFF_SIZE] = { 0 };
	char* dataPtr = (char*)data;
	int tempLen = 0, dataLen = 0;
	ulong&& startTime = GetTickCount();
	while (true)
	{
		int count = recv(dataPtr, 1);
		if (count == -1)
		{
			break;
		}

		if (count == 1)
		{
			tempLen++;
			dataPtr++;
		}

		//У��֡ͷ
		if (tempLen == 2)
		{
			if (data[0] == 0xEE || data[1] == 0xAA)
			{
				dataPtr = (char*)&data[2];
			}
			else
			{
				tempLen = 0;
				dataPtr = (char*)data;
			}
		}

		//��ȡ���ݳ���
		if (tempLen == 6)
		{
			memcpy(&dataLen, &data[2], 4);
		}

		//У��CRC
		if (tempLen == dataLen + 10)
		{
			uint crc32Recv = 0, crc32Result = 0;
			memcpy(&crc32Recv, &data[dataLen + 6], 4);
			crc32Result = crc32Algorithm((uchar const*)(&data[2]), dataLen + 4, 0);
			if (crc32Result != crc32Recv)
			{
				tempLen = 0;
				dataPtr = (char*)data;
			}
			else
			{
				if (g_debugInfo && *g_debugInfo)
				{
					printf("\nReceive Start--------------------\n");
					for (int i = 0; i < tempLen; i++)
						printf("%02X ", data[i]);
					printf("\nReceive End  --------------------\n");
				}

				if (recvLen)
				{
					*recvLen = dataLen;
				}
				memcpy(buffer, &data[6], dataLen);
				success = true;
				break;
			}
		}

		if (GetTickCount() - startTime > 5000)
		{
			break;
		}
		Sleep(1);
	}
	return success;
}

bool Nt::DvrClient::recvFrameDataEx(char* buffer, int* const len, const uchar& cmd, const uchar& sub)
{
	bool result = false, success = false;
	ulong&& startTime = GetTickCount();
	do 
	{
		while (true)
		{
			if (recvFrameData(buffer, len))
			{
				if ((uchar)buffer[0] == cmd && (uchar)buffer[1] == sub)
				{
					success = true;
					break;
				}
			}

			RUN_BREAK(GetTickCount() - startTime > 10000, Q_SPRINTF("CMD:%02X,SUB:%02X,�������ݳ�ʱ", cmd, sub));
		}

		if (!success)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

const size_t Nt::DvrClient::crc32Algorithm(uchar const* memoryAddr, const size_t& memoryLen, const size_t& oldCrc32)
{
	size_t oldcrc32 = oldCrc32, length = memoryLen, crc32, oldcrc;
	uchar ccc, t;

	while (length--)
	{
		t = (uchar)(oldcrc32 >> 24U) & 0xFFU;
		oldcrc = DvrTypes::crc32Table[t];
		ccc = *memoryAddr;
		oldcrc32 = (oldcrc32 << 8U | ccc);
		oldcrc32 = oldcrc32 ^ oldcrc;
		memoryAddr++;
	}
	crc32 = oldcrc32;
	return crc32;
}

const char* Nt::DvrClient::getAddress()
{
	return m_address;
}

const ushort& Nt::DvrClient::getPort()
{
	return m_port;
}

void Nt::DvrClient::setLastError(const QString& error)
{
	DEBUG_INFO() << error;
	m_lastError = error;
}

const QString& Nt::DvrClient::getLastError()
{
	return m_lastError;
}

/************************************************************************/
/* Mc::Mil realize                                                      */
/************************************************************************/
void Cc::Mil::run()
{
	DEBUG_INFO_EX("MIL�ɼ����߳�%lu������", (ulong)QThread::currentThreadId());
	QImage image;
	IplImage* currentImage = cvCreateImage(cvSize(m_function->m_cardConfig.width, m_function->m_cardConfig.height), 8, 3);
	if (!currentImage)
	{
		setLastError("currentImage�����ڴ�ʧ��");
		return;
	}

	while (!m_quit)
	{
		if (m_function->m_connect && m_capture)
		{
			MbufGetColor(MilImage, M_PACKED + M_BGR24, M_ALL_BANDS, currentImage->imageData);
			MbufClear(MilImage, 0);

			if (m_function->m_capture)
			{
				memcpy(m_function->m_cvAnalyze->imageData, currentImage->imageData, m_function->m_cardConfig.size);
				m_function->m_capture = false;
			}

			m_function->drawRectOnImage(currentImage);

			if (Misc::cvImageToQtImage(currentImage, &image))
			{
				m_function->updateImage(image);
			}
		}
		msleep(40);
	}
	cvReleaseImage(&currentImage);
	quit();
	DEBUG_INFO_EX("MIL�ɼ����߳�%lu���˳�", (ulong)QThread::currentThreadId());
	return;
}

void Cc::Mil::setLastError(const QString& error)
{
	DEBUG_INFO() << error;
	Misc::writeRunError(error);
	m_lastError = error;
}

Cc::Mil::Mil(QObject* parent)
{
	m_function = reinterpret_cast<Dt::Function*>(parent);
}

Cc::Mil::~Mil()
{
	if (m_open)
	{
		close();
	}

	if (isRunning())
	{
		wait(3000);
	}
	m_function = nullptr;
}

bool Cc::Mil::open(const QString& name, const int& channel)
{
	bool result = false;
	do
	{
		if (m_open)
		{
			result = true;
			break;
		}

		RUN_BREAK(channel < 0 || channel > 1, Q_SPRINTF("MIL�ɼ���ͨ�����Ϊ%d,��֧�ֵ�ͨ�����", channel));

		//MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);//���öԻ��򱨴�

		RUN_BREAK(!MappAlloc(M_DEFAULT, &MilApplication), "MappAllocʧ��");

		RUN_BREAK(!MsysAlloc(M_SYSTEM_MORPHIS, M_DEF_SYSTEM_NUM, M_SETUP, &MilSystem), "MsysAllocʧ��");

		RUN_BREAK(!MdigAllocA(MilSystem, M_DEFAULT, name.toLocal8Bit().data(), M_DEFAULT, &MilDigitizer), "MdigAllocʧ��");

		MIL_INT miX = MdigInquire(MilDigitizer, M_SIZE_X, M_NULL);
		MIL_INT miY = MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL);
		MIL_INT miBand = MdigInquire(MilDigitizer, M_SIZE_BAND, M_NULL);
		MbufAllocColor(MilSystem, miBand, miX, miY, 8L + M_UNSIGNED, M_IMAGE + M_BASIC_BUFFER_PURPOSE, &MilImage);
		MdigControl(MilDigitizer, M_GRAB_MODE, M_SYNCHRONOUS);
		MdigControl(MilDigitizer, M_CAMERA_LOCK, M_ENABLE);

		MdigControl(MilDigitizer, M_GRAB_AUTOMATIC_INPUT_GAIN, M_DISABLE);
		MdigControl(MilDigitizer, M_GRAB_INPUT_GAIN, 50);

		MappControl(M_ERROR, M_PRINT_DISABLE);

		MbufClear(MilImage, 0);

		RUN_BREAK(!MilDigitizer, "MilDigitizerʧ��");

		MdigControl(MilDigitizer, M_CAMERA_LOCK, M_DISABLE);
		MdigControl(MilDigitizer, M_CHANNEL, m_channel[channel]);
		MdigControl(MilDigitizer, M_CAMERA_LOCK, M_ENABLE);
		MdigGrabContinuous(MilDigitizer, MilImage);
		m_quit = false;
		result = true;
		m_open = true;
	} while (false);
	return result;
}

void Cc::Mil::close()
{
	m_open = false;
	m_quit = true;
	if (MilDigitizer) MdigHalt(MilDigitizer);
	if (MilImage) MbufFree(MilImage);
	if (MilDigitizer) MdigFree(MilDigitizer);
	if (MilSystem) MsysFree(MilSystem);
	if (MilApplication) MappFree(MilApplication);
}

bool Cc::Mil::isOpen()
{
	return m_open;
}

void Cc::Mil::startCapture()
{
	if (!this->isRunning())
	{
		this->start();
	}
	m_capture = true;
}

void Cc::Mil::endCapture()
{
	m_capture = false;
}

const QString& Cc::Mil::getLastError()
{
	return m_lastError;
}

/************************************************************************/
/* Nt::SfrServer realize                                                    */
/************************************************************************/
void Nt::SfrServer::setLastError(const QString& error)
{
	DEBUG_INFO() << error;
	m_lastError = error;
}

Nt::SfrServer::SfrServer()
{
}

Nt::SfrServer::~SfrServer()
{
	closeListen();
}

void Nt::SfrServer::sfrProcThread(void* arg)
{
	Nt::SfrServer* sfrServer = static_cast<Nt::SfrServer*>(arg);
	while (!sfrServer->m_quit)
	{
		sockaddr_in clientAddr = { 0 };
		int addrLen = sizeof(sockaddr_in);
		SOCKET clientSocket = accept(sfrServer->m_socket, (sockaddr*)&clientAddr, &addrLen);
		if (clientSocket == -1)
		{
			break;
		}
		if (sfrServer->m_client != INVALID_SOCKET)
		{
			closesocket(sfrServer->m_client);
			sfrServer->m_client = INVALID_SOCKET;
		}
		sfrServer->m_client = clientSocket;
		Sleep(100);
	}
	return;
}

bool Nt::SfrServer::startListen(const ushort& port)
{
	bool result = false;
	do
	{
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(sockVersion, &wsaData) != 0)
		{
			setLastError(Q_SPRINTF("WSAStartup��ʼ��ʧ��,�������:%d", WSAGetLastError()));
			break;
		}

		m_socket = socket(AF_INET, SOCK_STREAM, 0);

		if (m_socket == INVALID_SOCKET)
		{
			setLastError(Q_SPRINTF("�׽��ֳ�ʼ��ʧ��,�������:%d", WSAGetLastError()));
			break;
		}

		int timeout = 1000, optval = 1;
		setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
		setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
		setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));

		memset(&m_sockAddr, 0x00, sizeof(sockaddr_in));
		m_sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		m_sockAddr.sin_family = AF_INET;
		m_sockAddr.sin_port = htons(port);

		if (bind(m_socket, (const sockaddr*)&m_sockAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		{
			setLastError(Q_SPRINTF("SFR����˰�ʧ��,�������:%d", WSAGetLastError()));
			break;
		}

		if (listen(m_socket, 128) == SOCKET_ERROR)
		{
			setLastError(Q_SPRINTF("SFR����˼���ʧ��,�������:%d", WSAGetLastError()));
			break;
		}

		_beginthread(Nt::SfrServer::sfrProcThread, 0, this);
		result = true;
	} while (false);
	return result;
}

bool Nt::SfrServer::getSfr(const char* filePath, float& sfr)
{
	bool result = false;
	do
	{
		int sendLen = 208;
		char sendData[256] = { 0 };

		sprintf(sendData, "$THC001%s", filePath);
		sendData[sendLen - 1] = '$';
		int count = send(sendData, sendLen);
		if (count != sendLen)
		{
			break;
		}

		int recvLen = 208;
		char recvData[256] = { 0 };
		count = recv(recvData, recvLen);
		//���SFR���Ǹ����д��������,���ʧ�ܻ᷵��SOCKET_ERROR
		if (count == SOCKET_ERROR || recvData[0] != '$')
		{
			sfr = 0.0f;
		}
		else
		{
			if (strncmp(recvData, "$HTR000", 7))
			{
				setLastError("SFR�ͻ��������쳣,$HTR000");
				break;
			}

			if (sscanf(&recvData[7], "%f", &sfr) != 1)
			{
				setLastError("SFR�ͻ��������쳣,���ֵ��Ϊ1");
				break;
			}
		}
		result = true;
	} while (false);
	return result;
}

int Nt::SfrServer::send(const char* buffer, const int& len)
{
	int count = len, result = 0;
	while (count > 0)
	{
		result = ::send(m_client, buffer, count, 0);
		if (result == SOCKET_ERROR)
		{
			setLastError(Q_SPRINTF("����ʧ��,�׽��ִ���,�������:%d", WSAGetLastError()));
			return -1;
		}

		if (result == 0)
		{
			setLastError(Q_SPRINTF("����ʧ��,���ݰ���ʧ,�������:%d", WSAGetLastError()));
			return len - count;
		}

		buffer += result;
		count -= result;
	}
	return len;
}

int Nt::SfrServer::recv(char* buffer, const int& len)
{
	int count = len, result = 0;
	while (count > 0)
	{
		result = ::recv(m_client, buffer, count, 0);
		if (result == SOCKET_ERROR)
		{
			setLastError(Q_SPRINTF("����ʧ��,�׽��ִ���,�������:%d", WSAGetLastError()));
			return -1;
		}

		if (result == 0)
		{
			setLastError(Q_SPRINTF("����ʧ��,���ݰ���ʧ,�������:%d", WSAGetLastError()));
			return len - count;
		}

		buffer += result;
		count -= result;
	}
	return len;
}

void Nt::SfrServer::closeListen()
{
	m_quit = true;
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	
	if (m_client != INVALID_SOCKET)
	{
		closesocket(m_client);
		m_client = INVALID_SOCKET;
	}
	WSACleanup();
}

const QString& Nt::SfrServer::getLastError()
{
	return m_lastError;
}

Dt::Tap::Tap(QObject* parent)
{
	m_detectionType = BaseTypes::DT_TAP;
}

Dt::Tap::~Tap()
{
	SAFE_DELETE_A(m_serialPortTool);
}

bool Dt::Tap::initInstance()
{
	bool result = false;
	do
	{
		if (!Dt::Function::initInstance())
		{
			break;
		}

		QFileInfo fileInfo("./App/curl/curl.exe");
		RUN_BREAK(!fileInfo.exists(), "��ʧ���./App/curl/curl.exe");
		
		m_serialPortTool = new SerialPortTool[2];
		RUN_BREAK(!m_serialPortTool, "���ڹ��߷����ڴ�ʧ��");

		connect(&m_serialPortTool[0], &SerialPortTool::readyReadSignal, this, &Tap::screenUartHandler);
		connect(&m_serialPortTool[1], &SerialPortTool::readyReadSignal, this, &Tap::screenUartHandler);
		result = true;
	} while (false);
	return result;
}

bool Dt::Tap::openDevice()
{
	bool result = false;
	do 
	{
		if (!Dt::Function::openDevice())
		{
			break;
		}

		SerialPortConfig leftConfig, rigthConfig;
		leftConfig.port = m_defConfig->hardware.expandCom1;
		leftConfig.baud = m_defConfig->hardware.expandBaud1;
		leftConfig.dataBit = 8;
		leftConfig.flow = 0;
		leftConfig.stopBit = 1;
		leftConfig.verify = 2;

		rigthConfig = leftConfig;
		rigthConfig.port = m_defConfig->hardware.expandCom2;
		rigthConfig.baud = m_defConfig->hardware.expandBaud2;

		if (!m_serialPortTool[0].openSerialPort(leftConfig))
		{
			setLastError(Q_SPRINTF("�򿪴���%dʧ��", leftConfig.port), false, true);
		}

		if (!m_serialPortTool[1].openSerialPort(rigthConfig))
		{
			setLastError(Q_SPRINTF("�򿪴���%dʧ��", rigthConfig.port), false, true);
		}
		result = true;
	} while (false);
	return result;
}

bool Dt::Tap::closeDevice()
{
	bool result = false;
	do 
	{
		if (!Dt::Function::closeDevice())
		{
			break;
		}

		m_serialPortTool[0].closeSerialPort();
		m_serialPortTool[1].closeSerialPort();
		result = true;
	} while (false);
	return result;
}

bool Dt::Tap::checkUSBByJson(const QString& url)
{
	setCurrentStatus("���USB");
	bool result = false;
	do
	{
		QProcess process;
		process.start("./App/curl/curl.exe -i " + url);
		RUN_BREAK(!process.waitForFinished(10000),QString("����%1ʧ��").arg(url));

		QByteArray bytes = process.readAllStandardOutput();
		QString output(bytes.data());
		int start = output.lastIndexOf("{");
		int end = output.lastIndexOf("}");
		QString json = output.mid(start, end);

		QJsonParseError jsonError;
		QJsonDocument serverJson = QJsonDocument::fromJson(json.toLocal8Bit().data(), &jsonError);
		RUN_BREAK(jsonError.error != QJsonParseError::NoError,"������JSON��ʽ����");

		addListItem(QString("������JSON����:\n%1").arg(serverJson.toJson().data()));
		QString fileName = "./Config/TAPUSBJson.json";
		if (!QFileInfo(fileName).exists())
		{
			QFile file(fileName);
			RUN_BREAK(!file.open(QFile::WriteOnly),"д��TAPUSBJsonʧ��");
			file.write(serverJson.toJson());
			file.close();
		}

		QFile file(fileName);
		RUN_BREAK(!file.open(QFile::ReadOnly), "��ȡTAPUSBJsonʧ��");
		bytes = file.readAll();
		file.close();

		QJsonDocument configJson(QJsonDocument::fromJson(bytes, &jsonError));
		RUN_BREAK(jsonError.error != QJsonParseError::NoError, QString("%1��ʽ����").arg(fileName));

		RUN_BREAK(!(serverJson == configJson), "JSON�ļ�������Ϣ�Ա�ʧ��");
		result = true;
	} while (false);
	WRITE_LOG("%s ���USB", OK_NG(result));
	addListItemEx(Q_SPRINTF("���USB %s", OK_NG(result)));
	return result;
}

void Dt::Tap::screenUartHandler(const QString& port, const QByteArray& bytes)
{
	do
	{
		QString hex = bytes.toHex();
		if (hex.length() != 8)
		{
			break;
		}

		QStringList dataList;
		for (int i = 0; i <= 3; i++)
		{
			dataList[i] = hex.mid(i * 2, 2);
		}

		bool valid = true;
		for (int i = 0; i < dataList.length(); i++)
		{
			if (dataList.at(i).isEmpty())
			{
				valid = false;
				break;
			}
		}

		if (!valid)
		{
			break;
		}

		if (!(dataList.at(0) == "80" && dataList.at(1) == "01" && dataList.at(2) == "01"))
		{
			break;
		}

		if (((0 - dataList.at(2).toInt(nullptr, 16)) & 0xff) == dataList.at(3).toInt(nullptr, 16))
		{
			for (int i = 0; i < 2; i++)
			{
			}
		}
	} while (false);
	return;
}

Cc::CaptureCard::CaptureCard(QObject* parent)
{
	m_function = static_cast<Dt::Function*>(parent);
}

Cc::CaptureCard::~CaptureCard()
{
}

const QString& Cc::CaptureCard::getLastError()
{
	return m_lastError;
}

bool Cc::CaptureCard::openDevice(const int& id, const int& count)
{
	bool result = false;

	do 
	{
		if (count == 2)
		{
			result = true;
			break;
		}

		RUN_BREAK(!m_video.open(id), "����Ƶ�ɼ���ʧ��");

		double width = m_video.get(CV_CAP_PROP_FRAME_WIDTH);
		double height = m_video.get(CV_CAP_PROP_FRAME_HEIGHT);

		m_scalew = (double)VIDEO_WIDGET_WIDTH / width;
		m_scaleh = (double)VIDEO_WIDGET_HEIGHT / height;
		connect(&m_timer, &QTimer::timeout, this, &CaptureCard::getImageSlot);
		m_open = true;
		result = true;
	} while (false);
	return result;
}

bool Cc::CaptureCard::closeDevice()
{
	if (m_open)
	{
		stopCapture();
		disconnect(&m_timer, &QTimer::timeout, this, &CaptureCard::getImageSlot);
		m_video.release();
		m_open = false;
	}
	return true;
}

bool Cc::CaptureCard::isOpen()
{
	return m_open;
}

bool Cc::CaptureCard::startCapture()
{
	if (m_open)
		m_timer.start(m_fps);
	return true;
}

bool Cc::CaptureCard::stopCapture()
{
	if (m_timer.isActive())
		m_timer.stop();
	return true;
}

void Cc::CaptureCard::setFPS(const int fps)
{
	m_fps = fps;
}

void Cc::CaptureCard::getImageSlot()
{
	if (!m_video.isOpened())
	{
		DEBUG_INFO() << "δ����Ƶ�ɼ����豸";
		return;
	}

	m_video >> m_mat;

	if (m_mat.empty())
		return;

	Size dSzie = Size(m_mat.cols * m_scalew, m_mat.rows * m_scaleh);
	cv::resize(m_mat, m_mat, dSzie);

	if (m_function->m_capture)
	{
		memcpy(m_function->m_cvAnalyze->imageData, m_mat.data, m_mat.total() * 3);
		m_function->m_capture = false;
	}
	
	m_function->drawRectOnImage(m_mat);
	QImage qimg;
	Misc::cvImageToQtImage(&m_mat, &qimg);
	m_function->updateImage(qimg);
}

void Cc::CaptureCard::setLastError(const QString& error)
{
	DEBUG_INFO() << error;
	m_lastError = error;
}
