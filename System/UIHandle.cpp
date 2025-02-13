// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UIHandle.h"
#include "UI/BaseWidget.h"
#include "System/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

UUIHandle::UUIHandle() : Super()
{
	
}

void UUIHandle::InitializeUIHandle()
{
	World = GetWorld();
	BaseGameInstance = Cast<UBaseGameInstance>(GetOuter());
	CurrentWidget = nullptr;

	TitleWidgetClass = LoadClass<UBaseWidget>(this, TEXT("/Game/BluePrints/UI/WBP_TitleUI.WBP_TitleUI_C"));
	InGameWidgetClass = LoadClass<UBaseWidget>(this, TEXT("/Game/BluePrints/UI/WBP_InGameUI.WBP_InGameUI_C"));
	EndingWidgetClass = LoadClass<UBaseWidget>(this, TEXT("/Game/BluePrints/UI/WBP_EndingUI.WBP_EndingUI_C"));
	FadeWidgetClass = LoadClass<UBaseWidget>(this, TEXT("/Game/BluePrints/UI/WBP_FadeUI.WBP_FadeUI_C"));
	OptionsWidgetClass = LoadClass<UBaseWidget>(this, TEXT("/Game/BluePrints/UI/WBP_OptionsWidget.WBP_OptionsWidget_C"));
	ResultWidgetClass = LoadClass<UBaseWidget>(this, TEXT("/Game/BluePrints/UI/BP_ResultUI.BP_ResultUI_C"));

	if (IsValid(TitleWidgetClass))
	{
		TitleWidget = CreateWidget<UBaseWidget>(BaseGameInstance, TitleWidgetClass);
	}

	if (IsValid(InGameWidgetClass))
	{
		InGameWidget = CreateWidget<UBaseWidget>(BaseGameInstance, InGameWidgetClass);
	}

	if (IsValid(EndingWidgetClass))
	{
		EndingWidget = CreateWidget<UBaseWidget>(BaseGameInstance, EndingWidgetClass);
	}

	if (IsValid(FadeWidgetClass))
	{
		FadeWidget = CreateWidget<UBaseWidget>(BaseGameInstance, FadeWidgetClass);
	}

	if (IsValid(OptionsWidgetClass))
	{
		OptionsWidget = CreateWidget<UBaseWidget>(BaseGameInstance, OptionsWidgetClass);
	}

	if (IsValid(ResultWidgetClass))
	{
		ResultWidget = CreateWidget<UBaseWidget>(BaseGameInstance, ResultWidgetClass);
	}

	if (IsValid(TitleWidget))
	{
		TitleWidget->InitWidget();
	}

	if (IsValid(OptionsWidget))
	{
		OptionsWidget->InitWidget();
	}

	if (IsValid(InGameWidget))
	{
		InGameWidget->InitWidget();
		UIObservers.Add(InGameWidget);
	}

	if (IsValid(EndingWidget))
	{
		EndingWidget->InitWidget();
		UIObservers.Add(EndingWidget);
	}

	if (IsValid(ResultWidget))
	{
		ResultWidget->InitWidget();
		UIObservers.Add(ResultWidget);
	}
}

void UUIHandle::SetWorld(UWorld* world)
{
	World = world;
}

void UUIHandle::AddToViewportByScene(ESceneName Scene)
{
	switch (Scene)
	{
	case ESceneName::TitleScene:
		AddToViewportByName(EWidgetName::TitleWidget);
		break;

	case ESceneName::EasyLevelScene:
		AddToViewportByName(EWidgetName::InGameWidget);
		break;

	case ESceneName::NormalLevelScene:
		AddToViewportByName(EWidgetName::InGameWidget);
		break;

	case ESceneName::HardLevelScene:
		AddToViewportByName(EWidgetName::InGameWidget);
		break;

	case ESceneName::EndingScene:
		AddToViewportByName(EWidgetName::EndingWidget);
		break;
	}
}

void UUIHandle::AddToViewportByName(EWidgetName WidgetName)
{
	if (IsValid(CurrentWidget))
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	switch (WidgetName)
	{
	case EWidgetName::TitleWidget:
		CurrentWidget = TitleWidget;
		CallChangeCursorMode(true);
		break;

	case EWidgetName::InGameWidget:
		CurrentWidget = InGameWidget;
		InGameWidget->Update();
		CallChangeCursorMode(false);
		break;

	case EWidgetName::EndingWidget:
		CurrentWidget = EndingWidget;
		EndingWidget->Update();
		CallChangeCursorMode(true);
		break;
	}

	if (!IsValid(CurrentWidget))
	{
		return;
	}
	
	if (!IsValid(BaseGameInstance->GetGameViewportClient()))
	{
		return;
	}

	CurrentWidget->AddToViewport();

	if (CurrentWidget == EndingWidget)
	{
		EndingWidget->Action();
	}
}

void UUIHandle::FadeIn()
{
	if (!IsValid(FadeWidget))
	{
		return;
	}
	FadeWidget->AddToViewport(10);
	FadeWidget->Action();

	World->GetTimerManager().ClearTimer(FadeTimer);
	World->GetTimerManager().SetTimer(FadeTimer, FTimerDelegate::CreateLambda([&]()
		{
			FadeWidget->RemoveFromViewport();
		}), 0.5f, false);
}

void UUIHandle::FadeOut()
{
	if (!IsValid(FadeWidget))
	{
		return;
	}
	FadeWidget->AddToViewport(10);
	FadeWidget->Update();
	CallChangeCursorMode(true);
}

void UUIHandle::OpenOptionUI()
{
	if (!IsValid(OptionsWidget))
	{
		return;
	}

	OptionsWidget->AddToViewport(1);
	OptionsWidget->Action();
	BaseGameInstance->PlayClickedUISound();//
	CallChangeCursorMode(true);
}

void UUIHandle::CloseOptionUI()
{
	if (!IsValid(OptionsWidget))
	{
		return;
	}

	World->GetTimerManager().ClearTimer(OptionsTimer);
	OptionsWidget->Update();
	BaseGameInstance->PlayClickedUISound();//

	World->GetTimerManager().SetTimer(OptionsTimer, FTimerDelegate::CreateLambda([&]()
		{
			OptionsWidget->RemoveFromViewport();
		}), 0.5f, false);

	if (!IsValid(BaseGameInstance))
	{
		return;
	}

	if (BaseGameInstance->GetCurrentSceneName() != ESceneName::TitleScene)
	{
		CallChangeCursorMode(false);
	}
}

void UUIHandle::OpenResult()
{
	if (!IsValid(ResultWidget))
	{
		return;
	}

	ResultWidget->Action();
	ResultWidget->AddToViewport(2);
	CallChangeCursorMode(true);
}

TArray<TScriptInterface<IGameStateObserver>>& UUIHandle::GetUIObservers()
{
	return UIObservers;
}

void UUIHandle::ClickedMoveToTitle()
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	BaseGameInstance->MoveToTitle();
	BaseGameInstance->PlayClickedUISound();
}

void UUIHandle::ClickedMoveToNext()
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	BaseGameInstance->MoveToNextLevel();
	BaseGameInstance->PlayClickedUISound();
}

void UUIHandle::ClickedQuitGame()
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	BaseGameInstance->QuitGame();
	BaseGameInstance->PlayClickedUISound();
}

void UUIHandle::ClickedBGMVolume(float VolumeValue)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	BaseGameInstance->SetBGMVolume(VolumeValue);
	BaseGameInstance->PlayClickedUISound();
}

void UUIHandle::ClickedEffectVolume(float VolumeValue)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	BaseGameInstance->SetEffectVolume(VolumeValue);
	BaseGameInstance->PlayClickedUISound();
}

bool UUIHandle::IsTitleScene()
{
	if (!IsValid(BaseGameInstance))
	{
		return false;
	}
	return BaseGameInstance->GetCurrentSceneName() == ESceneName::TitleScene;
}

int32 UUIHandle::GetPlayTime()
{
	if (!IsValid(BaseGameInstance))
	{
		return -1;
	}
	return BaseGameInstance->GetPlayTime();
}

void UUIHandle::CallChangeCursorMode(bool bIsVisible)
{
	if (!IsValid(BaseGameInstance))
	{
		return;
	}
	BaseGameInstance->ChangeCursorMode(bIsVisible);
}

void UUIHandle::OpenPausedUI()
{
	if (IsTitleScene())
	{
		return;
	}
	OpenOptionUI();
}