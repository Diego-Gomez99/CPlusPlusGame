
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CastleArch.h"
#include "Components/SpotLightComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpotLightComponent* CastleSpotLight;


	bool CantakeSkull;

	void SkullTaken();

private:
	//**Interactuate Component**//

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CollisionBox;
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* SkullComponent;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BasetresureMesh;
	
	
	//**Audio Classes**//
	UPROPERTY(EditInstanceOnly)
	USoundBase* Audio;
	UAudioComponent* AudioComponent;
	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* SkullAudio;




UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult& SweepResult);

UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	

};
