// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "RC_CarBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAudioComponent;
class UMaterialParameterCollection;

UCLASS()
class VEHICLE_MECHANICS_API ARC_CarBase : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
private:
	void Throttle(float Value);
	void Reverse(float Value);
	void Steering(float Value);
	void HandbrakePressed();
	void HandBrakeReleased();
	void LookUpDown(float Value);
	void LookRL(float Value);

	void BrakeLightToggle(bool bValue);

	void IncreaseSmokeExhaust();
	void DecreaseSmokeExhaust();

	void ActivateWheelTrail(bool bIsHandbrake);
	void DeActivateWheelTrail();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

public:

	ARC_CarBase();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* CameraComp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAudioComponent* EngineSoundComp;

	//Rear Light
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialParameterCollection* MPC_Car;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraComponent* NS_ExhaustR;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraComponent* NS_ExhaustL;

	//Wheel Trails
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraComponent* NS_RRTrail;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraComponent* NS_RLTrail;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraComponent* NS_FRTrail;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraComponent* NS_FLTrail;

};
