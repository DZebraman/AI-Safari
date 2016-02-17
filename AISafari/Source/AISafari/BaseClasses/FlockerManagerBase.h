// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FlockerBase.h"
#include "ActorReferenceStruct.h"
#include "GameFramework/Actor.h"
#include "OctTreeBase.h"
#include "FlockerManagerBase.generated.h"

UCLASS()
class AISAFARI_API AFlockerManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlockerManagerBase();

	GlobalStorage* storage;

	OctTreeBase* octTreeHead;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "init vars")
		TSubclassOf<AFlockerBase> spawnObject;
	UPROPERTY(EditAnywhere, Category = "init vars")
		FVector location;
	UPROPERTY(EditAnywhere, Category = "init vars")
		float spawnRandomDist;
	UPROPERTY(EditAnywhere, Category = "init vars")
		int32 numAgents = 200;

	AFlockerBase** flockers;
	
	//updates 10 times a second
	virtual void updateOctree();

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
