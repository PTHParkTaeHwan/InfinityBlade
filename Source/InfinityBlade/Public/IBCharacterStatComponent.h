// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Components/ActorComponent.h"
#include "IBCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFINITYBLADE_API UIBCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UIBCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;	
	
public:
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	float GetAttack();

	FOnHPIsZeroDelegate OnHPIsZero;

private:
	struct FIBCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentShieldEnergy;

};
