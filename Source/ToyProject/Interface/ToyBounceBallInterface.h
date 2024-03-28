// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ToyBounceBallInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UToyBounceBallInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOYPROJECT_API IToyBounceBallInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void BoomAttack() = 0;
	virtual void DamageByBoomAttack() = 0;
};
