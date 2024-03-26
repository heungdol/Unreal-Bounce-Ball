// Fill out your copyright notice in the Description page of Project Settings.


#include "ToyPlayerState.h"

#include "AbilitySystemComponent.h"

AToyPlayerState::AToyPlayerState()
{
	ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));

	// 멀티 활성화
	//ASC->SetIsReplicated(true);
}

void AToyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (ASC == nullptr) 
	{
		UE_LOG(LogTemp, Log, TEXT("NO ASC from Player State in BeginPlay ()"));
	}
}

UAbilitySystemComponent* AToyPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
