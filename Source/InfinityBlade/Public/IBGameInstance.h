// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "IBGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FIBCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FIBCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30), WalkSpeed(10.0f), ShieldEnergy(100.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ShieldEnergy;
};



UCLASS()
class INFINITYBLADE_API UIBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UIBGameInstance();

	virtual void Init() override;
	FIBCharacterData* GetIBCharacterData(int32 Level);
	
private:
	UPROPERTY()
	class UDataTable* IBCharacterTable;	
};
