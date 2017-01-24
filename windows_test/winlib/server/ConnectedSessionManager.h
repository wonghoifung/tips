#pragma once

class CConnectedSessionManager : public CMultiThreadSync<CConnectedSessionManager>
{
public:
	CConnectedSessionManager(VOID);
	virtual ~CConnectedSessionManager(VOID);

	BOOL Begin(SOCKET hListenSocket);

	VOID End(VOID);

	VOID WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength);

private:
	std::vector<CConnectedSession*> m_vConnectedSessions;
};
