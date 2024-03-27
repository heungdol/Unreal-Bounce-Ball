
// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ToyCharacterJellyEffectComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "Curves/CurveVector.h"

// Sets default values for this component's properties
UToyCharacterJellyEffectComponent::UToyCharacterJellyEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UToyCharacterJellyEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast <ACharacter>(GetOwner());
}


// Called every frame
void UToyCharacterJellyEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickJellyEffectByBounceBallVelocity();
}

void UToyCharacterJellyEffectComponent::TickJellyEffectByBounceBallVelocity()
{
	if (GetMeshComponent() == nullptr)
	{
		return;
	}
	
	if (Character == nullptr
		|| Character->GetCharacterMovement() == nullptr)
	{
		return;
	}
	
	if (ScaleByRatio == nullptr)
	{
		return;
	}

	float CurrentRatio = Character->GetCharacterMovement()->Velocity.Z / MaxVelocityForScale;
	CurrentRatio = FMath::Abs(CurrentRatio);
	CurrentRatio = FMath::Clamp(CurrentRatio, 0.0f, 1.0f);

	FVector NewScale = GetMeshComponent()->GetRelativeScale3D();
	NewScale *= ScaleByRatio->GetVectorValue(CurrentRatio);
	GetMeshComponent()->SetRelativeScale3D(NewScale);

	//FVector VelocityNormal = Character->GetCharacterMovement()->Velocity;
	//VelocityNormal.Normalize();

	//FRotator NewRotation = GetMeshComponent()->GetRelativeRotation();
	//NewRotation = VelocityNormal.Rotation() + NewRotation;
	//GetMeshComponent()->SetRelativeRotation(NewRotation);
}

