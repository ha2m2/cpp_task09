// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BGPlayerState.h"
#include "Net/UnrealNetwork.h"

ABGPlayerState::ABGPlayerState()
{
	bReplicates = true;

	CurrentGuessCount = 0;
	MaxGuessCount = 3;
}

void ABGPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGPlayerState, CurrentGuessCount);
	DOREPLIFETIME(ABGPlayerState, MaxGuessCount);
}

void ABGPlayerState::AddGuessCount()
{
	CurrentGuessCount++;
}

bool ABGPlayerState::IsOutOfChances() const
{
	return CurrentGuessCount >= MaxGuessCount;
}

FString ABGPlayerState::GetGuessStateString() const
{
	return FString::Printf(TEXT("[%d / %d]"), CurrentGuessCount, MaxGuessCount);
}

/* 테스트용
void ABGPlayerState::OnRep_CurrentGuessCount()
{
	UE_LOG(LogTemp, Warning, TEXT("[Client] Replicated Guess Count: %s"), *GetGuessStateString());
}
*/