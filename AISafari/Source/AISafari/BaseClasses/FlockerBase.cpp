// Fill out your copyright notice in the Description page of Project Settings.

#include "AISafari.h"
#include "FlockerBase.h"
#include "FlockerManagerBase.h"
#include <map>

// Sets default values
AFlockerBase::AFlockerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bHasManager = false;
}

// Called when the game starts or when spawned
void AFlockerBase::BeginPlay()
{
	Super::BeginPlay();
	//does this flocker have a manager, or is it auto
	if (manager){
		bHasManager = true;
	}else{
		//sets this array = to the global list of flockers that are the same type
		flockers = (AFlockerBase**)storage->getArray(1);
		numFlockers = storage->getArrayLength(1);
	}

	//converts human readable threshold into square so we don't have to use sqrt later
	centroid =  pos + (FMath::VRand() * 20);
	initDistThresh = distThreshold;
	distThreshold *= distThreshold;
	
}

//Getter / Setters
FVector AFlockerBase::getPos(){return pos;}
void AFlockerBase::setPos(FVector _pos){pos = _pos;}
FVector AFlockerBase::getCentroid(){ return pos;}
void AFlockerBase::setCentroid(FVector _centroid){ centroid = _centroid; }

void AFlockerBase::calcForces(){
	acc = FVector::ZeroVector;

	acc += seek(centroid) * cohesion;

	if (sepV != FVector::ZeroVector)
		acc += flee(sepV) * separation;
	fwd = FMath::Lerp(fwd, align(), 0.1);
}

// Averages the forward vector of all local flockers
FVector AFlockerBase::align(){
	FVector align = FVector::ZeroVector;//localFlockers[0]->GetActorForwardVector();
	for (int i = 0; i < localFlockers.size(); i++){
		align += localFlockers[i]->GetActorForwardVector();
	}
	return (align /= localFlockers.size());
}

void AFlockerBase::findFlee(){
	float closest = 1000 * 1000;
	sepV = FVector::ZeroVector;

	std::vector<AActor*> tempFlock = *octantRef->getActors();

	for (int i = 0; i < tempFlock.size(); ++i){
		FVector tempVec = tempFlock[i]->GetActorLocation();
		float dist = FVector::DistSquared(pos, tempVec);
		if (dist < closest){
			sepV = tempVec;
			closest = dist;
		}
	}
}

//finds the centroid
FVector AFlockerBase::calcCentroid(){
	FVector _centroid = FVector::ZeroVector;

	

	for (int i = 0; i < localFlockers.size(); i++){
		FVector tempVec = localFlockers[i]->GetActorLocation();
		_centroid += tempVec;
		

	}

	if (_centroid != FVector::ZeroVector)
		return (_centroid /= localFlockers.size());
	else
		return centroid;
}

void AFlockerBase::findLocalOct(){
	OctantBase* thisOct = octantRef;
	int upRecurse = 0;
	while (thisOct->hasHead() && upRecurse++ < 3){
		thisOct = thisOct->getHead();
	}
	nearestNeighborRecurse(thisOct);
}

void AFlockerBase::nearestNeighborRecurse(OctantBase* thisOct){
	if (thisOct->hasChildren()){
		OctantBase** children = *thisOct->getChildren();
		for (int i = 0; i < 8; ++i){

			FVector tempCenter = children[i]->getCenter();
			FVector tempHW = children[i]->getRadius();

			// Checks if any of the octant's verts are in the desired range.
			// if they are it recurses into that octant to see if which actors are within the range

			if (FVector::DistSquared(tempCenter + tempHW, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}

			FVector centerOther;
			centerOther.X = tempHW.X; centerOther.Y = tempHW.Y; centerOther.Z = -tempHW.Z;
			if (FVector::DistSquared(centerOther, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}

			centerOther.X = -tempHW.X; centerOther.Y = tempHW.Y; centerOther.Z = tempHW.Z;
			if (FVector::DistSquared(centerOther, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}

			centerOther.X = -tempHW.X; centerOther.Y = tempHW.Y; centerOther.Z = -tempHW.Z;
			if (FVector::DistSquared(centerOther, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}

			centerOther.X = tempHW.X; centerOther.Y = -tempHW.Y; centerOther.Z = tempHW.Z;
			if (FVector::DistSquared(centerOther, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}

			centerOther.X = -tempHW.X; centerOther.Y = -tempHW.Y; centerOther.Z = tempHW.Z;
			if (FVector::DistSquared(centerOther, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}

			centerOther.X = tempHW.X; centerOther.Y = -tempHW.Y; centerOther.Z = -tempHW.Z;
			if (FVector::DistSquared(centerOther, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}

			else if (FVector::DistSquared(tempCenter - tempHW, pos) < distThreshold){
				nearestNeighborRecurse(children[i]);
				continue;
			}	
		}
	}
	std::vector<AActor*> octFlock = *thisOct->getActors();
	for (int i = 0; i < octFlock.size(); i++){
		if (FVector::DistSquared(pos, octFlock[i]->GetActorLocation()) < distThreshold)
		localFlockers.push_back(octFlock[i]);
	}
}

//finds all flockers withing a certain radius
void AFlockerBase::findLocal(){
	//localFlockers.clear();
	//localFlockerID.clear();

	for (int i = 0; i < numFlockers; i++){
		if (FVector::DistSquared(pos, flockers[i]->GetActorLocation()) < distThreshold){

			
			bool breakLoop = false;
			//is this data inside localFlocker?
			for (int k = 0; k < localFlockerID.size(); k++){
				breakLoop = (i == localFlockerID[k]);
				if (breakLoop) { break; }
			} if (breakLoop) continue;
			localFlockers.push_back(flockers[i]);
			localFlockerID.push_back(i);
		}
	}
}

void AFlockerBase::cullLocal(){
	for (int i = 0; i < localFlockers.size(); ++i){
		if (FVector::DistSquared(pos, flockers[i]->GetActorLocation()) > distThreshold){
			localFlockers.erase(localFlockers.begin() + i);
			localFlockerID.erase(localFlockerID.begin() + i);
			continue;
		}
		else if (localFlockers[i]->GetActorLocation() == pos){
			localFlockers.erase(localFlockers.begin() + i);
			localFlockerID.erase(localFlockerID.begin() + i);
		}
	}
}

// Called every frame
void AFlockerBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (!bHasManager){
		if (!octantRef){
			findLocal();
			cullLocal();
		}
		else{
			//localFlockers = *(octantRef->getActors());
			localFlockers.clear();
			findLocalOct();
		}
		centroid = calcCentroid();
		findFlee();
	}

	//(mostly)Prevents stragglers by increasing the threshold organically based on how many connections 
	distThreshold = FMath::Lerp((initDistThresh * 5)*(initDistThresh * 5), initDistThresh*initDistThresh, FMath::Clamp(localFlockers.size() / 10.f, 0.f, 1.f));

	calcForces();
	vel *= 0.9f;

	vel += acc;
	vel.ClampMaxSize(maxSpeed);
	pos += vel * DeltaTime;

	//DrawDebugLine(GetWorld(), pos, centroid, FColor::Red,false,-1.f,(uint8)'\000',2.f);

	/*for (int i = 0; i < localFlockers.size(); ++i){
		DrawDebugLine(GetWorld(), pos, localFlockers[i]->GetActorLocation(), FColor::Black, false, -1.f, (uint8)'\000', 1.f);
	}*/

	SetActorLocation(pos);
	SetActorRotation(fwd.Rotation());
}

