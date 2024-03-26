// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AT/AbilityTask_MoveToGround.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

//UAbilityTask_MoveToGround::UAbilityTask_MoveToGround(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	bTickingTask = true;
//	bSimulatedTask = true;
//	bIsFinished = false;
//}

UAbilityTask_MoveToGround* UAbilityTask_MoveToGround::MoveToGround(UGameplayAbility* OwningAbility, FName TaskInstanceName, float InputMoveSpeed/*, float InputBouncePower*/)
{
	UAbilityTask_MoveToGround* MyObj = NewAbilityTask<UAbilityTask_MoveToGround>(OwningAbility, TaskInstanceName);

	MyObj->MoveSpeed = InputMoveSpeed;
	//MyObj->BouncePower = InputBouncePower;

	MyObj->bTickingTask = true;
	MyObj->bSimulatedTask = true;
	MyObj->bIsFinished = false;

	return MyObj;
}

void UAbilityTask_MoveToGround::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
}

void UAbilityTask_MoveToGround::Activate()
{
	Super::Activate();

	//UE_LOG(LogTemp, Log, TEXT("Move to Ground Task Activate"));

	//AActor* MyActor = GetAvatarActor();
	//ACharacter* MyCharacter = nullptr;
	//UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	//if (MyActor)
	//{
	//	MyCharacter = Cast <ACharacter>(MyActor);
	//	if (MyCharacter)
	//	{
	//		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	//	}
	//}
}

void UAbilityTask_MoveToGround::TickTask(float DeltaTime)
{
	//UE_LOG(LogTemp, Log, TEXT("Move to Ground Task Tick... Pre"));

	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);

	AActor* MyActor = GetAvatarActor();
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	//UE_LOG(LogTemp, Log, TEXT("Move to Ground Task Tick..."));

	if (MyActor)
	{
		MyCharacter = Cast <ACharacter>(MyActor);
		if (MyCharacter)
		{
			MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
		}
	}

	if (IsValid(MyCharacterMovementComponent) 
		&& MyCharacterMovementComponent->IsFalling() == false)
	{
		bIsFinished = true;

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGroundReached.Broadcast();
		}

		EndTask();
	}
	else
	{
		//MyCharacterMovementComponent->Velocity.Z = MoveSpeed * -1.0f;
		MyCharacterMovementComponent->Velocity = FVector(0, 0, MoveSpeed * -1.0f/* * DeltaTime*/);
		MyCharacterMovementComponent->UpdateComponentVelocity();

		//MyActor->AddActorWorldOffset(FVector(0, 0, MoveSpeed * -1.0f * DeltaTime));
	}
	
	//else
	//{
	//	bIsFinished = true;
	//	EndTask();
	//}
}


