// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/ToySpringArmComponent.h"

void UToySpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = GetOwner();
	if (Actor != nullptr)
	{
		DesiredCameraZLocation = Actor->GetActorLocation().Z;

		DesiredCameraYLocation
			= CurrentCameraYLocation
			= PrevCameraYLocation
			= Actor->GetActorLocation().Y;
	}
}

void UToySpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);

	if (bIsUsingCustomLag == false)
	{
		return;
	}

	AActor* Actor = GetOwner();
	if (Actor == nullptr)
	{
		return;
	}

	FVector DesiredLocation = FVector::ZeroVector;

	float CurrentOffset = Actor->GetActorLocation().Z - DesiredCameraZLocation;
	if (FMath::Abs(CurrentOffset) > MaxCameraZOffset)
	{
		DesiredCameraZLocation = Actor->GetActorLocation().Z + -FMath::Sign(CurrentOffset) * MaxCameraZOffset;
	}
	
	DesiredLocation.Z = DesiredCameraZLocation;

	DesiredCameraYLocation = Actor->GetActorLocation().Y;
	CurrentCameraYLocation = FMath::FInterpTo(PrevCameraYLocation, DesiredCameraYLocation, DeltaTime, CameraYInterpSpeed);
	DesiredLocation.Y = CurrentCameraYLocation;
	PrevCameraYLocation = CurrentCameraYLocation;

	SetWorldLocation(DesiredLocation);
}
