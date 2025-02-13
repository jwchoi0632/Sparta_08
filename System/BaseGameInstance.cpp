// Fill out your copyright notice in the Description page of Project Settings.


#include "System/BaseGameInstance.h"
#include "System/BaseGameState.h"
#include "System/SceneHandle.h"
#include "System/UIHandle.h"
#include <Kismet/KismetSystemLibrary.h>

UBaseGameInstance::UBaseGameInstance() : Super()
{
	//SceneHandle = NewObject<USceneHandle>();
}

void UBaseGameInstance::AddTotalScore(int32 AddValue)
{
	TotalScore += AddValue;
}

int32 UBaseGameInstance::GetTotalScore()
{
	return TotalScore;
}

void UBaseGameInstance::AddPlayTime(int32 AddValue)
{
	PlayTime += AddValue;
}

int32 UBaseGameInstance::GetPlayTime()
{
	return PlayTime;
}

ESceneName UBaseGameInstance::GetCurrentSceneName()
{
	if (!IsValid(SceneHandle))
	{
		return ESceneName::Faild;
	}
	SceneHandle->CheckCurrentScene();

	return SceneHandle->GetCurrentScene();
}

void UBaseGameInstance::PlayClickedUISound()
{
	if (!IsValid(SoundHandle))
	{
		return;
	}
	SoundHandle->PlayUISound();
}

void UBaseGameInstance::PlaySoundAtAudioComp(EAudioCategory AudioCategory, UAudioComponent* AudioComp, int32 index)
{
	if (!IsValid(SoundHandle))
	{
		return;
	}
	SoundHandle->PlaySoundByCategory(AudioCategory, index, AudioComp);
}

//UAudioComponent* UBaseGameInstance::PlayEffectSound(EAudioCategory AudioCategory, int32 index, AActor* Owner)
//{
//	if (!IsValid(SoundHandle))
//	{
//		return nullptr;
//	}
//	return SoundHandle->PlayEffectSound(AudioCategory, index, Owner);
//}
//
//void UBaseGameInstance::StopEffectSound(UAudioComponent* AudioComp)
//{
//	if (!IsValid(SoundHandle))
//	{
//		return;
//	}
//	SoundHandle->StopEffectSound(AudioComp);
//}

void UBaseGameInstance::SetBGMVolume(float VolumeValue)
{
	if (!IsValid(SoundHandle))
	{
		return;
	}
	SoundHandle->SetBGMVolume(VolumeValue);
}

void UBaseGameInstance::SetEffectVolume(float VolumeValue)
{
	if (!IsValid(SoundHandle))
	{
		return;
	}
	SoundHandle->SetEffectVolume(VolumeValue);
}

USceneHandle* UBaseGameInstance::GetSceneHandle()
{
	if (!IsValid(SceneHandle))
	{
		return nullptr;
	}
	return SceneHandle;
}

UUIHandle* UBaseGameInstance::GetUIHandle()
{
	if (!IsValid(UIHandle))
	{
		return nullptr;
	}
	return UIHandle;
}

USoundHandle* UBaseGameInstance::GetSoundHandle()
{
	if (!IsValid(SoundHandle))
	{
		return nullptr;
	}
	return SoundHandle;
}

void UBaseGameInstance::Init()
{
	Super::Init();
	SceneHandle = NewObject<USceneHandle>(this);
	UIHandle = NewObject<UUIHandle>(this);
	SoundHandle = NewObject<USoundHandle>(this);

	if (IsValid(SceneHandle))
	{
		SceneHandle->InitializeSceneHandle();
	}

	if (IsValid(UIHandle))
	{
		UIHandle->InitializeUIHandle();
	}

	if (IsValid(SoundHandle))
	{
		SoundHandle->InitializeSoundHandle();
	}

	TotalScore = 0;
	PlayTime = 0;
}

void UBaseGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);

	if (!IsValid(NewWorld))
	{
		//UE_LOG(LogTemp, Warning, TEXT("GameInstance Not Changed World. NewWorld Valid False"));
		return;
	}
	FWorldContext* NewWorldContext = const_cast<FWorldContext*>(GEngine->GetWorldContextFromWorld(NewWorld));

	if (!IsValid(SceneHandle))
	{
		//UE_LOG(LogTemp, Warning, TEXT("SceneHandle Not Changed World. SceneHandle Valid False"));
		return;
	}
	SceneHandle->SetWorld(NewWorldContext->World());

	if (!IsValid(UIHandle))
	{
		//UE_LOG(LogTemp, Warning, TEXT("UIHandle Not Changed World. UI Valid False"));
		return;
	}
	UIHandle->SetWorld(NewWorldContext->World());
}

void UBaseGameInstance::QuitGame()
{
	//게임이 종료될 때 필요한 기능 처리.
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UBaseGameInstance::StartedGameState()
{
	BaseGameState = Cast<ABaseGameState>(GetWorld()->GetGameState());
	AddObserverToGameState();
	OnUIByScene();
	PlayBGMByScene();
}

void UBaseGameInstance::SceneFadeIn()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->FadeIn();
}

void UBaseGameInstance::DestroyedGameState()
{
	BaseGameState = nullptr;
}

void UBaseGameInstance::OnPause()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->OpenOptionUI();
}

void UBaseGameInstance::AddObserverToGameState()
{
	if (!IsValid(UIHandle))
	{
		return;
	}

	for (auto& Observer : UIHandle->GetUIObservers())
	{
		BaseGameState->RegisterGameStateObserver(Observer);
	}
}

void UBaseGameInstance::OnUIByScene()
{
	if (!IsValid(SceneHandle))
	{
		return;
	}
	if (!IsValid(UIHandle))
	{
		return;
	}
	SceneFadeIn();
	SceneHandle->CheckCurrentScene();
	UIHandle->AddToViewportByScene(SceneHandle->GetCurrentScene());
}

void UBaseGameInstance::PlayBGMByScene()
{
	if (!IsValid(SceneHandle))
	{
		return;
	}
	if (!IsValid(SoundHandle))
	{
		return;
	}
	SceneHandle->CheckCurrentScene();
	SoundHandle->InitAudioComp();
	SoundHandle->PlayBGM(SceneHandle->GetCurrentScene());
}

void UBaseGameInstance::InitActorAudioComp(EAudioCategory AudioCategory, AActor* Owner)
{
	if (!IsValid(SoundHandle))
	{
		return;
	}
	SoundHandle->InitAudioCompAtOwner(AudioCategory, Owner);
}

void UBaseGameInstance::ChangeCursorMode(bool bIsVisible)
{
	if (!IsValid(BaseGameState))
	{
		return;
	}
	BaseGameState->ChangedCursorMode(bIsVisible);
}

void UBaseGameInstance::EndStage()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->OpenResult();
}

void UBaseGameInstance::SceneFadeOut()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	UIHandle->FadeOut();
}

void UBaseGameInstance::StopBGM()
{
	if (!IsValid(SoundHandle))
	{
		return;
	}
	SoundHandle->StopBGM();
}

void UBaseGameInstance::MoveToNextLevel()
{
	if (!IsValid(SceneHandle))
	{
		return;
	}
	SceneHandle->MoveNextLevel();
}

void UBaseGameInstance::MoveToTitle()
{
	if (!IsValid(SceneHandle))
	{
		return;
	}
	SceneHandle->OpenScene(ESceneName::TitleScene);
	TotalScore = 0;
}

