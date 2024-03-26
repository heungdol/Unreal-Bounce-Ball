// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GameplayAbility_CharacterDash.h"

#include "GAS/AT/AbilityTask_MoveSweepToLocation.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemBlueprintLibrary.h"


UGameplayAbility_CharacterDash::UGameplayAbility_CharacterDash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UGameplayAbility_CharacterDash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_CharacterDash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UGameplayAbility_CharacterDash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool ActivateResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	ACharacter* MyCharacter = nullptr;
	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(GetAvatarActorFromActorInfo());
	}
	APlayerController* MyPlayerController = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	FVector InputVector = FVector::ZeroVector;

	MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}
	
	if (MyCharacterMovementComponent != nullptr)
	{
		InputVector = MyCharacter->GetPendingMovementInputVector();
	}

	return ActivateResult;
}

void UGameplayAbility_CharacterDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	
	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		FVector EndLocation(0, 0, 0);
		EndLocation = MyCharacter->GetActorLocation() + FVector(0, DashDistance, 0);

		UAbilityTask_MoveSweepToLocation* AbilityTask = UAbilityTask_MoveSweepToLocation::MoveSweepToLocation
		(this, TEXT("Dash"), EndLocation, DashDuration, DashCurveFloat, nullptr);

		AbilityTask->OnTargetLocationReached.AddDynamic(this, &UGameplayAbility_CharacterDash::EndAbilityCallback);
		AbilityTask->ReadyForActivation();

		bIsActivaed = true;
	}
	else
	{
		// Can not activate this ability 
		EndAbilityCallback();
	}
}

void UGameplayAbility_CharacterDash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGameplayAbility_CharacterDash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (bIsActivaed == (int8)true)
	{
		bIsActivaed = false;

		ACharacter* MyCharacter = nullptr;
		UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

		if (ActorInfo != nullptr)
		{
			MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
		}

		if (MyCharacter != nullptr)
		{
			MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
		}
	}
}

void UGameplayAbility_CharacterDash::EndAbilityCallback()
{
	// 네트워크 관련 인자
	bool bReplicatedEndAbility = true;
	// 취소된 경우
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
