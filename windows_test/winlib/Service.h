#pragma once

class CService
{
public:
	CService();
	virtual ~CService();

	BOOL Install(LPCTSTR serviceName);
	BOOL Uninstall(LPCTSTR serviceName);

	BOOL Begin(LPTSTR serviceName);
	BOOL End();

	VOID RunCallback(DWORD argumentCount, LPTSTR* arguments);
	VOID CtrlHandlerCallback(DWORD opCode);

protected:
	virtual VOID OnStarted() = 0;
	virtual VOID OnStopped() = 0;

private:
	SERVICE_STATUS mServiceStatus;
	SERVICE_STATUS_HANDLE mServiceStatusHandle;
	TCHAR mServiceName[MAX_PATH];
};
