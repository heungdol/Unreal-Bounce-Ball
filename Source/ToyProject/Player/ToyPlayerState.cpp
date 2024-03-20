// Fill out your copyright notice in the Description page of Project Settings.


#include "ToyPlayerState.h"

#include "AbilitySystemComponent.h"

AToyPlayerState::AToyPlayerState()
{
	ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));

	// 멀티 활성화
	ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* AToyPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
