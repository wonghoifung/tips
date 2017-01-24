#include "stdafx.h"
#include "ServerIocp.h"

DWORD WINAPI KeepThreadCallback(LPVOID pParameter)
{
	CServerIocp *pOwner = (CServerIocp*) pParameter;
	pOwner->KeepThreadCallback();
	return 0;
}

CServerIocp::CServerIocp(VOID)
{
}

CServerIocp::~CServerIocp(VOID)
{
}

VOID CServerIocp::KeepThreadCallback(VOID)
{
	DWORD dwKeepAlive = 0xFFFF;
	while (TRUE)
	{
		DWORD dwResult = WaitForSingleObject(m_hKeepThreadDestroyEvent, 30000);
		if (dwResult == WAIT_OBJECT_0) return;
		m_oConnectedSessionManager.WriteAll(0x3000000, (BYTE*)&dwKeepAlive, sizeof(DWORD));
	}
}

VOID CServerIocp::OnIoConnected(VOID *pObject)
{
	CConnectedSession *pConnectedSession = reinterpret_cast<CConnectedSession*>(pObject);
	if (!CIocp::RegisterSocketToIocp(pConnectedSession->GetSocket(), reinterpret_cast<ULONG_PTR>(pConnectedSession))) 
		return;
	if (!pConnectedSession->InitializeReadForIocp())
	{ 
		pConnectedSession->Restart(m_pListen->GetSocket()); 
		return; 
	}
	pConnectedSession->SetConnected(TRUE);
}

VOID CServerIocp::OnIoDisconnected(VOID *pObject)
{
	CConnectedSession *pConnectedSession = reinterpret_cast<CConnectedSession*>(pObject);
	pConnectedSession->Restart(m_pListen->GetSocket());
	pConnectedSession->SetConnected(FALSE);
}

VOID CServerIocp::OnIoRead(VOID *pObject, DWORD dwDataLength)
{
	CConnectedSession *pConnectedSession = reinterpret_cast<CConnectedSession*>(pObject);
	DWORD dwProtocol = 0, dwPacketLength = 0;
	BYTE Packet[MAX_BUFFER_LENGTH] = {0,};

	if (pConnectedSession->ReadPacketForIocp(dwDataLength))
	{
		while (pConnectedSession->GetPacket(dwProtocol, Packet, dwPacketLength))
		{
			switch (dwProtocol)
			{
			case PT_REG_USER:
				PROC_PT_REG_USER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			case PT_QUERY_USER:
				PROC_PT_QUERY_USER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			case PT_REG_COMPUTER:
				PROC_PT_REG_COMPUTER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			case PT_QUERY_COMPUTER:
				PROC_PT_QUERY_COMPUTER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			case PT_REG_PROGRAM:
				PROC_PT_REG_PROGRAM(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			case PT_QUERY_PROGRAM:
				PROC_PT_QUERY_PROGRAM(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
			}
		}
	}

	if (!pConnectedSession->InitializeReadForIocp())
		pConnectedSession->Restart(m_pListen->GetSocket());
}

VOID CServerIocp::OnIoWrote(VOID *pObject, DWORD dwDataLength)
{

}

BOOL CServerIocp::Begin(VOID)
{
	if (!CIocp::Begin()) return FALSE;

	m_pListen = new CNetworkSession();

	if (!m_pListen->Begin())
	{
		CServerIocp::End();
		return FALSE;
	}

	if (!m_pListen->TcpBind())
	{
		CServerIocp::End();
		return FALSE;
	}

	if (!m_pListen->Listen(DEFAULT_PORT, MAX_USER))
	{
		CServerIocp::End();
		return FALSE;
	}

	if (!CIocp::RegisterSocketToIocp(m_pListen->GetSocket(), reinterpret_cast<ULONG_PTR>(m_pListen)))
	{
		CServerIocp::End();
		return FALSE;
	}

	if (!m_oConnectedSessionManager.Begin(m_pListen->GetSocket()))
	{
		CServerIocp::End();
		return FALSE;
	}

	m_hKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!m_hKeepThreadDestroyEvent)
	{ 
		CServerIocp::End(); 
		return FALSE; 
	}

	m_hKeepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!m_hKeepThread) 
	{ 
		CServerIocp::End(); 
		return FALSE; 
	}

	return TRUE;
}

VOID CServerIocp::End(VOID)
{
	std::map<std::wstring, USER*>::iterator it1;
	for (it1=m_mUser.begin();it1!=m_mUser.end();++it1)
		delete it1->second;

	m_mUser.clear();

	std::map<std::wstring, COMPUTER*>::iterator it2;
	for (it2=m_mComputer.begin();it2!=m_mComputer.end();++it2)
		delete it2->second;

	m_mComputer.clear();

	std::map<std::wstring, PROGRAM*>::iterator it3;
	for (it3=m_mProgram.begin();it3!=m_mProgram.end();++it3)
		delete it3->second;

	m_mProgram.clear();

	if (m_hKeepThread)
	{
		SetEvent(m_hKeepThreadDestroyEvent);

		WaitForSingleObject(m_hKeepThread, INFINITE);

		CloseHandle(m_hKeepThread);
		m_hKeepThread = NULL;
	}

	if (m_hKeepThreadDestroyEvent)
	{
		CloseHandle(m_hKeepThreadDestroyEvent);
		m_hKeepThreadDestroyEvent = NULL;
	}

	CIocp::End();

	m_oConnectedSessionManager.End();

	if (m_pListen)
	{
		m_pListen->End();
		delete m_pListen;
	}
}
