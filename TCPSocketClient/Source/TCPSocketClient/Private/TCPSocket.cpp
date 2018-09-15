// Fill out your copyright notice in the Description page of Project Settings.

#include "TCPSocket.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include <String>

UTCPSocketClientSetting::UTCPSocketClientSetting(const FObjectInitializer& Initializer) : Super(Initializer)
{

}

////////////////////////////////////////////////
//////////////UTCPSocket Function///////////////
////////////////////////////////////////////////

UTCPSocket::UTCPSocket(const FObjectInitializer& Initializer) : Super(Initializer), bEnableTick(false)
{

}

void UTCPSocket::Tick(float DeltaTime)
{
	if (IsKeepingConnect())
	{
		ReceiveMessage();
	}
	else
	{
		DisConnect();
		OnLostConnectionDelegate.Broadcast();
	}
}

bool UTCPSocket::Connect(const FString& Address, const uint32& Port)
{
	if (m_Socket)
		DisConnect();

	FSocket* m_Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCPSocketClient"), false);
	FIPv4Address ipv4ip;
	FIPv4Address::Parse(Address, ipv4ip);

	TSharedRef<FInternetAddr> SockAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ipv4ip.Value, Port);
	bool bConnectSuccess = m_Socket->Connect(*SockAddr);

	if (bConnectSuccess)
		bEnableTick = true;
	else
		bEnableTick = false;

	return bConnectSuccess;
}

bool UTCPSocket::DisConnect()
{
	if (m_Socket)
	{
		m_Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(m_Socket);
		m_Socket = NULL;
	}
	return true;
}

bool UTCPSocket::SendMessage()
{
	bool bSendSuccess = false;
	if (m_Socket)
	{
		FString TestStr = TEXT("123");
		/*
			在发送FString的字符串时,获取的字符串长度与实际发送长度不匹配,导致服务器取得长度不对,无法正常解析字符串。
			When sending a string of FString, the length of the obtained string does not match the actual length of the sent string, 
			causing the server to get the wrong length and unable to parse the string properly.

			std::string str = TCHAR_TO_UTF8(*TestStr);
			size_t len = str.length();
		*/

		int32 sent = 0;
		bSendSuccess = m_Socket->Send((uint8*)TCHAR_TO_UTF8(*TestStr), TestStr.Len(), sent);
	}
	return bSendSuccess;
}

void UTCPSocket::ReceiveMessage()
{
	if (m_Socket)
	{
		uint32 DataSize;
		TArray<uint8> BinaryData;
		while (m_Socket->HasPendingData(DataSize))
		{
			BinaryData.Init(0, FMath::Min(DataSize, 65507u));

			int32 BytesRead = 0;
			m_Socket->Recv(BinaryData.GetData(), BinaryData.Num(), BytesRead);
		}
	}
}

bool UTCPSocket::IsKeepingConnect()
{
	if (m_Socket)
	{
		if (m_Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		{
			bool HasPendingConnection = false;
			bool Res = m_Socket->HasPendingConnection(HasPendingConnection);

			if (Res & !HasPendingConnection)
				return true;
		}
	}
	return false;
}

void UTCPSocket::GetError(FString& ErrorContext)
{

}