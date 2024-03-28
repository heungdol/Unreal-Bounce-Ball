// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "Interface/ToyBounceBallInterface.h"

#include "ToyBounceBall.generated.h"

UCLASS()
class TOYPROJECT_API AToyBounceBall : public ACharacter, public IAbilitySystemInterface, public IToyBounceBallInterface
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

	virtual void OnRep_PlayerState() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION (BlueprintCallable)
	virtual void BoomAttack() override;
	
	virtual void DamageByBoomAttack() override;

// Ability System
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr <class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray <TSubclassOf <class UGameplayAbility>> GameplayStartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap <int32, TSubclassOf <class UGameplayAbility>> GameplayActionAbilities;

// Ball Setting
public:
	UPROPERTY (EditDefaultsOnly, Category=BounceBall)
	TObjectPtr <class UStaticMeshComponent> BounceBallMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Bounce Ball Jelly Effect Info")
	TObjectPtr <class UToyCharacterJellyEffectComponent> BounceBallJellyEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bounce Ball Jelly Effect Info")
	TObjectPtr <class UToyJellyEffectData> JellyEffectBoomAttackData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bounce Ball Jelly Effect Info")
	TObjectPtr <class UToyJellyEffectData> JellyEffectDamageByBoomAttackData;


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


	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr <class UToySpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr <class UToyCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UInputAction> InputActionMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<int32, TObjectPtr <class UInputAction>> InputPressedActions;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//TMap<int32, TObjectPtr <class UInputAction>> InputReleasedActions;

	UPROPERTY()
	float InputAxisMoveX = 0;

	// Functions
public:
	void MoveByAxisInput(const struct FInputActionValue& Value);

	void FixXVelocityZero();
	void FixXLocationZero();

	float GetInputAxisMoveX() const;

	void BindPressedActionByInputID(int32 InputID);
	void BindReleasedActionByInputID(int32 InputID);


	UFUNCTION ()
	void BallCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Ability로 뺄 예정
	void MoveRandom();

protected:
	void SetupGAS();
};
