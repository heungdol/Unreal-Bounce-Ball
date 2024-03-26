// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayAbility_InputTag.h"

#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UMyGameplayAbility_InputTag::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UMyGameplayAbility_InputTag::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	EndAbilityCallback(0.0f);
}

void UMyGameplayAbility_InputTag::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UAbilityTask_WaitInputRelease* AbilityTask = UAbilityTask_WaitInputRelease::WaitInputRelease (this);

	//AbilityTask->OnRelease.AddDynamic(this, &UMyGameplayAbility_InputTag::EndAbilityCallback);
	//AbilityTask->ReadyForActivation();
}

void UMyGameplayAbility_InputTag::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UMyGameplayAbility_InputTag::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	UE_LOG(LogTemp, Log, TEXT("End Ability"));
}

void UMyGameplayAbility_InputTag::EndAbilityCallback(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	UE_LOG(LogTemp, Log, TEXT("End Callback"));
}
