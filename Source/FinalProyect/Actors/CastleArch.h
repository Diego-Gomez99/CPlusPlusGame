// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "CastleArch.generated.h"

UCLASS()
class FINALPROYECT_API ACastleArch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACastleArch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
UPROPERTY(EditDefaultsOnly)
UStaticMeshComponent* ArchMesh;

/*Lights Components*/
/*====================*/
UPROPERTY(EditDefaultsOnly)
UPointLightComponent* PointLight;

/*Lights Partcile System*/
/*====================*/
UPROPERTY(EditDefaultsOnly)
UParticleSystemComponent* Particle_Component;
UPROPERTY(EditInstanceOnly, Category = "New Particle System Template")
UParticleSystem* ParticleSys;


public:
UFUNCTION(BlueprintCallable)
void GreenLights();
};
