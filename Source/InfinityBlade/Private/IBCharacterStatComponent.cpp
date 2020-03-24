// Fill out your copyright notice in the Description page of Project Settings.

#include "IBCharacterStatComponent.h"
#include "IBGameInstance.h"

// Sets default values for this component's properties
UIBCharacterStatComponent::UIBCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UIBCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UIBCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UIBCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto IBGameInstance = Cast<UIBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != IBGameInstance);
	CurrentStatData = IBGameInstance->GetIBCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		SetSE(CurrentStatData->ShieldEnergy);
	}
	else
	{
		ABLOG(Warning, TEXT("doesn't exist"));
	}


}

void UIBCharacterStatComponent::SetDamage(float NewDamage, bool DefenseModeOn)
{
	ABCHECK(nullptr != CurrentStatData);
	
	if (!DefenseModeOn)
	{
		SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
	}
	else if (DefenseModeOn)
	{
		float TempDamage = NewDamage / 2.0f;
		if (CurrentShieldEnergy >= TempDamage)
		{
			SetSE(FMath::Clamp<float>(CurrentShieldEnergy - TempDamage, 0.0f, CurrentStatData->ShieldEnergy));
		}
		else if (CurrentShieldEnergy < TempDamage && CurrentShieldEnergy > 0.0f)
		{
			float temp = (TempDamage - CurrentShieldEnergy) * 2.0f;
			SetSE(0.0f);
			SetHP(FMath::Clamp<float>(CurrentHP - temp, 0.0f, CurrentStatData->MaxHP));
		}
		else if (CurrentShieldEnergy <= 0.0f)
		{
			SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
		}
	}
}

void UIBCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		OnHPIsZero.Broadcast();
	}

}

void UIBCharacterStatComponent::SetSE(float NewSE)
{
	CurrentShieldEnergy = NewSE;
	OnSEChanged.Broadcast();
}

float UIBCharacterStatComponent::GetAttack()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float UIBCharacterStatComponent::GetHPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

float UIBCharacterStatComponent::GetSERatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->ShieldEnergy < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentShieldEnergy / CurrentStatData->ShieldEnergy);
}


