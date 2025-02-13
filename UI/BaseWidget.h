// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Observer/GameStateObserver.h"
#include "BaseWidget.generated.h"

class UUIHandle;

UCLASS()
class SPARTAPRJ06_API UBaseWidget : public UUserWidget, public IGameStateObserver
{
	GENERATED_BODY()
	
public:	
	UFUNCTION()
	virtual void ChangedCurrentScoreNotify(int32 Score) override;
	UFUNCTION()
	virtual void ChangedTotalScoreNotify(int32 Score) override;
	UFUNCTION()
	virtual void ChangedCurrentHpNotify(int32 Hp) override;
	UFUNCTION()
	virtual void ChangedMaxHpNotify(int32 Hp) override;
	UFUNCTION()
	virtual void ChangedWaveTime(int32 TimeValue) override;
	UFUNCTION()
	virtual void ChangedWave(int32 WaveValue) override;
	UFUNCTION()
	virtual void ChangedStage(int32 StageValue) override;
	UFUNCTION()
	virtual void ChangedPlayerState(EDebuffState TargetState, bool bIsActive) override;

	virtual void InitWidget();
	virtual void Action();
	virtual void Update();

protected:
	TObjectPtr<UUIHandle> UIHandle;
	int32 TotalScore;
	int32 CurrentScore;
	int32 MaxHp;
	int32 CurrentHp;
	int32 WaveTime;
	int32 Wave;
	int32 Stage;
};
