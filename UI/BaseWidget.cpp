// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseWidget.h"
#include "System/BaseGameInstance.h"

void UBaseWidget::ChangedCurrentScoreNotify(int32 Score)
{
	CurrentScore = Score;
}

void UBaseWidget::ChangedTotalScoreNotify(int32 Score)
{
	TotalScore = Score;
}

void UBaseWidget::ChangedCurrentHpNotify(int32 Hp)
{
	CurrentHp = Hp;
}

void UBaseWidget::ChangedMaxHpNotify(int32 Hp)
{
	MaxHp = Hp;
}

void UBaseWidget::ChangedWaveTime(int32 TimeValue)
{
	WaveTime = TimeValue;
}

void UBaseWidget::ChangedWave(int32 WaveValue)
{
	Wave = WaveValue;
}

void UBaseWidget::ChangedStage(int32 StageValue)
{
	Stage = StageValue;
}

void UBaseWidget::ChangedPlayerState(EDebuffState TargetState, bool bIsActive)
{

}

void UBaseWidget::InitWidget()
{
	TObjectPtr<UBaseGameInstance> BaseGameInstance = Cast<UBaseGameInstance>(GetOuter());

	if (!IsValid(BaseGameInstance))
	{
		return;
	}

	UIHandle = BaseGameInstance->GetUIHandle();
	CurrentScore = 0;
	TotalScore = 0;
	CurrentHp = 0;
	MaxHp = 0;
	WaveTime = 0;
	Wave = 0;
	Stage = 0;
}

void UBaseWidget::Action()
{

}

void UBaseWidget::Update()
{

}
