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
		CurrentHP = CurrentStatData->MaxHP;
		CurrentShieldEnergy = CurrentStatData->ShieldEnergy;
	}
	else
	{
		ABLOG(Warning, TEXT("doesn't exist"));
	}


}

void UIBCharacterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(nullptr != CurrentStatData);
	
	if (CurrentShieldEnergy >= NewDamage)
	{
		CurrentShieldEnergy = FMath::Clamp<float>(CurrentShieldEnergy - NewDamage, 0.0f, CurrentStatData->ShieldEnergy);
	}
	else if (CurrentShieldEnergy < NewDamage && CurrentShieldEnergy > 0.0f)
	{
		float temp = NewDamage - CurrentShieldEnergy;
		CurrentShieldEnergy = 0.0f;
		CurrentHP = FMath::Clamp<float>(CurrentHP - temp*2.0f, 0.0f, CurrentStatData->MaxHP);

	}
	else if (CurrentShieldEnergy <= 0.0f)
	{
		CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage * 2.0f, 0.0f, CurrentStatData->MaxHP);
	}
	
	//CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);

	if (CurrentHP <= 0.0f)
	{
		OnHPIsZero.Broadcast();
	}
}

float UIBCharacterStatComponent::GetAttack()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}


