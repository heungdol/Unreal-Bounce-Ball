// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"

#include "ToyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TOYPROJECT_API AToyPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AToyPlayerState();

// Ability System 
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr <class UAbilitySystemComponent> ASC;

	//UPROPERTY()
	//TObjectPtr <class UABCharacterAttributeSet> AttributeSet;

};
