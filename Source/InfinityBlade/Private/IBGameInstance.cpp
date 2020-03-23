// Fill out your copyright notice in the Description page of Project Settings.

#include "IBGameInstance.h"

UIBGameInstance::UIBGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Book/GamaData/IBCharacterData.IBCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_IBCHARACTER(*CharacterDataPath);
	ABCHECK(DT_IBCHARACTER.Succeeded());
	IBCharacterTable = DT_IBCHARACTER.Object;
	ABCHECK(IBCharacterTable->RowMap.Num() > 0);

}

void UIBGameInstance::Init()
{
	Super::Init();
}

FIBCharacterData * UIBGameInstance::GetIBCharacterData(int32 Level)
{
	return IBCharacterTable->FindRow<FIBCharacterData>(*FString::FromInt(Level), TEXT(""));
}




