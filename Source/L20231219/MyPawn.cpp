// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rocket.h"

// Sets default values
AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	RotateComponent = CreateDefaultSubobject<UMyActorComponent>(TEXT("RotateComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(
		TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'")
	);
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(
		TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'")
	);
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	Left->AddLocalOffset(FVector(37.5f, -21.0f, 1.0f));
	Right->AddLocalOffset(FVector(37.5f, 21.0f, 1.0f));
	Arrow->AddLocalOffset(FVector(100.0f, 0, 0));
	SpringArm->TargetArmLength = 150;
	SpringArm->bEnableCameraLag = true;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	RotateComponent->Components.Add(Left);
	RotateComponent->Components.Add(Right);
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector());

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AActor>(
		ARocket::StaticClass(),
		Arrow->K2_GetComponentLocation(),
		Arrow->K2_GetComponentRotation()
	);
}

void AMyPawn::Pitch(float Value)
{
	if (Value != 0)
	{
		AddActorLocalRotation(
			FRotator(UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60 * Value, 0, 0)
		);
	}
}

void AMyPawn::Roll(float Value)
{
	if (Value != 0)
	{
		AddActorLocalRotation(
			FRotator(0, 0, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 60 * Value)
		);
	}
}

