// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinalProyect/Character/Mycharacter.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Arrow Settings");
	float ArrowDamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow Settings");
	float Force;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	AMycharacter* PlayerRef;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);

	void DestroyActor();

};
