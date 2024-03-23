// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GameplayAbility_CharacterStomp.h"

#include "GAS/AT/AbilityTask_MoveToGround.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

UGameplayAbility_CharacterStomp::UGameplayAbility_CharacterStomp()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	bIsActivaed = false;
}

void UGameplayAbility_CharacterStomp::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_CharacterStomp::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

bool UGameplayAbility_CharacterStomp::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool ActivateResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return ActivateResult;
}

void UGameplayAbility_CharacterStomp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		UAbilityTask_MoveToGround* AbilityTask = UAbilityTask_MoveToGround::MoveToGround
		(this, TEXT("Stomp"), StompMoveSpeed);

		AbilityTask->OnGroundReached.AddDynamic(this, &UGameplayAbility_CharacterStomp::EndAbilityCallback);
		AbilityTask->ReadyForActivation();

		//MyCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_None);
		//MyCharacterMovementComponent->Velocity = FVector::ZeroVector;
		bIsActivaed = true;
	}
	else
	{
		// Can not activate this ability 
		EndAbilityCallback();
	}
}

void UGameplayAbility_CharacterStomp::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGameplayAbility_CharacterStomp::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

			// 땅에 닿을 시 튀어오르도록
			MyCharacter->LaunchCharacter(FVector(0, 0, StompBouncePower), false, true);
		}
	}
}

void UGameplayAbility_CharacterStomp::EndAbilityCallback()
{
	// 네트워크 관련 인자
	bool bReplicatedEndAbility = true;
	// 취소된 경우
	bool bWasCancelled = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

//void UGameplayAbility_CharacterStomp::StompBounceCallback()
//{
//	ACharacter* MyCharacter = nullptr;
//	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
//
//	if (CurrentActorInfo != nullptr)
//	{
//		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
//	}
//
//	if (MyCharacter != nullptr)
//	{
//		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
//	}
//
//	if (MyCharacterMovementComponent != nullptr)
//	{
//		MyCharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
//
//		// 땅에 닿을 시 튀어오르도록
//		MyCharacter->LaunchCharacter(FVector(0, 0, StompBouncePower), false, true);
//	}
//}
