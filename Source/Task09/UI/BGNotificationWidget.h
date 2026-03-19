// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGNotificationWidget.generated.h"

/**
 *
 */
UCLASS()
class TASK09_API UBGNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNotificationText(const FString& Message);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NotificationText;
};
