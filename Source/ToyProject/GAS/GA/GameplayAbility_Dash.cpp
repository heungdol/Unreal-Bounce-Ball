// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GameplayAbility_Dash.h"

#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_MoveToLocation.h"


#include "Components/CapsuleComponent.h"

#include "Character/ToyBounceBall.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemBlueprintLibrary.h"


UGameplayAbility_Dash::UGameplayAbility_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
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

	UE_LOG(LogTemp, Log, TEXT("Activate Ability Dash"));

	AToyBounceBall* BounceBall = Cast <AToyBounceBall> (ActorInfo->AvatarActor);

	//if (BounceBall == nullptr)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Can not Find Pawn from Ability Dash"));
	//}

	if (BounceBall != nullptr && FMath::Abs (BounceBall->GetMoveInputXDirection()) > SMALL_NUMBER )
	{
		UE_LOG(LogTemp, Log, TEXT("Setting Ability Dash "));

		FVector EndLocation(0, 0, 0);
		EndLocation = BounceBall->GetActorLocation() + FVector(0, BounceBall->GetMoveInputXDirection() * DashDistance, 0);

		UAbilityTask_MoveToLocation* AbilityTask = UAbilityTask_MoveToLocation::MoveToLocation
		(this, TEXT("Dash"), EndLocation, DashDuration, DashCurveFloat, nullptr);

		AbilityTask->OnTargetLocationReached.AddDynamic(this, &UGameplayAbility_Dash::EndAbilityCallback);
		AbilityTask->ReadyForActivation();

		//if (BounceBall->GetCapsuleComponent() != nullptr)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("GetCapsuleComponent"));
		//	BounceBall->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UGameplayAbility_Dash::EndAbilityByHitCallback);
		//}
		//
		UE_LOG(LogTemp, Log, TEXT("ReadyForActivation Dash "));

		/*FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ActorInfo->AvatarActor, ActivationOwnedTags.GetByIndex(0), PayloadData);*/

		UE_LOG(LogTemp, Log, TEXT("Trigger Ability"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Can not Activate Dash Ability"));

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

	ACharacter* Ball = Cast <ACharacter>(ActorInfo->AvatarActor);
	if (Ball != nullptr)
	{
		UCharacterMovementComponent* BallMovementComponent = Ball->GetCharacterMovement();
		if (BallMovementComponent != nullptr)
		{
			BallMovementComponent->Velocity = FVector::ZeroVector;
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

	UE_LOG(LogTemp, Log, TEXT("End Ability Dash"));
}

//void UGameplayAbility_Dash::EndAbilityByHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	//// 네트워크 관련 인자
//	//bool bReplicatedEndAbility = true;
//	//// 취소된 경우
//	//bool bWasCancelled = false;
//
//	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
//
//	//UE_LOG(LogTemp, Log, TEXT("End Ability Dash"));
//}
