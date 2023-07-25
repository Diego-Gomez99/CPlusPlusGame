// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SpotLightComponent.h"
#include "CastleArch.h"

// Sets default values
ACastleArch::ACastleArch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arch Mesh"));
	ArchMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACastleArch::BeginPlay()
{
	Super::BeginPlay();
	
	PointLight = FindComponentByClass<UPointLightComponent>();
	PointLight->MaxDrawDistance = 6000.0f;
	PointLight->SetIntensity(4.0f);
	
	Particle_Component = FindComponentByClass<UParticleSystemComponent>();
	ParticleSys = LoadObject<UParticleSystem>(nullptr,TEXT("/Game/Particles_Wind_Control_System/Blueprints/Components/Particles/P_Candle_Fire_ParticleGreen"));
}


void ACastleArch::GreenLights()
{
	PointLight->SetUseTemperature(false);
	PointLight->SetLightColor(FLinearColor::Green);
	Particle_Component->SetTemplate(ParticleSys);
}