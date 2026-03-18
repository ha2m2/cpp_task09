// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BGChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/BGPlayerController.h"

void UBGChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChatInputTextBox)
	{
		ChatInputTextBox->OnTextCommitted.AddDynamic(this, &UBGChatInput::OnTextCommitted);
	}
}

void UBGChatInput::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		ABGPlayerController* PC = Cast<ABGPlayerController>(GetOwningPlayer());
		if (PC)
		{
			PC->ServerProcessGuess(Text.ToString());
		}

		ChatInputTextBox->SetText(FText::GetEmpty());
	}
}