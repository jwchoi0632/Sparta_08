// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/EnumSetClass.h"
#include "SoundHandle.generated.h"

class UBaseGameInstance;

UCLASS()
class SPARTAPRJ06_API USoundHandle : public UObject
{
	GENERATED_BODY()
	
public:
	USoundHandle();

	void InitializeSoundHandle();
	void InitAudioComp();
	void InitAudioCompAtOwner(EAudioCategory CategoryName, AActor* Owner);
	UAudioComponent* CreateAudioComp(AActor* Owner);
	void SetWorld(UWorld* world);

	void SetBGMVolume(float VolumeValue);
	void SetEffectVolume(float VolumeValue);
	float GetBGMVolume();
	float GetEffectVolume();

	void PlaySound(UAudioComponent* AudioComp, TArray<TObjectPtr<USoundBase>>& SoundArray, int32 index, AActor* Owner = nullptr);
	void PlaySoundByCategory(EAudioCategory CategoryName, int32 index, UAudioComponent* AudioComp = nullptr, AActor* Owner = nullptr);
	void PlayBGM(ESceneName SceneName);
	void PlayUISound();
	void PlaySoundAtLocation(EAudioCategory CategoryName, int32 index, FVector Location);
	/*UAudioComponent* PlayEffectSound(EAudioCategory CategoryName, int32 index, AActor* Owner);
	void StopEffectSound(UAudioComponent* AudioComp);*/
	void StopBGM();

protected:
	void LoadBGMSet();
	void LoadUISet();
	void LoadPlayerSet();
	void LoadMonsterSet();
	void LoadItemSet();
	void LoadEtcSet();

protected:
	UPROPERTY(VisibleAnywhere, Category = "AudioComp")
	TObjectPtr<UAudioComponent> BGMAudioComp;
	UPROPERTY(VisibleAnywhere, Category = "AudioComp")
	TObjectPtr<UAudioComponent> UIAudioComp;
	UPROPERTY(VisibleAnywhere, Category = "AudioComp")
	TObjectPtr<UAudioComponent> PlayerAudioComp;
	UPROPERTY(VisibleAnywhere, Category = "AudioComp")
	TArray<TObjectPtr<UAudioComponent>> MonsterAudioComps;
	UPROPERTY(VisibleAnywhere, Category = "AudioComp")
	TArray<TObjectPtr<UAudioComponent>> ItemAudioComps;
	UPROPERTY(VisibleAnywhere, Category = "AudioComp")
	TArray<TObjectPtr<UAudioComponent>> AudioCompPool;
	UPROPERTY(VisibleAnywhere, Category = "AudioComp")
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioCompPool;

	UPROPERTY(VisibleAnywhere, Category = "SoundSet")
	TArray<TObjectPtr<USoundBase>> BGMSet;
	UPROPERTY(VisibleAnywhere, Category = "SoundSet")
	TArray<TObjectPtr<USoundBase>> UISet;
	UPROPERTY(VisibleAnywhere, Category = "SoundSet")
	TArray<TObjectPtr<USoundBase>> PlayerSet;
	UPROPERTY(VisibleAnywhere, Category = "SoundSet")
	TArray<TObjectPtr<USoundBase>> MonsterSet;
	UPROPERTY(VisibleAnywhere, Category = "SoundSet")
	TArray<TObjectPtr<USoundBase>> ItemSet;
	UPROPERTY(VisibleAnywhere, Category = "SoundSet")
	TArray<TObjectPtr<USoundBase>> EtcSet;

	TObjectPtr<UWorld> World;
	TObjectPtr<UBaseGameInstance> BaseGameInstance;

	float BGMVolume = 1;
	float EffectVolume = 1;
};
