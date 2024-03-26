// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_MoveSweepToLocation.h"

#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "ToyProject.h"

#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

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

	MyObj->bTickingTask = true;
	MyObj->bSimulatedTask = true;
	MyObj->bIsFinished = false;

	//MyObj->ForceNetUpdate();


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
	//if (GetWorld()->GetNetMode() == NM_Client)
	//{
	//	return;
	//}
	//
	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* CharMoveComp = nullptr;
	if (MyActor)
	{
		MyCharacter = Cast<ACharacter>(MyActor);
		if (MyCharacter)
		{
			CharMoveComp = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
		}
	}

	//if (MyCharacter == nullptr || MyCharacter->IsLocallyControlled() == false)
	//{
	//	return;
	//}

	if (bIsFinished)
	{
		return;
	}

	UGameplayTask::TickTask(DeltaTime);
	
	if (MyActor)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeMoveWillEnd)
		{
			bIsFinished = true;

			// Teleport in attempt to find a valid collision spot
			//MyActor->TeleportTo(TargetLocation, MyActor->GetActorRotation());

			FVector OffsetLocation = /*CurrentCharacterLocation + */(TargetLocation - PrevLocation);
			//MyActor->AddActorWorldOffset(OffsetLocation, true);
			CharMoveComp->Velocity = OffsetLocation / DeltaTime;// FVector::ZeroVector;
			CharMoveComp->UpdateComponentVelocity();

		
			//if (!bIsSimulating)
			//{
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnTargetLocationReached.Broadcast();
				}
				EndTask();
			//}

			if (MyActor->GetLocalRole() == ROLE_Authority)
			{
				MyActor->ForceNetUpdate();
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
			PrevLocation = CurrentLocation;

			//UE_LOG(LogTemp, Log, TEXT("Start Location: %s, End Location %s")
			//	, *StartLocation.ToCompactString(), *TargetLocation.ToCompactString());

			//UE_LOG(LogTemp, Log, TEXT("%f %f %f"), OffsetLocation.X, OffsetLocation.Y, OffsetLocation.Z);

			//MyActor->AddActorWorldOffset(OffsetLocation,  true);
			CharMoveComp->Velocity = OffsetLocation / DeltaTime;// FVector::ZeroVector;
			CharMoveComp->UpdateComponentVelocity();

						
			//if (MyActor->GetLocalRole() == ROLE_Authority)
			//{
			//	MyActor->ForceNetUpdate();
			//}
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

	//if (!bIsSimulating)
	//{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnTargetLocationReached.Broadcast();
		}

		AActor* MyActor = GetAvatarActor();
		if (MyActor != nullptr && MyActor->GetLocalRole() == ROLE_Authority)
		{
			MyActor->ForceNetUpdate();
		}

		EndTask();
	//}
}

void UAbilityTask_MoveSweepToLocation::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(UAbilityTask_MoveSweepToLocation, StartLocation);
	DOREPLIFETIME(UAbilityTask_MoveSweepToLocation, TargetLocation);
	DOREPLIFETIME(UAbilityTask_MoveSweepToLocation, PrevLocation);
	DOREPLIFETIME(UAbilityTask_MoveSweepToLocation, DurationOfMovement);
	DOREPLIFETIME(UAbilityTask_MoveSweepToLocation, LerpCurve);
	DOREPLIFETIME(UAbilityTask_MoveSweepToLocation, LerpCurveVector);
}
