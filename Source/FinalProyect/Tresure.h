// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tresure.generated.h"

UCLASS()
class FINALPROYECT_API ATresure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATresure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
UPROPERTY(BlueprintReadOnly)
FString Interactive_Text;

UPROPERTY(BlueprintReadOnly)
bool CanTakeSkull=false;


private:
//**Interactuate Component**//
class UBoxComponent* CollisionBox;
class USkeletalMeshComponent* SkullComponent;

//**Audio Classes**//
UPROPERTY(EditInstanceOnly)
class USoundBase* Audio;
class UAudioComponent* AudioComponent;
class UAudioComponent* SkullAudio;

UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult& SweepResult);

UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

UFUNCTION(BlueprintCallable)	
void SkullTaken();	

};
