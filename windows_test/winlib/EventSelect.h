#pragma once

class CEventSelect
{
public:
	CEventSelect();
	virtual ~CEventSelect();

protected:
	virtual VOID OnIoRead() = 0;
	//virtual VOID OnIoWrote(VOID) = 0;
	virtual VOID OnIoConnected() = 0;
	virtual VOID OnIoDisconnected()	= 0;

public:
	BOOL Begin(SOCKET socket);
	BOOL End();
	VOID SelectThreadCallback();

private:
	HANDLE mSelectEventHandle;
	HANDLE mStartupEventHandle;
	HANDLE mDestroyEventHandle;
	HANDLE mSelectThreadHandle;
	SOCKET mSocket;
};
