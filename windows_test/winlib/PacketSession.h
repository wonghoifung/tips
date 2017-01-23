#pragma once

typedef struct _READ_PACKET_INFO
{
	CHAR RemoteAddress[14];
	USHORT RemotePort;
	DWORD PacketNumber;
} READ_PACKET_INFO;

class CPacketSession : public CNetworkSession
{
public:
	CPacketSession();
	virtual ~CPacketSession();

	BOOL Begin();
	BOOL End();

	BOOL ReadPacketForIocp(DWORD readLength);
	BOOL ReadPacketForEventSelect();

	BOOL ReadFromPacketForIocp(LPSTR remoteAddress, USHORT& remotePort, DWORD readLength);
	BOOL ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT& remotePort);

	BOOL WritePacket(DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength);
	BOOL WriteComplete();

	BOOL ResetUdp();

	BOOL GetPacket(DWORD& protocol, BYTE* packet, DWORD& packetLength);
	BOOL GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD& protocol, BYTE* packet, DWORD& packetLength);

private:
	BYTE mPacketBuffer[MAX_BUFFER_LENGTH * 3];
	INT mRemainLength;
	DWORD mCurrentPacketNumber;
	DWORD mLastReadPacketNumber;
	CCircularQueue WriteQueue;
	//std::map<std::string, DWORD> mLastReadPacketInfoMapForUdp;
	std::vector<READ_PACKET_INFO> mLastReadPacketInfoVectorForUdp;
};
