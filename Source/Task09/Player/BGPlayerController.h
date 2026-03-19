// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BGPlayerController.generated.h"

/**
 *
 */
UCLASS()
class TASK09_API ABGPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UBGChatInput> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<class UBGChatInput> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UBGNotificationWidget> NotificationWidgetClass;

	UPROPERTY()
	TObjectPtr<class UBGNotificationWidget> NotificationWidgetInstance;

public:
	UFUNCTION(Server, Reliable)
	void ServerProcessGuess(const FString& InGuessString);

	UFUNCTION(Client, Reliable)
	void ClientShowNotification(const FString& Message);

	UFUNCTION(Client, Unreliable)
	void ClientPrintMessage(const FString& Message);
};
