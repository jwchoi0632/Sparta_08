// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "EndingWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SPARTAPRJ06_API UEndingWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnMoveToTitle();
	UFUNCTION()
	void OnQuitGame();
	UFUNCTION()
	void HoveredMoveTitleButton();
	UFUNCTION()
	void UnhoveredMoveTitleButton();
	UFUNCTION()
	void HoveredQuitGameButton();
	UFUNCTION()
	void UnhoveredQuitGameButton();

	virtual void InitWidget() override;
	virtual void Action() override;
	virtual void Update() override;

private:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> EndingCredit;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveTitleButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Line18;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Line15;
};
