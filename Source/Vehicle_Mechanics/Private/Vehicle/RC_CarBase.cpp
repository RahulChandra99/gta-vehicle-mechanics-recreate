// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/RC_CarBase.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MaterialParameterCollection.generated.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "NiagaraComponent.h"

ARC_CarBase::ARC_CarBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600;
	SpringArm->SetRelativeLocation(FVector(0,0,140));
	SpringArm->SetRelativeRotation(FRotator(-10,0,0));
	SpringArm->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);

	EngineSoundComp = CreateDefaultSubobject<UAudioComponent>("EngineSounds");
	EngineSoundComp->SetupAttachment(GetRootComponent());

	NS_ExhaustR = CreateDefaultSubobject<UNiagaraComponent>("ExhaustRight");
	NS_ExhaustR->SetupAttachment(GetMesh(),TEXT("R_Exhaust"));
	NS_ExhaustL = CreateDefaultSubobject<UNiagaraComponent>("ExhaustLeft");
	NS_ExhaustL->SetupAttachment(GetMesh(),TEXT("L_Exhaust"));

	NS_RRTrail = CreateDefaultSubobject<UNiagaraComponent>("NS_RRTrail");
	NS_RRTrail->SetupAttachment(GetMesh(),TEXT("RR_Ground"));
	NS_RLTrail = CreateDefaultSubobject<UNiagaraComponent>("NS_RLTrail");
	NS_RLTrail->SetupAttachment(GetMesh(),TEXT("RL_Ground"));
	NS_FRTrail = CreateDefaultSubobject<UNiagaraComponent>("NS_FRTrail");
	NS_FRTrail->SetupAttachment(GetMesh(),TEXT("FR_Ground"));
	NS_FLTrail = CreateDefaultSubobject<UNiagaraComponent>("NS_FLTrail");
	NS_FLTrail->SetupAttachment(GetMesh(),TEXT("FL_Ground"));

}

void ARC_CarBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	// Get the vehicle movement component
	UChaosWheeledVehicleMovementComponent* VehicleMovement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if (!VehicleMovement) { return ;}
	
	// Get the engine rotation speed
	float RPM = VehicleMovement->GetEngineRotationSpeed();

	EngineSoundComp->SetFloatParameter(TEXT("RPM"),RPM);

	if(RPM > 1500 && RPM < 4000)
		IncreaseSmokeExhaust();
	else
		DecreaseSmokeExhaust();
}

void ARC_CarBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Throttle", this, &ARC_CarBase::Throttle);
	PlayerInputComponent->BindAxis("Reverse", this, &ARC_CarBase::Reverse);
	PlayerInputComponent->BindAxis("MoveLR", this, &ARC_CarBase::Steering);
	PlayerInputComponent->BindAxis("LookLR", this, &ARC_CarBase::LookRL);
	PlayerInputComponent->BindAxis("LookUD", this, &ARC_CarBase::LookUpDown);
	
	
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ARC_CarBase::HandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ARC_CarBase::HandBrakeReleased);

}

void ARC_CarBase::Throttle(float Value)
{
	GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void ARC_CarBase::Reverse(float Value)
{
	GetVehicleMovementComponent()->SetBrakeInput(Value);

	if(GetVehicleMovementComponent()->GetForwardSpeed() < 20)
		return;

	if(Value>0 && !NS_FRTrail->IsActive())
	{
		ActivateWheelTrail(false);
	}
	else if(Value == 0 && NS_FRTrail->IsActive())
	{
		DeActivateWheelTrail();
	}
		
}

void ARC_CarBase::Steering(float Value)
{
	GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void ARC_CarBase::HandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
	BrakeLightToggle(true);

	ActivateWheelTrail(true);
}

void ARC_CarBase::HandBrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
	BrakeLightToggle(false);

	DeActivateWheelTrail();
}

void ARC_CarBase::LookUpDown(float Value)
{
	AddControllerPitchInput(-Value);
}

void ARC_CarBase::LookRL(float Value)
{
	AddControllerYawInput(Value);
}

void ARC_CarBase::BrakeLightToggle(bool bValue)
{
	if(!MPC_Car) { return;}
	if(bValue)
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Car, FName("EdgeIntensity"),70000);
	}
	else
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Car, FName("EdgeIntensity"),1);
	}
}

void ARC_CarBase::IncreaseSmokeExhaust()
{
	NS_ExhaustL->SetFloatParameter(FName("SpawnRate"),300);
	NS_ExhaustR->SetFloatParameter(FName("SpawnRate"),300);
	
}

void ARC_CarBase::DecreaseSmokeExhaust()
{
	NS_ExhaustL->SetFloatParameter(FName("SpawnRate"),20);
	NS_ExhaustR->SetFloatParameter(FName("SpawnRate"),20);
}

void ARC_CarBase::ActivateWheelTrail(bool bIsHandbrake)
{
	NS_RRTrail->Activate();
	NS_RLTrail->Activate();

	if(bIsHandbrake) return;
		
	NS_FRTrail->Activate();
	NS_FLTrail->Activate();
}

void ARC_CarBase::DeActivateWheelTrail()
{
	NS_RRTrail->Deactivate();
	NS_RLTrail->Deactivate();
	NS_FRTrail->Deactivate();
	NS_FLTrail->Deactivate();
}
