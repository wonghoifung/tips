#pragma once

class CConnectedSession : public CPacketSession
{
public:
	CConnectedSession(VOID);
	virtual ~CConnectedSession(VOID);

	BOOL Restart(SOCKET hListenSocket);

	VOID SetConnected(BOOL bConnected) { CThreadSync Sync; m_bConnected = bConnected; }
	BOOL GetConnected(VOID) { CThreadSync Sync; return m_bConnected; }

private:
	BOOL m_bConnected;
};
