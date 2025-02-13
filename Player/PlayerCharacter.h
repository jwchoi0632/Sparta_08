// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class ABaseGameState;

UCLASS()
class SPARTAPRJ06_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void PauseGame(const FInputActionValue& Value);
	UFUNCTION()
	void MoveNextLevel(const FInputActionValue& Value);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void AddHp(int32 AddValue);

	int32 GetMaxHp();
	int32 GetCurrentHp();
	void SetMoveSpeed(float Speed );
	float GetMoveSpeed();
	void SetIsReverse(bool Value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotateSpeed = 1.5;

protected:
	TObjectPtr<ABaseGameState> BaseGameState;
	int32 MaxHp;
	int32 CurrentHp;
	bool bIsReverse = false;
};
