// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "GameFramework/Actor.h"
#include "IBItemWeapon.generated.h"

UCLASS()
class INFINITYBLADE_API AIBItemWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIBItemWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = ItemWeapon)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = ItemWeapon)
	USkeletalMeshComponent* Weapon;
	
	
};
