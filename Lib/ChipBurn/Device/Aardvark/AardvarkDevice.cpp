#include "AardvarkDevice.h"

AardvarkDevice::AardvarkDevice()
{

}

AardvarkDevice::~AardvarkDevice()
{
	close();
}

bool AardvarkDevice::open(int port, int bitrate, int timeout)
{
	bool result = false;
	do
	{
		Aardvark handle = aa_open(port);
		if (handle <= 0)
		{
			setLastError(QString().sprintf("����¼��ͨ��%dʧ��", port));
			break;
		}

		aa_configure(handle, AA_CONFIG_SPI_I2C);

		aa_i2c_pullup(handle, AA_I2C_PULLUP_BOTH);

		aa_target_power(handle, AA_TARGET_POWER_BOTH);

		aa_i2c_slave_disable(handle);

		if (bitrate != aa_i2c_bitrate(handle, bitrate))
		{
			setLastError("���ò����ʴ���");
			aa_close(handle);
			break;
		}

		if (timeout != aa_i2c_bus_timeout(handle, timeout))
		{
			setLastError("���ó�ʱ����");
			aa_close(handle);
			break;
		}

		m_handle = handle;
		m_port = port;
		m_open = true;
		result = true;
	} while (false);
	return result;
}

bool AardvarkDevice::close()
{
	bool result = false;
	do
	{
		if (m_open && (aa_close(m_handle) != 1))
		{
			setLastError("�ر���¼��ʧ��");
			break;
		}

		m_open = false;
		m_handle = 0;
		m_port = -1;
		result = true;
	} while (false);
	return result;
}

int AardvarkDevice::readData(uchar* data, ushort size, ushort slave)
{
	int result = aa_i2c_read(m_handle, slave, AA_I2C_NO_FLAGS, size, data);
	if (result < 0)
	{
		setLastError(QString().sprintf("��ȡʧ��,%s", aa_status_string(result)));
	}
	else if (result == 0)
	{
		setLastError("��ȡʧ��,�ӵ�ַ����ȷ���������");
	}
	else if (result != size)
	{
		setLastError(QString().sprintf("��ȡ��ʧ%d���ֽ�,����岻�ȶ�������̫��", size - result));
	}
	return result;
}

int AardvarkDevice::writeData(const uchar* data, ushort size, ushort slave)
{
	int result = aa_i2c_write(m_handle, slave, AA_I2C_NO_FLAGS, size, data);
	if (result < 0)
	{
		setLastError(QString().sprintf("д��ʧ��,%s", aa_status_string(result)));
	}
	else if (result == 0)
	{
		setLastError("д��ʧ��,�ӵ�ַ����ȷ���������");
	}
	else if (result != size)
	{
		setLastError(QString().sprintf("д�붪ʧ%d���ֽ�,����岻�ȶ����ٶ�̫��", size - result));
	}
	return result;
}
