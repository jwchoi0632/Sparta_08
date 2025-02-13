// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "TitleWidget.generated.h"

class UButton;

UCLASS()
class SPARTAPRJ06_API UTitleWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnStartButton();
	UFUNCTION()
	void OnOptionButton();
	UFUNCTION()
	void OnExitButton();
	UFUNCTION()
	void HoveredStartButton();
	UFUNCTION()
	void UnHoveredStartButton();
	UFUNCTION()
	void HoveredOptionsButton();
	UFUNCTION()
	void UnHoveredOptionsButton();
	UFUNCTION()
	void HoveredExitButton();
	UFUNCTION()
	void UnHoveredExitButton();

	virtual void InitWidget() override;

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> StartButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
};
