// Fill out your copyright notice in the Description page of Project Settings.


#include "ToyBounceBall.h"

#include "Components/StaticMeshComponent.h"
//#include "GameFramework/SpringArmComponent.h"
//#include "Camera/CameraComponent.h"
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

#include "Camera/ToyCameraComponent.h"
#include "Camera/ToySpringArmComponent.h"

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
	if (SpringArmComponent = CreateDefaultSubobject <UToySpringArmComponent>(TEXT("Camera Spring Arm")))
	{
		SpringArmComponent->SetupAttachment(RootComponent);
		SpringArmComponent->TargetArmLength = 3000.0f;
		
		//SpringArmComponent->bEnableCameraLag = true;
		//SpringArmComponent->CameraLagMaxDistance = 0.0f;
		//SpringArmComponent->CameraLagSpeed = 1.0f;
	}

	if (CameraComponent = CreateDefaultSubobject <UToyCameraComponent>(TEXT("Camera Component")))
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
	//BouneBallMovementComponent = GetCharacterMovement();
	if (IsValid(GetCharacterMovement()))
	{
		GetCharacterMovement()->AirControl = 1;
		GetCharacterMovement()->MaxWalkSpeed = 800;
		GetCharacterMovement()->MaxAcceleration = 10000;

	}

	// 충돌 델리게이트 등록
	if (GetCapsuleComponent() != nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("Add Component Hit Delegate"));

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
		// Trigger 및 Completed를 수행해야 입력이 끝났을 때도 처리 가능 
		EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AToyBounceBall::MoveByAxisInput);
		EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Completed, this, &AToyBounceBall::MoveByAxisInput);

		for (const auto& InputAction : InputPressedActions)
		{
			EnhancedInputComponent->BindAction(InputAction.Value, ETriggerEvent::Triggered, this, &AToyBounceBall::BindPressedActionByInputID, InputAction.Key);
		}

		// 명목상 릴리즈 액션도 만들어 놓음
		for (const auto& InputAction : InputReleasedActions)
		{
			EnhancedInputComponent->BindAction(InputAction.Value, ETriggerEvent::Completed, this, &AToyBounceBall::BindReleasedActionByInputID, InputAction.Key);
		}

		UE_LOG(LogTemp, Log, TEXT("Init Ball"));
	}
}

UAbilitySystemComponent* AToyBounceBall::GetAbilitySystemComponent() const
{
	return ASC;
}

void AToyBounceBall::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AToyPlayerState* ToyPlayerState =  GetPlayerState <AToyPlayerState>())   //Cast <AToyPlayerState>(NewController))
	{
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
	}

	// 시작할 때 자동으로 콘솔 입력
	APlayerController* PlayerController = CastChecked <APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
}

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

void AToyBounceBall::MoveByAxisInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get <FVector2D>();

	InputAxisMoveX = FMath::Sign(MovementVector.X);
	InputAxisMoveX = (FMath::Abs(MovementVector.X) < SMALL_NUMBER) ? 0.0f : InputAxisMoveX;

	//UE_LOG(LogTemp, Log, TEXT("Move X: %f \t Move Y: %f"), InputAxisMoveX, MovementVector.Y);


	AddMovementInput(FVector(0, 1, 0), InputAxisMoveX);
	//UE_LOG(LogTemp, Log, TEXT("%f"), GetPendingMovementInputVector().Y);
}

float AToyBounceBall::GetInputAxisMoveX() const
{
	return InputAxisMoveX;
}

void AToyBounceBall::BindPressedActionByInputID(int32 InputID)
{
	if (ASC == nullptr)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec != nullptr)
	{
		Spec->InputPressed = true;

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

void AToyBounceBall::BindReleasedActionByInputID(int32 InputID)
{
	if (ASC == nullptr)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec != nullptr)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AToyBounceBall::BallCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(GetCharacterMovement()) == false)
	{
		return;
	}

	if (Hit.bBlockingHit == false)
	{
		return;
	}

	// 벽면과 부딪힌 경우
	float DotProductFloat = FVector::DotProduct(Hit.ImpactNormal, FVector(0, 1, 0));
	float DotProductFloatZ = FVector::DotProduct(Hit.ImpactNormal, FVector(0, 0, 1));

	if (FMath::Abs(DotProductFloat) > 0.5f)
	{
		// 하드하게 Velocity 설정
		GetCharacterMovement()->Velocity = FVector(0, FMath::Sign(DotProductFloat) * BounceWallPower, BounceWallPowerZ);
	}
	else if (DotProductFloatZ > 0.5f)
	{
		FVector BounceVector(0, 0, BounceGroundPower);
		LaunchCharacter(BounceVector, false, true);
	}
}

//void AToyBounceBall::SetMovementStateFalling()
//{
//	if (IsValid(GetCharacterMovement()) == false)
//	{
//		return;
//	}
//
//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
//}
//
//void AToyBounceBall::SetMovementStateNone()
//{
//	if (IsValid(GetCharacterMovement()) == false)
//	{
//		return;
//	}
//
//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
//}
//
//void AToyBounceBall::MoveRandom()
//{
//	if (IsValid(GetCharacterMovement()) == false)
//	{
//		return;
//	}
//
//	if (bIsMoveRandom == true)
//	{
//		float RandomMovePower = FMath::RandRange(0.0f, RandomPower);
//		FVector RandomMove(FMath::RandRange(-1, 1) * RandomMovePower, FMath::RandRange(-1, 1) * RandomMovePower, 0);
//		
//		GetCharacterMovement()->AddForce(RandomMove);
//	}
//}
//
//void AToyBounceBall::SetMovementVelocityZero()
//{
//	if (IsValid(GetCharacterMovement()) == false)
//	{
//		return;
//	}
//
//	GetCharacterMovement()->Velocity = FVector::ZeroVector;
//}


