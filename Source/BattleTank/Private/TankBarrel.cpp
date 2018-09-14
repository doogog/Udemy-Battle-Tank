// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Engine/World.h

void UTankBarrel::Elevate(float RelativeSpeed)
{
	float Time = GetWorld()->GetTimeSeconds();
	UE_LOG( LogTemp, Warning, TEXT("%f: Barrel->Elevate called at speed: %f"), Time, RelativeSpeed );
}