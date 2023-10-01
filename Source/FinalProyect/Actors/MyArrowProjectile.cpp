// Fill out your copyright notice in the Description page of Project Settings.


#include "MyArrowProjectile.h"
#include "FinalProyect/Character/Mycharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyArrowProjectile::AMyArrowProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetSimulatePhysics(true);

}

// Called when the game starts or when spawned
void AMyArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AMyArrowProjectile::OnHit);
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	PlayerRef = Cast<AMycharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	FVector AddForce = PlayerRef->ArrowShootDirection * Force * ProjectileMesh->GetMass();
	ProjectileMesh->AddForce(AddForce);

}


void AMyArrowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this) && OtherComp && OtherActor != PlayerRef)
	{
		if (OtherActor->ActorHasTag("Corpse"))
		{
			//**Crating Damage Event to apply enemy
			FDamageEvent DamageEvent;
			//Apply damage to enemy
			OtherActor->TakeDamage(ArrowDamageAmount, DamageEvent,GetInstigatorController(),this);
		}

		DestroyActor();
	}
	
}

void AMyArrowProjectile::DestroyActor()
{
	Destroy();
}

