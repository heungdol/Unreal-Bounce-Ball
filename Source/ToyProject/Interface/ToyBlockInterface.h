// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ToyBlockInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UToyBlockInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOYPROJECT_API IToyBlockInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetBlockActive(bool InActive) = 0;
	virtual void DamageBlock(int32 InDamage) = 0;
	virtual void DestroyBlock() = 0;
	virtual void ResetBlockState() = 0;
};
