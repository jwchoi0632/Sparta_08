// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SoundHandle.h"
#include "System/BaseGameInstance.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USoundHandle::USoundHandle() : Super()
{

}

void USoundHandle::InitializeSoundHandle()
{
	BaseGameInstance = Cast<UBaseGameInstance>(GetOuter());
	
	LoadBGMSet();
	LoadUISet();
	LoadPlayerSet();
	LoadMonsterSet();
	LoadItemSet();
	LoadEtcSet();
}

void USoundHandle::InitAudioComp()
{
	if (!IsValid(BGMAudioComp))
	{
		BGMAudioComp = UGameplayStatics::CreateSound2D(BaseGameInstance, BGMSet[0], BGMVolume);
		BGMAudioComp->bAutoDestroy = false;
	}
	
	if (!IsValid(UIAudioComp))
	{
		UIAudioComp = UGameplayStatics::CreateSound2D(BaseGameInstance, UISet[0], EffectVolume);
		UIAudioComp->bAutoDestroy = false;
	}

	PlayerAudioComp = nullptr;
	MonsterAudioComps.Empty();
	ItemAudioComps.Empty();
}

void USoundHandle::InitAudioCompAtOwner(EAudioCategory CategoryName, AActor* Owner) //풀링?
{
	if (!IsValid(Owner))
	{
		return;
	}

	switch (CategoryName)
	{
	case EAudioCategory::Player:
		PlayerAudioComp = CreateAudioComp(Owner);
		break;

	case EAudioCategory::Monster:
		MonsterAudioComps.Add(CreateAudioComp(Owner));
		break;

	case EAudioCategory::Item:
		ItemAudioComps.Add(CreateAudioComp(Owner));
		break;
	}
}

UAudioComponent* USoundHandle::CreateAudioComp(AActor* Owner)
{
	//TObjectPtr<UAudioComponent> NewAudioComp = NewObject<UAudioComponent>(Owner);
	TObjectPtr<UAudioComponent> NewAudioComp = Owner->GetComponentByClass<UAudioComponent>();
	//NewAudioComp->SetupAttachment(Owner->GetRootComponent());
	NewAudioComp->RegisterComponent();
	NewAudioComp->SetVolumeMultiplier(EffectVolume);
	NewAudioComp->bAutoDestroy = false;
	return NewAudioComp;
}

void USoundHandle::PlaySound(UAudioComponent* AudioComp, TArray<TObjectPtr<USoundBase>>& SoundArray, int32 index, AActor* Owner)
{
	if (!IsValid(AudioComp))
	{
		return;
	}

	if (!IsValid(SoundArray[index]))
	{
		return;
	}
	
	if (Owner != nullptr)
	{
		AudioComp->SetWorldLocation(Owner->GetActorLocation());
	}

	AudioComp->SetSound(SoundArray[index]);
	AudioComp->Play();
}

void USoundHandle::PlaySoundByCategory(EAudioCategory CategoryName, int32 index, UAudioComponent* AudioComp, AActor* Owner)
{
	switch (CategoryName)
	{
	case EAudioCategory::BGM:
		PlaySound(BGMAudioComp, BGMSet, index);
		break;

	case EAudioCategory::UI:
		PlaySound(UIAudioComp, UISet, index);
		break;

	case EAudioCategory::Player:
		PlaySound(PlayerAudioComp, PlayerSet, index);
		break;

	case EAudioCategory::Monster:
		PlaySound(AudioComp, MonsterSet, index);
		break;

	case EAudioCategory::Item:
		AudioComp->SetVolumeMultiplier(EffectVolume); //임시
		PlaySound(AudioComp, ItemSet, index);
		break;

	case EAudioCategory::Etc:

		break;
	}
}

void USoundHandle::PlayBGM(ESceneName SceneName)
{
	switch (SceneName)
	{
	case ESceneName::TitleScene:
		PlaySoundByCategory(EAudioCategory::BGM, 0);
		break;

	case ESceneName::EasyLevelScene:
		PlaySoundByCategory(EAudioCategory::BGM, 1);
		break;

	case ESceneName::NormalLevelScene:
		PlaySoundByCategory(EAudioCategory::BGM, 2);
		break;

	case ESceneName::HardLevelScene:
		PlaySoundByCategory(EAudioCategory::BGM, 3);
		break;

	case ESceneName::EndingScene:
		PlaySoundByCategory(EAudioCategory::BGM, 4);
		break;
	}
}

void USoundHandle::PlayUISound()
{
	PlaySoundByCategory(EAudioCategory::UI, 0);
}

void USoundHandle::PlaySoundAtLocation(EAudioCategory CategoryName, int32 index, FVector Location)
{

}

//UAudioComponent* USoundHandle::PlayEffectSound(EAudioCategory CategoryName, int32 index, AActor* Owner)
//{
//	if (!IsValid(Owner))
//	{
//		return;
//	}
//
//	TObjectPtr<UAudioComponent> AudioComp = nullptr;
//
//	if (AudioCompPool.Num() > 0)
//	{
//		AudioComp = AudioCompPool.Pop();
//		ActiveAudioCompPool.Add(AudioComp);
//	}
//	else
//	{
//		AudioComp = NewObject<UAudioComponent>(Owner);
//		AudioComp->RegisterComponent();
//		ActiveAudioCompPool.Add(AudioComp);
//	}
//
//	PlaySoundByCategory(CategoryName, index, AudioComp, Owner);
//
//	return AudioComp;
//}
//
//void USoundHandle::StopEffectSound(UAudioComponent* AudioComp)
//{
//	if (AudioComp == nullptr)
//	{
//		return;
//	}
//
//	ActiveAudioCompPool.Remove(AudioComp);
//	AudioCompPool.Add(AudioComp);
//	AudioComp->RegisterComponent();
//}

void USoundHandle::StopBGM()
{
	if (!IsValid(BGMAudioComp))
	{
		return;
	}
	BGMAudioComp->Stop();
}

void USoundHandle::LoadBGMSet()
{
	TObjectPtr<USoundBase> BGMSoundBase = nullptr;

	BGMSoundBase = LoadObject<USoundBase>(this, TEXT("/Game/Resources/Audio/Starter_Music01.Starter_Music01"));
	if (IsValid(BGMSoundBase))
	{
		BGMSet.Add(BGMSoundBase);
	}
	BGMSoundBase = nullptr;

	BGMSoundBase = LoadObject<USoundBase>(this, TEXT("/Game/Classical_Musicbox/CUE/CUE_Chopin_Nocturne_Op_27_No_2_Arranged_for_Music_Box_wav_Cue.CUE_Chopin_Nocturne_Op_27_No_2_Arranged_for_Music_Box_wav_Cue"));
	if (IsValid(BGMSoundBase))
	{
		BGMSet.Add(BGMSoundBase);
	}
	BGMSoundBase = nullptr;

	BGMSoundBase = LoadObject<USoundBase>(this, TEXT("/Game/Classical_Musicbox/CUE/CUE_Debussy_Arabesque_No_1_L66_Arranged_for_Music_Box_wav_Cue.CUE_Debussy_Arabesque_No_1_L66_Arranged_for_Music_Box_wav_Cue"));
	if (IsValid(BGMSoundBase))
	{
		BGMSet.Add(BGMSoundBase);
	}
	BGMSoundBase = nullptr;

	BGMSoundBase = LoadObject<USoundBase>(this, TEXT("/Game/Classical_Musicbox/CUE/CUE_Chopin_Fantaisie_Impromptu_Op_66_Arranged_for_Music_Box_wav_Cue.CUE_Chopin_Fantaisie_Impromptu_Op_66_Arranged_for_Music_Box_wav_Cue"));
	if (IsValid(BGMSoundBase))
	{
		BGMSet.Add(BGMSoundBase);
	}
	BGMSoundBase = nullptr;

	BGMSoundBase = LoadObject<USoundBase>(this, TEXT("/Game/Classical_Musicbox/CUE/CUE_Strauss_The_Blue_Danube_Op_314_Short_Version_Arranged_for_Music_Box_wav_Cue.CUE_Strauss_The_Blue_Danube_Op_314_Short_Version_Arranged_for_Music_Box_wav_Cue"));
	if (IsValid(BGMSoundBase))
	{
		BGMSet.Add(BGMSoundBase);
	}
	BGMSoundBase = nullptr;
}

void USoundHandle::LoadUISet()
{
	TObjectPtr<USoundBase> ClickedUI = nullptr;

	ClickedUI = LoadObject<USoundBase>(this, TEXT("/Game/UI_SFX_Free_Pack/Cues/Buttons/button_press_699_Cue.button_press_699_Cue"));
	if (IsValid(ClickedUI))
	{
		UISet.Add(ClickedUI);
	}
	ClickedUI = nullptr;
}

void USoundHandle::LoadPlayerSet()
{

}

void USoundHandle::LoadMonsterSet()
{

}

void USoundHandle::LoadItemSet()
{
	TObjectPtr<USoundBase> ItemEffectSound = nullptr;

	ItemEffectSound = LoadObject<USoundBase>(this, TEXT("/Game/Resources/Audio/Pickup_Coin_A_Cue.Pickup_Coin_A_Cue"));
	if (IsValid(ItemEffectSound))
	{
		ItemSet.Add(ItemEffectSound);
	}
	ItemEffectSound = nullptr;

	ItemEffectSound = LoadObject<USoundBase>(this, TEXT("/Game/Resources/Audio/Crate_Break_Cue.Crate_Break_Cue"));
	if (IsValid(ItemEffectSound))
	{
		ItemSet.Add(ItemEffectSound);
	}
	ItemEffectSound = nullptr;

	ItemEffectSound = LoadObject<USoundBase>(this, TEXT("/Game/Resources/Audio/Fire01_Cue.Fire01_Cue"));
	if (IsValid(ItemEffectSound))
	{
		ItemSet.Add(ItemEffectSound);
	}
	ItemEffectSound = nullptr;

	ItemEffectSound = LoadObject<USoundBase>(this, TEXT("/Game/Resources/Audio/Explosion_Cue.Explosion_Cue"));
	if (IsValid(ItemEffectSound))
	{
		ItemSet.Add(ItemEffectSound);
	}
	ItemEffectSound = nullptr;
}

void USoundHandle::LoadEtcSet()
{

}

void USoundHandle::SetWorld(UWorld* world)
{
	World = world;
}

void USoundHandle::SetBGMVolume(float VolumeValue)
{
	BGMVolume = VolumeValue;
	if (!IsValid(BGMAudioComp))
	{
		return;
	}
	BGMAudioComp->SetVolumeMultiplier(BGMVolume);
}

void USoundHandle::SetEffectVolume(float VolumeValue)
{
	EffectVolume = VolumeValue;

	if (IsValid(UIAudioComp))
	{
		UIAudioComp->SetVolumeMultiplier(EffectVolume);
	}

	if (IsValid(PlayerAudioComp))
	{
		PlayerAudioComp->SetVolumeMultiplier(EffectVolume);
	}

	if (MonsterAudioComps.Num() > 0)
	{
		for (auto& AudioComp : MonsterAudioComps)
		{
			if (!IsValid(AudioComp))
			{
				continue;
			}
			AudioComp->SetVolumeMultiplier(EffectVolume);
		}
	}

	if (ItemAudioComps.Num() > 0)
	{
		for (auto& AudioComp : ItemAudioComps)
		{
			if (!IsValid(AudioComp))
			{
				continue;
			}
			AudioComp->SetVolumeMultiplier(EffectVolume);
		}
	}
}

float USoundHandle::GetBGMVolume()
{
	return BGMVolume;
}

float USoundHandle::GetEffectVolume()
{
	return EffectVolume;
}


