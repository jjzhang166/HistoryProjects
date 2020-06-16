#ifndef DVR_NETWORK_PROTOCOL
#define DVR_NETWORK_PROTOCOL
/*
byte	0		1		2		3		4		5		6		7		8		��		n - 1		n		n + 1		n + 2		n + 3
name	EE	    AA	    LEN								CMD		SUB		DATA						CRC32
		����ͷ			���ݳ��� = CMD + SUB + DATA		����	������	����						У�� = CRC32(LEN + CMD + SUB + DATA)



APP��SoC��ʹ����ͬ�Ĵ����Э�飬���а�����

HDR	����ͷ	�̶�ΪEE��AA
LEN	���ݳ���	ָ����� + ������� + �����ݡ��ĳ���
CMD	����	���[3 - �����Э��]
SUB	������	���[3 - �����Э��]
DATA	����	���[3 - �����Э��]������Ϊ��
CRC32	У����	�����ȡ� + ����� + ������� + �����ݡ���CRC32ֵ

SOCKETͨѶ�˿ں�		2000
*/
#include <iostream>

struct Connecting {
	enum {
		CMD,
	};
	enum {
		HANDSHAKE,
		PAIRING_REQS,
		PAIRING_CHECK,
		HEARTBEAT = 0x10,
		AP_CONNECT_REQS = 0x20,
	};
}connecting_t;

struct UiControl {
	unsigned char cmd;
	enum {
		UI_REQ_PREVIEW,
		UI_REQ_FILES,
		UI_REQ_CONFIG,
		UI_REQ_PLAYBACK,
	};
}uiControl_t;

struct FastControl {
	unsigned char cmd;
	enum {
		FAST_CYCLE_RECORD,
		FAST_EMERGE = 0x10,
		FAST_PHOTOGRAPHY = 0x11,
	};
}fastControl_t;
class DvrNetworkProtocol
{
private:

protected:

public:
	DvrNetworkProtocol();
	~DvrNetworkProtocol();
	void pack();
};
#endif //!DVR_NETWORK_PROTOCOL
