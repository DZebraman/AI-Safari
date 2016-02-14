// Fill out your copyright notice in the Description page of Project Settings.

#include "AISafari.h"
#include "FlockerManagerBase.h"


// Sets default values
AFlockerManagerBase::AFlockerManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	storage = storage->getInstance();
}

// Called when the game starts or when spawned
void AFlockerManagerBase::BeginPlay()
{
	Super::BeginPlay();
	flockers = new AFlockerBase*[numAgents];
	for (int i = 0; i < numAgents; ++i){
		FVector tempLocation = location + FMath::VRand() * spawnRandomDist;
		flockers[i] = (AFlockerBase*)GetWorld()->SpawnActor(spawnObject, &tempLocation);
	}
	storage->setArray(1,numAgents,(AActor**)flockers);

	octTreeHead = new OctTreeBase(1,GetWorld());
}

// Called every frame
void AFlockerManagerBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	octTreeHead->findMinMax();
	octTreeHead->subdivide();
	octTreeHead->draw();
}

