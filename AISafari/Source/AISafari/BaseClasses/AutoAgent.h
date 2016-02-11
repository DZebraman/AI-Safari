// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "ActorReferenceStruct.h"
#include "GameFramework/Actor.h"
#include "AutoAgent.generated.h"

//--------FWD Declaration-----------
class OctantBase;

UCLASS()
class AISAFARI_API AAutoAgent : public AActor
{
	GENERATED_BODY()
	
protected:
	FVector pos, vel, acc, fwd;

public:	
	// Sets default values for this actor's properties
	AAutoAgent();

	GlobalStorage *storage;

	//UPROPERY(EditAnywhere, Category = "init vars");
	UPROPERTY(EditAnywhere, Category = "init var")
		float maxSpeed;

	OctantBase* octantRef;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual FVector seek(FVector target);
	virtual FVector flee(FVector target);
	virtual FVector pursue(FVector target, FVector targetPrev);
	virtual FVector evade(FVector target, FVector targetPrev);
	virtual FVector arrive(FVector target);
	virtual void setOctant(OctantBase* _octant);

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
