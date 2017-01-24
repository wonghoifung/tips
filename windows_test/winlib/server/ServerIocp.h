#pragma once

class CServerIocp : public CIocp
{
public:
	CServerIocp(VOID);
	virtual ~CServerIocp(VOID);
	BOOL Begin(VOID);
	VOID End(VOID);
	VOID KeepThreadCallback(VOID);

protected:
	VOID OnIoRead(VOID *pObject, DWORD dwDataLength);
	VOID OnIoWrote(VOID *pObject, DWORD dwDataLength);
	VOID OnIoConnected(VOID *pObject);
	VOID OnIoDisconnected(VOID *pObject);

private:
	VOID PROC_PT_REG_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_REG_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_REG_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);

private:
	CNetworkSession* m_pListen;
	CConnectedSessionManager m_oConnectedSessionManager;

	HANDLE m_hKeepThread;
	HANDLE m_hKeepThreadDestroyEvent;

	std::map<std::wstring, USER*> m_mUser;
	std::map<std::wstring, COMPUTER*> m_mComputer;
	std::map<std::wstring, PROGRAM*> m_mProgram;
};
