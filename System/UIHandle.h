// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "System/EnumSetClass.h"
#include "Observer/GameStateObserver.h"
#include "UIHandle.generated.h"

class UBaseWidget;
class UBaseGameInstance;

UCLASS()
class SPARTAPRJ06_API UUIHandle : public UObject
{
	GENERATED_BODY()
	
public:
	UUIHandle();

	void InitializeUIHandle();
	void SetWorld(UWorld* world);
	void AddToViewportByScene(ESceneName SceneName);
	void AddToViewportByName(EWidgetName WidgetName);
	void FadeIn();
	void FadeOut();
	void OpenOptionUI();
	void CloseOptionUI();
	void OpenResult();
	void OpenPausedUI();

	TArray<TScriptInterface<IGameStateObserver>>& GetUIObservers();

	void ClickedMoveToTitle();
	void ClickedMoveToNext();
	void ClickedQuitGame();
	void ClickedBGMVolume(float VolumeValue);
	void ClickedEffectVolume(float VolumeValue);

	bool IsTitleScene();
	int32 GetPlayTime();

protected:
	void CallChangeCursorMode(bool bIsVisible);

public:
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UBaseWidget> TitleWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UBaseWidget> InGameWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UBaseWidget> EndingWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UBaseWidget> FadeWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UBaseWidget> OptionsWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UBaseWidget> ResultWidgetClass;

	TObjectPtr<UWorld> World;
	TObjectPtr<UBaseGameInstance> BaseGameInstance;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBaseWidget> TitleWidget;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBaseWidget> InGameWidget;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBaseWidget> EndingWidget;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBaseWidget> FadeWidget;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBaseWidget> OptionsWidget;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBaseWidget> ResultWidget;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UBaseWidget> CurrentWidget;

	FTimerHandle FadeTimer;
	FTimerHandle OptionsTimer;

	TArray<TScriptInterface<IGameStateObserver>> UIObservers;

	bool bOpenedOptions = false;
};
