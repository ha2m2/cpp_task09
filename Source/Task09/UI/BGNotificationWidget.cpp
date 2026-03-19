// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BGNotificationWidget.h"
#include "Components/TextBlock.h"

void UBGNotificationWidget::SetNotificationText(const FString& Message)
{
	if (NotificationText)
	{
		NotificationText->SetText(FText::FromString(Message));
	}
}
