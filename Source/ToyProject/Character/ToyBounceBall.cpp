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
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (IsValid(CharacterMovementComponent))
	{
		CharacterMovementComponent->AirControl = 1;
		CharacterMovementComponent->MaxWalkSpeed = 800;
		CharacterMovementComponent->MaxAcceleration = 10000;

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
	BounceWhenHittingGround();

	// X 위치 제한
	FixXVelocityZero();
	FixXLocationZero();
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

		EnhancedInputComponent->BindAction(InputActionA, ETriggerEvent::Triggered, this, &AToyBounceBall::ActActionA);
		EnhancedInputComponent->BindAction(InputActionB, ETriggerEvent::Triggered, this, &AToyBounceBall::ActActionB);
		EnhancedInputComponent->BindAction(InputActionLT, ETriggerEvent::Triggered, this, &AToyBounceBall::ActActionLT);
	}
}

void AToyBounceBall::BounceWhenHittingGround()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (MovementComponent == nullptr)
	{
		return;
	}

	if (MovementComponent->IsFalling() == false)
	{
		FVector BounceVector(0, 0, BouncePower);
		LaunchCharacter(BounceVector, false, true);
	}
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

void AToyBounceBall::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get <FVector2D>();

	UE_LOG(LogTemp, Log, TEXT("Move X: %f \t Move Y: %f"), MovementVector.X, MovementVector.Y);

	AddMovementInput(FVector(0, 1, 0), FMath::Sign(MovementVector.X));
}

void AToyBounceBall::ActActionA()
{
}

void AToyBounceBall::ActActionB()
{
}

void AToyBounceBall::ActActionLT()
{
}

