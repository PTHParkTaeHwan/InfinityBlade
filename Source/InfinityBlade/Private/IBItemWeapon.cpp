// Fill out your copyright notice in the Description page of Project Settings.

#include "IBItemWeapon.h"


// Sets default values
AIBItemWeapon::AIBItemWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));

	RootComponent = Trigger;
	Weapon->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMeshComponent> SM_WEAPON(TEXT(""));
	if (SM_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SM_WEAPON.Object);
	}
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

