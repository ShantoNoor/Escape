// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorOpen.generated.h" 

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UDoorOpen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

	float GetTotalMassOfActors() const;

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpen;

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnClose;

private:
	FRotator CurrentRotation;

	float InitialYaw;
	bool IsDoorOppened = false;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float MassThatOpens;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY() UAudioComponent* AudioComponent = nullptr;
};
