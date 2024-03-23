// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GameplayAbility_Dash.h"

//#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "GAS/AT/AbilityTask_MoveSweepToLocation.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemBlueprintLibrary.h"


UGameplayAbility_Dash::UGameplayAbility_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bIsActivaed = false;
}

void UGameplayAbility_Dash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_Dash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UGameplayAbility_Dash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool ActivateResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	return ActivateResult;
}

void UGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	FVector InputVector = FVector::ZeroVector;

	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
		InputVector = MyCharacter->GetPendingMovementInputVector();

		//UE_LOG(LogTemp, Log, TEXT("Assess To Character "));
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	//UE_LOG(LogTemp, Log, TEXT("%f"), InputVector.Y);

	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr
		&& FMath::Abs (InputVector.Y) > SMALL_NUMBER )
	{
		FVector EndLocation(0, 0, 0);
		EndLocation = MyCharacter->GetActorLocation() + FVector(0, FMath::Sign (InputVector.Y) * DashDistance, 0);

		UAbilityTask_MoveSweepToLocation* AbilityTask = UAbilityTask_MoveSweepToLocation::MoveSweepToLocation
		(this, TEXT("Dash"), EndLocation, DashDuration, DashCurveFloat, nullptr);

		AbilityTask->OnTargetLocationReached.AddDynamic(this, &UGameplayAbility_Dash::EndAbilityCallback);
		AbilityTask->ReadyForActivation();

		MyCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_None);
		bIsActivaed = true;
	}
	else
	{
		// Can not activate this ability 
		EndAbilityCallback();
	}
}

void UGameplayAbility_Dash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGameplayAbility_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

		if (MyCharacterMovementComponent != nullptr)
		{
			MyCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
			MyCharacterMovementComponent->Velocity = FVector::ZeroVector;
		}
	}
}

void UGameplayAbility_Dash::EndAbilityCallback()
{
	// 네트워크 관련 인자
	bool bReplicatedEndAbility = true;
	// 취소된 경우
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}