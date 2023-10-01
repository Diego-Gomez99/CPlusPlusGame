
#pragma once


#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "FinalProyect/Actors/Tresure.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraShakeBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mycharacter.generated.h"

UCLASS()
class FINALPROYECT_API AMycharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMycharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsUsingSword;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Options")
	int ArrowsAmount;
	UPROPERTY(BlueprintReadWrite, Category = "Character Options")
	int ArrowsAdded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Options")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Character Options")
	float ArrowDistance;
	UPROPERTY(BlueprintReadWrite)
	bool IsArrowAiming;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCameraComponent* CameraComponent;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ArrowMesh;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* PlayerSwordMesh;


	FVector ArrowDirection;  //Getting the socket location for the projectile spawn

	//**PlayerController**//
	APlayerController* MyPlayerController;

	//**Tresure Actor**//
	ATresure* TresureRef;

private:

	//Handle// Controls animations workflow 
	//======//
	FTimerHandle Delay;

	// Setting the SpeedMovement  
	UPROPERTY(EditAnywhere, Category = "Speed")
	float SpeedMovement;

	//Components//
	//====//
	UCharacterMovementComponent* MovementComponent;
	

	//Animation Montage
	UAnimMontage* AMontage;
	
	//Getting AnimDelays
	float AnimDelay;

	//Bool to control player movement worflow
	bool Canplayermove = true;

public:
	UPROPERTY(BlueprintReadWrite)
	FVector SpawnArrowLocation;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<class AMyArrowProjectile> Projectile;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<class UCameraShakeBase> CameraShake;


	//**We save the direction whre the arrow would be shooted **/
	FVector ArrowShootDirection;

	UFUNCTION(BlueprintCallable)
	void SphereTrace();

private:
	FVector CameraDirection; //Camera Direction to 

	//Get Axis Inputs
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	//**--Camera Settings--**
	UPROPERTY(EditDefaultsOnly, Category = "Camera Options")
	float PitchMax; 
	UPROPERTY(EditDefaultsOnly, Category = "Camera Options")
	float PitchMaxAimming;

	float PitchMin;
	float OriginalPitchMax;

	//--**Camera Options**--//
	UPROPERTY(EditDefaultsOnly, Category = "Camera Options")
	float NoZoom;
	UPROPERTY(EditDefaultsOnly, Category = "Camera Options")
	float ZoomIn;

	//**Sphere Trace Radius**//
	UPROPERTY(EditDefaultsOnly, Category = "Sphere Trace Parameters")
	float SphereRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Sphere Trace Parameters")
	float SwordDamageAmount;

	//ShootCenterOfScreen
	void ShootArrow();

	//Void Actions Inputs// Functions to work with all the players inputs  
	//====//   
	void Jump();
	void WalkSlowPressed();
	void WalkSlowReleased();
	void SwitchWeapon();
	void ActionAttack();
	void BlockOrAimPressed();
	void BlockOrAimReleased();
	void SwordAttack();
	void SetPlayerMove();
	void InteractuateAction();
	void UpdateClampCamera(float pitchmin, float pitchmax);
	
};
