// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpen.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UDoorOpen::UDoorOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();

	CurrentRotation = GetOwner()->GetActorRotation();
	InitialYaw += CurrentRotation.Yaw;
	OpenAngle += InitialYaw;

	if(!PressurePlate) UE_LOG(LogTemp, Error, TEXT("%s has opendoor component attach to it with no PressurePlate set!"), *GetOwner()->GetName());

	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
}


// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetTotalMassOfActors() >= MassThatOpens)
	{
		// DoorLastOpened = GetWorld()->GetTimeSeconds();
		OpenDoor(DeltaTime);
	}
	else
	{
		// if(DoorCloseDelay <= GetWorld()->GetTimeSeconds() - DoorLastOpened)
		// {
		// }
		CloseDoor(DeltaTime);
	}
}

void UDoorOpen::OpenDoor(float DeltaTime) 
{
	// if(AudioComponent && FMath::IsNearlyEqual(InitialYaw, CurrentRotation.Yaw, 1.f)) AudioComponent->Play();
	// CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, OpenAngle, DeltaTime);
	// CurrentRotation.Yaw = OpenAngle;
	// GetOwner()->SetActorRotation(CurrentRotation);
	// IsDoorOppened = true;

	OnOpen.Broadcast();
}
void UDoorOpen::CloseDoor(float DeltaTime) 
{
	// CurrentRotation.Yaw = FMath::Lerp(CurrentRotation.Yaw, InitialYaw, DeltaTime);
	// CurrentRotation.Yaw = InitialYaw;
	// GetOwner()->SetActorRotation(CurrentRotation);
	// if(AudioComponent && IsDoorOppened && FMath::IsNearlyEqual(InitialYaw, CurrentRotation.Yaw, 1.f)) 
	// {
	// 	AudioComponent->Play();
	// 	IsDoorOppened = false;
	// }
	OnClose.Broadcast();
}

float UDoorOpen::GetTotalMassOfActors() const
{
	float TotalMassOfOverlapingActors = 0.f;

	TArray<AActor*> OverlappingActors;

	if(!PressurePlate) { return TotalMassOfOverlapingActors; }
	PressurePlate->GetOverlappingActors(OverlappingActors);

	if(OverlappingActors.Num())
	{
		for(int32 i = 0; i < OverlappingActors.Num(); i++)
		{
			TotalMassOfOverlapingActors += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			UE_LOG(LogTemp, Error, TEXT("%s in Position TM = %f"), *OverlappingActors[i]->GetName(), TotalMassOfOverlapingActors);
		}
	}

	return TotalMassOfOverlapingActors;
}