// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/EnumSetClass.h"
#include "SceneHandle.generated.h"

class UBaseGameInstance;

UCLASS()
class SPARTAPRJ06_API USceneHandle : public UObject
{
	GENERATED_BODY()
	
public:
	USceneHandle();
	void InitializeSceneHandle();
	ESceneName GetCurrentScene() const;
	void OpenScene(ESceneName SceneName);
	void MoveNextLevel();
	void SetWorld(UWorld* world);
	void CheckCurrentScene();

private:


public:


private:
	const FName TitleSceneName = FName("TitleLevel");
	const FName EasySceneName = FName("BasicLevel");
	const FName NormalScnenName = FName("IntermediateLevel");
	const FName HardSceneName = FName("AdvancedLevel");
	const FName EndingSceneName = FName("EndingLevel");

	ESceneName CurrentScene;
	TObjectPtr<UWorld> World;
	TObjectPtr<UBaseGameInstance> BaseGameInstance;
};
