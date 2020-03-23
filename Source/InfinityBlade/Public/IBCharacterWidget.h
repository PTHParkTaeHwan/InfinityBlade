// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InfinityBlade.h"
#include "Blueprint/UserWidget.h"
#include "IBCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFINITYBLADE_API UIBCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UIBCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UIBCharacterStatComponent> CurrentCharacterStat;
	
	UPROPERTY()
	class UProgressBar* HPProgressBar;

	UPROPERTY()
	class UProgressBar* SEProgressBar;
	
};
