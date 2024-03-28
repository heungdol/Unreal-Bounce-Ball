// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ToyStatComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

#include "ToyProject.h"


UToyStatComponent::UToyStatComponent()
{
	// 틱은 사용하지 않는다.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
}

void UToyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		IntStat(MaxHealth, BaseArmor);
	}
	//else
	//{
	//	OnHealthReplicate.Broadcast();
	//}
}

void UToyStatComponent::IntStat(int32 InMaxHealth, int32 InBaseArmor)
{
	InitHealth(InMaxHealth);
	InitArmor(InBaseArmor);
}

void UToyStatComponent::InitHealth(int32 InMaxHealth)
{
	MaxHealth = InMaxHealth;
	MaxHealth = FMath::Clamp(MaxHealth, 1, MaxHealth);

	ResetCurrentHealth();
}

void UToyStatComponent::InitArmor(int32 InBaseArmor)
{
	BaseArmor = InBaseArmor;
	
	ResetCurrentArmor();
}

void UToyStatComponent::AddCurrentHealth(int32 InHealth)
{
	if (InHealth == 0)
	{
		return;
	}

	CurrentHealth += InHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	// 증가에 따라 다른 델리게이트 수행
	if (InHealth > 0)
	{
		OnHealthHeal.Broadcast();
	}
	else
	{
		OnHealthDamage.Broadcast();
	}
	
	// 죽음 델리게이트는 한 번만 수행
	if (CurrentHealth == 0)
	{
		if (bIsDead == false)
		{
			OnHealthZero.Broadcast();
		}

		bIsDead = true;
	}
}

void UToyStatComponent::ResetCurrentHealth()
{
	CurrentHealth = MaxHealth;
	bIsDead = false;

	OnHealthReset.Broadcast();
}

void UToyStatComponent::AddCurrentArmor(int32 InArmor)
{
	CurrentArmor += InArmor;
}

void UToyStatComponent::MultiplyCurrentArmor(float InMulti)
{
	CurrentArmor = (int32)((float)CurrentArmor * InMulti);
}

void UToyStatComponent::ResetCurrentArmor()
{
	CurrentArmor = BaseArmor;
}

//void UToyStatComponent::OnRep_CurrentHealth()
//{
//	;
//}
//
//void UToyStatComponent::OnRep_CurrentArmor()
//{
//	//OnReplicate.Broadcast();
//}

void UToyStatComponent::OnRep_CurrentHealth()
{
	OnHealthReplicate.Broadcast();

	//UE_LOG(LogTemp, Log, TEXT("Replicated: Health -> %i"), CurrentHealth);
}

void UToyStatComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(UToyStatComponent, CurrentHealth);
	DOREPLIFETIME(UToyStatComponent, CurrentArmor);
}

