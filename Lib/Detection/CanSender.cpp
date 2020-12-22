#include "CanSender.h"
#include <process.h>

UINT WINAPI SendCanProc(void* pArgs)
{
	CanSender* pSSM = (CanSender*)pArgs;
	int iEndTime = 0, iCount = 0;
	MsgNode msgs[100] = { 0 };

	while (!pSSM->m_bQuit)
	{
		while (pSSM->m_bStart)
		{
			//send msg
			iEndTime = pSSM->m_time.getEndTime();
			for (int i = 0; i < MAX_MSG_COUNT; i++)
			{
				if (pSSM->m_msgs[i].bValid
					&& iEndTime - pSSM->m_msgs[i].iTime >= pSSM->m_msgs[i].iCycle)
				{
					pSSM->m_msgs[i].iTime = iEndTime;
					memcpy(&msgs[iCount], &pSSM->m_msgs[i].msg, sizeof(MsgNode));
					if (pSSM->m_msgs[i].emST == ST_Event)
					{
						pSSM->m_msgs[i].iSendCount--;
						if (pSSM->m_msgs[i].iSendCount <= 0)
						{
							//停止发送
							pSSM->m_msgs[i].bValid = false;
						}
					}
					else if (pSSM->m_msgs[i].emST == ST_PE)
					{
						if (pSSM->m_msgs[i].iSendCount > 0)
						{
							pSSM->m_msgs[i].iSendCount--;
							if (pSSM->m_msgs[i].iSendCount <= 0)
							{
								//发送默认值
								for (int j = 0; j < MAX_MSG_COUNT; j++)
								{
									if (pSSM->m_msgs[i].msg.id == pSSM->m_msgBackup[j].msg.id)
									{
										memcpy(&pSSM->m_msgs[i], &pSSM->m_msgBackup[j], sizeof(CanMsg));
									}
								}
							}
						}
					}

					iCount++;
					if (iCount >= 100)
					{
						break;
					}
				}
			}

			if (iCount > 0)
			{
				pSSM->m_pIConnMgr->MultiSend(msgs, iCount);
				iCount = 0;
			}
			Sleep(1);
		}
		Sleep(100);
	}
	return 0;
}
CanSender::CanSender()
{
	m_bQuit = false;
	m_bStart = false;
	m_hSend = nullptr;
	m_pIConnMgr = nullptr;
	memset(m_msgs, 0, sizeof(CanMsg) * MAX_MSG_COUNT);
	memset(m_msgBackup, 0, sizeof(CanMsg) * MAX_MSG_COUNT);
	m_hSend = (HANDLE)_beginthreadex(nullptr, 0, &SendCanProc, this, 0, 0);
}

CanSender::CanSender(IConnMgr* const pIConnMgr)
{
	m_bQuit = false;
	m_bStart = false;
	m_hSend = nullptr;
	m_pIConnMgr = pIConnMgr;
	memset(m_msgs, 0, sizeof(CanMsg) * MAX_MSG_COUNT);
	memset(m_msgBackup, 0, sizeof(CanMsg) * MAX_MSG_COUNT);
	m_hSend = (HANDLE)_beginthreadex(nullptr, 0, &SendCanProc, this, 0, 0);
}

CanSender::~CanSender()
{
	if (nullptr != m_hSend)
	{
		m_bQuit = true;
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hSend, 2000))
		{
			TerminateThread(m_hSend, 0);
		}
		m_hSend = nullptr;
	}
}

bool CanSender::Init(IConnMgr* const pIConnMgr)
{
	bool bRet = false;
	do
	{
		m_pIConnMgr = pIConnMgr;
		if (!m_pIConnMgr)
		{
			break;
		}

		memcpy(m_msgBackup, m_msgs, sizeof(CanMsg) * MAX_MSG_COUNT);
		bRet = true;
	} while (false);
	return bRet;
}

bool CanSender::AddMsg(const MsgNode& msg, const int& iDelay, const SendType& emST, const int& iSendCount)
{
	bool bRet = false;
	do
	{
		bool bFind = false;
		for (int i = 0; i < MAX_MSG_COUNT; i++)
		{
			if (m_msgs[i].bValid)
			{
				//如果存在则直接覆盖
				if (m_msgs[i].msg.id == msg.id)
				{
					memcpy(&m_msgs[i].msg, &msg, sizeof(MsgNode));
					m_msgs[i].iCycle = iDelay;
					m_msgs[i].iSendCount = iSendCount;
					m_msgs[i].emST = emST;
					bFind = true;
					break;
				}
			}
		}

		if (!bFind)
		{
			//新增
			for (int i = 0; i < MAX_MSG_COUNT; i++)
			{
				if (!m_msgs[i].bValid)
				{
					memcpy(&m_msgs[i].msg, &msg, sizeof(MsgNode));
					m_msgs[i].iSendCount = iSendCount;
					m_msgs[i].emST = emST;
					m_msgs[i].iCycle = iDelay;
					m_msgs[i].bValid = true;
					m_msgs[i].iTime = 0;

					if (emST != ST_Event)
					{
						memcpy(&m_msgBackup[i], &m_msgs[i], sizeof(CanMsg));
					}
					bFind = true;
					break;
				}
			}
		}

		if (!bFind)
		{
			SetLastError("报文已满");
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CanSender::AddMsg(const CanMsg& msg)
{
	bool bRet = false;
	do
	{
		bool bFind = false;
		for (int i = 0; i < MAX_MSG_COUNT; i++)
		{
			if (m_msgs[i].bValid)
			{
				//如果存在则直接覆盖
				if (m_msgs[i].msg.id == msg.msg.id)
				{
					memcpy(&m_msgs[i].msg, &msg, sizeof(MsgNode));
					m_msgs[i].iCycle = msg.iCycle;
					m_msgs[i].iSendCount = msg.iSendCount;
					m_msgs[i].emST = msg.emST;
					bFind = true;
					break;
				}
			}
		}
		if (!bFind)
		{
			//新增
			for (int i = 0; i < MAX_MSG_COUNT; i++)
			{
				if (!m_msgs[i].bValid)
				{
					memcpy(&m_msgs[i].msg, &msg, sizeof(MsgNode));
					m_msgs[i].iSendCount = msg.iSendCount;
					m_msgs[i].emST = msg.emST;
					m_msgs[i].iCycle = msg.iCycle;
					m_msgs[i].bValid = true;
					m_msgs[i].iTime = 0;

					if (msg.emST != ST_Event)
					{
						memcpy(&m_msgBackup[i], &m_msgs[i], sizeof(CanMsg));
					}
					bFind = true;
					break;
				}
			}
		}

		if (!bFind)
		{
			SetLastError("报文已满");
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CanSender::AddMsg(const std::initializer_list<CanMsg>& msg)
{
	bool bRet = false, bSuccess = true;
	do 
	{
		for (int i = 0; i < msg.size(); i++)
		{
			if (!AddMsg(msg.begin()[i]))
			{
				bSuccess = false;
				break;
			}
		}

		if (!bSuccess)
		{
			break;
		}
		bRet = true;
	} while (false);
	return true;
}

void CanSender::DeleteMsgs(const std::initializer_list<MsgNode>& msgs)
{
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		for (int j = 0; j < msgs.size(); j++)
		{
			if (m_msgs[i].msg.id == msgs.begin()[j].id)
			{
				memset(&m_msgs[i], 0, sizeof(CanMsg));
				break;
			}
		}
	}
	return;
}

void CanSender::DeleteMsgs(const std::initializer_list<int>& ids)
{
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		for (int j = 0; j < ids.size(); j++)
		{
			if (m_msgs[i].msg.id == ids.begin()[j])
			{
				memset(&m_msgs[i], 0, sizeof(CanMsg));
				break;
			}
		}
	}
	return;
}

void CanSender::DeleteOneMsg(const MsgNode& msg)
{
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		if (m_msgs[i].msg.id == msg.id)
		{
			memset(&m_msgs[i], 0, sizeof(CanMsg));
			break;
		}
	}
	return;
}

void CanSender::DeleteOneMsg(const int& id)
{
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		if (m_msgs[i].msg.id == id)
		{
			memset(&m_msgs[i], 0, sizeof(CanMsg));
			break;
		}
	}
	return;
}

void CanSender::DeleteAllMsgs()
{
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		if (m_msgs[i].bValid)
		{
			memset(&m_msgs[i], 0x00, sizeof(CanMsg));
		}
	}
	return;
}

bool CanSender::GetMsgData(const int& ID, UCHAR* ucData)
{
	bool bRet = false;
	do
	{
		bool bFind = false;
		for (int i = 0; i < MAX_MSG_COUNT; i++)
		{
			if (!m_msgs[i].bValid)
			{
				continue;
			}

			if (ID == m_msgs[i].msg.id)
			{
				memcpy(ucData, m_msgs[i].msg.data, m_msgs[i].msg.dlc);
				bFind = true;
			}
		}

		if (!bFind)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool CanSender::SetMsgData(const int& ID, const UCHAR* const ucData)
{
	bool bRet = false;
	do
	{
		bool bFind = false;
		for (int i = 0; i < MAX_MSG_COUNT; i++)
		{
			if (!m_msgs[i].bValid)
			{
				continue;
			}

			if (ID == m_msgs[i].msg.id)
			{
				memcpy(m_msgs[i].msg.data, ucData, m_msgs[i].msg.dlc);
				bFind = true;
			}
		}
		if (!bFind)
		{
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

void CanSender::Start()
{
	m_time.getStartTime();
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		if (m_msgs[i].bValid)
		{
			m_msgs[i].iTime = 0;
		}
	}
	m_bStart = true;
	return;
}

void CanSender::Stop()
{
	m_bStart = false;
	return;
}

bool CanSender::EnableExternalMsg(bool enable)
{
	bool bRet = false;
	do
	{
		if (!enable)
		{
			bRet = true;
			break;
		}

		CanMsg* stcmsg = nullptr;
		bool bFind = false;
		for (int i = 0; i < MAX_MSG_COUNT; i++)
		{
			if (m_msgs[i].bValid)
			{
				continue;
			}
			stcmsg = &m_msgs[i];
			stcmsg->bValid = true;
			bFind = true;
			break;
		}

		if (!bFind)
		{
			break;
		}

		for (int i = 0; i < m_jsonTool->getCanMsgCount(); i++)
		{
			stcmsg->msg.id = m_jsonTool->getParsedCanMsg()[i].msg.id;
			stcmsg->iCycle = m_jsonTool->getParsedCanMsg()[i].iCycle;
			stcmsg->msg.dlc = m_jsonTool->getParsedCanMsg()[i].msg.dlc;
			stcmsg->emST = m_jsonTool->getParsedCanMsg()[i].emST;
			memcpy(stcmsg->msg.data, m_jsonTool->getParsedCanMsg()[i].msg.data, sizeof(stcmsg->msg.data));
		}
		bRet = true;
	} while (false);
	return bRet;
}

void CanSender::PauseMsg(const std::initializer_list<MsgNode>& msg)
{
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		for (int j = 0; j < msg.size(); j++)
		{
			if (m_msgs[i].msg.id == msg.begin()[j].id)
			{
				m_msgs[i].bValid = false;
				break;
			}
		}
	}
	return;
}

void CanSender::ProceedMsg(const std::initializer_list<MsgNode>& msg)
{
	for (int i = 0; i < MAX_MSG_COUNT; i++)
	{
		for (int j = 0; j < msg.size(); j++)
		{
			if (m_msgs[i].msg.id == msg.begin()[j].id)
			{
				m_msgs[i].bValid = true;
				break;
			}
		}
	}
	return;
}

bool CanSender::ReceSpecSigs(const int& iID, LPVOID pstSS, fpConfirmMsg fpConMsg,const int& iTime, char* szResult)
{
	bool bRet = false;
	do
	{
		int iStartTime = GetTickCount();
		bool bSuccess = false;
		MsgNode msg[4096];
		for (;;)
		{
			int iSize = 0;
			iSize = m_pIConnMgr->QuickReceive(msg, 4096, 100);
			for (int i = 0; i < iSize; i++)
			{
				if (msg[i].id == iID)
				{
					if (fpConMsg(pstSS, msg[i].data))
					{
						bSuccess = true;
						strcpy(szResult, "Pass");
					}
				}
			}
			Sleep(100);
			if ((int)(GetTickCount() - iStartTime) > iTime)
			{
				strcpy(szResult, "Fail(timeout)");
				break;
			}
		}
		bRet = true;
	} while (false);
	return bRet;
}

const char* CanSender::GetLastError()
{
	return m_szLastError;
}

void CanSender::SetLastError(const char* szError)
{
	strcpy_s(m_szLastError, sizeof(m_szLastError), szError);
}

