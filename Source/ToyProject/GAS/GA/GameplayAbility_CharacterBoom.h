// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_CharacterBoom.generated.h"

/** Notification delegate definition for when the gameplay ability is cancelled */
DECLARE_MULTICAST_DELEGATE(FOnGameplayAbilityBegin);

//DECLARE_MULTICAST_DELEGATE (F)

/**
 * 
 */
UCLASS()
class TOYPROJECT_API UGameplayAbility_CharacterBoom : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_CharacterBoom();

public:
	/** Input binding stub. */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Input binding stub. */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Destroys instanced-per-execution abilities. Instance-per-actor abilities should 'reset'. Any active ability state tasks receive the 'OnAbilityStateInterrupted' event. Non instance abilities - what can we do? */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	TSubclassOf <class UDamageType> BoomDamageType;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	float BoomDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	float BoomRange = 500.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = GAS)
	float BoomPower = 2000.0f;

	FOnGameplayAbilityBegin OnGameplayAbilityBegin;

};
