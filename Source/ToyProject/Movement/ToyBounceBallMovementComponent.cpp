// Fill out your copyright notice in the Description page of Project Settings.


#include "ToyBounceBallMovementComponent.h"

float UToyBounceBallMovementComponent::GetGravityZ() const
{
	float FinalGravityZ = Super::GetGravityZ();

	if (Velocity.Z > 0)
	{
		FinalGravityZ *= UppingGravityScale;
	}
	else
	{
		FinalGravityZ *= DowningGravityScale;
	}

	return FinalGravityZ;
}
