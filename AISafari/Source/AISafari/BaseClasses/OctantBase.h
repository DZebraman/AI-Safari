// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AISafari.h"
#include "AutoAgent.h"
#include <vector>

/**
 * 
 */
class AISAFARI_API OctantBase
{
protected:
	FVector center, halfwidth;
	OctantBase** children;
	OctantBase* head;
	//super simple check to see if point is in the box
	virtual bool pointInBox(FVector point);
	std::vector<AActor*> localActors;
	
	int maxRecursions = 5;
	int currentRecursion;

	int lowerActorLimit = 5;
	UWorld* world;
	bool hasChildren = false;

public:
	virtual void clearChildren();
	virtual void subdivide();
	virtual void setActors(std::vector<AActor*>& newActors);
	virtual void draw();
	virtual void setHead(OctantBase* _head);
	virtual void killChildren();
	virtual OctantBase* getHead();
	virtual std::vector<AActor*>* getActors();
	OctantBase(FVector _center, FVector _halfwidth, std::vector<AActor*>& newActors, int recursionDepth, UWorld* _world);
	~OctantBase();
};
