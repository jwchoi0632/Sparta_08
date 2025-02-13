// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "System/SceneHandle.h"
#include "System/UIHandle.h"
#include "System/SoundHandle.h"
#include "BaseGameInstance.generated.h"

class ABaseGameState;

UCLASS()
class SPARTAPRJ06_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBaseGameInstance();

	virtual void Init() override;
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

	void StartedGameState();
	void SceneFadeIn();
	void OnUIByScene();
	void PlayBGMByScene();
	void InitActorAudioComp(EAudioCategory AudioCategory, AActor* Owner);
	void OnPause();
	void ChangeCursorMode(bool bIsVisible);
	void EndStage();
	void SceneFadeOut();
	void StopBGM();
	void MoveToNextLevel();
	void MoveToTitle();
	void DestroyedGameState();
	void QuitGame();

	void AddTotalScore(int32 AddValue);
	int32 GetTotalScore();
	void AddPlayTime(int32 AddValue);
	int32 GetPlayTime();

	ESceneName GetCurrentSceneName();

	void PlayClickedUISound();
	void PlaySoundAtAudioComp(EAudioCategory AudioCategory, UAudioComponent* AudioComp, int32 index);
	//UAudioComponent* PlayEffectSound(EAudioCategory AudioCategory, int32 index, AActor* Owner);
	//void StopEffectSound(UAudioComponent* AudioComp);
	void SetBGMVolume(float VolumeValue);
	void SetEffectVolume(float VolumeValue);

	USceneHandle* GetSceneHandle();
	UUIHandle* GetUIHandle();
	USoundHandle* GetSoundHandle();
	
protected:
	void AddObserverToGameState();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Handle")
	TObjectPtr<USceneHandle> SceneHandle;
	UPROPERTY(VisibleAnywhere, Category = "Handle")
	TObjectPtr<UUIHandle> UIHandle;
	UPROPERTY(VisibleAnywhere, Category = "Handle")
	TObjectPtr<USoundHandle> SoundHandle;

	int32 TotalScore;
	int32 PlayTime;
	TObjectPtr<ABaseGameState> BaseGameState;
};
