// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// component
	Box = CreateDefaultSubobject< UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Body = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	
	// initial setting
	Body->AddLocalRotation(FRotator(270, 0, 0));

	// load static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(
		TEXT("/Script/Engine.StaticMesh'/Game/Rocket/Meshes/SM_Rocket.SM_Rocket'")
	);
	if (SM_Rocket.Succeeded())
	{
		Body->SetStaticMesh(SM_Rocket.Object);
	}

	// projectile setting
	Projectile->InitialSpeed = 3000;
	Projectile->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

