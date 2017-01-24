#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);
  
	if (!CMiniDump::Begin())
		return 0;

	CServerIocp ServerIocp;
	if (ServerIocp.Begin())
	{
		getchar();
		ServerIocp.End();
	}

	WSACleanup();
	CMiniDump::End();
	CoUninitialize();

	return 0;
}

