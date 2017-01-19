#pragma once

class CIocp
{
public:
	CIocp();
	virtual ~CIocp();
	
	BOOL Begin();
	BOOL End();
	BOOL RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);
	VOID WorkerThreadCallback();

protected:
	virtual VOID OnIoRead(VOID* object, DWORD dataLength) = 0;
	virtual VOID OnIoWrote(VOID* object, DWORD dataLength) = 0;
	virtual VOID OnIoConnected(VOID* object) = 0;
	virtual VOID OnIoDisconnected(VOID* object) = 0;

private:
	HANDLE mIocpHandle;
	DWORD mWorkerThreadCount;
	std::vector<HANDLE>	mWorkerThreadVector;
	HANDLE mStartupEventHandle;
};

