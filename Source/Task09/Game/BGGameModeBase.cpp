// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BGGameModeBase.h"

ABGGameModeBase::ABGGameModeBase()
{

}

void ABGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();

	UE_LOG(LogTemp, Warning, TEXT("[Server] Generated Secret Number: %s"), *SecretNumberString);
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

bool ABGGameModeBase::IsValidNumberString(const FString& InNumberString)
{
	if (InNumberString.Len() != 3)
	{
		return false;
	}

	TSet<TCHAR> UniqueChars;
	for (int32 i = 0; i < 3; ++i)
	{
		TCHAR c = InNumberString[i];

		if (c < '1' || c > '9')
		{
			return false;
		}
		UniqueChars.Add(c);
	}

	if (UniqueChars.Num() != 3)
	{
		return false;
	}

	return true;
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

void ABGGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	UE_LOG(LogTemp, Warning, TEXT("[Server] Game Reset! New Secret Number: %s"), *SecretNumberString);
}
