// Fill out your copyright notice in the Description page of Project Settings.


#include "Tresure.h"
#include "Components/BoxComponent.h"
#include "CastleArch.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Misc/OutputDeviceNull.h>

// Sets default values
ATresure::ATresure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BasetresureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tresure Base"));
	BasetresureMesh->SetupAttachment(RootComponent);

	SkullComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkullMesh"));
	SkullComponent->SetupAttachment(BasetresureMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CollisionBox->SetupAttachment(BasetresureMesh);
	
	SkullAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SkullTakenSound"));

	CastleSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("MainLight"));
	CastleSpotLight->SetupAttachment(BasetresureMesh);
}

// Called when the game starts or when spawned
void ATresure::BeginPlay()
{
	Super::BeginPlay();

	CantakeSkull = false;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATresure::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATresure::OnOverlapEnd);

	AudioComponent = UGameplayStatics::SpawnSoundAttached(Audio, GetRootComponent());

	SkullAudio = FindComponentByClass<UAudioComponent>();
}

void ATresure::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep,
                        const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		Interactive_Text = "Press (E) TO TAKE THE SKULL";
		CantakeSkull = true;
	}
	
}

void ATresure::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)	
{
	if (OtherActor->ActorHasTag("Player"))
	{
		Interactive_Text = "";
		CantakeSkull = false;
	}
	
}


void ATresure::SkullTaken()
{
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkullComponent->DestroyComponent(true);
	SkullAudio->Play();
	AudioComponent->Stop();

	TArray<AActor*> AllActors;
	TArray<ACastleArch*> Arches;
	TSubclassOf<AActor> CastleToFind = ACastleArch::StaticClass();

	//**Call the timeline that what created from BP

	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("SettingLightIntensity"), ar, NULL, true);

	UWorld* World = GetWorld();
	if (World)
	{
		// Find all actors of type AActor (the base class)
		UGameplayStatics::GetAllActorsOfClass(World, CastleToFind, AllActors);

		
		// Copy elements from AllActors to Arches
		for (AActor* Actor : AllActors)
		{
			ACastleArch* CastleArch = Cast<ACastleArch>(Actor);
			if (CastleArch)
			{
				Arches.Add(CastleArch);
			}
		}

		// Now Arches array contains instances of ACastleArch
		for (ACastleArch* ArchesFound : Arches)
		{
			if (ArchesFound)
			{
				ArchesFound->GreenLights();
			}
		}
	}
	
}

