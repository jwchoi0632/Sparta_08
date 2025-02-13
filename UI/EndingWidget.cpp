// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EndingWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "System/UIHandle.h"

void UEndingWidget::OnMoveToTitle()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->ClickedMoveToTitle();
}

void UEndingWidget::OnQuitGame()
{
	UIHandle->ClickedQuitGame();
}

void UEndingWidget::HoveredMoveTitleButton()
{
	MoveTitleButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UEndingWidget::UnhoveredMoveTitleButton()
{
	MoveTitleButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UEndingWidget::HoveredQuitGameButton()
{
	QuitGameButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UEndingWidget::UnhoveredQuitGameButton()
{
	QuitGameButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UEndingWidget::Action()
{
	Super::Action();

	PlayAnimation(EndingCredit);
}

void UEndingWidget::Update()
{
	Super::Update();

	int32 PlayTime = UIHandle->GetPlayTime();
	int32 Sec = PlayTime % 60;
	int32 Min = PlayTime / 60 % 60;
	int32 Hour = PlayTime / 360;
	FString TotalScoreText = FString::FromInt(TotalScore);
	
	Line18->SetText(FText::FromString(TotalScoreText));
	Line15->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d"), Hour, Min, Sec)));
}

void UEndingWidget::InitWidget()
{
	Super::InitWidget();

	MoveTitleButton->OnHovered.AddDynamic(this, &UEndingWidget::HoveredMoveTitleButton);
	MoveTitleButton->OnUnhovered.AddDynamic(this, &UEndingWidget::UnhoveredMoveTitleButton);
	QuitGameButton->OnHovered.AddDynamic(this, &UEndingWidget::HoveredQuitGameButton);
	QuitGameButton->OnUnhovered.AddDynamic(this, &UEndingWidget::UnhoveredQuitGameButton);

	MoveTitleButton->OnClicked.AddDynamic(this, &UEndingWidget::OnMoveToTitle);
	QuitGameButton->OnClicked.AddDynamic(this, &UEndingWidget::OnQuitGame);
}
