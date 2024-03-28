// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GameplayAbility_CharacterBoom.h"

//#include "Interface/ToyBounceBallInterface.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

// 커스텀 대미지 타입
//#include "Attack/DamageType_Boom.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UGameplayAbility_CharacterBoom::UGameplayAbility_CharacterBoom()
{
}

void UGameplayAbility_CharacterBoom::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_CharacterBoom::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_CharacterBoom::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 네트워크 관련 인자
	bool bReplicatedEndAbility = true;
	// 취소된 경우
	bool bWasCancelled = false;
	
	ACharacter* SourceCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	if (SourceCharacter == nullptr)
	{
		bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);

		return;
	}

	//IToyBounceBallInterface* BounceBall = Cast <IToyBounceBallInterface>(Pawn);
	//if (BounceBall == nullptr)
	//{
	//	bWasCancelled = true;
	//	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);

	//	return;
	//}

	OnGameplayAbilityBegin.Broadcast();

	TArray <FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, SourceCharacter);

	FVector Center = SourceCharacter->GetActorLocation();

	bool bResult = GetWorld()->OverlapMultiByChannel
	(OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(BoomRange), CollisionQueryParam);

	if (bResult)
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			//IToyBounceBallInterface* BounceBall = Cast <IToyBounceBallInterface>(OverlapResult.GetActor());
			//if (BounceBall == nullptr)
			//{
			//	continue;
			//}

			ACharacter* TargetCharacter = Cast <ACharacter>(OverlapResult.GetActor());
			if (TargetCharacter == nullptr)
			{
				continue;
			}

			FVector BoomDifference = TargetCharacter->GetActorLocation() - SourceCharacter->GetActorLocation();
			FVector BoomDirection = TargetCharacter->GetActorLocation() - SourceCharacter->GetActorLocation();
			BoomDirection.Normalize();

			float BoomRatio = BoomDifference.Length() / FMath::Clamp (BoomRange, 0.1f, BoomRange);
			BoomRatio = 1 - BoomRatio;

			TargetCharacter->LaunchCharacter(BoomDirection * BoomPower * BoomRatio, true, true);

			FDamageEvent DamageEvent (BoomDamageType);
			TargetCharacter->TakeDamage(BoomDamage, DamageEvent, SourceCharacter->GetController(), SourceCharacter);
		}
	}
	
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGameplayAbility_CharacterBoom::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGameplayAbility_CharacterBoom::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
