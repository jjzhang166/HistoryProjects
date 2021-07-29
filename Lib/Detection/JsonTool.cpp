#include "JsonTool.h"
#include "TextData.h"

#include <Can/Can.h>
#include <Uds/Uds.h>

static bool compareList(const QStringList& cmp1, const QStringList& cmp2)
{
	bool result = false;
	do 
	{
		if (cmp1.size() != cmp2.size())
		{
			break;
		}

		bool contains1 = true, contains2 = true;
		for (int i = 0; i < cmp1.size(); i++)
		{
			if (!cmp2.contains(cmp1[i]))
			{
				contains1 = false;
				break;
			}
		}

		for (int i = 0; i < cmp2.size(); i++)
		{
			if (!cmp1.contains(cmp2[i]))
			{
				contains2 = false;
				break;
			}
		}

		if (!contains1 || !contains2)
			break;

		result = true;
	} while (false);
	return result;
}

static bool removeList(const QStringList& list1, QStringList& list2)
{
	bool result = false;
	auto tempList = list2;
	for (auto& x : tempList)
	{
		if (!list1.contains(x))
		{
			result = true;
			list2.removeOne(x);
		}
	}
	return result;
}

JsonTool* JsonTool::m_self = nullptr;

void JsonTool::setLastError(const QString& error)
{
	m_lastError = error;
	if (m_errorList.size() > 1024)
	{
		m_errorList.clear();
	}
	m_errorList.append(error);
}

bool JsonTool::parseDeviceConfigData()
{
	bool result = false, success = true, convert = false;
	do
	{
		int structSize = sizeof(DeviceConfig) / sizeof(QString);

		if (structSize != m_deviceConfigKeyList.size())
		{
			setLastError("�豸���ýṹ���С����б�ƥ��");
			break;
		}

		if (structSize != m_deviceConfigValueList.size())
		{
			setLastError("�豸���ýṹ���С��ֵ�б�ƥ��");
			break;
		}

		QString* valuePtr = reinterpret_cast<QString*>(&m_defConfig.device);
		for (int i = 0; i < m_deviceConfigKeyList.length(); i++, valuePtr++)
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
		//Ӳ�����ýṹ���С,�������ݸ�ʽ��һ��,�޷�����У��,��������ֵ,������У��
		int* valuePtr = reinterpret_cast<int*>(&m_defConfig.hardware);
		for (int i = 0; i < m_hardwareConfigKeyList.length(); i++, valuePtr++)
		{
			if (i == 2)
			{
				m_defConfig.hardware.powerVoltage = getHardwareConfigValue(m_hardwareConfigKeyList.value(i)).toFloat(&convert);
			}
			else if (i == 3)
			{
				m_defConfig.hardware.powerCurrent = getHardwareConfigValue(m_hardwareConfigKeyList.value(i)).toFloat(&convert);
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
		int structSize = sizeof(RelayConfig) / sizeof(int);
		if (structSize != m_relayConfigKeyList.size())
		{
			setLastError("�̵������ýṹ���С����б�ƥ��");
			break;
		}

		if (structSize != m_relayConfigValueList.size())
		{
			setLastError("�̵������ýṹ���С��ֵ�б�ƥ��");
			break;
		}

		int* valuePtr = reinterpret_cast<int*>(&m_defConfig.relay);
		for (int i = 0; i < m_relayConfigKeyList.length(); i++, valuePtr++)
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
					if ((!color.contains("!=") && !color.contains("==")))
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
		int structSize = sizeof(RangeConfig) / (sizeof(float) * 2);
		if (structSize != m_rangeConfigKeyList.size())
		{
			setLastError("��Χ���ýṹ���С�����ƥ��");
			break;
		}

		if (structSize != m_rangeConfigValueList.size())
		{
			setLastError("��Χ���ýṹ���С��ֵ��ƥ��");
			break;
		}

		float* valuePtr = reinterpret_cast<float*>(&m_defConfig.range);
		for (int i = 0; i < m_rangeConfigKeyList.length(); i++, valuePtr++, valuePtr++)
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
		int structSize = sizeof(ThresholdConfig) / sizeof(float);
		if (structSize != m_thresholdConfigKeyList.size())
		{
			setLastError("��ֵ���ýṹ���С�����ƥ��");
			break;
		}

		if (structSize != m_thresholdConfigValueList.size())
		{
			setLastError("��ֵ���ýṹ���С��ֵ��ƥ��");
			break;
		}

		float* valuePtr = reinterpret_cast<float*>(&m_defConfig.threshold);
		for (int i = 0; i < m_thresholdConfigKeyList.length(); i++, valuePtr++)
		{
			*valuePtr = getThresholdConfigValue(m_thresholdConfigKeyList.value(i)).toFloat(&convert);
			if (!convert)
			{
				setLastError(QString("��ֵ����[%1]��ʽ����").arg(m_thresholdConfigKeyList.value(i)));
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
		int structSize = sizeof(EnableConfig) / sizeof(int);
		if (structSize != m_enableConfigKeyList.size())
		{
			setLastError("�������ýṹ���С�����ƥ��");
			break;
		}

		if (structSize != m_enableConfigValueList.size())
		{
			setLastError("�������ýṹ���С��ֵ��ƥ��");
			break;
		}

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

		m_hwdConfig.voltage = NO_THROW_NEW VoltageConfig[getVoltageConfigCount() + 1];
		VoltageConfig* voltage = m_hwdConfig.voltage;
		if (!voltage)
		{
			setLastError("��ѹ���÷����ڴ�ʧ��");
			break;
		}

		memset(voltage, 0x00, sizeof(VoltageConfig));

		for (int i = 0; i < getVoltageConfigCount(); i++)
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

		m_hwdConfig.current = NO_THROW_NEW CurrentConfig[getCurrentConfigCount() + 1];
		CurrentConfig* current = m_hwdConfig.current;
		if (!current)
		{
			setLastError("�������÷����ڴ�ʧ��");
			break;
		}
		memset(current, 0x00, sizeof(CurrentConfig));
		for (int i = 0; i < getCurrentConfigCount(); i++)
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

		m_hwdConfig.res = NO_THROW_NEW ResConfig[getResConfigCount() + 1];
		ResConfig* res = m_hwdConfig.res;
		if (!res)
		{
			setLastError("�������÷����ڴ�ʧ��");
			break;
		}
		memset(res, 0x00, sizeof(ResConfig));
		for (int i = 0; i < getResConfigCount(); i++)
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
	bool result = false, convert = false, success = false;
	do
	{
		const char* code[] = { "ASCII", "ASCR4", "INT", "USN", "BIN", "BCD","U08","ASCBCD44" };

		SAFE_DELETE_A(m_udsConfig.ver);

		m_udsConfig.ver = NO_THROW_NEW VersonConfig[getVerConfigCount() + 1];
		VersonConfig* ver = m_udsConfig.ver;
		if (!ver)
		{
			setLastError("�汾���÷����ڴ�ʧ��");
			break;
		}

		memset(ver, 0x00, sizeof(VersonConfig));
		for (int i = 0; i < getVerConfigCount(); i++)
		{
			strcpy(ver[i].name, Q_TO_C_STR(getParentVerConfigKeyList()[i]));

			success = true;
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

			success = false;
			for (int j = 0; j < sizeof(code) / sizeof(char*); j++)
			{
				if (!strcmp(ver[i].encode, code[j]))
				{
					success = true;
					break;
				}
			}

			if (!success)
			{
				setLastError(QString("�汾����[%1]�����ʽ����").arg(getParentVerConfigKeyList()[i]));
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

		m_udsConfig.dtc = NO_THROW_NEW DtcConfig[getDtcConfigCount() + 1];
		DtcConfig* dtc = m_udsConfig.dtc;
		if (!dtc)
		{
			setLastError("��Ϲ��������÷����ڴ�ʧ��");
			break;
		}
		memset(dtc, 0x00, sizeof(DtcConfig));
		for (int i = 0; i < getDtcConfigCount(); i++)
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

QString JsonTool::dtcCategoryConvert(const QString& dtc)
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

const QStringList& JsonTool::getErrorList()
{
	return m_errorList;
}

void JsonTool::setFolderName(const QString& name)
{
	m_folderName = name;
}

bool JsonTool::initInstance(bool update, const QString& folderName, const QStringList& fileName)
{
	bool result = false, success = true;
	do
	{
		QString dirName = m_folderName.isEmpty() ? folderName : m_folderName;
		QString jsonPath = QString("%1/JsonFile_%2").arg(dirName, JSON_VERSION);
		QString dcfPath = QString("%1/DcfFile_%2").arg(dirName, DCF_VERSION);

		QStringList pathList = { jsonPath,dcfPath };
		for (int i = 0; i < pathList.count(); i++)
		{
			if (!QDir(pathList[i]).exists())
			{
				QDir dir;
				if (!dir.mkpath(pathList[i]))
				{
					success = false;
					setLastError(QString("�����ļ���%1ʧ��").arg(pathList[i]));
					break;
				}
			}
		}

		if (!success)
		{
			break;
		}

		bool (JsonTool:: * readFnc[])(const QString&) =
		{
			&JsonTool::readDefJsonFile,
			&JsonTool::readHwdJsonFile,
			&JsonTool::readUdsJsonFile,
			&JsonTool::readImgJsonFile,
			&JsonTool::readOthJsonFile
		};

		bool (JsonTool:: * writeFnc[])(const QString&) =
		{
			&JsonTool::writeDefJsonFile,
			&JsonTool::writeHwdJsonFile,
			&JsonTool::writeUdsJsonFile,
			&JsonTool::writeImgJsonFile,
			&JsonTool::writeOthJsonFile
		};

		bool (JsonTool:: * updateFnc[])(const QString&) =
		{
			&JsonTool::updateDefJsonFile,
			&JsonTool::updateHwdJsonFile,
			&JsonTool::updateUdsJsonFile,
			&JsonTool::updateImgJsonFile,
			&JsonTool::updateOthJsonFile
		};

		RUN_BREAK(fileName.size() != sizeof(readFnc) / sizeof(*readFnc) ||
			fileName.size() != sizeof(writeFnc) / sizeof(*writeFnc) ||
			fileName.size() != sizeof(updateFnc) / sizeof(*updateFnc),
			"�ļ����б��뺯��ָ�����鲻��Ӧ");

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

QStringList JsonTool::getAllMainKey()
{
	//���б�˳�򲻿��޸�,���򽫻ᵼ��SettingDlg���ر���
	static QStringList keys =
	{
		"�豸����",//0
		"Ӳ������",//1
		"�̵�������",//2
		"��Χ����",//3
		"��ֵ����",//4
		"ͼ������",//5
		"������ѹ����",//6
		"��ѹ����",//7
		"��������",//8
		"��������",//9
		"��̬��������",//10
		"�汾����",//11
		"��Ϲ���������",//12
		"��������",//13
		"�û�����"//14
	};
	return keys;
}

QString JsonTool::getLibVersion()
{
	return LIB_VERSION;
}

QString JsonTool::getJsonVersion()
{
	return JSON_VERSION;
}

QString JsonTool::getDcfVersion()
{
	return DCF_VERSION;
}

bool JsonTool::writeDcfFile(const QString& name)
{
	bool result = false;
	do 
	{
		QFile file(name);
		if (!file.open(QFile::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}

		int length = strlen(TextData::ntscData);
		if (file.write(TextData::ntscData, length) != length)
		{
			setLastError("д�����ݳ���У��ʧ��");
			break;
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::readJsonFile(const QString& name, QJsonObject& rootObj)
{
	bool result = false;
	do 
	{
		QFile file(name);
		RUN_BREAK(!file.open(QFile::ReadOnly), file.errorString());
		QByteArray bytes = file.readAll();
		file.close();

		QJsonParseError jsonError;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &jsonError));
		RUN_BREAK(jsonError.error != QJsonParseError::NoError, jsonError.errorString());
		rootObj = jsonDoc.object();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::writeJsonFile(const QString& name, const QJsonObject& rootObj)
{
	bool result = false;
	do 
	{
		QJsonDocument doc(rootObj);
		QByteArray bytes = doc.toJson();
		QFile file(name);
		RUN_BREAK(!file.open(QFile::WriteOnly), file.errorString());
		if (bytes.length() != file.write(bytes))
		{
			setLastError(QString("д��%1����У��ʧ��").arg(name));
			file.close();
			break;
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::testDefJsonFile(const QString& name)
{
	bool result = false, save = false;
	do
	{
		QJsonObject rootObj;
		RUN_BREAK(!readJsonFile(name, rootObj), "�Լ��ȡdef.json�ļ�ʧ��," + getLastError())

		QStringList keyList =
		{
			"�豸����",
			"Ӳ������",
			"�̵�������",
			"�û�����",
			"��Χ����",
			"��ֵ����",
			"��������"
		};

		QList<QStringList*>memberKeyList = 
		{ 
			&m_deviceConfigKeyList,
			&m_hardwareConfigKeyList,
			&m_relayConfigKeyList,
			&m_userConfigKeyList,
			&m_rangeConfigKeyList,
			&m_thresholdConfigKeyList,
			&m_enableConfigKeyList 
		};

		QList<QStringList*>memberValueList = 
		{
			&m_deviceConfigValueList,
			&m_hardwareConfigValueList,
			&m_relayConfigValueList,
			&m_userConfigValueList,
			&m_rangeConfigValueList,
			&m_thresholdConfigValueList,
			&m_enableConfigValueList
		};

		const QString(JsonTool:: * valueFnc[])(const QString&) =
		{
			&JsonTool::getDeviceConfigDefaultValue,
			&JsonTool::getHardwareConfigDefaultValue,
			&JsonTool::getRelayConfigDefaultValue,
			&JsonTool::getUserConfigDefaultValue,
			&JsonTool::getRangeConfigDefaultValue,
			&JsonTool::getThresholdConfigDefaultValue,
			&JsonTool::getEnableConfigDefaultValue
		};

		for (int i = 0; i < keyList.count(); i++)
		{
			QStringList objectKeyList, loseKeyList, loseValueList;
			if (rootObj.contains(keyList[i]))
			{
				loseKeyList = *memberKeyList[i];
				QJsonObject jsonObject = rootObj.value(keyList[i]).toObject();
				objectKeyList = jsonObject.keys();
				QStringList oldObjectKeyList = objectKeyList;
				if (removeList(*memberKeyList[i], objectKeyList))
				{
					QStringList removeObjList;
					for (int j = 0; j < oldObjectKeyList.size(); j++)
					{
						if (!objectKeyList.contains(oldObjectKeyList[j]))
						{
							removeObjList.push_back(oldObjectKeyList[j]);
						}
					}

					for (auto& x : removeObjList)
					{
						m_errorList.append(QString("%1 δ֪{ �� }:{ %2 } ���Զ�ɾ��").
							arg(keyList[i], x));
						jsonObject.remove(x);
					}
				}

				if (!compareList(*memberKeyList[i], objectKeyList))
				{
					for (int j = 0; j < memberKeyList[i]->size(); j++)
					{
						for (int k = 0; k < objectKeyList.size(); k++)
						{
							if (memberKeyList[i]->at(j) == objectKeyList[k])
							{
								loseKeyList.removeOne(objectKeyList[k]);
							}
						}
					}

					for (int j = 0; j < loseKeyList.size(); j++)
					{
						loseValueList.append((this->*valueFnc[i])(loseKeyList[j]));
					}

					for (int j = 0; j < loseKeyList.size(); j++)
					{
						jsonObject.insert(loseKeyList[j], loseValueList[j]);
						m_errorList.append(QString("%1 ��ʧ{ �� / ֵ }:{ %2 / %3 } ���Զ����").
							arg(keyList[i], loseKeyList[j], loseValueList[j]));
					}
				}

				if (jsonObject != rootObj.value(keyList[i]).toObject())
				{
					save = true;
					rootObj.insert(keyList[i], jsonObject);
				}
			}
		}

		if (save)
		{
			RUN_BREAK(!writeJsonFile(name, rootObj), "�Լ�д��def.jsonʧ��,"
				+ getLastError());
		}
		result = true;
	} while (false);
	return result;
}

#define NEW_IMAGE_JSON 1
bool JsonTool::readDefJsonFile(const QString& name)
{
	bool result = false, success = true;
	do
	{
		testDefJsonFile(name);

		QJsonObject rootObj;
		RUN_BREAK(!readJsonFile(name, rootObj), "��ȡdef.json�����ļ�ʧ��," + getLastError());

#if NEW_IMAGE_JSON
		QStringList keyList = 
		{ 
			"�豸����",
			"Ӳ������",
			"�̵�������",
			"�û�����",
			"��Χ����",
			"��ֵ����",
			"��������" 
		};

		QList<QJsonObject*>objList =
		{
			&m_deviceConfigObj,
			&m_hardwareConfigObj,
			&m_relayConfigObj,
			&m_userConfigObj,
			&m_rangeConfigObj,
			&m_thresholdConfigObj,
			&m_enableConfigObj
		};

		bool (JsonTool:: * parseFnc[])() = 
		{
			&JsonTool::parseDeviceConfigData,
			&JsonTool::parseHardwareConfigData,
			&JsonTool::parseRelayPortConfigData,
			&JsonTool::parseUserConfigData,
			&JsonTool::parseRangeConfigData,
			&JsonTool::parseThresholdConfigData,
			&JsonTool::parseEnableConfigData
		};

#else
		QStringList keyList = { "�豸����","Ӳ������","�̵�������","�û�����",
			"ͼ������","��Χ����","��ֵ����","��������" };

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
#endif
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

#if !NEW_IMAGE_JSON
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
#endif

	/*��ֵ����*/
	for (int i = 0; i < m_thresholdConfigKeyList.length(); i++)
	{
		thresholdConfigObj.insert(m_thresholdConfigKeyList[i], m_thresholdConfigValueList[i]);
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
#if !NEW_IMAGE_JSON
	rootObj.insert("ͼ������", imageConfigObj);
#endif
	rootObj.insert("�̵�������", relayIoConfigObj);
	rootObj.insert("��ֵ����", thresholdConfigObj);
	rootObj.insert("��������", enableConfigObj);

	bool result = false;
	do 
	{
		RUN_BREAK(!writeJsonFile(name, rootObj), "д��def.json����ʧ��," + getLastError());
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
#if !NEW_IMAGE_JSON
	rootObj.insert("ͼ������", m_imageConfigObj);
#endif
	rootObj.insert("�̵�������", m_relayConfigObj);
	rootObj.insert("��ֵ����", m_thresholdConfigObj);
	rootObj.insert("��������", m_enableConfigObj);

	bool result = false;
	do
	{
		RUN_BREAK(!writeJsonFile(name, rootObj), "����def.json�����ļ�ʧ��," + getLastError());
		result = true;
	} while (false);
	return result;
}

bool JsonTool::readHwdJsonFile(const QString& name)
{
	bool result = false, success = true;
	do
	{
		QJsonObject root;
		RUN_BREAK(!readJsonFile(name, root), "��ȡhwd.json�����ļ�ʧ��," + getLastError());

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

		RUN_BREAK(!writeJsonFile(name, rootObj), "д��hwd.json�����ļ�ʧ��," + getLastError());
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

		RUN_BREAK(!writeJsonFile(name, rootObj), "����hwd.json�����ļ�ʧ��," + getLastError());
		result = true;
	} while (false);
	return result;
}

bool JsonTool::testImgJsonFile(const QString& name)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj;
		RUN_BREAK(!readJsonFile(name, rootObj), "�Լ��ȡimg.json�ļ�ʧ��," + getLastError());

		if (rootObj.contains("ͼ������"))
		{
			QJsonObject parentObj = rootObj.value("ͼ������").toObject();
		}

		result = true;
	} while (false);
	return result;
}

bool JsonTool::readImgJsonFile(const QString& name)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj;
		RUN_BREAK(!readJsonFile(name, rootObj), "��ȡimg.json�����ļ�ʧ��," + getLastError());

		if (!rootObj.contains("ͼ������"))
		{
			setLastError(QString("%1�����ļ�δ�ҵ�������ͼ������").arg(name));
			break;
		}

		m_imageConfigObj = rootObj.value("ͼ������").toObject();
		if (!parseImageConfigData())
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::writeImgJsonFile(const QString& name)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj;

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

		rootObj.insert("ͼ������", imageConfigObj);

		RUN_BREAK(!writeJsonFile(name, rootObj), "д��img.json����ʧ��," + getLastError());
		result = true;
	} while (false);
	return result;
}

bool JsonTool::updateImgJsonFile(const QString& name)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj;
		rootObj.insert("ͼ������", m_imageConfigObj);
		RUN_BREAK(!writeJsonFile(name, rootObj), "����img.json�����ļ�ʧ��," + getLastError());
		result = true;
	} while (false);
	return result;
}

bool JsonTool::readOthJsonFile(const QString& name)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj;
		RUN_BREAK(!readJsonFile(name, rootObj), "��ȡoth.json�����ļ�ʧ��," + getLastError());
		RUN_BREAK(!rootObj.contains("��������"), "��ʧ����[��������]");
		m_othConfigObj = rootObj.value("��������").toObject();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::writeOthJsonFile(const QString& name)
{
	bool result = false;
	do 
	{
		QJsonObject rootObj;
		rootObj.insert("��������",QJsonObject());
		RUN_BREAK(!writeJsonFile(name, rootObj), "д��oth.json�����ļ�ʧ��," + getLastError());
		result = true;
	} while (false);
	return result;
}

bool JsonTool::updateOthJsonFile(const QString& name)
{
	return true;
}

bool JsonTool::readUdsJsonFile(const QString& name)
{
	bool result = false, success = false;
	do
	{
		QJsonObject root;
		RUN_BREAK(!readJsonFile(name, root), "��ȡuds.json�����ļ�ʧ��," + getLastError());
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

		RUN_BREAK(!writeJsonFile(name, rootObj), "д��uds.json�����ļ�ʧ��," + getLastError());
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

		RUN_BREAK(!writeJsonFile(name, rootObj), "����uds.json�����ļ�ʧ��," + getLastError());
		result = true;
	} while (false);
	return result;
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

const DeviceConfig& JsonTool::getParsedDeviceConfig()
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
		if (key == "���볤��" || key == "�ɼ���ͨ����" || key == "�ɼ���ͨ����" ||
			key == "CAN������" || key == "CAN��չ֡")
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
			if (value != MV800_CC && value != MOR_CC && value != ANY_CC)
			{
				setLastError("�ɼ�����֧��MOR,MV800,ANY");
				break;
			}
		}
		else if (key == "�����ж�")
		{
			if (value.isEmpty())
			{
				setLastError("�����ж����Ϊ��,��дNULL");
				break;
			}
		}
		else if (key == "UDS����")
		{
			if (!Uds::getSupportUds().contains(value))
			{
				setLastError(QString("[%1]����֧�ֵķ�Χ��").arg(value));
				break;
			}
		}
		else if (key == "CAN����")
		{
			if (!Can::getSupportCan().contains(value))
			{
				setLastError(QString("[%1]����֧�ֵķ�Χ��").arg(value));
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
	static QStringList explain = 
	{ 
		"�������", 
		"�����ͣ�鿴��ʾ", 
		"ZLG ADV KVASER UART GCANFD",
		"Ĭ��:500",
		"0����,1����",
		"MOR MV800 ANY",
		"�����ͣ�鿴��ʾ",
		"MV800,AV1��1,AV2��0",
		"ǰNλ�ַ���", 
		"�����ܳ���"
	};
	return explain;
}

const QString JsonTool::getDeviceConfigDefaultValue(const QString& key)
{
	QString result = "0";
	for (int i = 0; i < m_deviceConfigKeyList.size(); i++)
	{
		if (m_deviceConfigKeyList[i] == key)
		{
			result = m_deviceConfigValueList.at(i);
			break;
		}
	}
	return result;
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

const HardwareConfig& JsonTool::getParseHardwareConfig()
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
	static QStringList explain = 
	{ 
		"���ڱ��",
		"Ĭ��:19200",
		"��λ:��(V)",
		"��λ:����(A)",
		"���ڱ��",
		"Ĭ��:19200",
		"���ڱ��",
		"Ĭ��:9600",
		"���ڱ��",
		"Ĭ��:9600",
		"Ԥ����չ",
		"Ԥ����չ",
		"Ԥ����չ",
		"Ԥ����չ",
		"Ԥ����չ",
		"Ԥ����չ",
		"Ԥ����չ",
		"Ԥ����չ"
	};
	return explain;
}

const QString JsonTool::getHardwareConfigDefaultValue(const QString& key)
{
	QString result = "0";
	for (int i = 0; i < m_hardwareConfigKeyList.size(); i++)
	{
		if (m_hardwareConfigKeyList[i] == key)
		{
			result = m_hardwareConfigValueList.at(i);
			break;
		}
	}
	return result;
}

const QString JsonTool::getRelayConfigValue(const QString& key)
{
	return m_relayConfigObj[key].toString();
}

const int JsonTool::getRelayConfigCount()
{
	return m_relayConfigObj.count();
}

const RelayConfig& JsonTool::getParsedRelayConfig()
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
	static QStringList explain = 
	{ 
		"����",
		"����ӦѲ�����Ƶ�Դ",
		"���ڼ�����",
		"���ڼ���ѹ",
		"����ͼ��ת��",
		"���ڽ���¼��",
		"���ڲ�������",
		"����TS��",
		"���NG��",
		"���OK��"
	};
	return explain;
}

const QString JsonTool::getRelayConfigDefaultValue(const QString& key)
{
	QString result = "0";
	for (int i = 0; i < m_relayConfigKeyList.size(); i++)
	{
		if (m_relayConfigKeyList[i] == key)
		{
			result = m_relayConfigValueList.at(i);
			break;
		}
	}
	return result;
}

const QStringList& JsonTool::getUserConfigKeyList()
{
	return m_userConfigKeyList;
}

const QStringList& JsonTool::getUserConfigExplain()
{
	static QStringList explain = {"�û���","����"};
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

bool JsonTool::setUserConfigValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		if (!m_userConfigObj.contains(key))
		{
			setLastError(QString("�Ƿ��ļ�ֵ[%1]").arg(key));
			break;
		}
		m_userConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

bool JsonTool::getUserPrivileges()
{
	const QString&& userName = getUserConfigValue("�û���").toUpper();
	return (userName == "ROOT" || userName == "INVO");
}

const QString JsonTool::getUserConfigDefaultValue(const QString& key)
{
	QString result = "0";
	for (int i = 0; i < m_userConfigKeyList.size(); i++)
	{
		if (m_userConfigKeyList[i] == key)
		{
			result = m_userConfigValueList.at(i);
			break;
		}
	}
	return result;
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

const RangeConfig& JsonTool::getParsedRangeConfig()
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
	static QStringList explain = { "��λ:MB(���ֽ�)","��λ:MM(����)",
		"��λ:MM(����)","��λ:��(��)","��λ:PX(����)",
		"��λ:A(����)","��λ:A(����)" };
	return explain;
}

const QString JsonTool::getRangeConfigDefaultValue(const QString& key)
{
	QString result = "0";
	for (int i = 0; i < m_rangeConfigKeyList.size(); i++)
	{
		if (m_rangeConfigKeyList[i] == key)
		{
			result = m_rangeConfigValueList.at(i);
			break;
		}
	}
	return result;
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
	return m_thresholdConfigKeyList;
}

const ThresholdConfig& JsonTool::getParsedThresholdConfig()
{
	return m_defConfig.threshold;
}

bool JsonTool::setThresholdConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		float data = value.toFloat(&convert);
		if (!convert)
		{
			setLastError(QString("%1,����Ϊ����").arg(key));
			break;
		}

		if (key == "������ʱ")
		{
			if (data <= 0.1000000001f)
			{
				setLastError(QString("%1,����С�ڵ���0").arg(key));
				break;
			}
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
	static QStringList explain = { "��λ:MS(����)","��λ:A(����)","��λ:A(����)" };
	return explain;
}

const QString JsonTool::getThresholdConfigDefaultValue(const QString& key)
{
	QString result = "0";
	for (int i = 0; i < m_thresholdConfigKeyList.size(); i++)
	{
		if (m_thresholdConfigKeyList[i] == key)
		{
			result = m_thresholdConfigValueList.at(i);
			break;
		}
	}
	return result;
}

const ImageConfig& JsonTool::getParsedImageConfig()
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
	static QStringList explain0 = { "1����,0����","1����,0����","1����,0����","1����,0����" };
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
	static QStringList explain0 = { "1����,0����","1����,0����","1����,0����","1����,0����" };
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
	static QStringList explain = 
	{ 
		"0����,1����",
		"0����,1����", 
		"0����,1����",
		"0����,1����", 
		"0����,1����",
		"0����,1����",
		"0����,1����",
		"0����,1����",
		"0����,1����",
		"0����,1����",
		"0����,1����" 
	};
	return explain;
}

const QString JsonTool::getEnableConfigDefaultValue(const QString& key)
{
	QString result = "0";
	for (int i = 0; i < m_enableConfigKeyList.size(); i++)
	{
		if (m_enableConfigKeyList[i] == key)
		{
			result = m_enableConfigValueList.at(i);
			break;
		}
	}
	return result;
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
			QStringList data = { "ASCII","ASCR4","INT","USN","BIN","BCD","U08","ASCBCD44" };
			if (!data.contains(newValue))
			{
				setLastError(QString("%1,%2��֧�ֵĸ�ʽ").arg(childKey, newValue));
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
	static QStringList explain = 
	{ 
		"���ݱ�ʶ��",
		"֧�ֱ���[ASCII ASCR4 INT USN BIN BCD U08 ASCBCD44]",
		"��ʶ������" 
	};
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

const QString JsonTool::getOthConfigValue(const QString& key)
{
	return m_othConfigObj.value(key).toString();
}

const int JsonTool::getOthConfigCount()
{
	return m_othConfigObj.count();
}

//void JsonTool::setSkipItem(const SkipItem& item, bool skip)
//{
//	if (item > 0xff)
//		return;
//	m_skipItemVec[item] = skip;
//}

bool JsonTool::getSkipItem(const SkipItem& item)
{
	bool result = false;
	switch (item)
	{
	case SkipItem::SI_JC:
		result = m_defConfig.enable.codeJudge;
		break;
	case SkipItem::SI_QS:
		result = m_defConfig.enable.queryStation;
		break;
	case SkipItem::SI_SN:
		result = m_defConfig.enable.snReadWrite;
		break;
	case SkipItem::SI_DATE:
		result = m_defConfig.enable.dateReadWrite;
		break;
	default:
		break;
	}
	return result;
}

//void JsonTool::deleteSkipSymbol(QString& code)
//{
//	QString temp = code;
//	for (int i = 0; i < temp.length();)
//	{
//		bool exist = false;
//		if (temp.mid(i * 2, 2) == SKIP_QS_SYMBOL)
//		{
//			setSkipItem(SI_QS, true);
//			exist = true;
//		}
//
//		if (temp.mid(i * 2, 2) == SKIP_SN_SYMBOL)
//		{
//			setSkipItem(SI_SN, true);
//			exist = true;
//		}
//
//		if (temp.mid(i * 2, 2) == SKIP_DATE_SYMBOL)
//		{
//			setSkipItem(SI_DATE, true);
//			exist = true;
//		}
//
//		if (exist)
//		{
//			i = 0;
//			temp = temp.remove(temp.mid(i * 2, 2));
//		}
//		else
//			i++;
//	}
//	code = temp;
//}

//bool JsonTool::getSkipCode()
//{
//	return m_defConfig.device.codeJudge == "NULL" &&
//		m_defConfig.device.codeLength.toInt() == 0;
//}

