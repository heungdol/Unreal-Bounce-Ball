// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ToyBounceBallActionData.h"

#include "AbilitySystemComponent.h"


//TSubclassOf <UGameplayAbility> UToyBounceBallActionData::GetAbilityByInputAxis(float InputAxis) const
//{
//    if (bIsSeperatedAbilityByInputAxis == false)
//    {
//        return NeutralInputAxisAbility;
//    }
//
//    if (FMath::Abs(InputAxis) < SMALL_NUMBER)
//    {
//        return NeutralInputAxisAbility;
//    }
//
//    if (InputAxis < 0)
//    {
//        return LeftInputAxisAbility;
//    }
//
//    return RightInputAxisAbility;
//}

void UToyBounceBallActionData::GiveAbilitiesToASC(UAbilitySystemComponent* ASC)
{
    if (ASC == nullptr)
    {
        return;
    }

	FGameplayAbilitySpec GameplayAbilityActionSpec(NeutralInputAxisAbility);
	GameplayAbilityActionSpec.InputID = InputID * 10 + 0;   // 0: 중립, 1: 왼쪽, 2: 오른쪽
	ASC->GiveAbility(GameplayAbilityActionSpec);

    if (bIsSeperatedAbilityByInputAxis == true)
    {
        FGameplayAbilitySpec GameplayAbilityActionLeftSpec(LeftInputAxisAbility);
        GameplayAbilityActionLeftSpec.InputID = InputID * 10 + 1;   // 0: 중립, 1: 왼쪽, 2: 오른쪽
        ASC->GiveAbility(GameplayAbilityActionLeftSpec);

        FGameplayAbilitySpec GameplayAbilityActionRightSpec(RightInputAxisAbility);
        GameplayAbilityActionRightSpec.InputID = InputID * 10 + 2;   // 0: 중립, 1: 왼쪽, 2: 오른쪽
        ASC->GiveAbility(GameplayAbilityActionRightSpec);
    }
}
