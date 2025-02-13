// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultWidget.h"
#include "System/UIHandle.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UResultWidget::OnNextLevelButton()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->ClickedMoveToNext();
}

void UResultWidget::OnMoveTitleButton()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->ClickedMoveToTitle();
}

void UResultWidget::OnExitGameButton()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->ClickedQuitGame();
}

void UResultWidget::HoveredMoveSceneButton()
{
	MoveSceneButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UResultWidget::UnhoveredMoveSceneButton()
{
	MoveSceneButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UResultWidget::HoveredExitButton()
{
	ExitButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UResultWidget::UnhoveredExitButton()
{
	ExitButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UResultWidget::InitWidget()
{
	Super::InitWidget();

	MoveSceneButton->OnHovered.AddDynamic(this, &UResultWidget::HoveredMoveSceneButton);
	MoveSceneButton->OnUnhovered.AddDynamic(this, &UResultWidget::UnhoveredMoveSceneButton);
	ExitButton->OnHovered.AddDynamic(this, &UResultWidget::HoveredExitButton);
	ExitButton->OnUnhovered.AddDynamic(this, &UResultWidget::UnhoveredExitButton);

	ExitButton->OnClicked.AddDynamic(this, &UResultWidget::OnExitGameButton);
}

void UResultWidget::Action()
{
	Super::Action();

	MoveSceneButton->OnClicked.Clear();

	FString TitleString = FString::Printf(TEXT(""));
	FString ButtonText = FString::Printf(TEXT(""));

	if (CurrentHp <= 0)
	{
		TitleString = FString::Printf(TEXT("Game Over.."));
		ButtonText = FString::Printf(TEXT("처음으로"));
		MoveSceneButton->OnClicked.AddDynamic(this, &UResultWidget::OnMoveTitleButton);
	}
	else
	{
		TitleString = FString::Printf(TEXT("Stage Clear!"));
		ButtonText = FString::Printf(TEXT("다음으로"));
		MoveSceneButton->OnClicked.AddDynamic(this, &UResultWidget::OnNextLevelButton);
	}

	TitleText->SetText(FText::FromString(TitleString));
	MoveButtonText->SetText(FText::FromString(ButtonText));
	CurrentScoreText->SetText(FText::FromString(FString::FromInt(CurrentScore)));
	TotalScoreText->SetText(FText::FromString(FString::FromInt(TotalScore)));
}
