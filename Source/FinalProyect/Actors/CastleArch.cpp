// Fill out your copyright notice in the Description page of Project Settings.

#include "CastleArch.h"
#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SpotLightComponent.h"


// Sets default values
ACastleArch::ACastleArch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ArchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arch Mesh"));
	ArchMesh->SetupAttachment(RootComponent);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	PointLight->SetupAttachment(ArchMesh);

	Particle_Component = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	Particle_Component->SetupAttachment(ArchMesh);
}

// Called when the game starts or when spawned
void ACastleArch::BeginPlay()
{
	Super::BeginPlay();
}


void ACastleArch::GreenLights()
{
	if (ParticleSys)
	{
		PointLight->SetUseTemperature(false);
		PointLight->SetLightColor(FLinearColor::Green);
		Particle_Component->SetTemplate(ParticleSys);
	}
	
}