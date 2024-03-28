// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ToyBlock.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Component/ToyStatComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

#include "ToyProject.h"

#include "Component/ToyJellyEffectComponent.h"
#include "Data/ToyJellyEffectData.h"

// Sets default values
AToyBlock::AToyBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	bNetLoadOnClient = true;

	BlockBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Box Component"));
	BlockBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = BlockBoxComponent;

	BlockStaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh Component"));
	BlockStaticMeshComponent->SetupAttachment(BlockBoxComponent);
	BlockStaticMeshComponent->SetIsReplicated(true);
	BlockStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef (TEXT("/Engine/BasicShapes/Cube1.Cube1"));
	if (BoxMeshRef.Object != nullptr)
	{
		BlockStaticMeshComponent->SetStaticMesh(BoxMeshRef.Object);
	}

	StatComponent = CreateDefaultSubobject <UToyStatComponent>(TEXT("Stat Component"));
	StatComponent->SetIsReplicated(true);

	JellyEffectComponent = CreateDefaultSubobject <UToyJellyEffectComponent>(TEXT("Jelly Effect Component"));
	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->SetMeshComponent(BlockStaticMeshComponent);
		JellyEffectComponent->SetIsReplicated(false);
	}
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
}

// Called when the game starts or when spawned
void AToyBlock::BeginPlay()
{
	Super::BeginPlay();

	bIsRestted = false;

	if (StatComponent != nullptr)
	{
		StatComponent->OnHealthDamage.AddUObject(this, &AToyBlock::DamageBlock);
		StatComponent->OnHealthZero.AddUObject(this, &AToyBlock::BreakBlock);
		StatComponent->OnHealthZero.AddUObject(this, &AToyBlock::Die);
		StatComponent->OnHealthReset.AddUObject(this, &AToyBlock::ResetBlock);

		StatComponent->
			OnHealthReplicate.AddLambda([&]()
			{
				if (bIsRestted == true)
				{
					return;
				}

				bIsRestted = true;

				if ((StatComponent->GetCurrentHealth() > 0))
				{
					RespawnTimerHandle.Invalidate();
					SetBlockActive(true);
				}
				else
				{
					SetBlockActive(false);
				}
			});
	}
}

// Called every frame
//void AToyBlock::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

void AToyBlock::SetBlockActive(bool InActive)
{
	if (BlockStaticMeshComponent != nullptr)
	{
		BlockStaticMeshComponent->SetVisibility (InActive);
	}

	SetActorEnableCollision(InActive);
}

void AToyBlock::DamageBlock()
{
	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->PlayJellyEffect(JellyEffectDamageData);
	}
}

void AToyBlock::HealBlock()
{

}

void AToyBlock::BreakBlock()
{
	SetBlockActive(false);
}

void AToyBlock::ResetBlock()
{	
	RespawnTimerHandle.Invalidate();

	SetBlockActive(true);

	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->PlayJellyEffect(JellyEffectRespawnData);
	}
}

UToyStatComponent* AToyBlock::GetStatComponent()
{
	return StatComponent;
}

void AToyBlock::AddHealth(int32 InHealth)
{
	if (GetStatComponent() == nullptr)
	{
		return;
	}

	GetStatComponent()->AddCurrentHealth(InHealth);
}

void AToyBlock::Die()
{
	//SetBlockActive(false);

	if (RespawnTime > SMALL_NUMBER)
	{
		RespawnTimerHandle.Invalidate();

		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, 
			FTimerDelegate::CreateLambda([this]()
			{
				if (GetStatComponent() == nullptr)
				{
					return;
				}

				GetStatComponent()->ResetCurrentHealth();
				
			}), RespawnTime, false);
	}

	//UE_LOG(LogTemp, Log, TEXT("Health Zero?: %i"), GetStatComponent()->GetCurrentHealth());
}