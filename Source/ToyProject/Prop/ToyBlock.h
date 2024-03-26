// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/ToyBlockInterface.h"

#include "ToyBlock.generated.h"

UCLASS()
class TOYPROJECT_API AToyBlock : public AActor, public IToyBlockInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToyBlock();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostInitProperties() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetBlockActive(bool InActive) override;
	virtual void DamageBlock(int32 InDamage) override;
	virtual void DestroyBlock() override;
	virtual void ResetBlockState() override;

	UFUNCTION()
	void OnRep_IsActiveBlock ();

public:
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = Block)
	TObjectPtr <class UStaticMeshComponent> BlockStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Block)
	TObjectPtr <class UBoxComponent> BlockBoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Block)
	FVector BlockSize = FVector (100, 100, 100);
	
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = Block)
	int32 MaxHealth = 100;

	UPROPERTY(Replicated, VisibleAnywhere, Category = Block)
	int32 CurrentHealth = 100;

	UPROPERTY(ReplicatedUsing = OnRep_IsActiveBlock, VisibleAnywhere, Category = Block)
	bool bIsActiveBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Block)
	float RespawnTime = 10.0f;

	UPROPERTY ()
	FTimerHandle RespawnTimerHandle;
};
