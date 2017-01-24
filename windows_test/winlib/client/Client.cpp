#include "stdafx.h"

#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(Packet, Data);

class CTestClientSession : public CClientSession
{
protected:
	VOID OnIoConnected(VOID)
	{
	}

	VOID OnIoDisconnected(VOID)
	{
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);	

	CTestClientSession *pClientSession = new CTestClientSession();

	if (pClientSession->BeginTcp("127.0.0.1", DEFAULT_PORT))
	{
		while (TRUE)
		{
			BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
			DWORD dwProtocol = 0, dwPacketLength = 0;
			BYTE Packet[MAX_BUFFER_LENGTH] = {0,};

			system("cls");
			
			TCHAR szCommand[32] = {0,};
			_tprintf(_T("Enter command (USER/COMPUTER/PROGRAM) : "));
			ZeroMemory(szCommand, sizeof(szCommand));
			_getts(szCommand);
			
			if (!_tcscmp(_tcslwr(szCommand), _T("user")))
			{
				_tprintf(_T("=== USER ===\n"));
				_tprintf(_T("Enter command (REG/QUERY) : "));
				ZeroMemory(szCommand, sizeof(szCommand));
				_getts(szCommand);

				if (!_tcscmp(_tcslwr(szCommand), _T("reg")))
				{
					WCHAR szUserID[32] = {0,};
					WCHAR szUserName[32] = {0,};
					DWORD dwAge = 0;
					BYTE cSex = 0;
					WCHAR szAddress[64] = {0,};

					_tprintf(_T("Enter USER_ID : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szUserID, szCommand);

					_tprintf(_T("Enter USER_NAME : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szUserName, szCommand);

					_tprintf(_T("Enter AGE : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					dwAge = _ttoi(szCommand);

					_tprintf(_T("Enter SEX : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					cSex = _ttoi(szCommand);

					_tprintf(_T("Enter ADDRESS : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szAddress, szCommand);

					pClientSession->WritePacket(PT_REG_USER, 
						WriteBuffer, 
						WRITE_PT_REG_USER(WriteBuffer, 
						szUserID, 
						szUserName, 
						dwAge, 
						cSex, 
						szAddress));

					_tprintf(_T("Press any key..."));
					getchar();
				}
				else if (!_tcscmp(_tcslwr(szCommand), _T("query")))
				{
					WCHAR szUserID[32] = {0,};

					_tprintf(_T("Enter USER_ID : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szUserID, szCommand);

					pClientSession->WritePacket(PT_QUERY_USER, 
						WriteBuffer, 
						WRITE_PT_QUERY_USER(WriteBuffer, 
						szUserID));

					while (TRUE) if (!pClientSession->ReadPacket(dwProtocol, Packet, dwPacketLength)) if (dwProtocol == PT_QUERY_USER_RESULT) break;

					READ_PACKET(PT_QUERY_USER_RESULT);

					_tprintf(_T("QUERY : USER_ID(%s), USER_NAME(%s), AGE(%d), SEX(%d), ADDRESS(%s)\n"), Data.USER_ID, Data.USER_NAME, Data.AGE, Data.SEX, Data.ADDRESS);

					_tprintf(_T("Press any key..."));
					getchar();
				}
			}
			else if (!_tcscmp(_tcslwr(szCommand), _T("computer")))
			{
				_tprintf(_T("=== COMPUTER ===\n"));
				_tprintf(_T("Enter command (REG/QUERY) : "));
				ZeroMemory(szCommand, sizeof(szCommand));
				_getts(szCommand);

				if (!_tcscmp(_tcslwr(szCommand), _T("reg")))
				{
					WCHAR szComputerName[32] = {0,};
					WCHAR szIPAddress[32] = {0,};
					BYTE cCPUType = 0;
					DWORD dwRAM = 0;
					DWORD dwHDD = 0;

					_tprintf(_T("Enter COMPUTER_NAME : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szComputerName, szCommand);

					_tprintf(_T("Enter IP_ADDRESS : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szIPAddress, szCommand);

					_tprintf(_T("Enter CPU_TYPE : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					cCPUType = _ttoi(szCommand);

					_tprintf(_T("Enter RAM : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					dwRAM = _ttoi(szCommand);

					_tprintf(_T("Enter HDD : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					dwHDD = _ttoi(szCommand);

					pClientSession->WritePacket(PT_REG_COMPUTER, 
						WriteBuffer, 
						WRITE_PT_REG_COMPUTER(WriteBuffer, 
						szComputerName, 
						szIPAddress, 
						cCPUType, 
						dwRAM, 
						dwHDD));

					_tprintf(_T("Press any key..."));
					getchar();
				}
				else if (!_tcscmp(_tcslwr(szCommand), _T("query")))
				{
					WCHAR szComputerName[32] = {0,};

					_tprintf(_T("Enter COMPUTER_NAME : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szComputerName, szCommand);

					pClientSession->WritePacket(PT_QUERY_COMPUTER, 
						WriteBuffer, 
						WRITE_PT_QUERY_COMPUTER(WriteBuffer, 
						szComputerName));

					while (TRUE) if (!pClientSession->ReadPacket(dwProtocol, Packet, dwPacketLength)) if (dwProtocol == PT_QUERY_COMPUTER_RESULT) break;

					READ_PACKET(PT_QUERY_COMPUTER_RESULT);

					_tprintf(_T("QUERY : COMPUTER_NAME(%s), IP_ADDRESS(%s), CPU_TYPE(%d), RAM(%d), HDD(%d)\n"), Data.COMPUTER_NAME, Data.IP_ADDRESS, Data.CPU_TYPE, Data.RAM, Data.HDD);

					_tprintf(_T("Press any key..."));
					getchar();
				}
			}
			else if (!_tcscmp(_tcslwr(szCommand), _T("program")))
			{
				_tprintf(_T("=== PROGRAM ===\n"));
				_tprintf(_T("Enter command (REG/QUERY) : "));
				ZeroMemory(szCommand, sizeof(szCommand));
				_getts(szCommand);

				if (!_tcscmp(_tcslwr(szCommand), _T("reg")))
				{
					WCHAR szProgramName[32] = {0,};
					DWORD dwCOST = 0;
					WCHAR szComment[32] = {0,};

					_tprintf(_T("Enter PROGRAM_NAME : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szProgramName, szCommand);

					_tprintf(_T("Enter COST : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					dwCOST = _ttoi(szCommand);

					_tprintf(_T("Enter COMMENT : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szComment, szCommand);

					pClientSession->WritePacket(PT_REG_PROGRAM, 
						WriteBuffer, 
						WRITE_PT_REG_PROGRAM(WriteBuffer, 
						szProgramName, 
						dwCOST, 
						szComment)); 

					_tprintf(_T("Press any key..."));
					getchar();
				}
				else if (!_tcscmp(_tcslwr(szCommand), _T("query")))
				{
					WCHAR szProgramName[32] = {0,};

					_tprintf(_T("Enter PROGRAM_NAME : "));
					ZeroMemory(szCommand, sizeof(szCommand));
					_getts(szCommand);
					_tcscpy(szProgramName, szCommand);

					pClientSession->WritePacket(PT_QUERY_PROGRAM, 
						WriteBuffer, 
						WRITE_PT_QUERY_PROGRAM(WriteBuffer, 
						szProgramName));

					while (TRUE) if (!pClientSession->ReadPacket(dwProtocol, Packet, dwPacketLength)) if (dwProtocol == PT_QUERY_PROGRAM_RESULT) break;

					READ_PACKET(PT_QUERY_PROGRAM_RESULT);

					_tprintf(_T("QUERY : PROGRAM_NAME(%s), COST(%d), COMMENT(%s)\n"), Data.PROGRAM_NAME, Data.COST, Data.COMMENT);

					_tprintf(_T("Press any key..."));
					getchar();
				}
			}
		}
	}

	pClientSession->End();

	delete pClientSession;

	WSACleanup();

	return 0;
}

