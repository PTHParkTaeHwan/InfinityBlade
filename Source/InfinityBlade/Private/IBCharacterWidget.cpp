// Fill out your copyright notice in the Description page of Project Settings.

#include "IBCharacterWidget.h"
#include "IBCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UIBCharacterWidget::BindCharacterStat(class UIBCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnSEChanged.AddUObject(this, &UIBCharacterWidget::UpdateHPWidget);
	NewCharacterStat->OnHPChanged.AddUObject(this, &UIBCharacterWidget::UpdateHPWidget);
}

void UIBCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	SEProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_SEBar")));
	ABCHECK(nullptr != HPProgressBar);
	ABCHECK(nullptr != SEProgressBar);
	UpdateHPWidget();
}

void UIBCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
		if (nullptr != SEProgressBar)
		{
			SEProgressBar->SetPercent(CurrentCharacterStat->GetSERatio());
		}
	}
}
