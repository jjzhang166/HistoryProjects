#pragma once
#pragma execution_character_set("utf-8")

#include "CanFactory.h"
#include "CanSender.h"

namespace Can {

	/*
	* @getVersion,��ȡ�汾
	* @return,const char*
	*/
	const char* getVersion();

	/*
	* @getSupportCan,��ȡ֧�ֵ�CAN��
	* @return,QStringList
	*/
	QStringList getSupportCan();

	/*
	* @allocCanTransfer,����CAN����
	* @param1,CAN������
	* @return,CanTransfer*
	*/
	CanTransfer* allocCanTransfer(const QString& cardName);

	/*
	* @freeCanTransfer,�ͷ�CAN����
	* @param1,�ѷ����CanTransfer
	* @notice,���param1,��дnullptr,���ͷ����з����CanTransfer
	* @return,void
	*/
	void freeCanTransfer(CanTransfer* transfer = nullptr);

	/*
	* @getSupportMaxCanTransferCount,��ȡ֧�ֵ����CAN��������
	* @return,int
	*/
	int getSupportMaxCanTransferCount();

	/*
	* @getCurrentUsedCanTransferCount,��ȡ��ǰ��ʹ�õ�CAN��������
	* @return,const int
	*/
	int getCurrentUsedCanTransferCount();

	/*
	* @getCanTransferValue,��ȡCAN����
	* @param1,����,��Χ0~(MAX_CAN_TRANSFER_COUNT-1)
	* @return,CanTransfer*,�ɹ������ѷ���ĵ�ַ,ʧ��nullptr
	*/
	CanTransfer* getCanTransferValue(int index);

	/*
	* @getCanTransferIndex,��ȡCAN��������
	* @param1,�ѷ����CanTransfer
	* @return,const int,�ɹ������ѷ���ĵ�ַ������,ʧ��-1
	*/
	int getCanTransferIndex(const CanTransfer* transfer);

	/*
	* @alloclCanSender,����CAN������
	* @param1,CAN����
	* @return,CanSender*
	*/
	CanSender* allocCanSender(CanTransfer* transfer);

	/*
	* @freeCanSender,�ͷ�CAN������
	* @param1,CAN������
	* @return,void
	*/
	void freeCanSender(CanSender*& sender);
}

