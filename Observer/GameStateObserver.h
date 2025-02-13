// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "System/EnumSetClass.h"
#include "GameStateObserver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameStateObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPARTAPRJ06_API IGameStateObserver
{
	GENERATED_BODY()

public:
	virtual void ChangedCurrentScoreNotify(int32 Score) = 0;
	virtual void ChangedTotalScoreNotify(int32 Score) = 0;
	virtual void ChangedCurrentHpNotify(int32 Hp) = 0;
	virtual void ChangedMaxHpNotify(int32 Hp) = 0;
	virtual void ChangedWaveTime(int32 TimeValue) = 0;
	virtual void ChangedWave(int32 WaveValue) = 0;
	virtual void ChangedStage(int32 StageValue) = 0;
	virtual void ChangedPlayerState(EDebuffState TargetState, bool bIsActive) = 0;
};
