// Fill out your copyright notice in the Description page of Project Settings.


#include "ToyBounceBall.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Movement/ToyBounceBallMovementComponent.h"

#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Components/PrimitiveComponent.h"

#include "Player/ToyPlayerState.h"
#include "AbilitySystemComponent.h"

#include "AbilitySystemInterface.h"


// Sets default values
AToyBounceBall::AToyBounceBall()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 볼 메시 생성
	if (BounceBallMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Bounce Ball Mesh")))
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshRef 
		(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
		if (IsValid(SphereMeshRef.Object))
		{
			BounceBallMeshComponent->SetStaticMesh(SphereMeshRef.Object);
		}

		BounceBallMeshComponent->SetupAttachment(RootComponent);
		BounceBallMeshComponent->SetRelativeScale3D(FVector (BallRadius/50.0f));
	}

	// 기존 캡슐 크기 조정
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCapsuleRadius(BallRadius);
		GetCapsuleComponent()->SetCapsuleHalfHeight(BallRadius);
	}
	
	// 카메라 / 스프링 암 생성
	if (SpringArmComponent = CreateDefaultSubobject <USpringArmComponent>(TEXT("Camera Spring Arm")))
	{
		SpringArmComponent->SetupAttachment(RootComponent);
		SpringArmComponent->TargetArmLength = 3000.0f;
		
		SpringArmComponent->bEnableCameraLag = true;
		SpringArmComponent->CameraLagMaxDistance = 0.0f;
		SpringArmComponent->CameraLagSpeed = 1.0f;
	}

	if (CameraComponent = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera Component")))
	{
		CameraComponent->SetupAttachment(SpringArmComponent);
	}

	// TODO: 스크립트 내부에서 파생 Movement Component로 대처하는 과정
	////UCharacterMovementComponent* CharacterMovementComponent
	////	= GetMovementBase
	//UCharacterMovementComponent* NewCharacterMovementComponent
	//	= CreateDefaultSubobject <UToyBounceBallMovementComponent>(TEXT("Character Movement"));
	//UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();

	//if (NewCharacterMovementComponent)
	//{
	//	CharacterMovementComponent->DestroyComponent ();
	//	CharacterMovementComponent = NewCharacterMovementComponent;
	//}

	// 캐릭터 무브먼트 설정
	BouneBallMovementComponent = GetCharacterMovement();
	if (IsValid(BouneBallMovementComponent))
	{
		BouneBallMovementComponent->AirControl = 1;
		BouneBallMovementComponent->MaxWalkSpeed = 800;
		BouneBallMovementComponent->MaxAcceleration = 10000;

	}

	// 충돌 델리게이트 등록
	if (GetCapsuleComponent() != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Add Component Hit Delegate"));

		GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AToyBounceBall::BallCollisionHit);
	}
}

// Called when the game starts or when spawned
void AToyBounceBall::BeginPlay()
{
	Super::BeginPlay();

	// Input Mapping Context 설정
	APlayerController* PlayerController = Cast <APlayerController>(GetController());
	if (IsValid (PlayerController))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}	
}

// Called every frame
void AToyBounceBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 바닥에 닿을 시 바운스
	//BounceWhenHittingGround();

	// X 위치 제한
	FixXVelocityZero();
	FixXLocationZero();

	// 병든 공튀기기
	//MoveRandom();
}

// Called to bind functionality to input
void AToyBounceBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Input Action 매핑
	if (UEnhancedInputComponent* EnhancedInputComponent
		= Cast <UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AToyBounceBall::Move);

		for (const auto& InputAction : InputActions)
		{
			EnhancedInputComponent->BindAction(InputAction.Value, ETriggerEvent::Triggered, this, &AToyBounceBall::ActActionByInputID, InputAction.Key);
		}

		//EnhancedInputComponent->BindAction(InputActionA, ETriggerEvent::Triggered, this, &AToyBounceBall::ActActionA);
		//EnhancedInputComponent->BindAction(InputActionB, ETriggerEvent::Triggered, this, &AToyBounceBall::ActActionB);
		//EnhancedInputComponent->BindAction(InputActionLT, ETriggerEvent::Triggered, this, &AToyBounceBall::ActActionLT);
	}
}

UAbilitySystemComponent* AToyBounceBall::GetAbilitySystemComponent() const
{
	return ASC;
}

void AToyBounceBall::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//UE_LOG(LogTemp, Log, TEXT("AB"));

	if (AToyPlayerState* ToyPlayerState =  GetPlayerState <AToyPlayerState>())   //Cast <AToyPlayerState>(NewController))
	{
		//UE_LOG(LogTemp, Log, TEXT("AA"));

		ASC = ToyPlayerState->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(ToyPlayerState, this);
	}

	if (ASC != nullptr)
	{
		for (const auto& GameplayStartAbility : GameplayStartAbilities)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartAbility);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		for (const auto& GameplayActionAbility : GameplayActionAbilities)
		{
			FGameplayAbilitySpec GameplayAbilityActionSpec(GameplayActionAbility.Value);
			GameplayAbilityActionSpec.InputID = GameplayActionAbility.Key;
			ASC->GiveAbility(GameplayAbilityActionSpec);
		}

		//FGameplayAbilitySpec GameplayAbilityActionASpec(GameplayAbilityActionA);
		//GameplayAbilityActionASpec.InputID = 0;
		//ASC->GiveAbility(GameplayAbilityActionASpec);

		//FGameplayAbilitySpec GameplayAbilityActionBSpec(GameplayAbilityActionB);
		//GameplayAbilityActionBSpec.InputID = 1;
		//ASC->GiveAbility(GameplayAbilityActionBSpec);

		//FGameplayAbilitySpec GameplayAbilityActionLTSpec(GameplayAbilityActionLT);
		//GameplayAbilityActionLTSpec.InputID = 2;
		//ASC->GiveAbility(GameplayAbilityActionLTSpec);
	}

	// 시작할 때 자동으로 콘솔 입력
	APlayerController* PlayerController = CastChecked <APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
}

// 구조 수정

//void AToyBounceBall::BounceWhenHittingGround()
//{
//	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
//
//	if (MovementComponent == nullptr)
//	{
//		return;
//	}
//
//	if (MovementComponent->IsFalling() == false)
//	{
//		FVector BounceVector(0, 0, BounceGroundPower);
//		LaunchCharacter(BounceVector, false, true);
//	}
//}

void AToyBounceBall::FixXVelocityZero()
{
	FVector CurrentVelocity = GetCharacterMovement()->Velocity;
	FVector NewVelocity = CurrentVelocity;
	NewVelocity.X = 0;

	GetCharacterMovement()->Velocity = NewVelocity;
}

void AToyBounceBall::FixXLocationZero()
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation;
	NewLocation.X = 0;

	SetActorLocation(NewLocation);
}

void AToyBounceBall::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get <FVector2D>();

	//UE_LOG(LogTemp, Log, TEXT("Move X: %f \t Move Y: %f"), MovementVector.X, MovementVector.Y);

	MoveInputXDirection = FMath::Sign(MovementVector.X);

	AddMovementInput(FVector(0, 1, 0), MoveInputXDirection);
}

//void AToyBounceBall::ActActionA()
//{
//	UE_LOG(LogTemp, Log, TEXT("Action A"));
//
//	//if (IsValid (BouneBallMovementComponent))
//	//{
//	//	BouneBallMovementComponent->Velocity = FVector::ZeroVector;
//	//}
//
//	if (ASC != nullptr)
//	{
//		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(0);
//		if (Spec != nullptr)
//		{
//			if (Spec->IsActive())
//			{
//				ASC->AbilitySpecInputPressed(*Spec);
//			}
//			else
//			{
//				ASC->TryActivateAbility(Spec->Handle);
//			}
//		}
//	}
//	//else
//	//{
//	//	UE_LOG(LogTemp, Log, TEXT("NO ASC!!"));
//	//}
//}

//void AToyBounceBall::ActActionB()
//{
//	UE_LOG(LogTemp, Log, TEXT("Action B"));
//
//	if (ASC != nullptr)
//	{
//		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(1);
//		if (Spec != nullptr)
//		{
//			if (Spec->IsActive())
//			{
//				ASC->AbilitySpecInputPressed(*Spec);
//			}
//			else
//			{
//				ASC->TryActivateAbility(Spec->Handle);
//			}
//		}
//	}
//}
//
//void AToyBounceBall::ActActionLT()
//{
//	UE_LOG(LogTemp, Log, TEXT("Action LT"));
//
//	if (ASC != nullptr)
//	{
//		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(2);
//		if (Spec != nullptr)
//		{
//			if (Spec->IsActive())
//			{
//				ASC->AbilitySpecInputPressed(*Spec);
//			}
//			else
//			{
//				ASC->TryActivateAbility(Spec->Handle);
//			}
//		}
//	}
//}

void AToyBounceBall::ActActionByInputID(int32 InputID)
{
	if (ASC != nullptr)
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
		if (Spec != nullptr)
		{
			if (Spec->IsActive())
			{
				ASC->AbilitySpecInputPressed(*Spec);
			}
			else
			{
				ASC->TryActivateAbility(Spec->Handle);
			}
		}
	}
}

void AToyBounceBall::BallCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Log, TEXT("Something Hit"));

	if (BouneBallMovementComponent == nullptr)
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("... and this pawn has a movementComponent"));

	if (Hit.bBlockingHit == false)
	{
		//UE_LOG(LogTemp, Log, TEXT("Impact Normal %f %f %f"), Hit.ImpactNormal.X, Hit.ImpactNormal.Y, Hit.ImpactNormal.Z);
	
		return;
	}

	// 벽면과 부딪힌 경우
	float DotProductFloat = FVector::DotProduct(Hit.ImpactNormal, FVector(0, 1, 0));
	float DotProductFloatZ = FVector::DotProduct(Hit.ImpactNormal, FVector(0, 0, 1));

	if (FMath::Abs(DotProductFloat) > 0.5f)
	{
		// 하드하게 Velocity 설정
		if (IsValid(BouneBallMovementComponent))
		{
			BouneBallMovementComponent->Velocity = FVector(0, FMath::Sign(DotProductFloat) * BounceWallPower, BounceWallPowerZ);
		}
	}
	else if (DotProductFloatZ > 0.5f)
	{
		//if (IsValid(BouneBallMovementComponent))
		//{
		//	BouneBallMovementComponent->Velocity = FVector(0, 0, BounceGroundPower);
		//}
		FVector BounceVector(0, 0, BounceGroundPower);
		LaunchCharacter(BounceVector, false, true);
	}
}

void AToyBounceBall::MoveRandom()
{
	if (IsValid(BouneBallMovementComponent) && bIsMoveRandom == true)
	{
		float RandomMovePower = FMath::RandRange(0.0f, RandomPower);
		FVector RandomMove(FMath::RandRange(-1, 1) * RandomMovePower, FMath::RandRange(-1, 1) * RandomMovePower, 0);
		
		BouneBallMovementComponent->AddForce(RandomMove);
	}
}


