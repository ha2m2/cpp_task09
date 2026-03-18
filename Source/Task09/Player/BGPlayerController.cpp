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
	// 1. 서버 권한 확인 및 유효한 객체 확보
	ABGGameModeBase* GM = Cast<ABGGameModeBase>(GetWorld()->GetAuthGameMode());
	ABGPlayerState* PS = GetPlayerState<ABGPlayerState>();

	if (GM && PS)
	{
		// 2. 입력값 유효성 검사 (숫자 3자리, 중복 없음 등)
		if (!GM->IsValidNumberString(InGuessString))
		{
			UE_LOG(LogTemp, Error, TEXT("[Server] Invalid Input from %s"), *PS->GetPlayerName());
			return;
		}

		// 3. 시도 횟수 증가
		PS->AddGuessCount();

		// 4. 정답 판정 (Strike / Ball / OUT)
		// GM에 SecretNumberString을 가져올 Getter가 없다면 추가해야 합니다.
		FString Result = GM->JudgeGuess(GM->GetSecretNumber(), InGuessString);

		// 5. 결과 로그 출력 (서버/클라이언트 공통 확인용)
		UE_LOG(LogTemp, Warning, TEXT("Player [%s] Guess: %s -> Result: %s %s"),
			*PS->GetPlayerName(), *InGuessString, *Result, *PS->GetGuessStateString());

		// TODO: 승리 조건(3S) 달성 시 또는 기회 소진 시 게임 종료 처리 (Next Task)
	}
}