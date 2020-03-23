// Fill out your copyright notice in the Description page of Project Settings.

#include "IBItemWeapon.h"
#include "IBWeapon.h"
#include "IBCharacter.h"


// Sets default values
AIBItemWeapon::AIBItemWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));


	RootComponent = Trigger;
	Weapon->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword17/SK_Blade_HeroSword17.SK_Blade_HeroSword17"));
	if (SM_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SM_WEAPON.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_WEAPONGET(TEXT("/Game/InfinityBladeEffects/Effects/FX_Combat_Base/WeaponCombo/P_ComboFinish_Tap_01.P_ComboFinish_Tap_01"));
	if (P_WEAPONGET.Succeeded())
	{
		Effect->SetTemplate(P_WEAPONGET.Object);
		Effect->bAutoActivate = false;
	}

	Weapon->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemWeapon"));
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AIBWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AIBItemWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIBItemWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIBItemWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AIBItemWeapon::OnCharacterOverlap);
}

void AIBItemWeapon::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABLOG(Warning, TEXT("Overlap!!!"));
	
	auto IBCharacter = Cast<AIBCharacter>(OtherActor);
	ABCHECK(nullptr != IBCharacter);

	if (nullptr != IBCharacter && nullptr != WeaponItemClass)
	{
		if (IBCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AIBWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			IBCharacter->SetWeapon(NewWeapon);
			Effect->Activate(true);
			Weapon->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &AIBItemWeapon::OnEffectFinished);
		}
		else
		{
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *IBCharacter->GetName());
		}
	}
}

void AIBItemWeapon::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}

