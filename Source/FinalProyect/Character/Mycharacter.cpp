// Fill out your copyright notice in the Description page of Project Settings.


#include "Mycharacter.h"
#include "FinalProyect/Actors/MyArrowProjectile.h"
#include "FinalProyect/Actors/Tresure.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.inl>

// Sets default values
AMycharacter::AMycharacter()
{
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bIsUsingSword = true;

	//Setting CharacterMovement
	MovementComponent = GetCharacterMovement();
	
	//Setting springarm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	//Setting Camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh->SetupAttachment(RootComponent);

	PlayerSwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerSwordMesh"));
	PlayerSwordMesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AMycharacter::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<APlayerController>(GetController());

	MovementComponent->MaxWalkSpeed = SpeedMovement;
	ArrowMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"Arrow_Hand");	

	TresureRef = Cast<ATresure>(UGameplayStatics::GetActorOfClass(GetWorld(), ATresure::StaticClass()));

	PlayerSwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Sword_Hand");
	PlayerSwordMesh->SetVisibility(true);

	//Setting Pitchs
	OriginalPitchMax = PitchMax;
	PitchMin = -PitchMax;
	

	UpdateClampCamera(PitchMin, OriginalPitchMax);
}


// Called to bind functionality to input
void AMycharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis(TEXT("MoveForward"), this, &AMycharacter::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AMycharacter::MoveRight);
	InputComponent->BindAxis(TEXT("CameraPitch"), this,&APawn::AddControllerPitchInput);
	InputComponent->BindAxis(TEXT("CameraYawn"), this, &APawn::AddControllerYawInput);


	InputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &AMycharacter::Jump);
	InputComponent->BindAction(TEXT("WalkSlow"), IE_Pressed, this, &AMycharacter::WalkSlowPressed);
	InputComponent->BindAction(TEXT("WalkSlow"), IE_Released, this, &AMycharacter::WalkSlowReleased);
	InputComponent->BindAction(TEXT("SwitchWeapons"), IE_Pressed, this, &AMycharacter::SwitchWeapon);
	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMycharacter::ActionAttack);
	InputComponent->BindAction(TEXT("Block/Aim"), IE_Pressed, this, &AMycharacter::BlockOrAimPressed);
	InputComponent->BindAction(TEXT("Block/Aim"), IE_Released, this, &AMycharacter::BlockOrAimReleased);
	InputComponent->BindAction(TEXT("InteractuateAction"), IE_Pressed, this, &AMycharacter::InteractuateAction);
}


void AMycharacter::MoveForward(float Value)
{
	FVector MoveDirection = GetActorForwardVector();
	AddMovementInput(MoveDirection, Value);

}

void AMycharacter::MoveRight(float Value)
{
	FVector MoveDirection = GetActorRightVector();
	AddMovementInput(MoveDirection, Value);
	
}

void AMycharacter::Jump()
{
	if (!MovementComponent->IsFalling())
	{
		Super::Jump();
	}
}

void AMycharacter::WalkSlowPressed()
{
	MovementComponent->MaxWalkSpeed = SpeedMovement / 2;
}

void AMycharacter::WalkSlowReleased()
{
	MovementComponent->MaxWalkSpeed = SpeedMovement;
}

void AMycharacter::SwitchWeapon()
{
	if (bIsUsingSword && ArrowsAmount > 0)
	{
		AMontage = LoadObject<UAnimMontage>(nullptr,TEXT("/Game/MyCharacters/Animations/ArchAnim/Standing_Equip_Bow_Montage"));
		PlayAnimMontage(AMontage);
		bIsUsingSword = false;
	}
	else
	{
		if (IsArrowAiming) CameraComponent->SetFieldOfView(NoZoom);

		AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/SwordAnim/Withdrawing_Sword_Montage"));
		PlayAnimMontage(AMontage);
		bIsUsingSword = true;
		IsArrowAiming = false;
	}
}

void AMycharacter::ActionAttack()
{
	if (bIsUsingSword)
	{
		SwordAttack();
	}
	else if(IsArrowAiming)
	{
		ShootArrow();
	}
}

void AMycharacter::BlockOrAimPressed()
{

	if (bIsUsingSword)
	{
		Canplayermove = false;
		SetPlayerMove();
		AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/SwordAnim/SwordBlock_Montage"));
		AnimDelay = AMontage->GetPlayLength();
		PlayAnimMontage(AMontage);
		Canplayermove = true;
		GetWorldTimerManager().SetTimer(Delay, this, &AMycharacter::SetPlayerMove, AnimDelay, false);
	}
	else if(ArrowsAmount > 0)
	{
		CameraComponent->SetFieldOfView(ZoomIn); 
		WalkSlowPressed(); 
		AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/ArchAnim/BowDraw_Montage"));
		PlayAnimMontage(AMontage);
		// Reduce camera clamp while is aimming
		PitchMax = PitchMaxAimming;
		PitchMin = -PitchMax;
		IsArrowAiming = true;
		UpdateClampCamera(PitchMin,PitchMax);
	}
	else
	{
		SwitchWeapon();
		IsArrowAiming = false;
	}
	
}

void AMycharacter::BlockOrAimReleased()
{
	
	if(bIsUsingSword){}
	else
	{
		CameraComponent-> SetFieldOfView(NoZoom);
		WalkSlowReleased();
		IsArrowAiming = false;
	}
		//Restore Camera Clamp 
		PitchMax = OriginalPitchMax;
		PitchMin = -PitchMax;
		UpdateClampCamera(PitchMin, PitchMax);
		ArrowMesh->SetVisibility(false);
}

void AMycharacter::ShootArrow()
{
	
	int ViewportSizeX, ViewportSizeY;
	MyPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D ScreenCenter(ViewportSizeX / 2, ViewportSizeY / 2);

	FVector Worldlocation, Worlddirection;
	MyPlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, Worldlocation, Worlddirection);

	FVector StartLocation = Worldlocation;
	FVector EndLocation = Worldlocation + (Worlddirection * ArrowDistance);

	FHitResult Result;
	FCollisionQueryParams TraceParams;

	if (GetWorld()->LineTraceSingleByChannel(Result,StartLocation,EndLocation, ECC_Visibility, TraceParams))
	{
		FVector AimLocation = Result.Location;

		FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(TEXT("SetSpawnArrowLocation"), ar, NULL, true);

		ArrowShootDirection = (AimLocation - SpawnArrowLocation).GetSafeNormal();
		 
		GetWorld()->SpawnActor<AMyArrowProjectile>(Projectile, SpawnArrowLocation, CameraComponent->GetComponentRotation());
		ArrowsAmount--;
	}

	
	if(ArrowsAmount == 0)
	{
	AnimDelay = AMontage->GetPlayLength();
	GetWorldTimerManager().SetTimer(Delay, this, &AMycharacter::SwitchWeapon, AnimDelay, false);
	WalkSlowReleased();
	}
	else 
	{
		ArrowMesh->SetVisibility(false);
		BlockOrAimPressed();
	}
}

void AMycharacter::SwordAttack()
{
	AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/SwordAnim/SwordSlash_Montage"));
		/*Verify if the player is damaged*/
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();	
	if(AnimInstance && AnimInstance->Montage_IsPlaying(AMontage))
	{
		FString CurrentSection = AnimInstance->Montage_GetCurrentSection().ToString();
		FName SectionName = FName(CurrentSection);

		if(SectionName == FName(TEXT("Attack1")))   // Verify if the section is in the first section.
		{
			AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(),"AttackCombo",AMontage);
		}
	}
	else
	{
		PlayAnimMontage(AMontage, 1.0f, "Attack1");
	}
}

void AMycharacter::SetPlayerMove()
{
	if(Canplayermove)
	{
		MovementComponent->MaxWalkSpeed = SpeedMovement;
	}
	else
	{
		MovementComponent->MaxWalkSpeed = 0;
	}
}

void AMycharacter::InteractuateAction()
{
	if (TresureRef->CantakeSkull)
	{
		MyPlayerController->ClientPlayCameraShake(CameraShake);
		TresureRef->SkullTaken();
	}

}

void AMycharacter::UpdateClampCamera(float pitchmin, float pitchmax)
{
	MyPlayerController->PlayerCameraManager->ViewPitchMin = pitchmin;
	MyPlayerController->PlayerCameraManager->ViewPitchMax = pitchmax;
}

void AMycharacter::SphereTrace()
{
	//Defining the spheretrace parameters
	FVector StartLocation = PlayerSwordMesh->GetSocketLocation("Sword_Hand");
	FVector EndLocation = StartLocation;
	FHitResult HitResult;

	//Sphere trace Perform
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(this, StartLocation, EndLocation, SphereRadius, 
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame, HitResult, true);

	if (bHit)
	{
		AActor* ActorHitted = HitResult.GetActor();
		
		if (ActorHitted->ActorHasTag("Corpse"))
		{
			FDamageEvent DamageEvent;
			ActorHitted->TakeDamage(SwordDamageAmount, DamageEvent, GetInstigatorController(), this);
		}
	
	}
	


}
