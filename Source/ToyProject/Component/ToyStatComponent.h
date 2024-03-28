// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FHealthHeal)
DECLARE_MULTICAST_DELEGATE(FHealthDamage)
DECLARE_MULTICAST_DELEGATE(FHealthZero)
DECLARE_MULTICAST_DELEGATE(FHealthReset)
DECLARE_MULTICAST_DELEGATE(FHealthReplicate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOYPROJECT_API UToyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UToyStatComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CurrentHealth();

	//UFUNCTION()
	//void OnRep_IsDead ();

	//UFUNCTION()
	//void OnRep_CurrentArmor();

public:
	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Component")
	int32 MaxHealth = 100;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Component")
	int32 CurrentHealth;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Component")
	int32 BaseArmor = 20;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Component")
	int32 CurrentArmor;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Stat Component")
	bool bIsDead;

	
public:
	virtual void IntStat(int32 InMaxHealth, int32 InBaseArmor);
	virtual void InitHealth(int32 InMaxHealth);
	virtual void InitArmor(int32 InBaseArmor);

	FORCEINLINE virtual int32 GetMaxHealth() { return MaxHealth; }
	FORCEINLINE virtual int32 GetCurrentHealth() { return CurrentHealth; }
	virtual void AddCurrentHealth(int32 InHealth);
	virtual void ResetCurrentHealth();

	FORCEINLINE virtual bool GetIsDead() { return bIsDead; }

	// 일단 토이에서는 아머를 사용하지 않는다.
	FORCEINLINE virtual int32 GetBaseArmor() { return BaseArmor; }
	FORCEINLINE virtual int32 GetCurrentArmor() { return CurrentArmor; }
	virtual void AddCurrentArmor(int32 InArmor);
	virtual void MultiplyCurrentArmor(float InMulti);
	virtual void ResetCurrentArmor();
	
	// 체력 변화 델리게이트
	FHealthHeal OnHealthHeal;
	FHealthDamage OnHealthDamage;

	// 체력 오링 델리케이트
	FHealthZero OnHealthZero;

	// 체력 초기화 델리게이트
	FHealthReset OnHealthReset;

	// 동기화 델리게이트
	FHealthReplicate OnHealthReplicate;
};
