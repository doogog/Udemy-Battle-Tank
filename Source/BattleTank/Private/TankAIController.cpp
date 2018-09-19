// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"



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

	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ATank* ControlledTank = Cast<ATank>(GetPawn());

	if ( PlayerTank )
	{
		MoveToActor(PlayerTank, AcceptanceRadius);

		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		//ControlledTank->Fire();
	}
}