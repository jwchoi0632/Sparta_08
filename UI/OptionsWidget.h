// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "OptionsWidget.generated.h"

class UButton;
class USlider;

UCLASS()
class SPARTAPRJ06_API UOptionsWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void MoveToTitle();
	UFUNCTION()
	void ReturnGame();
	UFUNCTION()
	void ChangedBGMVolume(float Value);
	UFUNCTION()
	void ChangedEffectVolume(float Value);
	UFUNCTION()
	void HoveredMoveTitleButton();
	UFUNCTION()
	void UnhoveredMoveTitleButton();
	UFUNCTION()
	void HoveredCloseButton();
	UFUNCTION()
	void UnhoveredCloseButton();

	virtual void InitWidget() override;
	virtual void Action() override;
	virtual void Update() override;

	void PlayOpenOptionAnim();
	void PlayCloseOptionAnim();

public:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenOptionAnim;
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> CloseOptionAnim;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveTitleButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USlider> BGMSlider;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<USlider> EffectSlider;
};
