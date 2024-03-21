// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"

#include "ToyBounceBall.generated.h"

UCLASS()
class TOYPROJECT_API AToyBounceBall : public ACharacter, public IAbilitySystemInterface
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

	virtual void PossessedBy(AController* NewController) override;

// Ability System
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr <class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray <TSubclassOf <class UGameplayAbility>> GameplayStartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap <int32, TSubclassOf <class UGameplayAbility>> GameplayActionAbilities;

	//UPROPERTY(EditAnywhere, Category = GAS)
	//TSubclassOf <class UGameplayAbility> GameplayAbilityActionB;

	//UPROPERTY(EditAnywhere, Category = GAS)
	//TSubclassOf <class UGameplayAbility> GameplayAbilityActionLT;

// Ball Setting
public:
	UPROPERTY (EditDefaultsOnly, Category=BounceBall)
	TObjectPtr <class UStaticMeshComponent> BounceBallMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	float BallRadius = 50;  

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	float BounceGroundPower = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	float BounceWallPower = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	float BounceWallPowerZ = 2000.0f;

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	bool bIsMoveRandom = false;

	UPROPERTY(EditDefaultsOnly, Category = BounceBall)
	float RandomPower = 2000.0f;



	UPROPERTY(BlueprintReadOnly)
	TObjectPtr <class UCharacterMovementComponent> BouneBallMovementComponent;


	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr <class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr <class UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputAction> InputActionMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<int32, TObjectPtr <class UInputAction>> InputActions;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr <class UInputAction> InputActionB;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr <class UInputAction> InputActionLT;


	void Move(const struct FInputActionValue& Value);

	float MoveInputXDirection = 0;

	FORCEINLINE float GetMoveInputXDirection() const { return MoveInputXDirection; }

	//void BounceWhenHittingGround();

	void FixXVelocityZero();
	void FixXLocationZero();
	
	//void ActActionA();
	//void ActActionB();
	//void ActActionLT();

	void ActActionByInputID(int32 InputID);

	UFUNCTION ()
	void BallCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Ability로 뺄 예정
	void MoveRandom();
	
};
