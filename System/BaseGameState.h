// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Observer/GameStateObserver.h"
#include "BaseGameState.generated.h"

class UBaseGameInstance;
class ABasePlayerController;
class AItemSpawner;
class AMeteorSpawner;
class AObstaclesSpawner;
class APlayerCharacter;

UCLASS()
class SPARTAPRJ06_API ABaseGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ABaseGameState();
	virtual void BeginDestroy() override;
	void AddScore(int32 AddValue);
	int32 GetScore();
	void IncreaseCurrentCoin();
	void StartStage();
	void ChangeWave();
	void EndStage(bool bIsDie);
	void SetCurrentItemSpawner(AItemSpawner* Spawner);
	void SetCurrentMeteorSpawner(AMeteorSpawner* Spawner);
	void SetCurrentObstaclesSpawner(AObstaclesSpawner* Spawner);
	void BeginPlayPlayer(APlayerCharacter* PlayerCharacter);
	void UpdatedHp();
	void InputPauseKey();
	void ChangedCursorMode(bool bIsVisible);
	void SetPlayerAudioComp();
	void SetItemAudioComp(AActor* Item);
	void PlayItemEffectSound(int32 index, UAudioComponent* AudioComp);
	void PlayPlayerEffectSound(int32 index);
	AActor* GetPlayer();

	void StartSlow(float SlowRate);
	void EndSlow();
	void StartReverse();
	void EndReverse();

	void OnTriggeredPortal();
	void EndTriggeredPortal();
	void UsePortal();
	//UAudioComponent* PlayItemEffectSound(int32 index, AActor* Owner);
	//void StopItemEffectSound(UAudioComponent* AudioComp);

	void RegisterGameStateObserver(TScriptInterface<IGameStateObserver> Observer);
	void UnregisterGameStateObserver(TScriptInterface<IGameStateObserver> Observer);

	void NotifyCurrentScore();
	void NotifyTotalScore();
	void NotifyCurrentHp(int32 Hp);
	void NotifyMaxHp(int32 Hp);
	void NotifyStageTime(int32 Time);
	void NotifyStageNum(int32 Num);
	void NotifyWaveNum(int32 Num);
	void NotifyDebuff(EDebuffState TargetDebuff, bool bIsActive);

protected:
	virtual void BeginPlay() override;
	void SpawnBaseItem();
	void SpawnObstacles();
	void SpawnRandomExplosion();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	TSubclassOf<AActor> Portal;

protected:
	TArray<TScriptInterface<IGameStateObserver>> GameStateObservers;
	TObjectPtr<UBaseGameInstance> BaseGameInstance;
	TObjectPtr<AItemSpawner> ItemSpawner;
	TObjectPtr<AMeteorSpawner> MeteorSpawner;
	TObjectPtr<AObstaclesSpawner> ObstaclesSpawner;
	TObjectPtr<APlayerCharacter> PlayerPawn;
	TObjectPtr<ABasePlayerController> BasePlayerController;

	int32 Score;
	int32 TotalCoin;
	int32 CurrentCoin;
	int32 StageTime;
	int32 WaveTime;
	int32 StageNum;
	int32 WaveNum;
	const int32 MaxWave = 3;
	const int32 WaveChangeTime = 30;
	FTimerHandle StageTimer;
	bool bIsPause = false;
	bool bIsOverlapedPortal = false;
};
