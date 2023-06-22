// Fill out your copyright notice in the Description page of Project Settings.


#include "MyArrowProjectile.h"
#include "MycharacterCPlusPlus.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyArrowProjectile::AMyArrowProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetSimulatePhysics(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMesh(TEXT("/Game/MyCharacters/Arrow/source/arrow"));

	if (ArrowMesh.Succeeded())
	{
		ProjectileMesh->SetStaticMesh(ArrowMesh.Object);
		ProjectileMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ProjectileMesh->SetWorldScale3D(FVector(0.5f));
	}
}

// Called when the game starts or when spawned
void AMyArrowProjectile::BeginPlay()
{
	Super::BeginPlay();

	Force = 400000.0f;
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	FVector SpawnDirection = GetActorForwardVector();
	FVector AddForce = SpawnDirection * Force;
	ProjectileMesh->AddForce(AddForce);
}

// Called every frame
void AMyArrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


