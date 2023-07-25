// Fill out your copyright notice in the Description page of Project Settings.


#include "Tresure.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATresure::ATresure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATresure::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = FindComponentByClass<UBoxComponent>();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATresure::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ATresure::OnOverlapEnd);

	SkullComponent= FindComponentByClass<USkeletalMeshComponent>();

	AudioComponent = UGameplayStatics::SpawnSoundAttached(Audio, GetRootComponent());

	SkullAudio = FindComponentByClass<UAudioComponent>();
}

void ATresure::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep,
                        const FHitResult& SweepResult)
{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			Interactive_Text = "(E) TO TAKE THE SKULL";	
			CanTakeSkull = true;
		}
		

}

void ATresure::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)	
{
	Interactive_Text = "";
	CanTakeSkull = false;
}


void ATresure::SkullTaken()
{
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkullComponent->DestroyComponent();
	SkullAudio->Play();
	AudioComponent->Stop();
}

