// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Engine/Public/Tickable.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "TCPSocket.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLostConnectionDelegate);

/**
 * 
 */
UCLASS()
class TCPSOCKETCLIENT_API UTCPSocket : public UObject, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return bEnableTick; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(TCPSocket, STATGROUP_Tickables); }

	bool Connect(const FString& Address, const uint32& Port);

	bool DisConnect();

	bool SendMessage();

	bool IsKeepingConnect();

	void GetError(FString& ErrorContext);

	void ReceiveMessage();

private:

	FSocket* m_Socket;
	uint8 bEnableTick : 1;
	FOnLostConnectionDelegate OnLostConnectionDelegate;
	
};

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "TCPSocketClient Settings"))
class TCPSOCKETCLIENT_API UTCPSocketClientSetting : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(config, EditAnywhere, Category = Address)
	FString ConnectAddress;

	UPROPERTY(config, EditAnywhere, Category = Address)
	uint32 ConnectPort;

};