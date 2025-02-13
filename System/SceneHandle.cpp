// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SceneHandle.h"
#include "Kismet/GameplayStatics.h"
#include "System/BaseGameInstance.h"

USceneHandle::USceneHandle() : Super()
{
	
}

void USceneHandle::InitializeSceneHandle()
{
	CurrentScene = ESceneName::TitleScene;
	World = GetWorld();
	BaseGameInstance = Cast<UBaseGameInstance>(GetOuter());
}

ESceneName USceneHandle::GetCurrentScene() const
{
	return CurrentScene;
}

void USceneHandle::OpenScene(ESceneName SceneName)
{
	if (CurrentScene == SceneName)
	{
		return;
	}

	if (!IsValid(World))
	{
		return;
	}

	BaseGameInstance->SceneFadeOut();
	CurrentScene = SceneName;

	FTimerHandle FadeTimer;

	World->GetTimerManager().SetTimer(FadeTimer, FTimerDelegate::CreateLambda([&]()
		{
			switch (CurrentScene)
			{
			case ESceneName::TitleScene:
				UGameplayStatics::OpenLevel(World, TitleSceneName);
				break;

			case ESceneName::EasyLevelScene:
				UGameplayStatics::OpenLevel(World, EasySceneName);
				break;

			case ESceneName::NormalLevelScene:
				UGameplayStatics::OpenLevel(World, NormalScnenName);
				break;

			case ESceneName::HardLevelScene:
				UGameplayStatics::OpenLevel(World, HardSceneName);
				break;

			case ESceneName::EndingScene:
				UGameplayStatics::OpenLevel(World, EndingSceneName);
				break;
			}
		}), 0.5f, false);
}

void USceneHandle::MoveNextLevel()
{
	switch (CurrentScene)
	{
	case ESceneName::TitleScene:
		OpenScene(ESceneName::EasyLevelScene);
		break;

	case ESceneName::EasyLevelScene:
		OpenScene(ESceneName::NormalLevelScene);
		break;

	case ESceneName::NormalLevelScene:
		OpenScene(ESceneName::HardLevelScene);
		break;

	case ESceneName::HardLevelScene:
		OpenScene(ESceneName::EndingScene);
		break;
	}
}

void USceneHandle::SetWorld(UWorld* world)
{
	World = world;
}

void USceneHandle::CheckCurrentScene()
{
	if (!IsValid(World))
	{
		return;
	}

	FString WorldName = UGameplayStatics::GetCurrentLevelName(World);

	if (FName(WorldName) == TitleSceneName)
	{
		CurrentScene = ESceneName::TitleScene;
	}
	else if (FName(WorldName) == EasySceneName)
	{
		CurrentScene = ESceneName::EasyLevelScene;
	}
	else if (FName(WorldName) == NormalScnenName)
	{
		CurrentScene = ESceneName::NormalLevelScene;
	}
	else if (FName(WorldName) == HardSceneName)
	{
		CurrentScene = ESceneName::HardLevelScene;
	}
	else if (FName(WorldName) == EndingSceneName)
	{
		CurrentScene = ESceneName::EndingScene;
	}
}
