// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ToyJellyEffectData.h"

#include "Curves/CurveVector.h"

UToyJellyEffectData::UToyJellyEffectData()
{

}

FVector UToyJellyEffectData::GetLocationVectorByRatio(float InRatio)
{
	FVector ResultVector = FVector::ZeroVector;

	if (LerpCurveLocationVector == nullptr)
	{
		return ResultVector;
	}

	InRatio = FMath::Clamp(InRatio, 0.0f, 1.0f);
	ResultVector = LerpCurveLocationVector->GetVectorValue(InRatio);

	return ResultVector;

}

FVector UToyJellyEffectData::GetRotationVectorByRatio(float InRatio)
{
	FVector ResultRotator = FVector::ZeroVector;

	if (LerpCurveRotationVector == nullptr)
	{
		return ResultRotator;
	}

	InRatio = FMath::Clamp(InRatio, 0.0f, 1.0f);
	ResultRotator = LerpCurveRotationVector->GetVectorValue(InRatio);

	return ResultRotator;
}

FVector UToyJellyEffectData::GetScaleVectorByRatio(float InRatio)
{
	FVector ResultVector = FVector::OneVector;

	if (LerpCurveScaleVector == nullptr)
	{
		return ResultVector;
	}

	InRatio = FMath::Clamp(InRatio, 0.0f, 1.0f);
	ResultVector = LerpCurveScaleVector->GetVectorValue(InRatio);

	return ResultVector;
}
