// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ToyJellyEffectComponent.h"

#include "Components/MeshComponent.h"
#include "Data/ToyJellyEffectData.h"

//#include "ToyProject.h"

// Sets default values for this component's properties
UToyJellyEffectComponent::UToyJellyEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsPlayingJellyEffect = false;
}


// Called when the game starts
void UToyJellyEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsPlayingJellyEffect = false;
	
	if (GetMeshComponent() != nullptr)
	{
		StartMeshLocation = GetMeshComponent()->GetRelativeLocation();
		StartMeshRotation = GetMeshComponent()->GetRelativeRotation();
		StartMeshScale = GetMeshComponent()->GetRelativeScale3D();
	}
	else
	{
		StartMeshLocation = FVector::ZeroVector;
		StartMeshRotation = FRotator::ZeroRotator;
		StartMeshScale = FVector::OneVector;
	}
}

// Called every frame
void UToyJellyEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetMeshComponent()->SetRelativeLocation(StartMeshLocation);
	GetMeshComponent()->SetRelativeRotation(StartMeshRotation);
	GetMeshComponent()->SetRelativeScale3D(StartMeshScale);

	TickJellyEffect();
}

void UToyJellyEffectComponent::TickJellyEffect()
{
	if (bIsPlayingJellyEffect)
	{
		// 현재 젤리 이펙트 스케일 초기화
		CurrentJellyEffectScale = FVector::OneVector;

		// 만약 데이터가 없으면 초기화 후 수행하지 않음
		if (CurrentJellyEffectData == nullptr)
		{
			bIsPlayingJellyEffect = false;
			GetMeshComponent()->SetRelativeLocation(StartMeshLocation);
			GetMeshComponent()->SetRelativeRotation(StartMeshRotation);
			GetMeshComponent()->SetRelativeScale3D(StartMeshScale);
			//PrimaryComponentTick.bCanEverTick = false;
			SetComponentTickEnabled(false);

			//UE_LOG(LogTemp, Log, TEXT("Jelly Effect End... -> No Data"));
			return;
		}

		// 만약 시간이 모두 경과하면 초기화 후 수행하지 않음
		float CurrentJellyEffectTime = GetWorld()->GetTimeSeconds();
		if (CurrentJellyEffectTime > EndJellyEffectTime)
		{
			bIsPlayingJellyEffect = false;
			GetMeshComponent()->SetRelativeLocation(StartMeshLocation);
			GetMeshComponent()->SetRelativeRotation(StartMeshRotation);
			GetMeshComponent()->SetRelativeScale3D(StartMeshScale);
			//PrimaryComponentTick.bCanEverTick = false;
			SetComponentTickEnabled(false);

			//UE_LOG(LogTemp, Log, TEXT("Jelly Effect End... -> Time"));
			return;
		}

		// 젤리 이펙트 수행
		float CurrentJellyEffectRatio = (CurrentJellyEffectTime - PlayJellyEffectTime) / (EndJellyEffectTime - PlayJellyEffectTime);

		CurrentJellyEffectLocation = CurrentJellyEffectData->GetLocationVectorByRatio(CurrentJellyEffectRatio);
		CurrentJellyEffectRotation = FRotator::MakeFromEuler(CurrentJellyEffectData->GetRotationVectorByRatio(CurrentJellyEffectRatio));
		CurrentJellyEffectScale = CurrentJellyEffectData->GetScaleVectorByRatio(CurrentJellyEffectRatio);

		GetMeshComponent()->SetRelativeLocation(StartMeshLocation + CurrentJellyEffectLocation);
		GetMeshComponent()->SetRelativeRotation(StartMeshRotation + CurrentJellyEffectRotation);
		GetMeshComponent()->SetRelativeScale3D(StartMeshScale * CurrentJellyEffectScale);

		//UE_LOG(LogTemp, Log, TEXT("Jelly Effect ... Vector -> %s"), *CurrentJellyEffectScale.ToCompactString());
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Jelly Effect End..."));
		//PrimaryComponentTick.bCanEverTick = false;
		SetComponentTickEnabled(false);
	}
}

void UToyJellyEffectComponent::SetMeshComponent(UMeshComponent* InMeshComponent)
{
	MeshComponent = InMeshComponent;
}

UMeshComponent* UToyJellyEffectComponent::GetMeshComponent() const
{
	return MeshComponent;
}

void UToyJellyEffectComponent::PlayJellyEffect(UToyJellyEffectData* InJellyEffectData)
{
	//UE_LOG(LogTemp, Log, TEXT("Jelly Effect Ready..."));


	if (InJellyEffectData == nullptr)
	{
		return;
	}

	if (GetMeshComponent() == nullptr)
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Jelly Effect Start"));

	CurrentJellyEffectData = InJellyEffectData;

	PlayJellyEffectTime = GetWorld()->GetTimeSeconds();
	EndJellyEffectTime = PlayJellyEffectTime + CurrentJellyEffectData->GetJellyEffectTime();

	bIsPlayingJellyEffect = true;
	//PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

	// 젤리 이펙트 수행 (첫 틱)
	CurrentJellyEffectLocation = CurrentJellyEffectData->GetLocationVectorByRatio(0.0f);
	CurrentJellyEffectRotation = FRotator::MakeFromEuler (CurrentJellyEffectData->GetRotationVectorByRatio(0.0f));
	CurrentJellyEffectScale = CurrentJellyEffectData->GetScaleVectorByRatio(0.0f);

	GetMeshComponent()->SetRelativeLocation(StartMeshLocation + CurrentJellyEffectLocation);
	GetMeshComponent()->SetRelativeRotation(StartMeshRotation + CurrentJellyEffectRotation);
	GetMeshComponent()->SetRelativeScale3D(StartMeshScale * CurrentJellyEffectScale);

}

void UToyJellyEffectComponent::StopJellyEffect()
{
	bIsPlayingJellyEffect = false;

	GetMeshComponent()->SetRelativeLocation(StartMeshLocation);
	GetMeshComponent()->SetRelativeRotation(StartMeshRotation);
	GetMeshComponent()->SetRelativeScale3D(StartMeshScale);
}

