
//�ӿڿ����Ͷ���
#define VCI_PCI5121		1
#define VCI_PCI9810		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_PCI9820		5
#define VCI_CAN232		6

//�������÷���״ֵ̬
#define	STATUS_OK 1
#define STATUS_ERR 0

//CAN������
#define	ERR_CAN_OVERFLOW			0x0001	//CAN�������ڲ�FIFO���
#define	ERR_CAN_ERRALARM			0x0002	//CAN���������󱨾�
#define	ERR_CAN_PASSIVE				0x0004	//CAN��������������
#define	ERR_CAN_LOSE				0x0008	//CAN�������ٲö�ʧ
#define	ERR_CAN_BUSERR				0x0010	//CAN���������ߴ���

//ͨ�ô�����
#define	ERR_DEVICEOPENED			0x0100	//�豸�Ѿ���
#define	ERR_DEVICEOPEN				0x0200	//���豸����
#define	ERR_DEVICENOTOPEN			0x0400	//�豸û�д�
#define	ERR_BUFFEROVERFLOW			0x0800	//���������
#define	ERR_DEVICENOTEXIST			0x1000	//���豸������
#define	ERR_LOADKERNELDLL			0x2000	//װ�ض�̬��ʧ��
#define ERR_CMDFAILED				0x4000	//ִ������ʧ�ܴ�����
#define	ERR_BUFFERCREATE			0x8000	//�ڴ治��


//1.ZLGCANϵ�нӿڿ���Ϣ���������͡�
typedef  struct  _VCI_BOARD_INFO{
		unsigned short	hw_Version;
		unsigned short	fw_Version;
		unsigned short	dr_Version;
		unsigned short	in_Version;
		unsigned short	irq_Num;
		unsigned char	can_Num;
		char	str_Serial_Num[20];
		char	str_hw_Type[40];
		unsigned short	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//2.����CAN��Ϣ֡���������͡�
typedef  struct  _VCI_CAN_OBJ{
	unsigned int	ID;
	unsigned int	TimeStamp;
	unsigned char	TimeFlag;
	unsigned char	SendType;
	unsigned char	RemoteFlag;//�Ƿ���Զ��֡
	unsigned char	ExternFlag;//�Ƿ�����չ֡
	unsigned char	DataLen;
	unsigned char	Data[8];
	unsigned char	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.����CAN������״̬���������͡�
typedef struct _VCI_CAN_STATUS{
	unsigned char	ErrInterrupt;
	unsigned char	regMode;
	unsigned char	regStatus;
	unsigned char	regALCapture;
	unsigned char	regECCapture; 
	unsigned char	regEWLimit;
	unsigned char	regRECounter; 
	unsigned char	regTECounter;
	unsigned long	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//4.���������Ϣ���������͡�
typedef struct _ERR_INFO{
		unsigned int	ErrCode;
		unsigned char	Passive_ErrData[3];
		unsigned char	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//5.�����ʼ��CAN����������
typedef struct _INIT_CONFIG{
	unsigned long	AccCode;
	unsigned long	AccMask;
	unsigned long	Reserved;
	unsigned char	Filter;
	unsigned char	Timing0;	
	unsigned char	Timing1;	
	unsigned char	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

unsigned long __stdcall VCI_OpenDevice(unsigned long DeviceType,unsigned long DeviceInd,unsigned long Reserved);
unsigned long __stdcall VCI_CloseDevice(unsigned long DeviceType,unsigned long DeviceInd);
unsigned long __stdcall VCI_InitCAN(unsigned long DeviceType, unsigned long DeviceInd, unsigned long CANInd, PVCI_INIT_CONFIG pInitConfig);

unsigned long __stdcall VCI_ReadBoardInfo(unsigned long DeviceType,unsigned long DeviceInd,PVCI_BOARD_INFO pInfo);
unsigned long __stdcall VCI_ReadErrInfo(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_ERR_INFO pErrInfo);
unsigned long __stdcall VCI_ReadCANStatus(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_CAN_STATUS pCANStatus);

unsigned long __stdcall VCI_GetReference(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,unsigned long RefType,void* pData);
unsigned long __stdcall VCI_SetReference(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,unsigned long RefType,void* pData);

unsigned long __stdcall VCI_GetReceiveNum(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);
unsigned long __stdcall VCI_ClearBuffer(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);

unsigned long __stdcall VCI_StartCAN(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);
unsigned long __stdcall VCI_ResetCAN(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd);

unsigned long __stdcall VCI_Transmit(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_CAN_OBJ pSend,unsigned long Len);
unsigned long __stdcall VCI_Receive(unsigned long DeviceType,unsigned long DeviceInd,unsigned long CANInd,PVCI_CAN_OBJ pReceive,unsigned long Len,int WaitTime=-1);

