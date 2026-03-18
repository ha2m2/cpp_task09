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
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> ChatInputWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> NotificationWidgetClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> NotificationWidgetInstance;

};
