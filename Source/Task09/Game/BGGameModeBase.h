// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BGGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class TASK09_API ABGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABGGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	FString GenerateSecretNumber();

	FString GetSecretNumber() const { return SecretNumberString; }

	bool IsValidNumberString(const FString& InNumberString);

	FString JudgeGuess(const FString& InSecretNumberString, const FString& InGuessNumberString);

	void ResetGame();

	void BroadcastResultAndReset(const FString& ResultMessage);

private:
	FString SecretNumberString;
};
