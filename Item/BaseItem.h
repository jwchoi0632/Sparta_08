// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemInterface.h"
#include "Components/AudioComponent.h"
#include "System/BaseGameState.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPRJ06_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAudioComponent> AudioComp;

	UFUNCTION()
	virtual void OnOverlapItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnEndOverlapItem(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndext) override;

	UFUNCTION()
	virtual void ActionOverlapEvent(AActor* User) override;

	virtual FName GetItemType() const override;
	virtual void DestroyItem();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FName ItemType;

	bool bIsTriggered = false;
	TObjectPtr<ABaseGameState> BaseGameState;
	FTimerHandle TimerHandle;

	virtual void BeginPlay() override;

};
