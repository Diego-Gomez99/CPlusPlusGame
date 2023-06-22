// Fill out your copyright notice in the Description page of Project Settings.


#include "MycharacterCPlusPlus.h"
#include "MyArrowProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.inl>

// Sets default values
AMycharacterCPlusPlus::AMycharacterCPlusPlus()
{
	// Getting Player Controller
	MyPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting CharacterMovement
	MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = SpeedMovement;
	//Setting springarm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bUsePawnControlRotation = true;
	//Setting Camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->SetWorldLocation(FVector((7635.0f, 7510.0f, 88.0f)));
	//Creating Cog Static Mesh Component
	CogMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CogMesh"));
	CogMesh->SetupAttachment(GetMesh());  //Seting as Character Mesh child
	//Creating Sword Skeletal Mesh Component
	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(GetMesh());
	//Creating Bow Skeletal Mesh Component
	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	BowMesh->SetupAttachment(GetMesh());


	//Setting Pitchs
	PitchMax = 30.0f;
	PitchMin = -PitchMax;
}

// Called when the game starts or when spawned
void AMycharacterCPlusPlus::BeginPlay()
{
	Super::BeginPlay();


	if (CogMesh && SwordMesh && BowMesh && ArrowMesh)  // If CogMesh exist
	{
		CogMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Cog"); // Attach the CogMesh to Cog socket in the Skeletal Mesh
		CogMesh->AddRelativeLocation(FVector((0.0f,6.0f,14.0f))); // Adjust the position of the CogMesh

		SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Sword");

		BowMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Bow_Hand");
		ArrowMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Arrow_Hand");

		
	}
		ArrowMesh->SetVisibility(false);
	
}

// Called every frame
void AMycharacterCPlusPlus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		FRotator NewRotation = GetControlRotation();
		NewRotation.Yaw += CameraDirection.X;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(NewRotation);
	}
	{
		FRotator NewRotation = CameraComponent->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraDirection.Y, PitchMin, PitchMax);
		CameraComponent->SetWorldRotation(NewRotation);
	}


	
}

// Called to bind functionality to input
void AMycharacterCPlusPlus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis(TEXT("MoveForward"), this, &AMycharacterCPlusPlus::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AMycharacterCPlusPlus::MoveRight);
	InputComponent->BindAxis(TEXT("CameraPitch"), this, &AMycharacterCPlusPlus::CameraPitch);
	InputComponent->BindAxis(TEXT("CameraYawn"), this, &AMycharacterCPlusPlus::CameraYawn);

	InputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &AMycharacterCPlusPlus::Jump);
	InputComponent->BindAction(TEXT("WalkSlow"), IE_Pressed, this, &AMycharacterCPlusPlus::WalkSlowPressed);
	InputComponent->BindAction(TEXT("WalkSlow"), IE_Released, this, &AMycharacterCPlusPlus::WalkSlowReleased);
	InputComponent->BindAction(TEXT("SwitchWeapons"), IE_Pressed, this, &AMycharacterCPlusPlus::SwitchWeapon);
	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMycharacterCPlusPlus::ActionAttack);
	InputComponent->BindAction(TEXT("Block/Aim"), IE_Pressed, this, &AMycharacterCPlusPlus::BlockOrAimPressed);
	InputComponent->BindAction(TEXT("Block/Aim"), IE_Released, this, &AMycharacterCPlusPlus::BlockOrAimReleased);
}


void AMycharacterCPlusPlus::MoveForward(float Value)
{
	FVector MoveDirection = GetActorForwardVector();
	AddMovementInput(MoveDirection, Value);
}

void AMycharacterCPlusPlus::MoveRight(float Value)
{
	FVector MoveDirection = GetActorRightVector();
	AddMovementInput(MoveDirection, Value);
}

void AMycharacterCPlusPlus::CameraPitch(float Value)
{
	CameraDirection.Y = Value;
}

void AMycharacterCPlusPlus::CameraYawn(float Value)
{
	CameraDirection.X = Value;
}

void AMycharacterCPlusPlus::Jump()
{
	if (!MovementComponent->IsFalling())
	{
		Super::Jump();
	}
}

void AMycharacterCPlusPlus::WalkSlowPressed()
{
	MovementComponent->MaxWalkSpeed = SpeedMovement / 2;
}

void AMycharacterCPlusPlus::WalkSlowReleased()
{
	MovementComponent->MaxWalkSpeed = SpeedMovement;
}

void AMycharacterCPlusPlus::SwitchWeapon()
{
	if (bIsUsingSword && ArrowsAmount > 0)
	{
		AMontage = LoadObject<UAnimMontage>(nullptr,TEXT("/Game/MyCharacters/Animations/ArchAnim/Standing_Equip_Bow_Montage"));
		PlayAnimMontage(AMontage);
		bIsUsingSword = false;
	}
	else
	{
		if (IsArrowAiming) CameraComponent->SetFieldOfView(90.0f);

		AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/SwordAnim/Withdrawing_Sword_Montage"));
		PlayAnimMontage(AMontage);
		bIsUsingSword = true;
		IsArrowAiming = false;
	}
}

void AMycharacterCPlusPlus::ActionAttack()
{
	if (bIsUsingSword)
	{
		CanEnableInput = false;
		SetInputEnable();
		AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/SwordAnim/SwordSlash_Montage"));
		CanEnableInput = true;
		GetWorldTimerManager().SetTimer(Delay, this, &AMycharacterCPlusPlus::SetInputEnable, 1.35f, false);
		PlayAnimMontage(AMontage);
	}
	else if(IsArrowAiming)
	{
			AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/ArchAnim/Bow_Shoot_Montage"));
			PlayAnimMontage(AMontage);
			ShootArrow();
			ArrowsAmount--;	

			if(ArrowsAmount == 0)
			{
				AnimDelay = AMontage->GetPlayLength();
				GetWorldTimerManager().SetTimer(Delay, this, &AMycharacterCPlusPlus::SwitchWeapon, AnimDelay, false);
				WalkSlowReleased();
			}
			else 
			{
				ArrowMesh->SetVisibility(false);
				BlockOrAimPressed();
			}
	}
}

void AMycharacterCPlusPlus::BlockOrAimPressed()
{
	if (bIsUsingSword)
	{
		CanEnableInput = false;
		SetInputEnable();
		AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/SwordAnim/SwordBlock_Montage"));
		PlayAnimMontage(AMontage);
		AnimDelay = AMontage->GetPlayLength();
		CanEnableInput = true;
		GetWorldTimerManager().SetTimer(Delay, this, &AMycharacterCPlusPlus::SetInputEnable, AnimDelay, false);
	}
	else if(ArrowsAmount > 0)
	{
		CameraComponent->SetFieldOfView(50.0f);
		WalkSlowPressed();
		AMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/MyCharacters/Animations/ArchAnim/BowDraw_Montage"));
		PlayAnimMontage(AMontage);
		// Reduce camera clamp while is aimming
		PitchMax = 15.0f;
		PitchMin = -PitchMax;
		IsArrowAiming = true;
	}
	else
	{
		SwitchWeapon();
		IsArrowAiming = false;
	}
	
}

void AMycharacterCPlusPlus::BlockOrAimReleased()
{
	if (bIsUsingSword) {}
	else 
	{
		CameraComponent-> SetFieldOfView(90.0f);
		WalkSlowReleased();
		IsArrowAiming = false;
	}
	//Restore Camera Clamp 
	PitchMax = 30.0f;
	PitchMin = -PitchMax;
	//Hide Arrow Mesh
	ArrowMesh->SetVisibility(false);
}

void AMycharacterCPlusPlus::ShootArrow()
{
	FVector StartLocation = CameraComponent->GetComponentLocation();
	FVector EndLocation = ((CameraComponent->GetForwardVector()) * 50000.0f) + CameraComponent->GetComponentLocation();
	FVector SpawnArrowLocation = BowMesh->GetSocketLocation("ArrowSpawn");

	FCollisionQueryParams TraceParams(FName(TEXT("LineTraceByChannel")), true);

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, TraceParams))
	{
		AActor* HitActor = HitResult.GetActor();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation( SpawnArrowLocation,HitResult.Location);
		GetWorld()->SpawnActor<AMyArrowProjectile>(AMyArrowProjectile::StaticClass(),SpawnArrowLocation,LookAtRotation);
	}
	else
	{
		GetWorld()->SpawnActor<AMyArrowProjectile>(AMyArrowProjectile::StaticClass(), SpawnArrowLocation, CameraComponent->GetComponentRotation());
	}
}

void AMycharacterCPlusPlus::SetInputEnable()
{
	if (CanEnableInput)
	{
		EnableInput(MyPlayerController);
	}
	else
	{
		DisableInput(MyPlayerController);
	}

}