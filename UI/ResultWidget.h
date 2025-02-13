// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidget.h"
#include "ResultWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SPARTAPRJ06_API UResultWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnNextLevelButton();
	UFUNCTION()
	void OnMoveTitleButton();
	UFUNCTION()
	void OnExitGameButton();
	UFUNCTION()
	void HoveredMoveSceneButton();
	UFUNCTION()
	void UnhoveredMoveSceneButton();
	UFUNCTION()
	void HoveredExitButton();
	UFUNCTION()
	void UnhoveredExitButton();

	virtual void InitWidget() override;
	virtual void Action() override;

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveSceneButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveButtonText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentScoreText;
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalScoreText;
};
