#include "JsonTool.h"

JsonTool* JsonTool::m_self = nullptr;

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

bool JsonTool::initInstance(bool update, const QString& name)
{
	bool result = false;
	do 
	{
		QString&& binPath = "./Config/Bin";

		if (!QDir(binPath).exists())
		{
			QDir dir;
			if (!dir.mkpath(binPath))
			{
				setLastError(QString("����%1�ļ���ʧ��").arg(binPath));
				break;
			}
		}

		QString&& fileName = QString("./Config/%1").arg(name);
		if (!QFileInfo(fileName).exists() || update)
		{
			if (update ? !updateJsonFile(fileName) : !writeJsonFile(fileName))
			{
				break;
			}
		}

		if (!readJsonFile(fileName))
		{
			break;
		}
		result = true;
	} while (false);
	return result;
}

const QString& JsonTool::getLastError()
{
	return m_lastError;
}

const QStringList& JsonTool::getAllMainKey()
{
	static QStringList mainKey({ "�豸����","Ӳ������","�̵�������","��ֵ����","�û�����" ,"�ļ�����" });
	return mainKey;
}

const QString& JsonTool::getBurnModeTips()
{
	static QString modeKey = "0.ATC_016_SET 0x30\n1.CTC_016_SET 0x30\n2.CTC_019_SET 0x34,0x36\n3.EEP_AXS340 0x90\n4.FLASH_AXS340 0x90\n"
		"5.NET_AXS340\n6.CTC_CHANGAN_IMS 0x34\n7.EEP_GEELY_BX11 0x90\n8.CTC_EP30TAP_DMS 0x34\n9.ATC_BYD_OV7958 0x80";
	return modeKey;
}

const QString& JsonTool::getFilePathTips()
{
	static QString pathKey = "����·��:\n��:Config\\Bin\\XXX.YYY\n����\"Config\\Bin\\\"����Ǳ����ļ�·��,\n\"XXX\"Ϊ�ļ���,\".YYY\"Ϊ��׺��.\n\n"
		"����·��:\n��:\\\\192.168.2.2\\Bin\\XXX.YYY\n����\"\\\\192.168.2.2\\Bin\\\"����������ļ�·��,\n\"XXX\"Ϊ�ļ���,\".YYY\"Ϊ��׺��.";
	return pathKey;
}

/************************************************************************/
/* DeviceConfig                                                         */
/************************************************************************/
const QJsonObject& JsonTool::getDeviceConfigObj()
{
	return m_deviceConfigObj;
}

const QString JsonTool::getDeviceConfigValue(const QString& key)
{
	return m_deviceConfigObj[key].toString();
}

bool JsonTool::setDeviceConfigValue(const QString& key,const QString& value)
{
	bool result = false, convert = true;
	do 
	{
		if (key == "��Դ����")
		{
			if (value != IT6302 && value != IT6832)
			{
				setLastError(QString("%1,��֧��%2��%3").arg(key, IT6302, IT6832));
				break;
			}
		}
		else
		{
			if (value != "0" && value != "1")
			{
				setLastError(QString("%1,ֻ��Ϊ0����1").arg(key));
				break;
			}
		}

		if (!m_deviceConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}

		m_deviceConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getDeviceConfigKeyList()
{
	return m_deviceConfigKeyList;
}

const QStringList& JsonTool::getDeviceConfigValueList()
{
	return m_deviceConfigValueList;
}

const int JsonTool::getDeviceConfigCount()
{
	return m_deviceConfigKeyList.count();
}

DeviceConfig* JsonTool::getParsedDeviceConfig()
{
	return &m_deviceConfig;
}

const QStringList& JsonTool::getDeviceConfigExplain()
{
	static QStringList explain({ "��֧��IT6832,IT6302","0����,1����","0����,1����","0����,1����","0����,1����",
		"0����,1����,��֧��019����ͷ��¼,ʵ���Խ׶�" });
	return explain;
}

void JsonTool::printDeviceConfigData()
{
	QStringList keys = m_deviceConfigObj.keys();
	for (int i = 0; i < m_deviceConfigObj.count(); i++)
	{
		qDebug() << keys[i] << " " << m_deviceConfigObj[keys[i]].toString() << endl;
	}
}

void JsonTool::printDeviceConfigData(const DeviceConfig& deviceConfig)
{
	//qDebug() << "��Դ����:" << deviceConfig.powerType << endl;
	//qDebug() << "��Դ������:" << deviceConfig.powerBaud << endl;
	//qDebug() << "��Դͨ����:" << deviceConfig.powerChannel << endl;
	//qDebug() << "��Դ����:" << deviceConfig.powerCurrent << endl;
	//qDebug() << "��Դ����:" << deviceConfig.currentLimit << endl;
	//qDebug() << "��Դ��ʱ:" << deviceConfig.powerDelay << endl;
	//qDebug() << "��Դ״̬:" << deviceConfig.powerEnable << endl;
	//qDebug() << "��Դ���ں�:" << deviceConfig.powerPort << endl;
	//qDebug() << "��Դ��ѹ:" << deviceConfig.powerVoltage << endl;
	//qDebug() << "�̵���������:" << deviceConfig.relayBaud << endl;
	//qDebug() << "�̵�����ʱ:" << deviceConfig.relayDelay << endl;
	//qDebug() << "�̵������ں�:" << deviceConfig.relayPort << endl;
	//qDebug() << "�̵���״̬:" << deviceConfig.relayEnable << endl;
	//qDebug() << "�����ظ�:" << deviceConfig.codeRepeat << endl;
}

/************************************************************************/
/* HardwareConfig                                                       */
/************************************************************************/
const int JsonTool::getHardwareConfigCount()
{
	return m_hardwareConfigKeyList.count();
}

const QString JsonTool::getHardwareConfigValue(const QString& key)
{
	return m_hardwareConfigObj[key].toString();
}

const QStringList& JsonTool::getHardwareConfigKeyList()
{
	return m_hardwareConfigKeyList;
}

const QStringList& JsonTool::getHardwareConfigValueList()
{
	return m_hardwareConfigValueList;
}

const QStringList& JsonTool::getHardwareConfigExplain()
{
	static QStringList explain({ "�豸���ں�","IT6302,9600;IT6832,19200",
		"IT6302,3ͨ��;IT6832,1ͨ��","��Դ���µ���ʱ,��λ(ms)","�豸���ں�","Ĭ��19200","�̵����պ���ʱ,��λ(ms)" });
	return explain;
}

bool JsonTool::setHardwareConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toInt(&convert);
		if (!convert)
		{
			setLastError(QString("%1,ֻ��Ϊ����").arg(key));
			break;
		}

		if (key == "��Դͨ����")
		{
			if (value.toInt() > 2 || value.toInt() < 1)
			{
				setLastError(QString("%1,��Χ��֧��1~2").arg(key));
				break;
			}
		}

		if (!m_hardwareConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}
		
		m_hardwareConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

HardwareConfig* JsonTool::getParsedHardwareConfig()
{
	return &m_hardwareConfig;
}

const int JsonTool::getThresholdConfigCount()
{
	return m_thresholdConfigKeyList.count();
}

const QString JsonTool::getThresholdConfigValue(const QString& key)
{
	return m_thresholdConfigObj[key].toString();
}

const QStringList& JsonTool::getThresholdConfigKeyList()
{
	return m_thresholdConfigKeyList;
}

const QStringList& JsonTool::getThresholdConfigValueList()
{
	return m_thresholdConfigValueList;
}

bool JsonTool::setThresholdConfigValue(const QString& key, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		value.toFloat(&convert);
		if (!convert)
		{
			setLastError(QString("%1,ֻ��Ϊ����").arg(key));
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
	static QStringList explain({ "��λ:��(V)","��λ:����(A)" });
	return explain;
}

ThresholdConfig* JsonTool::getParsedThresholdConfig()
{
	return &m_thresholdConfig;
}

/************************************************************************/
/* RelayConfig                                                          */
/************************************************************************/
const QJsonObject& JsonTool::getRelayConfigObj()
{
	return m_relayConfigObj;
}

const QString JsonTool::getRelayConfigValue(const QString& key)
{
	return m_relayConfigObj[key].toString();
}

bool JsonTool::setRelayConfigValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		if (value != "0" && value != "1")
		{
			setLastError(QString("%1,����Ϊ0����1").arg(key));
			break;
		}

		if (!m_relayConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}
		m_relayConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}


const QStringList& JsonTool::getRelayConfigKeyList()
{
	return m_relayConfigKeyList;
}

const QStringList& JsonTool::getRelayConfigValueList()
{
	return m_relayConfigValueList;
}


const int JsonTool::getRelayConfigCount()
{
	return	m_relayConfigKeyList.count();
}

const QStringList& JsonTool::getRelayConfigExplain()
{
	static QStringList explain { "0����,1����","0����,1����", "0����,1����", "0����,1����", "0����,1����", "0����,1����",
	"0����,1����", "0����,1����", "0����,1����", "0����,1����", "0����,1����", "0����,1����", "0����,1����",
	"0����,1����", "0����,1����" };
	return explain;
}


RelayConfig* JsonTool::getParsedRelayConfig()
{
	return m_relayConfig;
}

/************************************************************************/
/* UserConfig                                                           */
/************************************************************************/
const QStringList& JsonTool::getUserConfigKeyList()
{
	return m_userConfigKeyList;
}

const QStringList& JsonTool::getUserConfigValueList()
{
	return m_userConfigValueList;
}

const QString JsonTool::getUserConfigValue(const QString& key)
{
	return m_userConfigObj[key].toString();
}

const int JsonTool::getUserConfigCount()
{
	return m_userConfigKeyList.count();
}

bool JsonTool::setUserConfigValue(const QString& key, const QString& value)
{
	bool result = false;
	do 
	{
		if (!m_userConfigObj.contains(key))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(key));
			break;
		}

		m_userConfigObj[key] = value;
		result = true;
	} while (false);
	return result;
}

const QStringList& JsonTool::getUserConfigExplain()
{
	static QStringList explain({ "��֤�˺�","��֤����" });
	return explain;
}

/************************************************************************/
/* FileConfig                                                           */
/************************************************************************/
const int JsonTool::getFileConfigCount()
{
	return m_fileConfigObj.count();
}

QJsonObject& JsonTool::getFileConfigObj()
{
	return m_fileConfigObj;
}

const QStringList JsonTool::getParentFileConfigKeyList()
{
	return m_fileConfigObj.keys();
}

const QStringList& JsonTool::getFileConfigKeyList()
{
	return m_fileConfigKeyList;
}

const QStringList& JsonTool::getFileConfigValueList()
{
	return m_fileConfigValueList;
}

const QString JsonTool::getFileConfigValue(const QString& parentKey, const QString& childKey)
{
	return m_fileConfigObj.value(parentKey).toObject()[childKey].toString();
}

bool JsonTool::setFileConfigKey(const QString& oldKey, const QString& newKey)
{
	bool result = false;
	do 
	{
		if (newKey.mid(0, 9) == "[��༭�ڵ�����]")
		{
			result = true;
			break;
		}

		if (newKey.isEmpty())
		{
			setLastError("����������Ϊ��");
			break;
		}

		if (!m_fileConfigObj.contains(oldKey))
		{
			setLastError(QString("%1,�Ƿ��ļ�").arg(oldKey));
			break;
		}
		QJsonObject object = m_fileConfigObj[oldKey].toObject();
		m_fileConfigObj.remove(oldKey);
		m_fileConfigObj.insert(newKey, object);
		result = true;
	} while (false);
	return result;
}

bool JsonTool::setFileConfigValue(const QString& parentKey, const QString& childKey, const QString& value)
{
	bool result = false, convert = false;
	do 
	{
		if (value == "[�������༭]")
		{
			result = true;
			break;
		}

		if (value.isEmpty())
		{
			setLastError(QString("%1,������Ϊ��").arg(childKey));
			break;
		}

		if (!m_fileConfigObj.contains(parentKey))
		{
			setLastError(QString("�Ƿ��ĸ���,%1").arg(parentKey));
			break;
		}

		if (childKey != "�ļ�·��" && childKey != "�������")
		{
			if (childKey == "��д��ַ" || childKey == "У���ַ")
			{
				if (!(value.contains("0x") || value.contains("0X")))
				{
					setLastError(QString("%1��ʽ����,����:��ʽΪ0x30").arg(childKey));
					break;
				}

				value.toInt(&convert, 16);
				if (!convert)
				{
					setLastError(QString("%1��ʽ����,����:��ʽΪ0x30").arg(childKey));
					break;
				}
			}
			else
			{
				int number = value.toInt(&convert);
				if (!convert)
				{
					setLastError(QString("%1��ʽ����,����ֻ��Ϊ����").arg(childKey));
					break;
				}

				if (childKey == "����ģʽ" || childKey == "У��ϵ�")
				{
					if (number < 0 || number > 1)
					{
						setLastError(QString("%1,ֻ��Ϊ0����1").arg(childKey));
						break;
					}
				}
			}
		}
		else
		{
			if (childKey == "�ļ�·��")
			{
				if (!QFileInfo(value).isFile())
				{
					setLastError(QString("%1,��Ч���ļ�·��").arg(childKey));
					break;
				}
			}
		}

		QJsonObject object = m_fileConfigObj[parentKey].toObject();
		if (!object.contains(childKey))
		{
			setLastError(QString("�Ƿ����Ӽ�,%1").arg(childKey));
			break;
		}

		object[childKey] = value;
		m_fileConfigObj.insert(parentKey, object);
		result = true;
	} while (false);
	return result;
}

bool JsonTool::getParsedFileConfig(const QString& nodeName)
{
	bool result = false, success = false;

	do 
	{
		if (nodeName == SUPER_PASSWORD)
		{
			result = true;
			break;
		}

		for (int i = 0; i < getFileConfigCount(); i++)
		{
			if (m_allFileConfig[i].nodeName == nodeName)
			{
				m_fileConfig = m_allFileConfig[i];

				if (!m_fileConfig.valid)
				{
					setLastError(QString("[%1]�ڵ���Ч,\n��������ģʽ���б༭").arg(nodeName));
					break;
				}

				/*�˴�������ʹ��memcpy,�������ǳ����,���±���*/
				//memcpy(&m_fileConfig, &m_allFileConfig[i], sizeof(FileConfig));

				QFile file(m_fileConfig.path);
				if (!file.open(QFile::ReadOnly))
				{
					m_fileConfig.valid = false;
					setLastError(QString("[%1]\n��%2�ļ�ʧ��,%3").arg(nodeName, m_fileConfig.path, file.errorString()));
					break;
				}

				m_fileConfig.data = file.readAll();
				if (m_fileConfig.data.size() != file.size())
				{
					m_fileConfig.valid = false;
					setLastError(QString("[%1]\n��ȡ%2У�����").arg(nodeName, m_fileConfig.path));
					file.close();
					break;
				}
				success = true;
				file.close();
				break;
			}

			if (i == getFileConfigCount() - 1)
			{
				setLastError(QString("δ�ҵ�[%1]�ڵ�,\n��������ģʽ�������").arg(nodeName));
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

const QStringList& JsonTool::getFileConfigExplain()
{
	static QStringList explain({ "�������ͣ���˴�","�����볤��","�ж�ǰN���ַ���,����ǿ�дNULL","slave��ַ����,Ĭ��7λ","��дslave��ַ","У��slave��ַ",
		"�������ͣ���˴�","0����,1����;������õײ����ʱ��ʹ��,Ĭ��0","�ײ���ȡ��ʱ(��λ:ms),Ĭ��0","�ײ��д����ʱ(��λ:ms),Ĭ��0",
		"Ӧ�ò��ȡ��ʱ(��λ:ms),Ĭ��0","Ӧ�ò�д����ʱ(��λ:ms),Ĭ��0","ģ�������ɹ�ʱ��(��λ:ms),Ĭ��3000ms",
		"��¼������,100kHz~800kHz,Ĭ��400kHz","��¼����ʱ,Ĭ��150ms","0����,1����;У�����������ϵ�,Ĭ��1" });
	return explain;
}

FileConfig* JsonTool::getParsedFileConfig()
{
	return &m_fileConfig;
}

FileConfig* JsonTool::getParsedAllFileConfig()
{
	return m_allFileConfig;
}

void JsonTool::printFileConfig()
{
	qDebug() << "�ļ�·��:" << m_fileConfig.path << endl;
	qDebug() << "���볤��:" << m_fileConfig.codeLength << endl;
	qDebug() << "�������:" << m_fileConfig.codeRule << endl;
	qDebug() << "��ַ����:" << m_fileConfig.addressLength << endl;
	qDebug("��д��ַ:0x%x\n", m_fileConfig.dataSlave);
	qDebug("У���ַ:0x%x\n", m_fileConfig.checkSlave);
	qDebug() << "��¼ģʽ:" << m_fileConfig.burnMode << endl;
	qDebug() << "����ģʽ:" << m_fileConfig.speedMode << endl;
	qDebug() << "�ײ��ȡ:" << m_fileConfig.libReadDelay << endl;
	qDebug() << "�ײ�д��:" << m_fileConfig.libWriteDelay << endl;
	qDebug() << "Ӧ���ȡ:" << m_fileConfig.appReadDelay << endl;
	qDebug() << "Ӧ��д��:" << m_fileConfig.appWriteDelay << endl;
	qDebug() << "������ʱ:" << m_fileConfig.rebootDelay << endl;
	qDebug() << "�豸����:" << m_fileConfig.deviceSpeed << endl;
	qDebug() << "�豸��ʱ:" << m_fileConfig.deviceTimeout << endl;
	qDebug() << endl << endl;
}

/************************************************************************/
/* protected function                                                   */
/************************************************************************/
void JsonTool::setLastError(const QString& err)
{
#ifdef QT_DEBUG
	qDebug() << __FUNCTION__ << " " << err << endl;
#endif
	m_lastError = err;
}

bool JsonTool::readJsonFile(const QString& name)
{
	bool result = false, success = true;
	do
	{
		QFile file(name);
		if (!file.open(QIODevice::ReadOnly))
		{
			setLastError(file.errorString());
			break;
		}

		QByteArray bytes = file.readAll();
		file.close();
		
		QJsonParseError jsonError;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(bytes, &jsonError));
		if (jsonError.error != QJsonParseError::NoError)
		{
			setLastError(jsonError.errorString());
			break;
		}

		QJsonObject rootObj = jsonDoc.object();

		QList<QJsonObject*> objectList = { 
			&m_deviceConfigObj,
			&m_hardwareConfigObj,
			&m_relayConfigObj,
			&m_thresholdConfigObj,
			&m_userConfigObj,
			&m_fileConfigObj
		};

		bool (JsonTool:: * parseFnc[])() = { 
			&JsonTool::parseDeviceConfigData,
			&JsonTool::parseHardwareConfigData,
			&JsonTool::parseRelayConfigData,
			&JsonTool::parseThresholdConfigData,
			&JsonTool::parseUserConfigData,
			&JsonTool::parseFileConfigData
		};


		for (int i = 0; i < getAllMainKey().count(); i++)
		{
			if (rootObj.contains(getAllMainKey()[i]))
			{
				*objectList[i] = rootObj.value(getAllMainKey()[i]).toObject();
				if (!(this->*parseFnc[i])())
				{
					success = false;
					setLastError(QString("[%1]����ʧ��").arg(getAllMainKey()[i]));
					break;
				}
			}
			else
			{
				success = false;
				setLastError(QString("[%1]�ڵ㶪ʧ").arg(getAllMainKey()[i]));
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

bool JsonTool::writeJsonFile(const QString& name)
{
	QJsonObject rootObj, deviceConfigObj, hardwareConfigObj,thresholdConfigObj,relayConfigObj;
	QJsonObject parentBinFileConfigObj, childBinFileConfigObj, userConfigObj;

	/*д�豸����*/
	for (int i = 0; i < getDeviceConfigCount(); i++)
	{
		deviceConfigObj.insert(getDeviceConfigKeyList()[i], getDeviceConfigValueList()[i]);
	}
	
	/*дӲ������*/
	for (int i = 0; i < getHardwareConfigCount(); i++)
	{
		hardwareConfigObj.insert(getHardwareConfigKeyList()[i], getHardwareConfigValueList()[i]);
	}

	/*д�̵�������*/
	for (int i = 0; i < getRelayConfigCount(); i++)
	{
		relayConfigObj.insert(getRelayConfigKeyList()[i], getRelayConfigValueList()[i]);
	}
	
	/*д��ֵ����*/
	for (int i = 0; i < getThresholdConfigCount(); i++)
	{
		thresholdConfigObj.insert(getThresholdConfigKeyList()[i], getThresholdConfigValueList()[i]);
	}

	/*д�ļ�������Ϣ*/
	for (int i = 0; i < m_fileConfigKeyList.count(); i++)
	{
		childBinFileConfigObj.insert(m_fileConfigKeyList[i], m_fileConfigValueList[i]);
	}

	parentBinFileConfigObj.insert("A39-HSYNC", childBinFileConfigObj);

	/*д�û�������Ϣ*/
	for (int i = 0; i < getUserConfigCount(); i++)
	{
		userConfigObj.insert(getUserConfigKeyList()[i], getUserConfigValueList()[i]);
	}

	rootObj.insert("�豸����", deviceConfigObj);
	
	rootObj.insert("Ӳ������", hardwareConfigObj);

	rootObj.insert("�̵�������", relayConfigObj);
	
	rootObj.insert("��ֵ����", thresholdConfigObj);

	rootObj.insert("�ļ�����", parentBinFileConfigObj);

	rootObj.insert("�û�����", userConfigObj);

	bool result = false;
	do
	{
		QJsonDocument doc(rootObj);
		QByteArray bytes = doc.toJson();
		QFile file(name);
		if (!file.open(QIODevice::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}
		if (bytes.count() != file.write(bytes))
		{
			file.close();
			setLastError("д�������ļ�У��ʧ��");
			break;
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::updateJsonFile(const QString& name)
{
	bool result = false;
	
	QJsonObject rootObj;
	
	rootObj.insert("�豸����", m_deviceConfigObj);

	rootObj.insert("Ӳ������", m_hardwareConfigObj);

	rootObj.insert("�̵�������", m_relayConfigObj);

	rootObj.insert("��ֵ����", m_thresholdConfigObj);

	rootObj.insert("�ļ�����", m_fileConfigObj);

	rootObj.insert("�û�����", m_userConfigObj);

	do
	{
		QJsonDocument doc(rootObj);
		QByteArray bytes = doc.toJson();
		QFile file(name);
		if (!file.open(QIODevice::WriteOnly))
		{
			setLastError(file.errorString());
			break;
		}

		if (bytes.count() != file.write(bytes))
		{
			file.close();
			setLastError("д�������ļ�У��ʧ��");
			break;
		}
		file.close();
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseDeviceConfigData()
{
	bool result = false, success = true;
	do
	{
		QString* valuePtr = reinterpret_cast<QString*>(&m_deviceConfig);
		for (int i = 0; i < getDeviceConfigCount(); i++, valuePtr++)
		{
			*valuePtr = getDeviceConfigValue(getDeviceConfigKeyList()[i]);
			if (valuePtr->isEmpty())
			{
				*valuePtr = "���ݴ���";
				success = false;
			}
		}

		if (!success)
		{
			setLastError("�����豸�������ݴ���");
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseHardwareConfigData()
{
	bool result = false, convert = false, success = true;
	do 
	{
		int* valuePtr = reinterpret_cast<int*>(&m_hardwareConfig);
		for (int i = 0; i < getHardwareConfigCount(); i++, valuePtr++)
		{
			*valuePtr = getHardwareConfigValue(getHardwareConfigKeyList()[i]).toInt(&convert);
			if (!convert)
			{
				success = false;
			}
		}

		if (!success)
		{
			setLastError("����Ӳ����������ʧ��");
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseThresholdConfigData()
{
	bool result = false, convert = false, success = true;
	do 
	{
		float* valuePtr = reinterpret_cast<float*>(&m_thresholdConfig);
		for (int i = 0; i < getThresholdConfigCount(); i++, valuePtr++)
		{
			*valuePtr = getThresholdConfigValue(getThresholdConfigKeyList()[i]).toFloat(&convert);
			if (!convert)
			{
				success = false;
			}
		}

		if (!success)
		{
			setLastError("������ֵ��������ʧ��");
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseRelayConfigData()
{
	bool result = true, convert = false, success = true;;
	do
	{
		for (int i = 0; i < getRelayConfigCount(); i++)
		{
			m_relayConfig[i].port = i;
			m_relayConfig[i].enable = getRelayConfigValue(getRelayConfigKeyList()[i]).toInt(&convert);
			if (!convert)
			{
				success = false;
			}
		}

		if (!success)
		{
			setLastError("�����̵�����������ʧ��");
			break;
		}
		result = true;
	} while (false);
	return result;
}

bool JsonTool::parseFileConfigData()
{
	bool resutl = false, valid = true, convert = false;
	do 
	{
		SAFE_DELETE_A(m_allFileConfig);

		m_allFileConfig = new(std::nothrow) FileConfig[getFileConfigCount()];
		if (!m_allFileConfig)
		{
			setLastError("�ļ����÷����ڴ�ʧ��");
			break;
		}
		
		for (int i = 0; i < getFileConfigCount(); i++)
		{
			valid = true;

			m_allFileConfig[i].path = getFileConfigValue(getFileConfigObj().keys()[i], "�ļ�·��");

			m_allFileConfig[i].nodeName = getFileConfigObj().keys()[i];
			
			m_allFileConfig[i].codeLength = getFileConfigValue(getFileConfigObj().keys()[i], "���볤��").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].codeRule = getFileConfigValue(getFileConfigObj().keys()[i], "�������");
			
			m_allFileConfig[i].addressLength = getFileConfigValue(getFileConfigObj().keys()[i], "��ַ����").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].dataSlave = getFileConfigValue(getFileConfigObj().keys()[i], "��д��ַ")
				.toInt(&convert, 16) >> (8 - m_allFileConfig[i].addressLength);
			if (!convert) valid = false;

			m_allFileConfig[i].checkSlave = getFileConfigValue(getFileConfigObj().keys()[i], "У���ַ")
				.toInt(&convert, 16) >> (8 - m_allFileConfig[i].addressLength);
			if (!convert) valid = false;

			m_allFileConfig[i].burnMode = getFileConfigValue(getFileConfigObj().keys()[i], "��¼ģʽ").toInt(&convert);
			if (!convert) valid = false;
			
			m_allFileConfig[i].speedMode = getFileConfigValue(getFileConfigObj().keys()[i], "����ģʽ").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].libReadDelay = getFileConfigValue(getFileConfigObj().keys()[i], "�ײ��ȡ").toInt(&convert);
			if (!convert) valid = false;
			
			m_allFileConfig[i].libWriteDelay = getFileConfigValue(getFileConfigObj().keys()[i], "�ײ�д��").toInt(&convert);
			if (!convert) valid = false;
			
			m_allFileConfig[i].appReadDelay = getFileConfigValue(getFileConfigObj().keys()[i], "Ӧ���ȡ").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].appWriteDelay = getFileConfigValue(getFileConfigObj().keys()[i], "Ӧ��д��").toInt(&convert);
			if (!convert) valid = false;
			
			m_allFileConfig[i].rebootDelay = getFileConfigValue(getFileConfigObj().keys()[i], "������ʱ").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].deviceSpeed = getFileConfigValue(getFileConfigObj().keys()[i], "�豸����").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].deviceTimeout = getFileConfigValue(getFileConfigObj().keys()[i], "�豸��ʱ").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].checkOutage = getFileConfigValue(getFileConfigObj().keys()[i], "У��ϵ�").toInt(&convert);
			if (!convert) valid = false;

			m_allFileConfig[i].valid = valid;
		}
		resutl = true;
	} while (false);
	return resutl;
}
