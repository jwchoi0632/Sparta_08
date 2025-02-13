// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "BaseDrone.generated.h"

UCLASS()
class SPARTAPRJ06_API ABaseDrone : public APawn
{
	GENERATED_BODY()

public:
	ABaseDrone();
	
	//components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> CollisionComponent;

	//movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<class UFloatingPawnMovement> MovementComponent;

	//camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	//input section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveUpDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveForwardBackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveLeftRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> TurnYawAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> TurnPitchAction;

	//player controller
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerController")
	TObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, Category = "Player Setting")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Player Setting")
	float RotateSpeed;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	const float RevisionMoveForwardMesh = 0.2f;
	const float RevisionMoveLeftMesh = 0.2f;
	const float RevisionMoveUpMesh = 0.4f;

	float Acceleration;
	float ForwardAcceleration;
	float LeftAcceleration;

	FVector MoveMeshDirection;
	//float Gravity;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveFB(const FInputActionValue& Value);
	UFUNCTION()
	void MoveLR(const FInputActionValue& Value);
	UFUNCTION()
	void MoveUD(const FInputActionValue& Value);
	UFUNCTION()
	void MoveUpDownEnd();
	UFUNCTION()
	void MoveForwardEnd();
	UFUNCTION()
	void MoveLeftEnd();
	UFUNCTION()
	void TurnYaw(const FInputActionValue& Value);
	UFUNCTION()
	void TurnPitch(const FInputActionValue& Value);
};
