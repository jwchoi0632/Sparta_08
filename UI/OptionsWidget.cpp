// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionsWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "System/UIHandle.h"


void UOptionsWidget::MoveToTitle()
{
	ReturnGame();

	if (!IsValid(UIHandle))
	{
		return;
	}

	UIHandle->ClickedMoveToTitle();
	/*if (UIHandle->IsTitleScene())
	{
		ReturnGame();
	}
	else
	{
		ReturnGame();
		UIHandle->ClickedMoveToTitle();
	}*/
}

void UOptionsWidget::ReturnGame()
{
	if (!IsValid(UIHandle))
	{
		return;
	}
	PlayCloseOptionAnim();
	UIHandle->CloseOptionUI();
}

void UOptionsWidget::ChangedBGMVolume(float Value)
{
	if (!IsValid(UIHandle))
	{
		return;
	}

	float VolumeValue = Value;
	UIHandle->ClickedBGMVolume(VolumeValue);
}

void UOptionsWidget::ChangedEffectVolume(float Value)
{
	if (!IsValid(UIHandle))
	{
		return;
	}

	float VolumeValue = Value;
	UIHandle->ClickedEffectVolume(VolumeValue);
}

void UOptionsWidget::HoveredMoveTitleButton()
{
	MoveTitleButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UOptionsWidget::UnhoveredMoveTitleButton()
{
	MoveTitleButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UOptionsWidget::HoveredCloseButton()
{
	CloseButton->SetRenderScale(FVector2D(1.05f, 1.05f));
}

void UOptionsWidget::UnhoveredCloseButton()
{
	CloseButton->SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UOptionsWidget::InitWidget()
{
	Super::InitWidget();

	MoveTitleButton->OnHovered.AddDynamic(this, &UOptionsWidget::HoveredMoveTitleButton);
	MoveTitleButton->OnUnhovered.AddDynamic(this, &UOptionsWidget::UnhoveredMoveTitleButton);
	CloseButton->OnHovered.AddDynamic(this, &UOptionsWidget::HoveredCloseButton);
	CloseButton->OnUnhovered.AddDynamic(this, &UOptionsWidget::UnhoveredCloseButton);

	MoveTitleButton->OnClicked.AddDynamic(this, &UOptionsWidget::MoveToTitle);
	CloseButton->OnClicked.AddDynamic(this, &UOptionsWidget::ReturnGame);
	BGMSlider->OnValueChanged.AddDynamic(this, &UOptionsWidget::ChangedBGMVolume);
	EffectSlider->OnValueChanged.AddDynamic(this, &UOptionsWidget::ChangedEffectVolume);
}

void UOptionsWidget::Action()
{
	Super::Action();

	PlayOpenOptionAnim();
}

void UOptionsWidget::Update()
{
	Super::Update();

	PlayCloseOptionAnim();
}

void UOptionsWidget::PlayOpenOptionAnim()
{
	PlayAnimation(OpenOptionAnim);
}

void UOptionsWidget::PlayCloseOptionAnim()
{
	PlayAnimation(CloseOptionAnim);
}
