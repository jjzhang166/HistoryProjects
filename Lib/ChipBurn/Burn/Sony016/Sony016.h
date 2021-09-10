#pragma once
#include "../BaseBurn.h"

#define SONY_CMD_MAX_CNT 16

#define SONY_CMD_FRAME_MAX_LEN 137

struct SonyCmdFrame
{
	uchar total_byte;
	uchar cmd_cnt;
	struct {
		uchar total_cmd_byte;
		uchar cmd_and_status_code;
		uchar data[128];
	} cmd[SONY_CMD_MAX_CNT];
	uchar check_sum;
};

enum SonyCmdList
{
	CMD_LOCK_UNLOCK = 0x00,

	CMD_READ_REG = 0x01,

	CMD_WRITE_REG = 0x02,

	CMD_READ_EEP = 0x03,

	CMD_WRITE_EEP = 0x04,

	CMD_ALL_SAVE_REG_TO_EEP_019 = 0x05,

	CMD_WRITE_CHECK_SUM_016 = 0x08,

	CMD_ERASE_EEP_019 = 0x08,
};

enum SonyStatusCode
{
	SC016_SUCCESS = 0x01,

	SC016_ERR_EFFI_NUM = 0xF0,

	SC016_ERR_CMD_NUM_NOT_EXIST = 0xF1,

	SC016_ERR_CAT_NUM = 0xF2,

	SC016_ERR_ADDR_OFFSET = 0xF3,

	SC016_ERR_ACCESS = 0xF4,

	SC016_ERR_CMD_NUM_NOT_MATCH = 0xF5,

	SC016_ERR_CHECK_SUM = 0xF6,

	SC016_ERR_TOTAL_BYTE_NUM = 0xF7,

	SC016_ERR_EEP_ACCESS = 0xFA,

	SC016_ERR_COMMUNICATION = 0xFC,
};

class Sony016 : public BaseBurn
{
public:
	Sony016();

	~Sony016();

	bool initialize();

	bool lock();

	bool unlock();

	bool readFile(uchar* data, uint size, int* percent);

	bool writeFile(const uchar* data, uint size, int* percent);

	bool readEeprom(uint address, uchar* data, ushort size);

	bool writeEeprom(uint address, const uchar* data, ushort size);
protected:
	bool readData(uint address, uchar* data, ushort size);

	bool writeData(uint address, const uchar* data, ushort size);

	/*
	* @packFrame,���֡
	* @param1,֡�ṹ��
	* @param2,���ݻ�����
	* @return,bool
	*/
	bool packFrame(SonyCmdFrame frame, uchar* data);

	/*
	* @unpackFrame,���֡
	* @param1,֡�ṹ��
	* @param2,���ݻ�����
	* @param3,���ݻ�������С
	* @return,bool
	*/
	bool unpackFrame(SonyCmdFrame* frame, const uchar* data, uchar size);

	/*
	* @readFrame,��ȡ֡
	* @param1,����֡
	* @param2,��ȡ��С
	* @param3,�ӵ�ַ
	* @return,bool
	*/
	bool readFrame(SonyCmdFrame* frame, uchar size);

	/*
	* @writeFrame,д��֡
	* @param1,����֡
	* @param2,�ӵ�ַ
	* @return,bool
	*/
	bool writeFrame(SonyCmdFrame frame);

	/*
	* @readRegister,���Ĵ���
	* @param1,�Ĵ�������
	* @param2,ƫ�Ƶ�ַ
	* @param3,��ȡ�Ļ�����
	* @param4,��ȡ�Ļ�������С
	* @return,bool
	*/
	bool readRegister(uchar category, ushort addrOffset, uchar* data, uchar dataSize);

	/*
	* @writeRegister,д�Ĵ���
	* @param1,�Ĵ�������
	* @param2,ƫ�Ƶ�ַ
	* @param3,д��Ļ�����
	* @param4,д��Ļ�������С
	* @return,bool
	*/
	bool writeRegister(uchar category, ushort addrOffset, const uchar* data, uchar dataSize);

	/*
	* @tempInit,��ʱ��ʼ��
	* @notice,���������;δ֪
	* @return,bool
	*/
	bool tempInit();

	/*
	* @tempUnint,��ʱδ��ʼ��
	* @notice,���������;δ֪
	* @return,bool
	*/
	bool tempUnint();

	/*
	* @setErrorInfo,���ô�����Ϣ
	* @param1,������
	* @return,void
	*/
	void setErrorInfo(uchar code);

	int m_cmdAddrLen = 2;
private:
	bool protect();

	bool unprotect();

	bool convertToAddress();

	bool changeFlashStatus();
};

