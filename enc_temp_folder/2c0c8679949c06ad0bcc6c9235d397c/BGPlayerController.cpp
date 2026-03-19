// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BGPlayerController.h"
#include "Player/BGPlayerState.h"
#include "Game/BGGameModeBase.h"
#include "UI/BGChatInput.h"
#include "UI/BGNotificationWidget.h"
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
		if (PS->IsOutOfChances())
		{
			ClientShowNotification(TEXT("You are already out of chances."));
			return;
		}

		FString ErrorMsg;
		if (!GM->IsValidNumberString(InGuessString, ErrorMsg))
		{
			ClientPrintMessage(ErrorMsg);
			return;
		}

		PS->AddGuessCount();
		FString Result = GM->JudgeGuess(GM->GetSecretNumber(), InGuessString);

		UE_LOG(LogTemp, Warning, TEXT("Player [%s] Guess: %s -> Result: %s %s"),
			*PS->GetPlayerName(), *InGuessString, *Result, *PS->GetGuessStateString());

		if (Result == TEXT("3S 0B"))
		{
			FString WinMsg = FString::Printf(TEXT("Player %s WIN!"), *PS->GetPlayerName());
			GM->BroadcastResultAndReset(WinMsg);
		}

		else if (GM->CheckAllPlayersOutOfChances())
		{
			GM->BroadcastResultAndReset(TEXT("DRAW! All players are out of chances."));
		}

		else if (PS->IsOutOfChances())
		{
			ClientShowNotification(TEXT("Out of chances. Waiting for others..."));
		}
	}
}

void ABGPlayerController::ClientShowNotification_Implementation(const FString& Message)
{
	if (IsLocalPlayerController() && NotificationWidgetClass)
	{
		if (!NotificationWidgetInstance)
		{
			NotificationWidgetInstance = CreateWidget<UBGNotificationWidget>(this, NotificationWidgetClass);
		}

		if (NotificationWidgetInstance && !NotificationWidgetInstance->IsInViewport())
		{
			NotificationWidgetInstance->AddToViewport();
		}

		if (NotificationWidgetInstance)
		{
			NotificationWidgetInstance->SetNotificationText(Message);
		}
	}
}

void ABGPlayerController::ClientPrintMessage_Implementation(const FString& Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, Message);
	}
}