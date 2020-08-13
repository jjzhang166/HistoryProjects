#include "JsonTool.h"

JsonTool* JsonTool::m_self = nullptr;

void JsonTool::setLastError(const QString& err)
{
#ifdef QT_DEBUG
	qDebug() << err << endl;
#endif
	m_lastError = err;
}

bool JsonTool::parseDeviceConfigData()
{
	bool result = false, success = true, convert = false;
	do
	{
		QString* valuePtr = reinterpret_cast<QString*>(&m_defConfig.device);
		for (size_t i = 0; i < m_deviceConfigKeyList.length(); i++, valuePtr++)
		{
			*valuePtr = getDeviceConfigValue(m_deviceConfigKeyList.value(i));
			if (valuePtr->isEmpty())
			{
				success = false;
				setLastError(QString("�豸����[%1]��ʽ����").arg(m_deviceConfigKeyList.value(i)));
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

bool JsonTool::parseHardwareConfigData()
{
	bool result = false, success = true, convert = false;
	do
	{
		int* valuePtr = reinterpret_cast<int*>(&m_defConfig.hardware);
		for (size_t i = 0; i < m_hardwareConfigKeyList.length(); i++, valuePtr++)
		{
			if (i == 2)
			{
				m_defConfig.hardware.powerVoltage = getHardwareConfigValue(m_hardwareConfigKeyList.value(i)).toFloat(&convert);
			}
			else
			{
				*valuePtr = getHardwareConfigValue(m_hardwareConfigKeyList.value(i)).toInt(&convert);
			}

			if (!convert)
			{
				success = false;
				setLastError(QString("Ӳ������[%1]��ʽ����").arg(m_hardwareConfigKeyList.value(i)));
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

bool JsonTool::parseRelayPortConfigData()
{
	bool result = false, success = true, convert = false;
	do
	{
		int* valuePtr = reinterpret_cast<int*>(&m_defConfig.relay);
		for (size_t i = 0; i < m_relayConfigKeyList.length(); i++, valuePtr++)
		{
			*valuePtr = getRelayConfigValue(m_relayConfigKeyList.value(i)).toInt(&convert);
			if (!convert)
			{
				success = false;
				setLastError(QString("�̵�������[%1]��ʽ����").arg(m_relayConfigKeyList.value(i)));
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

bool JsonTool::parseUserConfigData()
{
	return true;
}

bool JsonTool::parseImageConfigData()
{
	bool result = false, convert = true;
	do
	{
		int* valuePtr = nullptr;

		for (int i = 0; i < IMAGE_CHECK_COUNT; i++)
		{
			if (i == IMAGE_CHECK_COUNT - 1)
			{
				valuePtr = reinterpret_cast<int*>(&m_defConfig.image);
			}
			else if (i >= SMALL_RECT_ && i <= SMALL_RECT_ + BIG_RECT_ - 1)
			{
				valuePtr = reinterpret_cast<int*>(&(m_defConfig.image.bigRect[i - SMALL_RECT_]));
			}
			else
			{
				valuePtr = reinterpret_cast<int*>(&(m_defConfig.image.smallRect[i]));
			}

			for (int j = 0; j < m_childImageKeyList[i].count(); j++, (j || i == IMAGE_CHECK_COUNT - 1) ? valuePtr++ : valuePtr += sizeof(QString))
			{
				if (j || i == IMAGE_CHECK_COUNT - 1)
				{
					*valuePtr = getImageConfigValue(m_parentImageKeyList[i], m_childImageKeyList[i][j]).toInt(&convert);
				}
				else
				{
					const QString& color = m_defConfig.image.smallRect[i].color = getImageConfigValue(m_parentImageKeyList[i], m_childImageKeyList[i][j]);
					if ((!color.contains("!=") && !color.contains("==")) || color.length() != 4)
					{
						convert = false;
						setLastError(QString("%1[%2],�﷨����,\n�ж��﷨,!=��ɫ(�����ں�ɫ),==��ɫ(���ں�ɫ)").arg(m_parentImageKeyList[i], m_childImageKeyList[i][j]));
						break;
					}
				}

				if (!convert)
				{
					setLastError(QString("%1[%2],��ʽ����").arg(m_parentImageKeyList[i], m_childImageKeyList[i][j]));
					break;
				}
			}

			if (!convert)
			{
				break;
			}
		}

		if (!convert)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseRangeConfigData()
{
	bool result = false, success = true;
	do
	{
		float* valuePtr = reinterpret_cast<float*>(&m_defConfig.range);
		for (size_t i = 0; i < m_rangeConfigKeyList.length(); i++, valuePtr++, valuePtr++)
		{
			if (!parseRangeValue(getRangeConfigValue(m_rangeConfigKeyList.value(i)), *valuePtr, *(valuePtr + 1)))
			{
				setLastError(QString("��Χ����[%1]��ʽ����").arg(m_rangeConfigKeyList.value(i)));
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

bool JsonTool::parseThresholdConfigData()
{
	bool result = false, success = true, convert = false;
	do
	{
		float* valuePtr = reinterpret_cast<float*>(&m_defConfig.threshold);
		for (size_t i = 0; i < m_thresholdKeyList.length(); i++, valuePtr++)
		{
			*valuePtr = getThresholdConfigValue(m_thresholdKeyList.value(i)).toFloat(&convert);
			if (!convert)
			{
				setLastError(QString("��ֵ����[%1]��ʽ����").arg(m_thresholdKeyList.value(i)));
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

bool JsonTool::parseEnableConfigData()
{
	bool result = false, convert = false, success = true;
	do 
	{
		int* valuePtr = reinterpret_cast<int*>(&m_defConfig.enable);
		for (int i = 0; i < m_enableConfigKeyList.length(); i++, valuePtr++)
		{
			*valuePtr = getEnableConfigValue(m_enableConfigKeyList[i]).toInt(&convert);
			if (!convert)
			{
				setLastError(QString("��������[%1]��ʽ����").arg(m_enableConfigKeyList[i]));
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

bool JsonTool::parseVoltageConfigData()
{
	bool result = false, convert = false, success = true;
	do
	{
		SAFE_DELETE_A(m_hwdConfig.voltage);

		m_hwdConfig.voltage = NO_THROW_NEW VoltageConfig[getVoltageConfigCount()];
		VoltageConfig* voltage = m_hwdConfig.voltage;
		if (!voltage)
		{
			setLastError("��ѹ���÷����ڴ�ʧ��");
			break;
		}

		memset(voltage, 0x00, sizeof(VoltageConfig));

		for (size_t i = 0; i < getVoltageConfigCount(); i++)
		{
			strcpy(voltage[i].name, Q_TO_C_STR(getParentVoltageConfigKeyList()[i]));
			voltage[i].high = getVoltageConfigValue(getParentVoltageConfigKeyList()[i], "����").toFloat(&convert);
			if (!convert)
			{
				success = false;
				setLastError("��ѹ����[����]��ʽ����");
				break;
			}
			voltage[i].low = getVoltageConfigValue(getParentVoltageConfigKeyList()[i], "����").toFloat(&convert);
			if (!convert)
			{
				success = false;
				setLastError("��ѹ����[����]��ʽ����");
				break;
			}
			voltage[i].relay = getVoltageConfigValue(getParentVoltageConfigKeyList()[i], "�̵���IO").toInt(&convert);
			if (!convert)
			{
				success = false;
				setLastError("��ѹ����[�̵���IO]��ʽ����");
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

bool JsonTool::parseKeyVolConfigData()
{
	bool result = false, convert = false, success = true;
	do
	{
		m_hwdConfig.keyVol.hULimit = getKeyVolConfigValue("�ߵ�ƽ����").toFloat(&convert);
		if (!convert)
		{
			setLastError("������ѹ����[�ߵ�ƽ����]��ʽ����");
			break;
		}

		m_hwdConfig.keyVol.hLLimit = getKeyVolConfigValue("�ߵ�ƽ����").toFloat(&convert);
		if (!convert)
		{
			setLastError("������ѹ����[�ߵ�ƽ����]��ʽ����");
			break;
		}

		m_hwdConfig.keyVol.lULimit = getKeyVolConfigValue("�͵�ƽ����").toFloat(&convert);
		if (!convert)
		{
			setLastError("������ѹ����[�͵�ƽ����]��ʽ����");
			break;
		}

		m_hwdConfig.keyVol.lLLimit = getKeyVolConfigValue("�͵�ƽ����").toFloat(&convert);
		if (!convert)
		{
			setLastError("������ѹ����[�͵�ƽ����]��ʽ����");
			break;
		}

		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseCurrentConfigData()
{
	bool result = false, convert = false, success = true;
	do
	{
		SAFE_DELETE_A(m_hwdConfig.current);

		m_hwdConfig.current = NO_THROW_NEW CurrentConfig[getCurrentConfigCount()];
		CurrentConfig* current = m_hwdConfig.current;
		if (!current)
		{
			setLastError("�������÷����ڴ�ʧ��");
			break;
		}
		memset(current, 0x00, sizeof(CurrentConfig));
		for (size_t i = 0; i < getCurrentConfigCount(); i++)
		{
			strcpy(current[i].name, Q_TO_C_STR(getParentCurrentConfigKeyList()[i]));
			current[i].high = getCurrentConfigValue(getParentCurrentConfigKeyList()[i], "����").toFloat(&convert);
			if (!convert)
			{
				setLastError("��������[����]��ʽ����");
				break;
			}

			current[i].low = getCurrentConfigValue(getParentCurrentConfigKeyList()[i], "����").toFloat(&convert);
			if (!convert)
			{
				setLastError("��������[����]��ʽ����");
				break;
			}

			current[i].voltage = getCurrentConfigValue(getParentCurrentConfigKeyList()[i], "��ѹ").toFloat(&convert);
			if (!convert)
			{
				setLastError("��������[��ѹ]��ʽ����");
				break;
			}
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseStaticConfigData()
{
	bool result = false, convert = false;
	do
	{
		m_hwdConfig.staticCurrent.high = getStaticConfigValue("����").toFloat(&convert);
		if (!convert)
		{
			setLastError("��̬��������[����]��ʽ����");
			break;
		}

		m_hwdConfig.staticCurrent.low = getStaticConfigValue("����").toFloat(&convert);
		if (!convert)
		{
			setLastError("��̬��������[����]��ʽ����");
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseResConfigData()
{
	bool result = false, convert = false, success = true;
	do
	{
		SAFE_DELETE_A(m_hwdConfig.res);

		m_hwdConfig.res = NO_THROW_NEW ResConfig[getResConfigCount()];
		ResConfig* res = m_hwdConfig.res;
		if (!res)
		{
			setLastError("�������÷����ڴ�ʧ��");
			break;
		}
		memset(res, 0x00, sizeof(ResConfig));
		for (size_t i = 0; i < getResConfigCount(); i++)
		{
			strcpy(m_hwdConfig.res[i].name, Q_TO_C_STR(getParentResConfigKeyList()[i]));
			m_hwdConfig.res[i].high = getResConfigValue(getParentResConfigKeyList()[i], "����").toFloat(&convert);
			if (!convert)
			{
				success = false;
				break;
			}

			m_hwdConfig.res[i].low = getResConfigValue(getParentResConfigKeyList()[i], "����").toFloat(&convert);
			if (!convert)
			{
				success = false;
				break;
			}

			m_hwdConfig.res[i].relay = getResConfigValue(getParentResConfigKeyList()[i], "�̵���IO").toInt(&convert);
			if (!convert)
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

bool JsonTool::parseVerConfigData()
{
	bool result = false, convert = false, success = true;
	do
	{
		const char* code[] = { "ASCII", "ASCR4", "INT", "USN", "BIN", "BCD" };

		SAFE_DELETE_A(m_udsConfig.ver);

		m_udsConfig.ver = NO_THROW_NEW VersonConfig[getVerConfigCount()];
		VersonConfig* ver = m_udsConfig.ver;
		if (!ver)
		{
			setLastError("�汾���÷����ڴ�ʧ��");
			break;
		}

		memset(ver, 0x00, sizeof(VersonConfig));
		for (size_t i = 0; i < getVerConfigCount(); i++)
		{
			strcpy(ver[i].name, Q_TO_C_STR(getParentVerConfigKeyList()[i]));

			ushort did = getVerConfigValue(getParentVerConfigKeyList()[i], "DID").toUShort(&convert, 16);
			if (!convert)
			{
				success = false;
				setLastError("�汾����,DID��ʽ����");
				break;
			}

			ver[i].did[0] = did >> 8;
			ver[i].did[1] = did >> 0;

			strcpy(ver[i].setup, Q_TO_C_STR(getVerConfigValue(getParentVerConfigKeyList()[i], "ֵ")));
			strcpy(ver[i].encode, Q_TO_C_STR(getVerConfigValue(getParentVerConfigKeyList()[i], "����").toUpper()));
			for (size_t j = 0; j < sizeof(code) / sizeof(char*); j++)
			{
				if (!strcmp(ver[i].encode, code[j]))
				{
					success = true;
					break;
				}
				success = false;
			}

			if (!success)
			{
				setLastError(QString("�汾����[%1]�����ʽ����").arg(getParentVerConfigKeyList()[i]));
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

bool JsonTool::parseDtcConfigData()
{
	bool result = false, convert = false, success = true;
	do
	{
		SAFE_DELETE_A(m_udsConfig.dtc);

		m_udsConfig.dtc = NO_THROW_NEW DtcConfig[getDtcConfigCount()];
		DtcConfig* dtc = m_udsConfig.dtc;
		if (!dtc)
		{
			setLastError("��Ϲ��������÷����ڴ�ʧ��");
			break;
		}
		memset(dtc, 0x00, sizeof(DtcConfig));
		for (size_t i = 0; i < getDtcConfigCount(); i++)
		{
			strcpy(m_udsConfig.dtc[i].name, Q_TO_C_STR(getParentDtcConfigKeyList()[i]));
			QString dtc = dtcCategoryConvert(getDtcConfigValue(getParentDtcConfigKeyList()[i], "DTC"));
			if (dtc.isEmpty())
			{
				break;
			}

			size_t bytes = dtc.toUInt(&convert, 16);
			if (!convert)
			{
				setLastError("�������[DTC]��ʽ����");
				break;
			}

			m_udsConfig.dtc[i].dtc[0] = bytes >> 16;
			m_udsConfig.dtc[i].dtc[1] = bytes >> 8;
			m_udsConfig.dtc[i].dtc[2] = bytes >> 0;

			m_udsConfig.dtc[i].ignore = getDtcConfigValue(getParentDtcConfigKeyList()[i], "����").toInt(&convert);
			if (!convert)
			{
				setLastError("�������[����]��ʽ����");
				break;
			}
		}
		result = true;
	} while (false);
	return result;
}

const QString JsonTool::dtcCategoryConvert(const QString& dtc)
{
	QString result = "";
	bool convert = false;
	do
	{
		int category = -1;
		switch (dtc[0].toLatin1())
		{
		case 'B':category = 0x8; break;
		case 'C':category = 0x4; break;
		case 'P':category = 0x0; break;
		case 'U':category = 0xC; break;
		default:break;
		}

		if (category == -1)
		{
			setLastError("��Ϲ�����,�������಻���Ϲ���");
			break;
		}

		category += dtc.mid(1, 1).toInt(&convert);
		if (!convert)
		{
			setLastError("��Ϲ����볣��ת��ʧ��");
			break;
		}
		result = dtc;
		result.replace(dtc.mid(0, 2), QString::number(category, 16).toUpper());
	} while (false);
	return result;
}

bool JsonTool::parseCanMsgData()
{
	return true;
	bool result = false, success = true, convert = false;
	do
	{
		for (int i = 0; i < getCanMsgCount(); i++)
		{
			QString parentKey = getCanMsgKeyList()[i];
			m_canMsg[i].msg.id = getCanMsgValue(parentKey, "ID").toInt(&convert, 16);
			if (!convert)
			{
				setLastError(parentKey + "[ID]��ʽ����");
				success = false;
				break;
			}

			m_canMsg[i].msg.iDLC = getCanMsgValue(parentKey, "����").toInt(&convert);
			if (!convert)
			{
				setLastError(parentKey + "[����]��ʽ����");
				success = false;
				break;
			}

			QString canType = getCanMsgValue(parentKey, "����");
			if (canType == "����")
			{
				m_canMsg[i].emST = ST_Period;
			}
			else if (canType == "�¼�")
			{
				m_canMsg[i].emST = ST_Event;
			}
			else
			{
				setLastError(parentKey + "[����]��ʽ����,��֧�����ں��¼�");
				break;
			}

			m_canMsg[i].iCycle = getCanMsgValue(parentKey, "ʱ��").toInt(&convert);
			if (!convert)
			{
				setLastError(parentKey + "[ʱ��]��ʽ����");
				success = false;
				break;
			}

			m_canMsg[i].iSendCount = getCanMsgValue(parentKey, "����").toInt(&convert);
			if (!convert)
			{
				setLastError(parentKey + "[����]��ʽ����");
				success = false;
				break;
			}

			QStringList dataList = getCanMsgValue(getCanMsgKeyList()[i], "����").split(" ");
			for (int j = 0; j < dataList.size(); j++)
			{
				m_canMsg[i].msg.ucData[j] = dataList[j].toInt(&convert, 16);
				if (!convert)
				{
					setLastError(parentKey + "[����]��ʽ����");
					success = false;
					break;
				}
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

JsonTool::JsonTool(QObject* parent)
	: QObject(parent)
{
}

JsonTool::~JsonTool()
{
	SAFE_DELETE_A(m_hwdConfig.current);

	SAFE_DELETE_A(m_hwdConfig.res);

	SAFE_DELETE_A(m_hwdConfig.voltage);

	SAFE_DELETE_A(m_udsConfig.ver);

	SAFE_DELETE_A(m_udsConfig.dtc);
}

bool JsonTool::parseRangeValue(const QString& value, float& min, float& max)
{
	bool result = false;
	do
	{
		QStringList split = value.split("~", QString::SkipEmptyParts);
		if (split.size() != 2)
		{
			break;
		}

		bool convert = false;
		min = split[0].toFloat(&convert);
		if (!convert)
		{
			break;
		}
		max = split[1].toFloat(&convert);
		if (!convert)
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

JsonTool* JsonTool::getInstance()
{
	if (!m_self)
	{
		m_self = NO_THROW_NEW JsonTool;
	}
	return m_self;
}

void JsonTool::deleteInstance()
{
	SAFE_DELETE(m_self);
}

const QString& JsonTool::getLastError()
{
	return m_lastError;
}

bool JsonTool::initInstance(bool update, const QString& folderName, const QStringList& fileName)
{
	bool result = false, success = true;
	do
	{
		QString jsonPath = QString("%1/JsonFile_%2").arg(folderName, JSON_FILE_VER);
		QString dcfPath = QString("%1/DcfFile_%2").arg(folderName, DCF_FILE_VER);

		QStringList pathList = { jsonPath,dcfPath };
		for (int i = 0; i < pathList.count(); i++)
		{
			if (!QDir(pathList[i]).exists())
			{
				QDir dir;
				if (!dir.mkpath(pathList[i]))
				{
					success = false;
					setLastError(QString("%1 �����ļ���%2ʧ��").arg(__FUNCTION__, pathList[i]));
					break;
				}
			}
		}

		if (!success)
		{
			break;
		}

		bool (JsonTool:: * readFnc[])(const QString&) = { &JsonTool::readDefJsonFile,&JsonTool::readHwdJsonFile,
			&JsonTool::readUdsJsonFile,&JsonTool::readCanJsonFile };

		bool (JsonTool:: * writeFnc[])(const QString&) = { &JsonTool::writeDefJsonFile,&JsonTool::writeHwdJsonFile,
			&JsonTool::writeUdsJsonFile,&JsonTool::writeCanJsonFile };

		bool (JsonTool:: * updateFnc[])(const QString&) = { &JsonTool::updateDefJsonFile,&JsonTool::updateHwdJsonFile,
			&JsonTool::updateUdsJsonFile,&JsonTool::updateCanJsonFile };

		if (fileName.size() != sizeof(readFnc) / sizeof(*readFnc)
			|| fileName.size() != sizeof(writeFnc) / sizeof(*writeFnc)
			|| fileName.size() != sizeof(updateFnc) / sizeof(*updateFnc))
		{
			setLastError("�ļ����б��뺯��ָ�����鲻��Ӧ");
			break;
		}

		for (int i = 0; i < fileName.size(); i++)
		{
			const QString file = QString("./%1/%2").arg(jsonPath, fileName.value(i));
			if (!QFileInfo(file).exists() || update)
			{
				if (update ? !(this->*updateFnc[i])(file) : !(this->*writeFnc[i])(file))
				{
					success = false;
					break;
				}
			}

			if (!(this->*readFnc[i])(file))
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

const QStringList JsonTool::getAllMainKey()
{
	static QStringList keys = { "�豸����","Ӳ������","�̵�������","��Χ����","��ֵ����","ͼ������","������ѹ����","��ѹ����",
	"��������","��������","��̬��������","�汾����","��Ϲ���������","��������" };
	return keys;
}

const QString JsonTool::getLibrayVersion()
{
	return LIBRARY_VER;
}

const QString JsonTool::getJsonFileVersion()
{
	return JSON_FILE_VER;
}

const QString JsonTool::getDCFFileVersion()
{
	return DCF_FILE_VER;
}

bool JsonTool::readDefJsonFile(const QString& name)
{
	bool result = false, success = true;
	do
	{
		QFile file(name);
		if (!file.open(QFile::ReadOnly))
		{
			m_lastError = file.errorString();
			break;
		}

		QByteArray bytes = file.readAll();
		file.close();
		QJsonParseError jsonError;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &jsonError));
		if (jsonError.error != QJsonParseError::NoError)
		{
			m_lastError = jsonError.errorString();
			break;
		}

		QJsonObject rootObj = jsonDoc.object();

		QStringList keyList = { "�豸����","Ӳ������","�̵�������","�û�����","ͼ������","��Χ����","��ֵ����","��������" };
		
		QList<QJsonObject*>objList = { &m_deviceConfigObj,&m_hardwareConfigObj,&m_relayConfigObj,
		&m_userConfigObj,&m_imageConfigObj,&m_rangeConfigObj,&m_thresholdConfigObj,&m_enableConfigObj };

		bool (JsonTool:: * parseFnc[])() = { 
			&JsonTool::parseDeviceConfigData,
			&JsonTool::parseHardwareConfigData,
			&JsonTool::parseRelayPortConfigData,
			&JsonTool::parseUserConfigData,
			&JsonTool::parseImageConfigData,
			&JsonTool::parseRangeConfigData,
			&JsonTool::parseThresholdConfigData,
			&JsonTool::parseEnableConfigData
		};

		for (int i = 0; i < keyList.count(); i++)
		{
			if (rootObj.contains(keyList[i]))
			{
				*objList[i] = rootObj.value(keyList[i]).toObject();
				if (!(this->*parseFnc[i])())
				{
					success = false;
					break;
				}
			}
			else
			{
				setLastError(QString("%1�����ļ�δ�ҵ�������%2").arg(name, keyList[i]));
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

bool JsonTool::writeDefJsonFile(const QString& name)
{
	QJsonObject rootObj, deviceConfigObj, hardwareConfigObj,
		relayIoConfigObj, userConfigObj, rangeConfigObj,
		thresholdConfigObj, enableConfigObj;

	/*д�豸����*/
	for (int i = 0; i < m_deviceConfigKeyList.length(); i++)
	{
		deviceConfigObj.insert(m_deviceConfigKeyList[i], m_deviceConfigValueList[i]);
	}

	/*дӲ������*/
	for (int i = 0; i < m_hardwareConfigKeyList.length(); i++)
	{
		hardwareConfigObj.insert(m_hardwareConfigKeyList[i], m_hardwareConfigValueList[i]);
	}

	/*д�̵���IO�˿�����*/
	for (int i = 0; i < m_relayConfigKeyList.length(); i++)
	{
		relayIoConfigObj.insert(m_relayConfigKeyList[i], m_relayConfigValueList[i]);
	}

	/*д�û�����*/
	for (int i = 0; i < m_userConfigKeyList.length(); i++)
	{
		userConfigObj.insert(m_userConfigKeyList[i], m_userConfigValueList[i]);
	}

	/*��Χ����*/
	for (int i = 0; i < m_rangeConfigKeyList.length(); i++)
	{
		rangeConfigObj.insert(m_rangeConfigKeyList[i], m_rangeConfigValueList[i]);
	}

	/*ͼ������*/
	QJsonObject childImageCheckObj[IMAGE_CHECK_COUNT];

	for (int i = 0; i < IMAGE_CHECK_COUNT; i++)
	{
		for (int j = 0; j < m_childImageKeyList[i].size(); j++)
		{
			childImageCheckObj[i].insert(m_childImageKeyList[i].value(j), m_childImageValueList[i].value(j));
		}
	}

	QJsonObject imageConfigObj;
	for (int i = 0; i < m_parentImageKeyList.size(); i++)
	{
		imageConfigObj.insert(m_parentImageKeyList.value(i), childImageCheckObj[i]);
	}

	/*��ֵ����*/
	for (int i = 0; i < m_thresholdKeyList.length(); i++)
	{
		thresholdConfigObj.insert(m_thresholdKeyList[i], m_thresholdValueList[i]);
	}

	/*��������*/
	for (int i = 0; i < m_enableConfigKeyList.length(); i++)
	{
		enableConfigObj.insert(m_enableConfigKeyList[i], m_enableConfigValueList[i]);
	}

	rootObj.insert("�豸����", deviceConfigObj);
	rootObj.insert("Ӳ������", hardwareConfigObj);
	rootObj.insert("��Χ����", rangeConfigObj);
	rootObj.insert("�û�����", userConfigObj);
	rootObj.insert("ͼ������", imageConfigObj);
	rootObj.insert("�̵�������", relayIoConfigObj);
	rootObj.insert("��ֵ����", thresholdConfigObj);
	rootObj.insert("��������", enableConfigObj);

	bool result = false;
	do
	{
		QJsonDocument doc(rootObj);
		QByteArray bytes = doc.toJson();
		QFile file(name);
		if (!file.open(QIODevice::WriteOnly))
		{
			m_lastError = file.errorString();
			break;
		}

		if (bytes.length() != file.write(bytes))
		{
			m_lastError = QString("%1д��%2����У��ʧ��").arg(__FUNCTION__, name);
			file.close();
			break;
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::updateDefJsonFile(const QString& name)
{
	QJsonObject rootObj;
	rootObj.insert("�豸����", m_deviceConfigObj);
	rootObj.insert("Ӳ������", m_hardwareConfigObj);
	rootObj.insert("��Χ����", m_rangeConfigObj);
	rootObj.insert("�û�����", m_userConfigObj);
	rootObj.insert("ͼ������", m_imageConfigObj);
	rootObj.insert("�̵�������", m_relayConfigObj);
	rootObj.insert("��ֵ����", m_thresholdConfigObj);
	rootObj.insert("��������", m_enableConfigObj);

	bool result = false;
	do
	{
		QJsonDocument doc(rootObj);
		QByteArray bytes = doc.toJson();
		QFile file(name);
		if (!file.open(QIODevice::WriteOnly))
		{
			m_lastError = file.errorString();
			break;
		}

		if (bytes.length() != file.write(bytes))
		{
			m_lastError = QString("%1д��%2����У��ʧ��").arg(__FUNCTION__, name);
			file.close();
			break;
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::readHwdJsonFile(const QString& name)
{
	bool result = false, success = true;
	do
	{
		QFile file(name);
		if (!file.open(QFile::ReadOnly))
		{
			setLastError(file.errorString());
			break;
		}
		QByteArray bytes = file.readAll();
		file.close();
		QJsonParseError jsonError;
		QJsonDocument doc(QJsonDocument::fromJson(bytes, &jsonError));
		if (jsonError.error != QJsonParseError::NoError)
		{
			setLastError(jsonError.errorString());
			break;
		}
		QJsonObject root = doc.object();

		QStringList keyList = { "��ѹ����","������ѹ����","��������","��̬��������","��������" };
		
		QList<QJsonObject*> objList = { &m_voltageConfigObj,&m_keyVolConfigObj,&m_currentConfigObj
			,&m_staticConfigObj ,&m_resConfigObj };

		bool (JsonTool:: * parseFnc[])() = { 
			&JsonTool::parseVoltageConfigData,
			&JsonTool::parseKeyVolConfigData,
			&JsonTool::parseCurrentConfigData,
			&JsonTool::parseStaticConfigData,
			&JsonTool::parseResConfigData };

		for (int i = 0; i < keyList.count(); i++)
		{
			if (root.contains(keyList[i]))
			{
				*objList[i] = root.value(keyList[i]).toObject();
				if (!(this->*parseFnc[i])())
				{
					success = false;
					break;
				}
			}
			else
			{
				setLastError(QString("%1�����ļ�δ�ҵ�������%2").arg(name, keyList[i]));
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

bool JsonTool::writeHwdJsonFile(const QString& name)
{
	bool result = false;
	do
	{
		QJsonObject rootObj, voltageObj, keyVolObj, currentObj, staticObj, resObj;

		/*��ѹĬ������*/
		QList<QJsonObject> voltageList;
		for (int i = 0; i < 2; i++)
		{
			QJsonObject obj;
			for (int j = 0; j < getChildVoltageConfigKeyList().count(); j++)
			{
				obj.insert(getChildVoltageConfigKeyList()[j], getChildVoltageConfigValueList()[j]);
			}
			voltageList.append(obj);
		}

		voltageObj.insert("1.8V��ѹ", voltageList[0]);
		voltageObj.insert("3.3V��ѹ", voltageList[1]);

		/*����Ĭ������*/
		for (int i = 0; i < getKeyVolConfigKeyList().size(); i++)
		{
			keyVolObj.insert(getKeyVolConfigKeyList()[i], getKeyVolConfigValueList()[i]);
		}

		/*����Ĭ������*/
		QList<QJsonObject> currentList;
		for (int i = 0; i < 2; i++)
		{
			QJsonObject obj;
			for (int j = 0; j < getChildCurrentConfigKeyList().count(); j++)
			{
				obj.insert(getChildCurrentConfigKeyList()[j], getChildCurrentConfigValueList(i)[j]);
			}
			currentList.append(obj);
		}
		currentObj.insert("12V��������", currentList[0]);
		currentObj.insert("16V��������", currentList[1]);

		/*��̬����Ĭ������*/
		for (int i = 0; i < getStaticConfigKeyList().count(); i++)
		{
			staticObj.insert(getStaticConfigKeyList()[i], getStaticConfigValueList()[i]);
		}

		/*RES��������*/
		QList<QJsonObject> resList;
		for (int i = 0; i < 2; i++)
		{
			QJsonObject obj;
			for (int j = 0; j < getChildResConfigKeyList().count(); j++)
			{
				obj.insert(getChildResConfigKeyList()[j], getChildResConfigValueList()[j]);
			}
			resList.append(obj);
		}
		resObj.insert("12V�Եص���", resList[0]);
		resObj.insert("16V�Եص���", resList[1]);

		/*���ڵ�Ĭ������*/
		rootObj.insert("��ѹ����", voltageObj);
		rootObj.insert("������ѹ����", keyVolObj);
		rootObj.insert("��������", currentObj);
		rootObj.insert("��̬��������", staticObj);
		rootObj.insert("��������", resObj);

		QFile file(name);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}
		file.write(QJsonDocument(rootObj).toJson());
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::updateHwdJsonFile(const QString& name)
{
	bool result = false;
	do
	{
		QJsonObject rootObj;
		rootObj.insert("��ѹ����", m_voltageConfigObj);
		rootObj.insert("������ѹ����", m_keyVolConfigObj);
		rootObj.insert("��������", m_currentConfigObj);
		rootObj.insert("��̬��������", m_staticConfigObj);
		rootObj.insert("��������", m_resConfigObj);

		QFile file(name);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}
		file.write(QJsonDocument(rootObj).toJson());
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::readUdsJsonFile(const QString& name)
{
	bool result = false;
	do
	{
		QFile file(name);
		if (!file.open(QFile::ReadOnly))
		{
			setLastError(file.errorString());
			break;
		}
		QByteArray bytes = file.readAll();
		file.close();
		QJsonParseError jsonError;
		QJsonDocument doc(QJsonDocument::fromJson(bytes, &jsonError));
		if (jsonError.error != QJsonParseError::NoError)
		{
			setLastError(jsonError.errorString());
			break;
		}
		QJsonObject root = doc.object();

		if (root.contains("�汾����"))
		{
			m_verConfigObj = root.value("�汾����").toObject();
			parseVerConfigData();
		}

		if (root.contains("�������"))
		{
			m_dtcConfigObj = root.value("�������").toObject();
			parseDtcConfigData();
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::writeUdsJsonFile(const QString& name)
{
	bool result = false;
	do
	{
		QJsonObject rootObj, verObj0, verObj1, verObj2,
			dtcObj0, dtcObj1, dtcObj2;
		verObj0.insert("DID", "0xF187");
		verObj0.insert("����", "ASCII");
		verObj0.insert("ֵ", "A00087710");

		verObj1.insert("DID", "0xF193");
		verObj1.insert("����", "ASCII");
		verObj1.insert("ֵ", "0.03");

		verObj2.insert("����ECUӲ���汾��", verObj0);
		verObj2.insert("Ӧ�ó���汾��", verObj1);

		dtcObj0.insert("DTC", "U100900");
		dtcObj0.insert("����", "0");

		dtcObj1.insert("DTC", "U100587");
		dtcObj1.insert("����", "0");

		dtcObj2.insert("���ع����ѹ���ڹ�����ֵ", dtcObj0);
		dtcObj2.insert("���ع����ѹ���ڹ�����ֵ", dtcObj1);

		rootObj.insert("�汾����", verObj2);
		rootObj.insert("�������", dtcObj2);

		QFile file(name);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}
		file.write(QJsonDocument(rootObj).toJson());
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::updateUdsJsonFile(const QString& name)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj;
		rootObj.insert("�汾����", m_verConfigObj);
		rootObj.insert("�������", m_dtcConfigObj);

		QFile file(name);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}
		file.write(QJsonDocument(rootObj).toJson());
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::readCanJsonFile(const QString& name)
{
	bool result = false;
	do
	{
		QFile file(name);
		if (!file.open(QFile::ReadOnly))
		{
			setLastError(file.errorString());
			break;
		}
		QByteArray bytes = file.readAll();
		file.close();
		QJsonParseError jsonError;
		QJsonDocument doc(QJsonDocument::fromJson(bytes, &jsonError));
		if (jsonError.error != QJsonParseError::NoError)
		{
			setLastError(jsonError.errorString());
			break;
		}
		m_canMsgObj = doc.object();
		parseCanMsgData();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::writeCanJsonFile(const QString& name)
{
	bool result = false;
	do
	{
		QJsonObject rootObj, canObj0, canObj1;

		canObj0.insert("ID", "0x211");
		canObj0.insert("����", "8");
		canObj0.insert("����", "����");
		canObj0.insert("ʱ��", "100");
		canObj0.insert("����", "0");
		canObj0.insert("����", "0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07");

		canObj1.insert("ID", "0x985");
		canObj1.insert("����", "8");
		canObj1.insert("����", "�¼�");
		canObj1.insert("ʱ��", "50");
		canObj1.insert("����", "3");
		canObj1.insert("����", "0x07 0x01 0x02 0x03 0x04 0x05 0x06 0x07");

		QJsonObject keyObj0, keyObj1;

		keyObj0.insert("���߱���", canObj0);
		keyObj1.insert("���ѱ���", canObj1);

		rootObj.insert("���ڱ���", keyObj0);
		rootObj.insert("�¼�����", keyObj1);
		QFile file(name);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}
		file.write(QJsonDocument(rootObj).toJson());
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::updateCanJsonFile(const QString& name)
{
	return true;
}

const int JsonTool::getDeviceConfigCount()
{
	return m_deviceConfigObj.count();
}

const QString JsonTool::getDeviceConfigValue(const QString& key)
{
	return m_deviceConfigObj[key].toString();
}

const QStringList& JsonTool::getDeviceConfigKeyList()
{
	return m_deviceConfigKeyList;
}

const deviceConfig_t& JsonTool::getParsedDeviceConfig()
{
	return m_defConfig.device;
}

const QJsonObject& JsonTool::getDeviceConfigObj()
{
	return m_deviceConfigObj;
}

bool JsonTool::setDeviceConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		if (key == "���볤��" || key == "�ɼ���ͨ����" || key == "�ɼ���ͨ����")
		{
			int number = value.toInt(&convert);
			if (!convert)
			{
				setLastError(key + "����Ϊ����");
				break;
			}

			if (key == "�ɼ���ͨ����")
			{
				if (number <= 0 || number > 2)
				{
					setLastError(getDeviceConfigValue("�ɼ�������") + key + ",���֧��2��ͨ��ͬʱ����");
					break;
				}
			}

			if (key == "�ɼ���ͨ����")
			{
				if (number < 0 || number > 1)
				{
					setLastError(getDeviceConfigValue("�ɼ�������") + key + ",��֧��0����1ͨ��");
					break;
				}
			}
		}
		else if (key == "�ɼ�������")
		{
			if (value != "MV800" && value != "MOR")
			{
				setLastError("�ɼ�����֧��[MOR]����[MV800]");
				break;
			}
		}

		if (!m_deviceConfigObj.contains(key))
		{
			setLastError(QString("%1�Ƿ��ļ�").arg(key));
			break;
		}
		m_deviceConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getDeviceConfigExplain()
{
	static QStringList explain = { "�������", "����UDSЭ��", "֧��CAN��[ZLG ADV KVASER PORT]",
		"֧�ֲɼ���[MOR MV800]","���֧��2��ͨ��ͬʱ����,�˴�����1,ͨ���Ž�ʧЧ", "֧��0~1ͨ�����,[MV800,AV1��1,AV2��0ͨ��]",
		"֧�ּ��[Ӳ�� ����]","��ʾǰNλΪ��ǰ�ַ���", "����������ܳ���" };
	return explain;
}

const QString JsonTool::getHardwareConfigValue(const QString& key)
{
	return m_hardwareConfigObj[key].toString();
}

const int JsonTool::getHardwareConfigCount()
{
	return m_hardwareConfigObj.count();
}

const QStringList& JsonTool::getHardwareConfigKeyList()
{
	return m_hardwareConfigKeyList;
}

const hardwareConfig_t& JsonTool::getParseHardwareConfig()
{
	return m_defConfig.hardware;
}

bool JsonTool::setHardwareConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toInt(&convert);
		if (!convert)
		{
			setLastError(QString("%1����Ϊ����").arg(key));
			break;
		}

		if (!m_hardwareConfigObj.contains(key))
		{
			setLastError(QString("%1�Ƿ��ļ�").arg(key));
			break;
		}

		m_hardwareConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getHardwareConfigExplain()
{
	static QStringList explain = { "��Դ���ڱ��","������Ĭ��[19200]","ϵͳ��ѹ","�̵������ڱ��","������Ĭ��[19200]","��ѹ���ڱ��",
			"������Ĭ��[9600]","��̬�������ڱ��","������Ĭ��[9600]","Ԥ����չ","Ԥ����չ","Ԥ����չ","Ԥ����չ",
	"Ԥ����չ","Ԥ����չ","Ԥ����չ","Ԥ����չ" };
	return explain;
}

const QString JsonTool::getRelayConfigValue(const QString& key)
{
	return m_relayConfigObj[key].toString();
}

const int JsonTool::getRelayConfigCount()
{
	return m_relayConfigObj.count();
}

const relayConfig_t& JsonTool::getParsedRelayConfig()
{
	return m_defConfig.relay;
}

const QStringList& JsonTool::getRelayConfigKeyList()
{
	return m_relayConfigKeyList;
}

bool JsonTool::setRelayConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toInt(&convert);
		if (!convert)
		{
			setLastError(QString("%1����Ϊ����").arg(key));
			break;
		}

		if (!m_relayConfigObj.contains(key))
		{
			setLastError(QString("%1�Ƿ��ļ�").arg(key));
			break;
		}
		m_relayConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getRelayConfigExplain()
{
	static QStringList explain = { "����","����ӦѲ�����Ƶ�Դ","���ڼ�⾲̬����","���ڼ��Ӳ������ѹ","���ڲɼ�ȫ������","���ڽ���¼��",
			"���ڲ�������" };
	return explain;
}

const QString JsonTool::getUserConfigValue(const QString& key)
{
	return m_userConfigObj[key].toString();
}

const int JsonTool::getUserConfigCount()
{
	return m_userConfigObj.count();
}

void JsonTool::setUserConfigValue(const QString& key, const QString& value)
{
	if (m_userConfigObj.contains(key))
	{
		m_userConfigObj[key] = value;
	}
}

const QString JsonTool::getRangeConfigValue(const QString& key)
{
	return m_rangeConfigObj[key].toString();
}

const int JsonTool::getRangeConfigCount()
{
	return m_rangeConfigObj.count();
}

const QStringList& JsonTool::getRangeConfigKeyList()
{
	return m_rangeConfigKeyList;
}

const rangeConfig_t& JsonTool::getParsedRangeConfig()
{
	return m_defConfig.range;
}

bool JsonTool::setRangeConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		auto split = value.split("~",QString::SkipEmptyParts);
		if (split.size() != 2)
		{
			setLastError(QString("%1��ʽ����,��Χ��ʽ����Ϊ:[0.0~10.0]").arg(key));
			break;
		}

		for (int i = 0; i < split.size(); i++)
		{
			split[i].toFloat(&convert);
			if (!convert)
			{
				setLastError(QString("%1�е�����,����Ϊ����").arg(key));
				break;
			}
		}

		if (!convert)
		{
			break;
		}

		if (!m_rangeConfigObj.contains(key))
		{
			setLastError(QString("%1�Ƿ��ļ�").arg(key));
			break;
		}
		m_rangeConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getRangeConfigExplain()
{
	static QStringList explain = { "��λ:MB(���ֽ�)","��λ:MM(����)","��λ:MM(����)","��λ:��(��)","��λ:PX(����)" };
	return explain;
}

const QString JsonTool::getThresholdConfigValue(const QString& key)
{
	return m_thresholdConfigObj[key].toString();
}

const int JsonTool::getThresholdConfigCount()
{
	return m_thresholdConfigObj.count();
}

const QStringList& JsonTool::getThresholdConfigKeyList()
{
	return m_thresholdKeyList;
}

const thresholdConfig_t& JsonTool::getParsedThresholdConfig()
{
	return m_defConfig.threshold;
}

bool JsonTool::setThresholdConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toFloat(&convert);
		if (!convert)
		{
			setLastError(QString("%1,����Ϊ����").arg(key));
			break;
		}

		if (!m_thresholdConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}
		m_thresholdConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getThresholdConfigExplain()
{
	static QStringList explain = { "��λ:A(����)","��λ:A(����)" };
	return explain;
}

const imageConfig_t& JsonTool::getParsedImageConfig()
{
	return m_defConfig.image;
}

const int JsonTool::getImageConfigCount()
{
	return m_imageConfigObj.count();
}

const QStringList JsonTool::getParentImageKeyList()
{
	return m_parentImageKeyList;
}

void JsonTool::setChildImageKeyListSubscript(const int& subscript)
{
	m_childImageSubscript = subscript;
}

const QStringList JsonTool::getChildImageKeyList(const int& id)
{
	return m_childImageKeyList[id];
}

const QStringList& JsonTool::getChildImageKeyList()
{
	return m_childImageKeyList[m_childImageSubscript];
}

const QString JsonTool::getImageConfigValue(const QString& parentKey, const QString& childKey)
{
	return m_imageConfigObj.value(parentKey).toObject().value(childKey).toString();
}

bool JsonTool::setImageConfigValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		if (childKey != "��ɫ")
		{
			value.toInt(&convert);
			if (!convert)
			{
				setLastError(QString("%1,%2,����Ϊ����").arg(parentKey, childKey));
				break;
			}
		}

		if (!m_imageConfigObj.contains(parentKey))
		{
			setLastError(QString("%1�Ƿ��ļ�").arg(parentKey));
			break;
		}

		QJsonObject object = m_imageConfigObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			setLastError(QString("%1�Ƿ��ļ�").arg(childKey));
			break;
		}
		object[childKey] = value;
		m_imageConfigObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

const QStringList JsonTool::getImageConfigExplain(const int& i)
{
	static QStringList explain0 = { "1����,0����,���ý�R,G,B,�������ж�ʧЧ,��ɫ�����ж���Ч","1����,0����","1����,0����","1����,0����" };
	static QStringList explain1 = { "�ж��﷨,!=��ɫ(�����ں�ɫ),==��ɫ(���ں�ɫ)",
		"��ԭɫ:Red(��ɫ)","��ԭɫ:Green(��ɫ)","��ԭɫ:Blue(��ɫ)","��λ:PX(����)",
		"��λ:MM(����)","��λ:MM(����)","��λ:MM(����)","��λ:MM(����)" };

	if (i == getImageConfigCount() - 1)
	{
		return explain0;
	}
	return explain1;
}

const QStringList& JsonTool::getImageConfigExplain()
{
	static QStringList explain0 = { "1����,0����,���ý�R,G,B,�������ж�ʧЧ,��ɫ�����ж���Ч","1����,0����","1����,0����","1����,0����" };
	static QStringList explain1 = { "�ж��﷨,!=��ɫ(�����ں�ɫ),==��ɫ(���ں�ɫ)",
		"��ԭɫ:Red(��ɫ)","��ԭɫ:Green(��ɫ)","��ԭɫ:Blue(��ɫ)","��λ:PX(����)",
			"��λ:MM(����)","��λ:MM(����)","��λ:MM(����)","��λ:MM(����)" };

	if (m_childImageSubscript == getImageConfigCount() - 1)
	{
		return explain0;
	}
	return explain1;
}

const int JsonTool::getEnableConfigCount()
{
	return m_enableConfigKeyList.count();
}

const QStringList& JsonTool::getEnableConfigKeyList()
{
	return m_enableConfigKeyList;
}

const QStringList JsonTool::getEnableConfigValueList()
{
	return m_enableConfigValueList;
}

const QString JsonTool::getEnableConfigValue(const QString& key)
{
	return m_enableConfigObj[key].toString();
}

bool JsonTool::setEnableConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toInt(&convert);
		if (!convert)
		{
			setLastError(QString("%1,����Ϊ����").arg(key));
			break;
		}

		if (value != "0" && value != "1")
		{
			setLastError("��ֵֻ��Ϊ0��1");
			break;
		}

		if (!m_enableConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}
		m_enableConfigObj[key] = value; 
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getEnableConfigExplain()
{
	static QStringList explain = { "0����,1����","0����,1����", "0����,1����", "0����,1����", "0����,1����" ,"0����,1����" };
	return explain;
}

DefConfig* JsonTool::getParsedDefConfig()
{
	return &m_defConfig;
}

const int JsonTool::getVoltageConfigCount()
{
	return m_voltageConfigObj.count();
}

const QStringList& JsonTool::getChildVoltageConfigKeyList()
{
	static QStringList keys = { "����","����","�̵���IO" };
	return keys;
}
//631156
const QStringList& JsonTool::getChildVoltageConfigValueList()
{
	static QStringList explain = { "1.8", "1.0", "1" };
	return explain;
}

const QStringList JsonTool::getParentVoltageConfigKeyList()
{
	return m_voltageConfigObj.keys();
}

const QString JsonTool::getVoltageConfigValue(const QString& parentKey, const QString& childKey)
{
	return m_voltageConfigObj.value(parentKey).toObject().value(childKey).toString();
}

void JsonTool::setVoltageConfigKey(const QString& oldParentKey, const QString& newParentKey)
{
	do 
	{
		if (!m_voltageConfigObj.contains(oldParentKey))
		{
			break;
		}

		QJsonObject object = m_voltageConfigObj[oldParentKey].toObject();
		m_voltageConfigObj.remove(oldParentKey);
		m_voltageConfigObj.insert(newParentKey, object);
	} while (false);
	return;
}

bool JsonTool::setVoltageConfigValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		if (childKey != "�̵���IO")
		{
			value.toFloat(&convert);
			if (!convert)
			{
				setLastError(QString("%1,%2,����Ϊ����").arg(parentKey, childKey));
				break;
			}
		}
		else
		{
			value.toInt(&convert);
			if (!convert)
			{
				setLastError(QString("%1,%2,����Ϊ����").arg(parentKey, childKey));
				break;
			}
		}

		if (!m_voltageConfigObj.contains(parentKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(parentKey));
			break;
		}

		QJsonObject object = m_voltageConfigObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(childKey));
			break;
		}
		object[childKey] = value;
		m_voltageConfigObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

QJsonObject& JsonTool::getVoltageConfigObj()
{
	return m_voltageConfigObj;
}

const QStringList& JsonTool::getVoltageConfigExplain()
{
	static QStringList explain = { "��λ:V(��)","��λ:V(��)","�̵����ӿڱ��" };
	return explain;
}

const int JsonTool::getKeyVolConfigCount()
{
	return m_keyVolConfigObj.count();
}

const QStringList& JsonTool::getKeyVolConfigKeyList()
{
	static QStringList keys = { "�ߵ�ƽ����","�ߵ�ƽ����","�͵�ƽ����","�͵�ƽ����" };
	return keys;
}

const QStringList JsonTool::getKeyVolConfigValueList()
{
	return { "14.0","8.0","1.0","0.0" };
}

const QString JsonTool::getKeyVolConfigValue(const QString& key)
{
	return m_keyVolConfigObj.value(key).toString();
}

bool JsonTool::setKeyVolConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toFloat(&convert);
		if (!convert)
		{
			setLastError(QString("%1,����Ϊ����").arg(key));
			break;
		}

		if (!m_keyVolConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}
		m_keyVolConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getKeyVolConfigExplain()
{
	static QStringList explain = { "��λ:V(��)","��λ:V(��)","��λ:V(��)","��λ:V(��)" };
	return explain;
}

const int JsonTool::getCurrentConfigCount()
{
	return m_currentConfigObj.count();
}

const QStringList JsonTool::getParentCurrentConfigKeyList()
{
	return m_currentConfigObj.keys();
}

const QStringList& JsonTool::getChildCurrentConfigKeyList()
{
	static QStringList keys = { "����","����","��ѹ" };
	return keys;
}

const QStringList& JsonTool::getChildCurrentConfigValueList()
{
	static QStringList keys = { "0.0","0.0","0.0" };
	return keys;
}

const QStringList JsonTool::getChildCurrentConfigValueList(const int& i)
{
	QStringList list_[2]{ { "1.0","0.3","12.0" } ,{"1.2","0.4","16.0"} };
	return list_[i > 1 ? 1 : i];
}

const QString JsonTool::getCurrentConfigValue(const QString& parentKey, const QString& childKey)
{
	return m_currentConfigObj.value(parentKey).toObject().value(childKey).toString();
}

void JsonTool::setCurrentConfigKey(const QString& oldParentKey, const QString& newParentKey)
{
	do
	{
		if (!m_currentConfigObj.contains(oldParentKey))
		{
			break;
		}
		QJsonObject object = m_currentConfigObj[oldParentKey].toObject();
		m_currentConfigObj.remove(oldParentKey);
		m_currentConfigObj.insert(newParentKey, object);
	} while (false);
	return;
}

bool JsonTool::setCurrentConfigValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toFloat(&convert);
		if (!convert)
		{
			setLastError(QString("%1,%2����Ϊ����").arg(parentKey, childKey));
			break;
		}

		if (!m_currentConfigObj.contains(parentKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(parentKey));
			break;
		}

		QJsonObject object = m_currentConfigObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(childKey));
			break;
		}
		object[childKey] = value;
		m_currentConfigObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

QJsonObject& JsonTool::getCurrentConfigObj()
{
	return m_currentConfigObj;
}

const QStringList& JsonTool::getCurrentConfigExplain()
{
	static QStringList explain = { "��λ:A(����)", "��λ:A(����)", "��λ:V(��)" };
	return explain;
}

const int JsonTool::getStaticConfigCount()
{
	return m_staticConfigObj.count();
}

const QStringList& JsonTool::getStaticConfigKeyList()
{
	static QStringList keys = { "����","����" };
	return keys;
}

const QStringList JsonTool::getStaticConfigValueList()
{
	return { "50.0","10.0" };
}

const QString JsonTool::getStaticConfigValue(const QString& key)
{
	return m_staticConfigObj.value(key).toString();
}

bool JsonTool::setStaticConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toFloat(&convert);
		if (!convert)
		{
			setLastError(QString("%1,����Ϊ����").arg(key));
			break;
		}

		if (!m_staticConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}
		m_staticConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getStaticConfigExplain()
{
	static QStringList explain = { "��λ:��A(΢��)","��λ:��A(΢��)" };
	return explain;
}

const QStringList JsonTool::getParentResConfigKeyList()
{
	return m_resConfigObj.keys();
}

const QStringList& JsonTool::getChildResConfigKeyList()
{
	static QStringList keys = { "����","����","�̵���IO" };
	return keys;
}

const QStringList& JsonTool::getChildResConfigValueList()
{
	static QStringList keys = { "8000","5000","5" };
	return keys;
}

const int JsonTool::getResConfigCount()
{
	return m_resConfigObj.count();
}

const QString JsonTool::getResConfigValue(const QString& parentKey, const QString& childKey)
{
	return m_resConfigObj.value(parentKey).toObject().value(childKey).toString();
}

void JsonTool::setResConfigKey(const QString& oldParentKey, const QString& newParentKey)
{
	do 
	{
		if (!m_resConfigObj.contains(oldParentKey))
		{
			break;
		}

		QJsonObject object = m_resConfigObj[oldParentKey].toObject();
		m_resConfigObj.remove(oldParentKey);
		m_resConfigObj.insert(newParentKey, object);
	} while (false);
	return;
}

bool JsonTool::setResConfigValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		if (childKey != "�̵���IO")
		{
			value.toInt(&convert);
			if (!convert)
			{
				setLastError(QString("%1,%2,����Ϊ����").arg(parentKey, childKey));
				break;
			}
		}
		else
		{
			value.toFloat(&convert);
			if (!convert)
			{
				setLastError(QString("%1,%2,����Ϊ����").arg(parentKey, childKey));
				break;
			}
		}

		if (!m_resConfigObj.contains(parentKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(parentKey));
			break;
		}

		QJsonObject object = m_resConfigObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(childKey));
			break;
		}
		object[childKey] = value;
		m_resConfigObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

QJsonObject& JsonTool::getResConfigObj()
{
	return m_resConfigObj;
}

const QStringList& JsonTool::getResConfigExplain()
{
	static QStringList explain = { "��λ:��(ŷķ)","��λ:��(ŷķ)","�̵����ӿڱ��" };
	return explain;
}

HwdConfig* JsonTool::getParsedHwdConfig()
{
	return &m_hwdConfig;
}

const int JsonTool::getVerConfigCount()
{
	return m_verConfigObj.count();
}

const QStringList JsonTool::getParentVerConfigKeyList()
{
	return m_verConfigObj.keys();
}

const QStringList& JsonTool::getChildVerConfigKeyList()
{
	static QStringList keys = { "DID","����","ֵ" };
	return keys;
}

const QStringList& JsonTool::getChildVerConfigValueList()
{
	static QStringList keys = { "0xFFFF","ASCII","00000000" };
	return keys;
}

const QString JsonTool::getVerConfigValue(const QString& parentKey, const QString& childKey)
{
	return m_verConfigObj.value(parentKey).toObject().value(childKey).toString();
}

void JsonTool::setVerConfigKey(const QString& oldParentKey, const QString& newParentKey)
{
	do 
	{
		if (!m_verConfigObj.contains(oldParentKey))
		{
			break;
		}

		QJsonObject object = m_verConfigObj[oldParentKey].toObject();
		m_verConfigObj.remove(oldParentKey);
		m_verConfigObj.insert(newParentKey, object);
	} while (false);
	return;
}

bool JsonTool::setVerConfigValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		QString newValue = value;

		if (childKey == "DID")
		{
			if (!(newValue.contains("0x") || newValue.contains("0X")))
			{
				if (newValue.length() == 4)
				{
					newValue.insert(0, "0x");
				}
				else
				{
					setLastError(QString("%1,%2��ʽ����,���ո�ʽ0xF16C").arg(parentKey, childKey));
					break;
				}
			}
			else
			{
				if (!(newValue.length() == 6))
				{
					setLastError(QString("%1,%2��ʽ����,���ո�ʽ0xF16C").arg(parentKey, childKey));
					break;
				}
			}

			newValue.toInt(&convert, 16);
			if (!convert)
			{
				setLastError(QString("%1,%2��ʽ����,���ո�ʽ0xF16C").arg(parentKey, childKey));
				break;
			}
		}
		else if (childKey == "����")
		{
			QStringList data = {"ASCII","ASCR4","INT","USN","BIN","BCD"};
			if (!data.contains(newValue))
			{
				setLastError("%1,%2��֧�ֵĸ�ʽ");
				break;
			}
		}

		if (!m_verConfigObj.contains(parentKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(parentKey));
			break;
		}

		QJsonObject object = m_verConfigObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(childKey));
			break;
		}
		object[childKey] = newValue;
		m_verConfigObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

QJsonObject& JsonTool::getVerConfigObj()
{
	return m_verConfigObj;
}

const QStringList& JsonTool::getVerConfigExplain()
{
	static QStringList explain = { "���ݱ�ʶ��","֧�ֱ���[ASCII ASCR4 INT USN BIN BCD]","��ʶ������" };
	return explain;
}

const int JsonTool::getDtcConfigCount()
{
	return m_dtcConfigObj.count();
}

const QStringList JsonTool::getParentDtcConfigKeyList()
{
	return m_dtcConfigObj.keys();
}

const QStringList& JsonTool::getChildDtcConfigKeyList()
{
	static QStringList keys = { "DTC","����" };
	return keys;
}

const QStringList& JsonTool::getChildDtcConfigValueList()
{
	static QStringList keys = { "U100000","0" };
	return keys;
}

const QString JsonTool::getDtcConfigValue(const QString& parentKey, const QString& childKey)
{
	return m_dtcConfigObj.value(parentKey).toObject().value(childKey).toString();
}

void JsonTool::setDtcConfigKey(const QString& oldParentKey, const QString& newParentKey)
{
	do 
	{
		if (!m_dtcConfigObj.contains(oldParentKey))
		{
			break;
		}

		QJsonObject object = m_dtcConfigObj[oldParentKey].toObject();
		m_dtcConfigObj.remove(oldParentKey);
		m_dtcConfigObj.insert(newParentKey, object);
	} while (false);
	return;
}

bool JsonTool::setDtcConfigValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		if (childKey == "DTC")
		{
			if (dtcCategoryConvert(value).isEmpty())
			{
				setLastError("DTC�����ϱ������");
				break;
			}
		}
		else
		{
			value.toInt(&convert);
			if (!convert)
			{
				setLastError("���Ա���Ϊ����");
				break;
			}
		}

		if (!m_dtcConfigObj.contains(parentKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(parentKey));
			break;
		}

		QJsonObject object = m_dtcConfigObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(childKey));
			break;
		}
		object[childKey] = value;
		m_dtcConfigObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

QJsonObject& JsonTool::getDtcConfigObj()
{
	return m_dtcConfigObj;
}

const QStringList& JsonTool::getDtcConfigExplain()
{
	static QStringList explain =  { "��Ϲ�����","1����,0����" };
	return explain;
}

UdsConfig* JsonTool::getParsedUdsConfig()
{
	return &m_udsConfig;
}

const int JsonTool::getCanMsgCount()
{
	return m_canMsgObj.count();
}

const QStringList JsonTool::getCanMsgKeyList()
{
	return m_canMsgObj.keys();
}

const QString JsonTool::getCanMsgValue(const QString& parentKey, const QString& childKey)
{
	return m_canMsgObj.value(parentKey).toObject().value(childKey).toString();
}

void JsonTool::setCanMsgKey(const QString& oldParentKey, const QString& newParentKey)
{
	do 
	{
		if (!m_canMsgObj.contains(oldParentKey))
		{
			break;
		}
		QJsonObject object = m_canMsgObj[oldParentKey].toObject();
		m_canMsgObj.remove(oldParentKey);
		m_canMsgObj.insert(newParentKey, object);
	} while (false);
	return;
}

void JsonTool::setCanMsgValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	do 
	{
		if (!m_canMsgObj.contains(parentKey))
		{
			break;
		}
		QJsonObject object = m_canMsgObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			break;
		}
		object[childKey] = value;
		m_canMsgObj.insert(parentKey, object);
	} while (false);
	return;
}

const CanMsg* JsonTool::getParsedCanMsg()
{
	return m_canMsg;
}

QJsonObject& JsonTool::getCanMsgObj()
{
	return m_canMsgObj;
}
