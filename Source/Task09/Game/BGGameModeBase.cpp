// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BGGameModeBase.h"
#include "Player/BGPlayerController.h"
#include "Player/BGPlayerState.h"
#include "EngineUtils.h"

ABGGameModeBase::ABGGameModeBase()
{

}

void ABGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();

	UE_LOG(LogTemp, Warning, TEXT("[Server] Generated Secret Number: %s"), *SecretNumberString);
}

void ABGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ConnectedPlayerCount++;
	FString NewPlayerName = FString::Printf(TEXT("Player%d"), ConnectedPlayerCount);

	ABGPlayerState* PS = NewPlayer->GetPlayerState<ABGPlayerState>();
	if (PS)
	{
		PS->SetPlayerName(NewPlayerName);
	}

	FString JoinMsg = FString::Printf(TEXT("%s joined the game."), *NewPlayerName);
	UE_LOG(LogTemp, Warning, TEXT("[Server] %s"), *JoinMsg);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABGPlayerController* PC = Cast<ABGPlayerController>(It->Get());
		if (PC)
		{
			PC->ClientPrintMessage(JoinMsg);
		}
	}

	ABGPlayerController* NewPC = Cast<ABGPlayerController>(NewPlayer);
	if (NewPC)
	{
		NewPC->ClientShowNotification(TEXT("Connected to the game server."));
	}
}

FString ABGGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;

	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FString Result = TEXT("");

	for (int32 i = 0; i < 3; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, Numbers.Num() - 1);
		Result += FString::FromInt(Numbers[RandomIndex]);
		Numbers.RemoveAt(RandomIndex);
	}

	return Result;
}

FString ABGGameModeBase::JudgeGuess(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 Strike = 0;
	int32 Ball = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		for (int32 j = 0; j < 3; ++j)
		{
			if (InGuessNumberString[i] == InSecretNumberString[j])
			{
				if (i == j)
				{
					Strike++;
				}
				else
				{
					Ball++;
				}
				break;
			}
		}
	}

	if (Strike == 0 && Ball == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), Strike, Ball);
}

bool ABGGameModeBase::IsValidNumberString(const FString& InNumberString, FString& OutErrorMessage)
{
	if (InNumberString.Len() != 3)
	{
		OutErrorMessage = TEXT("3자리 숫자를 입력해주세요.");
		return false;
	}

	TSet<TCHAR> UniqueChars;
	for (int32 i = 0; i < 3; ++i)
	{
		TCHAR c = InNumberString[i];

		if (c < '1' || c > '9')
		{
			OutErrorMessage = TEXT("1에서 9 사이의 숫자만 입력해주세요.");
			return false;
		}
		UniqueChars.Add(c);
	}

	if (UniqueChars.Num() != 3)
	{
		OutErrorMessage = TEXT("중복되지 않은 숫자를 입력해주세요.");
		return false;
	}

	return true;
}

bool ABGGameModeBase::CheckAllPlayersOutOfChances()
{
	int32 TotalPlayers = 0;
	int32 OutPlayers = 0;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABGPlayerController* PC = Cast<ABGPlayerController>(It->Get());
		if (PC)
		{
			TotalPlayers++;
			ABGPlayerState* PS = PC->GetPlayerState<ABGPlayerState>();

			if (PS && PS->IsOutOfChances())
			{
				OutPlayers++;
			}
		}
	}

	return (TotalPlayers > 0 && TotalPlayers == OutPlayers);
}

void ABGGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	UE_LOG(LogTemp, Warning, TEXT("[Server] Game Reset! New Secret Number: %s"), *SecretNumberString);
}

void ABGGameModeBase::BroadcastResultAndReset(const FString& ResultMessage)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABGPlayerController* PC = Cast<ABGPlayerController>(It->Get());
		if (PC)
		{
			PC->ClientShowNotification(ResultMessage);

			ABGPlayerState* PS = PC->GetPlayerState<ABGPlayerState>();
			if (PS)
			{
				PS->CurrentGuessCount = 0;
			}
		}
	}

	ResetGame();
}
