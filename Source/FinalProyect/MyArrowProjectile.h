// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MycharacterCPlusPlus.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyArrowProjectile.generated.h"

UCLASS()
class FINALPROYECT_API AMyArrowProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyArrowProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//Creating MeshProjectile and the Force variable.
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ProjectileMesh;
		float Force;
};
