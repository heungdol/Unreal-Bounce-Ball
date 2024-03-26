// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToyBounceBallActionData.generated.h"

/**
 * 
 */
UCLASS()
class TOYPROJECT_API UToyBounceBallActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	//UFUNCTION()
	//TSubclassOf <class UGameplayAbility> GetAbilityByInputAxis(float InputAxis) const;

	UFUNCTION()
	void GiveAbilitiesToASC(class UAbilitySystemComponent* ASC);

	//UFUNCTION()
	//void 

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS_Data)
	int32 InputID = 0;
	
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = GAS_Data)
	bool bIsSeperatedAbilityByInputAxis = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS_Data)
	TSubclassOf <class UGameplayAbility> LeftInputAxisAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS_Data)
	TSubclassOf <class UGameplayAbility> NeutralInputAxisAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS_Data)
	TSubclassOf <class UGameplayAbility> RightInputAxisAbility;

};
