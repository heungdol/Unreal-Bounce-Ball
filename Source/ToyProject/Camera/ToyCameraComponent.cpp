// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ToyCameraComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "Math/UnrealMathUtility.h"

UToyCameraComponent::UToyCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
	bFOVOffsetByCharacterMovement = false;
}

void UToyCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);

	//UE_LOG(LogTemp, Log, TEXT("ToyCamera: GetCameraView"));

	if (bFOVOffsetByCharacterMovement == false)
	{
		return;
	}

	ACharacter* Character = Cast <ACharacter>(GetOwner());
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	if (Character != nullptr)
	{
		CharacterMovementComponent = Character->GetCharacterMovement();
	}

	if (CharacterMovementComponent != nullptr)
	{
		FVector CurrentCharacterVelocity = CharacterMovementComponent->Velocity;
		float CurrentCharacterVelocityLenght = CurrentCharacterVelocity.Length();

		DesiredFOVOffset = UKismetMathLibrary::MapRangeClamped
		(CurrentCharacterVelocityLenght, 0, CharacterMovespeedMax, 0, FOVOffsetMax);

		CurrentFOVOffset = FMath::FInterpTo(PrevFOVOffset, DesiredFOVOffset, DeltaTime, FOVOffsetInterpSpeed);

		PrevFOVOffset = CurrentFOVOffset;

		DesiredView.FOV = DesiredView.FOV + CurrentFOVOffset;
	}
}

//void UToyCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	//UE_LOG(LogTemp, Log, TEXT("ToyCamera: TickComponent"));
//}
//



