// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ToyJellyEffectComponent.h"

#include "ToyCharacterJellyEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOYPROJECT_API UToyCharacterJellyEffectComponent : public UToyJellyEffectComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToyCharacterJellyEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickJellyEffectByBounceBallVelocity();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jelly Effect Character")
	TObjectPtr <class ACharacter> Character;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jelly Effect Character")
	//TObjectPtr <class UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect Character")
	float MaxVelocityForScale = 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect Character")
	//float MaxScaleZ = 2f;
	TObjectPtr <class UCurveVector> ScaleByRatio;
};
