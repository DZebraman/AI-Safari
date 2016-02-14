// Fill out your copyright notice in the Description page of Project Settings.

#include "AISafari.h"
#include "AutoAgent.h"

// Sets default values
AAutoAgent::AAutoAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	storage = storage->getInstance();
}

// Called when the game starts or when spawned
void AAutoAgent::BeginPlay()
{
	Super::BeginPlay();
	pos = GetActorLocation();
	fwd = GetActorForwardVector();
}

FVector AAutoAgent::seek(FVector target){
	FVector dv = target - pos;
	dv.ClampMaxSize(maxSpeed);
	dv = dv - vel;

	//DrawDebugLine(GetWorld(), pos, pos + dv.ClampMaxSize(maxSpeed), FColor::Magenta, false, -1.f, (uint8)'\000', 1.f);

	return dv.ClampMaxSize(maxSpeed);
}

FVector AAutoAgent::flee(FVector target){
	FVector dv = target - pos;
	dv.ClampMaxSize(maxSpeed);
	dv = dv - vel;

	//DrawDebugLine(GetWorld(), pos, pos + dv.ClampMaxSize(maxSpeed), FColor::Magenta, false, -1.f, (uint8)'\000', 1.f);

	return -dv.ClampMaxSize(maxSpeed);
}

//Guesses at the future point by comparing the deltaPosition and adding that to the current pos
//flees estimated point
FVector AAutoAgent::evade(FVector target, FVector targetPrev){
	FVector targetV = targetPrev - target;
	return flee(target + targetV);
}

//Guesses at the future point by comparing the deltaPosition and adding that to the current pos
//seeks estimated point
FVector AAutoAgent::pursue(FVector target, FVector targetPrev){
	FVector targetV = targetPrev - target;
	return seek(target + targetV);
}

//SUPER BASIC
//clamps vel relative to how close the actor's position is to their target
FVector AAutoAgent::arrive(FVector target){
	return vel.ClampMaxSize(FMath::Clamp(FVector::Dist(pos, target), 0.f, maxSpeed));
}

void AAutoAgent::setOctant(OctantBase* _octant){ 
	octantRef = _octant;
}

// Called every frame

// Current setup can do everything here, or have every setting modified by a manager class
void AAutoAgent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

