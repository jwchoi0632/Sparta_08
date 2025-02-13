// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "BasePlayer.generated.h"


UCLASS()
class SPARTAPRJ06_API ABasePlayer : public APawn
{
	GENERATED_BODY()

public:
	ABasePlayer();

	//mesh section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCapsuleComponent> ColliderComponent;

	//movement section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<class UFloatingPawnMovement> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Foot Collision")
	TObjectPtr<class UBoxComponent> FootCollision;

	//camera section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	//input section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UEnhancedInputComponent> EnhancedInputComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController")
	TObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, Category = "Move And Rotate")
	float RotateSpeed;

	UPROPERTY(EditAnywhere, Category = "Move And Rotate")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Move And Rotate")
	float JumpSpeed;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void StartTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	inline void AddHp(int32 AddValue) { CurrentHp = FMath::Clamp(CurrentHp + AddValue, 0, MaxHp); }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	int32 MaxHp;
	int32 CurrentHp;

	bool bIsHit;
	bool bIsWalkable;
	bool bIsJumping;
	bool bIsFalling;
	float Acceleration;
	float JumpStrength;
	float Gravity;

	float MoveSpeedOnJumpX;
	float MoveSpeedOnJumpY;
	FVector2D MovementVector;
	FVector MoveForwardDirectionOnJump;
	FVector MoveRightDirectionOnJump;
	FVector GroundActorVector;

	AActor* CurrentGround;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void MoveStop(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void Jump(const FInputActionValue& Value);

	UFUNCTION()
	void StopJump(const FInputActionValue& Value);

	float CalculateDirection(const FVector& Velocity, const FRotator& BaseRotation);
	void MoveToGroundLocation();
};
