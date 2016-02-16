// Fill out your copyright notice in the Description page of Project Settings.

#include "AISafari.h"
#include "OctantBase.h"

OctantBase::OctantBase(FVector _center, FVector _halfwidth, std::vector<AActor*>& newActors, int recursionDepth, UWorld* _world)
{
	center = _center;
	halfwidth = _halfwidth;

	setActors(newActors);

	world = _world;

	if (halfwidth.Size() * 2 > 400)
		maxRecursions += ((halfwidth.Size() * 2) - 400) / 100 ;


	currentRecursion = recursionDepth;

	if (localActors.size() > lowerActorLimit && currentRecursion < maxRecursions)
		subdivide();
}

void OctantBase::setActors(std::vector<AActor*>& newActors){
	for (int i = 0; i < newActors.size(); ++i){
		if (pointInBox(newActors[i]->GetActorLocation())){
			localActors.push_back(newActors[i]);

			((AAutoAgent*)newActors[i])->setOctant(this);
		}
	}
}

void OctantBase::draw(){
	if (bHasChildren){
		for (int i = 0; i < 8; i++){
			children[i]->draw();
		}
	}
	DrawDebugBox(world, center, halfwidth, FColor::Red);
}

void OctantBase::killChildren(){
	if (bHasChildren){
		for (int i = 0; i < 8; i++){
			children[i]->killChildren();
			delete children[i];
		}
	}
}

void OctantBase::subdivide(){
	children = new OctantBase*[8];
	bHasChildren = true;
	children[0] = new OctantBase(center - halfwidth / 2, halfwidth / 2, localActors, currentRecursion + 1, world);
	children[0]->setHead(this);

	FVector octCenter;
	octCenter.X = center.X - halfwidth.X / 2; octCenter.Y = center.Y - halfwidth.Y / 2; octCenter.Z = center.Z + halfwidth.Z / 2;
	children[1] = new OctantBase(octCenter, halfwidth / 2, localActors, currentRecursion + 1,world);
	children[1]->setHead(this);

	octCenter.X = center.X + halfwidth.X / 2; octCenter.Y = center.Y - halfwidth.Y / 2; octCenter.Z = center.Z - halfwidth.Z / 2;
	children[2] = new OctantBase(octCenter, halfwidth / 2, localActors, currentRecursion + 1, world);
	children[2]->setHead(this);

	octCenter.X = center.X + halfwidth.X / 2; octCenter.Y = center.Y - halfwidth.Y / 2; octCenter.Z = center.Z + halfwidth.Z / 2;
	children[3] = new OctantBase(octCenter, halfwidth / 2, localActors, currentRecursion + 1, world);
	children[3]->setHead(this);

	octCenter.X = center.X - halfwidth.X / 2; octCenter.Y = center.Y + halfwidth.Y / 2; octCenter.Z = center.Z - halfwidth.Z / 2;
	children[4] = new OctantBase(octCenter, halfwidth / 2, localActors, currentRecursion + 1, world);
	children[4]->setHead(this);

	octCenter.X = center.X + halfwidth.X / 2; octCenter.Y = center.Y + halfwidth.Y / 2; octCenter.Z = center.Z - halfwidth.Z / 2;
	children[5] = new OctantBase(octCenter, halfwidth / 2, localActors, currentRecursion + 1, world);
	children[5]->setHead(this);

	octCenter.X = center.X - halfwidth.X / 2; octCenter.Y = center.Y + halfwidth.Y / 2; octCenter.Z = center.Z + halfwidth.Z / 2;
	children[6] = new OctantBase(octCenter, halfwidth / 2, localActors, currentRecursion + 1, world);
	children[6]->setHead(this);

	children[7] = new OctantBase(center + halfwidth / 2, halfwidth / 2, localActors, currentRecursion + 1, world);
	children[7]->setHead(this);
}

std::vector<AActor*>* OctantBase::getActors(){
	if (!&localActors){
		std::vector<AActor*>* butts = new std::vector<AActor*>;
		return butts;
	}
	return &localActors;
}

// if the point is outside any of these bounds then it isn't in the box
// will exit at first failure
bool OctantBase::pointInBox(FVector point){
	FVector temp = point;

	if (point.X < (center - halfwidth).X) return false;
	if (point.Y < (center - halfwidth).Y) return false;
	if (point.Z < (center - halfwidth).Z) return false;

	if (point.X > (center + halfwidth).X) return false;
	if (point.Y > (center + halfwidth).Y) return false;
	if (point.Z > (center + halfwidth).Z) return false;

	return true;
}

void OctantBase::clearChildren(){
	if (children){
		for (int i = 0; i < 8; i++){
			children[i]->clearChildren();
			delete children[i];
		}
		delete children;
	}
}

void OctantBase::setHead(OctantBase* _head){ head = _head; bHasHead = true; }
OctantBase* OctantBase::getHead(){ return head; }
bool OctantBase::hasHead(){return bHasHead;}
FVector OctantBase::getCenter(){ return center; }
FVector OctantBase::getRadius(){ return halfwidth; }
bool OctantBase::hasChildren(){ return bHasChildren; }
OctantBase*** OctantBase::getChildren(){ return &children; }

OctantBase::~OctantBase()
{
}
