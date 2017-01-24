#include "stdafx.h"
#include "ServerIocp.h"

#define READ_PACKET(PROTOCOL)\
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(pPacket, Data);

VOID CServerIocp::PROC_PT_REG_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	READ_PACKET(PT_REG_USER);

	USER *pUser = new USER();

	_tcscpy(pUser->szUserID, Data.USER_ID);
	_tcscpy(pUser->szUserName, Data.USER_NAME);
	_tcscpy(pUser->szAddress, Data.ADDRESS);
	pUser->cSex = Data.SEX;
	pUser->dwAge = Data.AGE;

	m_mUser.insert(std::make_pair(Data.USER_ID, pUser));
}

VOID CServerIocp::PROC_PT_QUERY_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	READ_PACKET(PT_QUERY_USER);

	std::map<std::wstring, USER*>::iterator it = m_mUser.find(Data.USER_ID);

	if (it != m_mUser.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer, 
			it->second->szUserID, 
			it->second->szUserName, 
			it->second->dwAge, 
			it->second->cSex, 
			it->second->szAddress));
	}
	else
	{
		USER User;
		ZeroMemory(&User, sizeof(USER));

		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer, 
			User.szUserID, 
			User.szUserName, 
			User.dwAge, 
			User.cSex, 
			User.szAddress));
	}
}

VOID CServerIocp::PROC_PT_REG_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	READ_PACKET(PT_REG_COMPUTER);

	COMPUTER *pComputer = new COMPUTER();

	_tcscpy(pComputer->szComputerName, Data.COMPUTER_NAME);
	_tcscpy(pComputer->szIPAddress, Data.IP_ADDRESS);
	pComputer->cCPUType = Data.CPU_TYPE;
	pComputer->dwRam = Data.RAM;
	pComputer->dwHDD = Data.HDD;

	m_mComputer.insert(std::make_pair(Data.COMPUTER_NAME, pComputer));
}

VOID CServerIocp::PROC_PT_QUERY_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	READ_PACKET(PT_QUERY_COMPUTER);

	std::map<std::wstring, COMPUTER*>::iterator it = m_mComputer.find(Data.COMPUTER_NAME);

	if (it != m_mComputer.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_COMPUTER_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer, 
			it->second->szComputerName, 
			it->second->szIPAddress, 
			it->second->cCPUType,
			it->second->dwRam, 
			it->second->dwHDD)); 
	}
	else
	{
		COMPUTER Computer;
		ZeroMemory(&Computer, sizeof(COMPUTER));

		pConnectedSession->WritePacket(
			PT_QUERY_COMPUTER_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer, 
			Computer.szComputerName,
			Computer.szIPAddress, 
			Computer.cCPUType, 
			Computer.dwRam, 
			Computer.dwHDD));
	}
}

VOID CServerIocp::PROC_PT_REG_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	READ_PACKET(PT_REG_PROGRAM);

	PROGRAM *pProgram = new PROGRAM();

	_tcscpy(pProgram->szProgramName, Data.PROGRAM_NAME);
	_tcscpy(pProgram->szComment, Data.COMMENT);
	pProgram->dwCost = Data.COST;

	m_mProgram.insert(std::make_pair(Data.PROGRAM_NAME, pProgram));
}

VOID CServerIocp::PROC_PT_QUERY_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	READ_PACKET(PT_QUERY_PROGRAM);

	std::map<std::wstring, PROGRAM*>::iterator it = m_mProgram.find(Data.PROGRAM_NAME);

	if (it != m_mProgram.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer, 
			it->second->szProgramName, 
			it->second->dwCost, 
			it->second->szComment)); 
	}
	else
	{
		PROGRAM Program;
		ZeroMemory(&Program, sizeof(PROGRAM));

		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer, 
			Program.szProgramName,
			Program.dwCost, 
			Program.szComment));
	}
}
