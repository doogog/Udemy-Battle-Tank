// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"



void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	/*ATank* PlayerTank = GetPlayerTank();
	if ( PlayerTank )
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController found player: %s"), *PlayerTank->GetName());
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController can't find player tank!"));
	}*/
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	APawn* PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	APawn* ControlledTank = GetPawn();

	if ( ensure(PlayerTank) && ensure(ControlledTank) )
	{
		UTankAimingComponent* AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		
		MoveToActor(PlayerTank, AcceptanceRadius);
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		if ( AimingComponent->GetFiringState() == EFiringState::Locked )
		{
			//UE_LOG(LogTemp, Warning, TEXT("AI Fire!!"));
			AimingComponentFire(AimingComponent);
		}
	}
}