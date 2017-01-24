#include "stdafx.h"
#include "ConnectedSessionManager.h"

CConnectedSessionManager::CConnectedSessionManager(VOID)
{
}

CConnectedSessionManager::~CConnectedSessionManager(VOID)
{
}

BOOL CConnectedSessionManager::Begin(SOCKET hListenSocket)
{
	CThreadSync Sync;	

	if (!hListenSocket)
		return FALSE;

	for (DWORD i=0;i<MAX_USER;i++)
	{
		CConnectedSession *pConnectedSession = new CConnectedSession();
		m_vConnectedSessions.push_back(pConnectedSession);
	}

	for (DWORD i=0;i<MAX_USER;i++)
	{
		if (!m_vConnectedSessions[i]->Begin())
		{
			CConnectedSessionManager::End();
			return FALSE;
		}

		if (!m_vConnectedSessions[i]->Accept(hListenSocket))
		{
			CConnectedSessionManager::End();
			return FALSE;
		}
	}

	return TRUE;
}

VOID CConnectedSessionManager::End(VOID)
{
	CThreadSync Sync;

	for (DWORD i=0;i<m_vConnectedSessions.size();i++)
	{
		m_vConnectedSessions[i]->End();

		delete m_vConnectedSessions[i];
	}

	m_vConnectedSessions.clear();
}

VOID CConnectedSessionManager::WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength)
{
	CThreadSync Sync;

	for (DWORD i=0;i<m_vConnectedSessions.size();i++)
	{
		if (m_vConnectedSessions[i]->GetConnected())
			m_vConnectedSessions[i]->WritePacket(dwProtocol, pData, dwLength);
	}
}
