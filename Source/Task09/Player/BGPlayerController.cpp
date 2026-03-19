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

		UE_LOG(LogTemp, Warning, TEXT("[%s] Guess: %s -> Result: %s %s"),
			*PS->GetPlayerName(), *InGuessString, *Result, *PS->GetGuessStateString());

		FString FeedbackMsg = FString::Printf(TEXT("[%s] 입력: %s -> %s %s"),
			*PS->GetPlayerName(), *InGuessString, *Result, *PS->GetGuessStateString());
		ClientPrintMessage(FeedbackMsg);

		if (Result == TEXT("3S 0B"))
		{
			FString WinMsg = FString::Printf(TEXT("%s 승리!"), *PS->GetPlayerName());
			GM->BroadcastResultAndReset(WinMsg);
		}

		else if (GM->CheckAllPlayersOutOfChances())
		{
			GM->BroadcastResultAndReset(TEXT("무승부!"));
		}

		else if (PS->IsOutOfChances())
		{
			ClientShowNotification(TEXT("기회를 전부 소진하여 다른 플레이어를 기다립니다.."));
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