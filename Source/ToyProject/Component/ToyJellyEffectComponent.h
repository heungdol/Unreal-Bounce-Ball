// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToyJellyEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOYPROJECT_API UToyJellyEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToyJellyEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TickJellyEffect();

public:
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category ="Jelly Effect")
	TObjectPtr <class UMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Jelly Effect")
	TObjectPtr <class UToyJellyEffectData> CurrentJellyEffectData;

	//

	bool bIsPlayingJellyEffect;

	float PlayJellyEffectTime;
	float EndJellyEffectTime;

	FVector StartMeshLocation;
	FRotator StartMeshRotation;
	FVector StartMeshScale;

	FVector CurrentJellyEffectLocation;
	FRotator CurrentJellyEffectRotation;
	FVector CurrentJellyEffectScale;

public:
	virtual void SetMeshComponent(class UMeshComponent* InMeshComponent);

	virtual class UMeshComponent* GetMeshComponent() const;

	virtual void PlayJellyEffect(class UToyJellyEffectData* InJellyEffectData);
	
	virtual void StopJellyEffect();
};
