// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "InGameWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UCanvasPanel;

UCLASS()
class SPARTAPRJ06_API UInGameWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateHpText();
	UFUNCTION()
	void UpdateScoreText();

	virtual void ChangedCurrentScoreNotify(int32 Score) override;
	virtual void ChangedCurrentHpNotify(int32 Hp) override;
	virtual void ChangedWaveTime(int32 TimeValue) override;
	virtual void ChangedWave(int32 WaveValue) override;
	virtual void ChangedStage(int32 StageValue) override;
	virtual void ChangedPlayerState(EDebuffState TargetState, bool bIsActive) override;
	virtual void InitWidget() override;
	virtual void Update() override;

	void UpdateTimeText();
	void UpdateWaveText();
	void UpdateStageText();
	void UpdateSlowState(bool bIsSlow);
	void UpdateReverseState(bool bIsReverse);
	void UpdateBlindState(bool bIsBlind);

public:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> StageInfoText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveInfoText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeInfoText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> SlowPanel;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ReversePanel;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCanvasPanel> BlindPanel;
};
