// Fill out your copyright notice in the Description page of Project Settings.

#include "AISafari.h"
#include "OctTreeBase.h"

OctTreeBase::OctTreeBase(int key, UWorld* _world)
{
	world = _world;

	storage =storage->getInstance();

	actors = storage->getArray(key);
	numActors = storage->getArrayLength(key);

	findMinMax();
}

void OctTreeBase::subdivide(){
	headActor->killChildren();
	headActor->subdivide();
}

void OctTreeBase::draw(){
	headActor->draw();
}

void OctTreeBase::findMinMax(){
	min = actors[0]->GetActorLocation();
	max = actors[0]->GetActorLocation();
	centroid = actors[0]->GetActorLocation();

	actorsV.clear();
	actorsV.push_back(actors[0]);


	for (int i = 1; i < numActors; i++){
		FVector tempPos = actors[i]->GetActorLocation();
		actorsV.push_back(actors[i]);
		centroid += tempPos;

		/*if (max.X < tempPos.X) max.X = tempPos.X;
		else if (min.X > tempPos.X) min.X = tempPos.X;

		if (max.Y < tempPos.Y) max.Y = tempPos.Y;
		else if (min.Y > tempPos.Y) min.Y = tempPos.Y;

		if (max.Z < tempPos.Z) max.Z = tempPos.Z;
		else if (min.Z > tempPos.Z) min.Z = tempPos.Z;*/
	}
	centroid /= numActors;

	float minDist = FVector::Dist(centroid, actors[0]->GetActorLocation());
	float maxDist = minDist;

	for (int i = 1; i < numActors; i++){
		float dist = FVector::Dist(centroid, actors[i]->GetActorLocation());
		if (dist > maxDist) { maxDist = dist; max = actors[i]->GetActorLocation(); }
		if (dist < minDist) { minDist = dist; min = actors[i]->GetActorLocation(); }
	}

	//halfwidth = (max - min) / 2;
	float dist = FVector::Dist(centroid, max);
	float dist2 = FVector::Dist(centroid, min);
	if (FVector::Dist(centroid, max) > FVector::Dist(centroid, min))
		halfwidth = FVector(dist);
	else
		halfwidth = FVector(dist2);

	headActor = new OctantBase(centroid, halfwidth, actorsV, 0,world);
}

OctTreeBase::~OctTreeBase()
{
}
