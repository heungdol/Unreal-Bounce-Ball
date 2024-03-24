// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "ToyCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOYPROJECT_API UToyCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UToyCameraComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	//UPROPERTY ()
	//class ACharacter* TargetCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bFOVOffsetByCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float PrevFOVOffset;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float DesiredFOVOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float CurrentFOVOffset;

	//

	//UPROPERTY (EditDefaultsOnly, BlueprintReadWrite)
	//float FOVOffsetMin = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	float FOVOffsetMax = 20;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	float FOVOffsetInterpSpeed = 1;

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//float FOVOffsetMin = -5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	float CharacterMovespeedMax = 1000;
};
