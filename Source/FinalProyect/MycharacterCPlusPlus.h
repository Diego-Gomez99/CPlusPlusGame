// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MycharacterCPlusPlus.generated.h"

UCLASS()
class FINALPROYECT_API AMycharacterCPlusPlus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMycharacterCPlusPlus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Verify if is Switching the weapon.
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
		bool bIsUsingSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Options")
		int ArrowsAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Options")
		UStaticMeshComponent* ArrowMesh;

	//Skeletals Mesh Component
	UPROPERTY(BlueprintReadWrite)
		USkeletalMeshComponent* SwordMesh;
	UPROPERTY(BlueprintReadWrite)
		USkeletalMeshComponent* BowMesh;
	UPROPERTY(BlueprintReadWrite)
		bool IsArrowAiming;
	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* CameraComponent;

	FVector ArrowDirection;

private:

	//Setting Delay Variable
	FTimerHandle Delay;

	//Setting Player Controller
	APlayerController* MyPlayerController;

	// Setting the SpeedMovement  
	UPROPERTY(EditAnywhere, Category = "Speed")
		float SpeedMovement = 600.0f;

	
	//Movement Component
	UCharacterMovementComponent* MovementComponent;
	// Spring Arm Component
	USpringArmComponent* SpringArm;
	//Animation Montage
	UAnimMontage* AMontage;
	

	//Statics Mesh Component
	UPROPERTY(EditAnywhere, Category = "Character Options")
	UStaticMeshComponent* CogMesh;
	
	
	//Getting AnimDelays
	float AnimDelay;

	//Disable/EnableInput boolean
	bool CanEnableInput;

	//Vectors 
	FVector CameraDirection;

	//Get Axis Inputs
	void MoveForward(float Value);
	void MoveRight(float Value);
	void CameraPitch(float Value);
	void CameraYawn(float Value);
	
	//Pitch Clamps
	float PitchMax, PitchMin;

	//ShootCenterOfScreen
	void ShootArrow();

	//Get Actions Inputs
	void Jump();
	void WalkSlowPressed();
	void WalkSlowReleased();
	void SwitchWeapon();
	void ActionAttack();
	void BlockOrAimPressed();
	void BlockOrAimReleased();
	void SetInputEnable();
};
