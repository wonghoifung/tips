#include "stdafx.h"
#include "ConnectedSession.h"

CConnectedSession::CConnectedSession(VOID)
{
	m_bConnected = FALSE;
}

CConnectedSession::~CConnectedSession(VOID)
{
}

BOOL CConnectedSession::Restart(SOCKET hListenSocket)
{
	CThreadSync Sync;
	End();
	return Begin() && Accept(hListenSocket);
}
