// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ToyBlock.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

#include "ToyProject.h"

#include "Component/ToyJellyEffectComponent.h"
#include "Data/ToyJellyEffectData.h"

// Sets default values
AToyBlock::AToyBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	bNetLoadOnClient = true;

	BlockBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	RootComponent = BlockBoxComponent;

	BlockStaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh Component"));
	BlockStaticMeshComponent->SetupAttachment(BlockBoxComponent);
	BlockStaticMeshComponent->SetIsReplicated(false);
	//BlockStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef (TEXT("/Engine/BasicShapes/Cube1.Cube1"));
	if (BoxMeshRef.Object != nullptr)
	{
		BlockStaticMeshComponent->SetStaticMesh(BoxMeshRef.Object);
	}

	JellyEffectComponent = CreateDefaultSubobject <UToyJellyEffectComponent>(TEXT("Jelly Effect Component"));
	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->SetMeshComponent(BlockStaticMeshComponent);
	}

	//CurrentHealth = MaxHealth;
	//bIsActiveBlock = true;
}

void AToyBlock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (BlockStaticMeshComponent != nullptr)
	{
		BlockStaticMeshComponent->SetRelativeScale3D(BlockSize / 100.0f);
	}

	if (BlockBoxComponent != nullptr)
	{
		BlockBoxComponent->SetBoxExtent(BlockSize / 2.0f);
	}
}

void AToyBlock::PostInitProperties()
{
	Super::PostInitProperties();
}

void AToyBlock::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//if (HasAuthority())
	//{
	//	CurrentHealth = MaxHealth;
	//	bIsActiveBlock = true;
	//}
	//else
	//{

	//}
}

// Called when the game starts or when spawned
void AToyBlock::BeginPlay()
{
	Super::BeginPlay();
	
	//if (HasAuthority())
	//{
	//	CurrentHealth = MaxHealth;
	//	bIsActiveBlock = true;
	//}

	if (HasAuthority())
	{
		CurrentHealth = MaxHealth;
		//bIsActiveBlock = true;
	}
	else
	{

	}

	SetBlockActive(CurrentHealth > 0);
}

// Called every frame
void AToyBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToyBlock::DamageBlock(int32 InDamage)
{
	CurrentHealth -= InDamage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, CurrentHealth);

	if (CurrentHealth == 0)
	{
		DestroyBlock();
	}

	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->PlayJellyEffect(JellyEffectDamageData);
	}

	//TOY_LOG(LogTemp, Log, TEXT("Block Current Health: %i"), CurrentHealth);
}

void AToyBlock::DestroyBlock()
{
	//bIsActiveBlock = false;
	SetBlockActive(CurrentHealth > 0);

	if (RespawnTime > SMALL_NUMBER)
	{
		RespawnTimerHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AToyBlock::ResetBlockState, RespawnTime, false);
	}
}

void AToyBlock::SetBlockActive(bool InActive)
{
	if (BlockStaticMeshComponent != nullptr)
	{
		BlockStaticMeshComponent->SetVisibility (InActive);
	}

	SetActorEnableCollision(InActive);
}

void AToyBlock::ResetBlockState()
{	
	CurrentHealth = MaxHealth;
	
	//bIsActiveBlock = true;
	SetBlockActive(CurrentHealth > 0);

	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->PlayJellyEffect(JellyEffectRespawnData);
	}
}

void AToyBlock::OnRep_CurrentHealth()
{
	SetBlockActive(CurrentHealth > 0);
}

//void AToyBlock::OnRep_IsActiveBlock()
//{
//	SetBlockActive(bIsActiveBlock);
//}

void AToyBlock::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(AToyBlock, CurrentHealth);
	//DOREPLIFETIME(AToyBlock, bIsActiveBlock);
}
