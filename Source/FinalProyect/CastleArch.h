// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
UPROPERTY(EditInstanceOnly)
UStaticMeshComponent* ArchMesh;

/*Lights Components*/
/*====================*/
class UPointLightComponent* PointLight;

/*Lighst Partcile System*/
/*====================*/
class UParticleSystemComponent* Particle_Component;
class UParticleSystem* ParticleSys;

UFUNCTION(BlueprintCallable)
void GreenLights();
};
