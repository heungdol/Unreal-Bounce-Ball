// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToyJellyEffectData.generated.h"

/**
 * 
 */
UCLASS()
class TOYPROJECT_API UToyJellyEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UToyJellyEffectData();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	float JellyEffectTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	TObjectPtr<class UCurveVector> LerpCurveLocationVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	TObjectPtr<class UCurveVector> LerpCurveRotationVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jelly Effect")
	TObjectPtr<class UCurveVector> LerpCurveScaleVector;


public:
	FORCEINLINE float GetJellyEffectTime() { return JellyEffectTime; }

	UFUNCTION()
	FVector GetLocationVectorByRatio(float InRatio);
	
	UFUNCTION()
	FVector GetRotationVectorByRatio(float InRatio);

	UFUNCTION ()
	FVector GetScaleVectorByRatio(float InRatio);

};
