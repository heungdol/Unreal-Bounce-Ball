// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_MoveSweepToLocation.h"

#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"

#include "Engine/World.h"

#include "Components/CapsuleComponent.h"


UAbilityTask_MoveSweepToLocation* UAbilityTask_MoveSweepToLocation::MoveSweepToLocation
(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, float Duration, UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve)
{
	UAbilityTask_MoveSweepToLocation* MyObj = NewAbilityTask<UAbilityTask_MoveSweepToLocation>(OwningAbility, TaskInstanceName);

	if (MyObj->GetAvatarActor() != nullptr)
	{
		MyObj->StartLocation = MyObj->GetAvatarActor()->GetActorLocation();
	}

	MyObj->TargetLocation = Location;
	MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);		// Avoid negative or divide-by-zero cases
	MyObj->TimeMoveStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeMoveWillEnd = MyObj->TimeMoveStarted + MyObj->DurationOfMovement;
	MyObj->LerpCurve = OptionalInterpolationCurve;
	MyObj->LerpCurveVector = OptionalVectorInterpolationCurve;

	return MyObj;
}


void UAbilityTask_MoveSweepToLocation::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
}

void UAbilityTask_MoveSweepToLocation::Activate()
{
	Super::Activate();

	PrevLocation = StartLocation;
	
	AActor* MyActor = GetAvatarActor();
	if (MyActor)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter && MyCharacter->GetCapsuleComponent())
		{
			MyCharacter->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &UAbilityTask_MoveSweepToLocation::HitCallback);
		}
	}
}

void UAbilityTask_MoveSweepToLocation::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		//if (MyActor)
		//{
		//	FVector NewLocation = StartCharacterLocation + (TargetLocation - PrevLocation);
		//	MyActor->SetActorLocation(NewLocation, true);
		//}

		return;
	}

	UGameplayTask::TickTask(DeltaTime);

	AActor* MyActor = GetAvatarActor();
	//FVector CurrentCharacterLocation = FVector::ZeroVector;
	if (MyActor)
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->SetMovementMode(MOVE_Custom, 0);
			}

			//CurrentCharacterLocation = MyCharacter->GetActorLocation();
		}

		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd)
		{
			bIsFinished = true;

			// Teleport in attempt to find a valid collision spot
			//MyActor->TeleportTo(TargetLocation, MyActor->GetActorRotation());

			FVector OffsetLocation = /*CurrentCharacterLocation + */(TargetLocation - PrevLocation);
			MyActor->AddActorWorldOffset(OffsetLocation, true);
		
			if (!bIsSimulating)
			{
				MyActor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnTargetLocationReached.Broadcast();
				}
				EndTask();
			}
		}
		else
		{
			FVector CurrentLocation;

			float MoveFraction = (CurrentTime - TimeMoveStarted) / DurationOfMovement;
			if (LerpCurveVector)
			{
				const FVector ComponentInterpolationFraction = LerpCurveVector->GetVectorValue(MoveFraction);
				CurrentLocation = FMath::Lerp<FVector, FVector>(StartLocation, TargetLocation, ComponentInterpolationFraction);
			}
			else
			{
				if (LerpCurve)
				{
					MoveFraction = LerpCurve->GetFloatValue(MoveFraction);
				}

				CurrentLocation = FMath::Lerp<FVector, float>(StartLocation, TargetLocation, MoveFraction);
			}

			FVector OffsetLocation = /*CurrentCharacterLocation + */(CurrentLocation - PrevLocation);
			MyActor->AddActorWorldOffset(OffsetLocation, true);

			PrevLocation = CurrentLocation;
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UAbilityTask_MoveSweepToLocation::HitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Log, TEXT("HitCallback from Ability Task"));

	bIsFinished = true;

	if (!bIsSimulating)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnTargetLocationReached.Broadcast();
		}

		EndTask();
	}
}
