// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

void UInGameWidget::InitWidget()
{
	Super::InitWidget();

	Update();
}

void UInGameWidget::Update()
{
	Super::Update();

	UpdateScoreText();
	UpdateHpText();
	UpdateTimeText();
	UpdateWaveText();
	UpdateStageText();
	UpdateSlowState(false);
	UpdateReverseState(false);
	UpdateBlindState(false);
}

void UInGameWidget::UpdateTimeText()
{
	int32 min = WaveTime / 60;
	int32 sec = WaveTime % 60;

	float g = FMath::Clamp(WaveInfoText->GetColorAndOpacity().GetColor(FWidgetStyle::FWidgetStyle()).G - (1.0f / 30.0f), 0.01f, 1.0f);
	float b = FMath::Clamp(WaveInfoText->GetColorAndOpacity().GetColor(FWidgetStyle::FWidgetStyle()).B - (1.0f / 30.0f), 0.01f, 1.0f);

	TimeInfoText->SetText(FText::FromString(FString::Printf(TEXT("%02d : %02d"), min, sec)));
	WaveInfoText->SetColorAndOpacity(FSlateColor(FColor(255, 255 * g, 255 * b)));
}

void UInGameWidget::UpdateWaveText()
{
	WaveInfoText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), Wave)));
	WaveInfoText->SetColorAndOpacity(FSlateColor(FColor(255, 255, 255)));
}

void UInGameWidget::UpdateStageText()
{
	StageInfoText->SetText(FText::FromString(FString::Printf(TEXT("Stage %d"), Stage)));
}

void UInGameWidget::UpdateSlowState(bool bIsSlow)
{
	if (bIsSlow)
	{
		SlowPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SlowPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInGameWidget::UpdateReverseState(bool bIsReverse)
{
	if (bIsReverse)
	{
		ReversePanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ReversePanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInGameWidget::UpdateBlindState(bool bIsBlind)
{
	if (bIsBlind)
	{
		BlindPanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BlindPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInGameWidget::ChangedCurrentScoreNotify(int32 Score)
{
	Super::ChangedCurrentScoreNotify(Score);

	UpdateScoreText();
}

void UInGameWidget::ChangedCurrentHpNotify(int32 Hp)
{
	Super::ChangedCurrentHpNotify(Hp);

	UpdateHpText();
}

void UInGameWidget::ChangedWaveTime(int32 TimeValue)
{
	Super::ChangedWaveTime(TimeValue);

	UpdateTimeText();
}

void UInGameWidget::ChangedWave(int32 WaveValue)
{
	Super::ChangedWave(WaveValue);

	UpdateWaveText();
}

void UInGameWidget::ChangedStage(int32 StageValue)
{
	Super::ChangedStage(StageValue);

	UpdateStageText();
}

void UInGameWidget::ChangedPlayerState(EDebuffState TargetState, bool bIsActive)
{
	Super::ChangedPlayerState(TargetState, bIsActive);

	switch (TargetState)
	{
	case EDebuffState::Slow:
		UpdateSlowState(bIsActive);
		break;

	case EDebuffState::Reverse:
		UpdateReverseState(bIsActive);
		break;

	case EDebuffState::Blind:
		UpdateBlindState(bIsActive);
		break;
	}
}

void UInGameWidget::UpdateHpText()
{
	HpText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHp, MaxHp)));
	float HpValue = (float)CurrentHp / (float)MaxHp;
	HpBar->SetPercent(HpValue);
}

void UInGameWidget::UpdateScoreText()
{
	ScoreText->SetText(FText::FromString(FString::FromInt(CurrentScore)));
}
