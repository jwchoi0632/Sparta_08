// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumSetClass.generated.h"

UENUM(BlueprintType)
enum class ESceneName : uint8
{
	TitleScene = 0,
	EasyLevelScene,
	NormalLevelScene,
	HardLevelScene,
	EndingScene,
	Faild
};

UENUM(BlueprintType)
enum class EWidgetName : uint8
{
	FadeWidget = 0,
	TitleWidget,
	InGameWidget,
	EndingWidget
};

UENUM(BlueprintType)
enum class EAudioCategory : uint8
{
	BGM = 0,
	UI,
	Player,
	Monster,
	Item,
	Etc
};

UENUM(BlueprintType)
enum class EDebuffState : uint8
{
	Slow = 0,
	Reverse,
	Blind
};

UCLASS()
class SPARTAPRJ06_API UEnumSetClass : public UObject
{
	GENERATED_BODY()
	
};