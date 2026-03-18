// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BGPlayerController.h"
#include "Player/BGPlayerState.h"
#include "Game/BGGameModeBase.h"
#include "UI/BGChatInput.h"
#include "Blueprint/UserWidget.h"

void ABGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController() && ChatInputWidgetClass)
	{
		ChatInputWidgetInstance = CreateWidget<UBGChatInput>(this, ChatInputWidgetClass);
		if (ChatInputWidgetInstance)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void ABGPlayerController::ServerProcessGuess_Implementation(const FString& InGuessString)
{
	ABGGameModeBase* GM = Cast<ABGGameModeBase>(GetWorld()->GetAuthGameMode());
	ABGPlayerState* PS = GetPlayerState<ABGPlayerState>();

	if (GM && PS)
	{
		if (!GM->IsValidNumberString(InGuessString))
		{
			UE_LOG(LogTemp, Error, TEXT("[Server] Invalid Input from %s"), *PS->GetPlayerName());
			return;
		}

		PS->AddGuessCount();

		FString Result = GM->JudgeGuess(GM->GetSecretNumber(), InGuessString);

		UE_LOG(LogTemp, Warning, TEXT("Player [%s] Guess: %s -> Result: %s %s"),
			*PS->GetPlayerName(), *InGuessString, *Result, *PS->GetGuessStateString());

	}
}

void ABGPlayerController::ClientShowNotification_Implementation(const FString& Message)
{
	if (IsLocalPlayerController() && NotificationWidgetClass)
	{
		if (!NotificationWidgetInstance)
		{
			NotificationWidgetInstance = CreateWidget<UUserWidget>(this, NotificationWidgetClass);
		}

		if (NotificationWidgetInstance && !NotificationWidgetInstance->IsInViewport())
		{
			NotificationWidgetInstance->AddToViewport();
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[NOTICE] %s"), *Message));
	UE_LOG(LogTemp, Warning, TEXT("[Client] Notification: %s"), *Message);
}