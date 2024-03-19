// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ToyBounceBall.generated.h"

UCLASS()
class TOYPROJECT_API AToyBounceBall : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AToyBounceBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// Ball Setting
public:
	UPROPERTY (EditDefaultsOnly, Category=BounceBall)
	TObjectPtr <class UStaticMeshComponent> BounceBallMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	float BallRadius = 50;  

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	float BouncePower = 2000.0f;


	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr <class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr <class UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputAction> InputActionMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputAction> InputActionA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputAction> InputActionB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputAction> InputActionLT;

	void Move(const struct FInputActionValue& Value);

	void BounceWhenHittingGround();

	void FixXVelocityZero();
	void FixXLocationZero();
	
	void ActActionA();
	void ActActionB();
	void ActActionLT();

};
