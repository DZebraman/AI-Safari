// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AutoAgent.h"
#include "OctantBase.h"
#include "ActorReferenceStruct.h"
#include <vector>
/**
 * 
 */

class AISAFARI_API OctTreeBase
{
protected:
	OctantBase* headActor;

	GlobalStorage * storage;
	AActor** actors;
	std::vector<AActor*> actorsV;
	int numActors;
	UWorld* world;
	FVector min, max, centroid, halfwidth;

public:
	OctTreeBase(int key, UWorld* _world);
	void subdivide();
	void draw();
	void findMinMax();
	~OctTreeBase();
};
