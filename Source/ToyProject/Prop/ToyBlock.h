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

	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;


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

	//UFUNCTION()
	//void OnRep_IsActiveBlock ();

	UFUNCTION()
	void OnRep_CurrentHealth();

public:
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = "Block Info")
	TObjectPtr <class UStaticMeshComponent> BlockStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Block Info")
	TObjectPtr <class UBoxComponent> BlockBoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Block Info")
	FVector BlockSize = FVector (100, 100, 100);
	
	UPROPERTY (EditDefaultsOnly, BlueprintReadWrite, Category = "Block Info")
	int32 MaxHealth = 100;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere, Category = "Block Info")
	int32 CurrentHealth = 100;

	//UPROPERTY(ReplicatedUsing = OnRep_IsActiveBlock, VisibleAnywhere, Category = "Block Info")
	//bool bIsActiveBlock;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Block Info")
	float RespawnTime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Block Jelly Effect Info")
	TObjectPtr <class UToyJellyEffectComponent> JellyEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Block Jelly Effect Info")
	TObjectPtr <class UToyJellyEffectData> JellyEffectDamageData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Block Jelly Effect Info")
	TObjectPtr <class UToyJellyEffectData> JellyEffectRespawnData;


	UPROPERTY ()
	FTimerHandle RespawnTimerHandle;
};
