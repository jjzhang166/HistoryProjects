#pragma once
#pragma execution_character_set("utf-8")
/*
* @notice,�������ܻ������������,��Ҫʹ�ñ�׼����п���
*/

#include <QObject>

#include <Windows.h>
#include <vector>
#include <deque>
#include <algorithm>

#include "../Device/Aardvark/AardvarkDevice.h"

#define C_SPRINTF(format, ...) QString().sprintf(format,##__VA_ARGS__)

#define LIB_RUN_BREAK(success,format,...)\
if (success)\
{\
	setLastError(C_SPRINTF(format,##__VA_ARGS__));\
	break;\
}

//#define SLAVE_ADDR_96706	(0x90 >> 1)

#define SLAVE_ADDR_96706    (0x9C >> 1)

#define SLAVE_ADDR_96701	(0x80 >> 1)

//����IMS BX11
#define SLAVE_ADDR_934		(0x60 >> 1)

#define SLAVE_ADDR_914		(0xD0 >> 1)

//#define SLAVE_ADDR_914		(0xC0 >> 1)

/*
* @ChipType,оƬ����
* @notice,��������
* CT + оƬ���� + �洢���� + ��Ŀ����
* CT ����ChipType����д,
* UNIVERSAL_SET ����Ϊͨ�ü���,�ܶ�
* ��Ŀ����һ�����.
*/
enum ChipType
{
	CT_UNKNOWN,

	CT_ASX340_FLASH_UNIVERSAL_SET,

	CT_ASX340_EEPROM_UNIVERSAL_SET,

	CT_ASX340_EEPROM_GEELY_BX11,

	CT_SONY016_FLASH_UNIVERSAL_SET,

	CT_SONY016_EEPROM_UNIVERSAL_SET,

	CT_SONY019_FLASH_UNIVERSAL_SET,

	CT_SONY019_FLASH_CHANGAN_IMS,

	CT_SONY019_FLASH_EP30TAP_DMS,

	CT_OV7958_FLASH_UNIVERSAL_SET,

	CT_OVX1E_FLASH_UNIVERSAL_SET
};

class BaseBurn
{
public:
	BaseBurn();

	virtual ~BaseBurn();

	BaseBurn(const BaseBurn&) = delete;

	BaseBurn& operator=(const BaseBurn&) = delete;

	/*
	* @setDevice,�����豸
	* @param1,�豸
	* @return,void
	*/
	void setDevice(BaseDevice* device);

	/*
	* @setChipType,����оƬ����
	* @param1,оƬ����
	* @return,void
	*/
	void setChipType(ChipType type);

	/*
	* @getType,��ȡоƬ����
	* @return,CamerType
	*/
	ChipType getChipType() const;

	/*
	* @getLastError,��ȡ���մ���
	* @retur,const std::string&
	*/
	const QString& getLastError();

	/*
	* @setSlave,���ôӵ�ַ
	* @param1,�ӵ�ַ
	* @return,void
	*/
	void setSlave(ushort slave);

	/*
	* @getSlave,��ȡ�ӵ�ַ
	* @return,ushort
	*/
	ushort getSlave() const;

	/*
	* @setReadWriteDelay,���ö�д��ʱ
	* @param1,����ʱ
	* @param2,д��ʱ
	* @return,void
	*/
	void setReadWriteDelay(int readDelay, int writeDelay);

	/*
	* @setJump0xFF,��������0xff
	* @param1,�Ƿ�����
	* @return,void
	*/
	void setJump0xFF(int jump);

public:
	/*
	* @initialize,��ʼ��
	* @param1,оƬ����
	* @return,bool
	*/
	virtual bool initialize() = 0;

	/*
	* @readFile,���ļ�
	* @param1,��ȡ�����ݻ�����
	* @param2,��ȡ��С
	* @param3,��ȡ����
	* @return,bool
	*/
	virtual bool readFile(uchar* data, uint size, int* percent) = 0;

	/*
	* @writeFile,д�ļ�
	* @param1,д������ݻ�����
	* @param2,д��Ĵ�С
	* @param3,д�����
	* @return,bool
	*/
	virtual bool writeFile(const uchar* data, uint size, int* percent) = 0;

	/*
	* @readFlash,��ȡ����
	* @param1,��ʼ��ַ
	* @param2,��ȡ�����ݻ�����
	* @param3,��ȡ��С
	* @return,bool
	*/
	virtual bool readFlash(uint address, uchar* data, ushort size);

	/*
	* @writeFlash,д������
	* @param1,��ʼ��ַ
	* @param2,д������ݻ�����
	* @param3,д���С
	* @return,bool
	*/
	virtual bool writeFlash(uint address, const uchar* data, ushort size);

	/*
	* @readEeprom,��ȡ��ɲ���ֻ���洢��
	* @param1,��ʼ��ַ
	* @param2,��ȡ�����ݻ�����
	* @param3,��ȡ��С
	* @return,bool
	*/
	virtual bool readEeprom(uint address, uchar* data, ushort size);

	/*
	* @writeEeprom,д���ɲ���ֻ���洢��
	* @param1,��ʼ��ַ
	* @param2,д������ݻ�����
	* @param3,д���С
	* @return,bool
	*/
	virtual bool writeEeprom(uint address, const uchar* data, ushort size);

	/*
	* @lock,����
	* @return,bool
	*/
	virtual bool lock();

	/*
	* @unlock,����
	* @return,bool
	*/
	virtual bool unlock();

public:
	/*
	* @i2cAddrRead,i2c��ַ��ȡ
	* @param1,��ʼ��ַ
	* @param2,��ַ����
	* @param3,���ݻ�����
	* @param4,��ȡ���ݴ�С
	* @param5,�ӵ�ַ
	* @return,bool
	*/
	bool i2cAddrRead(uint addr, uchar addrLen, uchar* data, ushort dataSize, ushort slave = 0);

	/*
	* @i2cAddrWrite,i2c��ַд��
	* @param1,��ʼ��ַ
	* @param2,��ַ����
	* @param3,���ݻ�����
	* @param4,д�����ݴ�С
	* @param5,�Ƿ�У��
	* @param6,�ӵ�ַ
	* @return,bool
	*/
	bool i2cAddrWrite(uint addr, uchar addrLen, const uchar* data, ushort dataSize, bool check = false, ushort slave = 0);

	/*
	* @i2cAddrWrite,i2c��ַд��
	* @param1,��ʼ��ַ
	* @param2,��ַ����
	* @param3,��ʼ�������б�
	* @param4,�ӵ�ַ
	* @return,bool
	*/
	bool i2cAddrWrite(uint addr, uchar addrLen, const std::initializer_list<uchar>& data, ushort slave = 0);

protected:
	/*
	* @readData,��ȡ����
	* @param1,��ʼ��ַ
	* @param2,��ȡ�����ݻ�����
	* @param3,��ȡ��С
	* @return,bool
	*/
	virtual bool readData(uint address, uchar* data, ushort size) = 0;

	/*
	* @writeData,д������
	* @param1,��ʼ��ַ
	* @param2,д������ݻ�����
	* @param3,д���С
	* @return,bool
	*/
	virtual bool writeData(uint address, const uchar* data, ushort size) = 0;

	/*
	* @setLastError,�������մ���
	* @param1,������Ϣ
	* @return,void
	*/
	void setLastError(const QString& error);

	/*
	* @i2cRead,i2c��
	* @param1,��ȡ������
	* @param2,��ȡ�Ĵ�С
	* @param3,slave��ַ
	* @return,ʵ�ʶ�ȡ�Ĵ�С
	*/
	int i2cRead(uchar* data, ushort size, ushort slave = 0);

	/*
	* @i2cWrite,i2cд
	* @param1,д�������
	* @param2,д��Ĵ�С
	* @param3,slave��ַ
	* @return,ʵ��д��Ĵ�С
	*/
	int i2cWrite(const uchar* data, ushort size, ushort slave = 0);

	/*
	* @i2cWrite,i2cд
	* @param1,д��������б�
	* @param2,�ӵ�ַ
	* @return,ʵ��д��Ĵ�С
	*/
	int i2cWrite(const std::initializer_list<uchar>& data, ushort slave = 0);

	/*
	* @printLog,��ӡ��־
	* @param1,��־����[R,S]
	* @param2,��־����
	* @param3,���ݴ�С
	* @param4,�ӵ�ַ
	* @return,void
	*/
	void printLog(const QString& type, const uchar* data, int size, ushort slave);

	/*
	* @msleep,����
	* @param1,���߶��ٺ���
	* @return,void
	*/
	void msleep(uint ms);

	/*
	* @calculateSector,��������
	* @parma1,���ݻ�����
	* @param2,���ݻ�������С
	* @param3,����
	* @return,std::vector<uint>
	*/
	std::deque<uint> calculateSector(const uchar* buffer, uint size, uint multiple = 256, uchar value = 0xff);

	/*
	* @findNearNumber,Ѱ�����ڵ�ַ
	* @param1,ҪѰ�ҵĵ�ַ
	* @param2,����
	* @param3,�Ƿ񰴽�������
	* @return,�ɹ������ҵ��ĵ�ַ,ʧ��(uint)-1δ�ҵ�
	*/
	uint findNearAddress(uint address, uint multiple, bool lessThan = true);

	/*
	* @getPercent,��ȡ�ٷֱ�
	* @param1,��ǰ
	* @param2,����
	* @return,int
	*/
	int getPercent(uint current, uint total);

protected:

	ChipType m_chipType = CT_UNKNOWN;

	int m_readDelay = 0;

	int m_writeDelay = 0;
private:
	QString m_lastError = "No error";

	ushort m_slave = 0;

	bool m_jump0xff = false;

	BaseDevice* m_device = nullptr;
};