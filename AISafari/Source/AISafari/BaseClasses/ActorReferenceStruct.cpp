// Fill out your copyright notice in the Description page of Project Settings.

#include "AISafari.h"
#include "ActorReferenceStruct.h"


GlobalStorage::GlobalStorage(){

}

GlobalStorage* GlobalStorage::getInstance(){
	static GlobalStorage* instance = new GlobalStorage();
	return instance;
}

void GlobalStorage::setArray(int key, int length, AActor** _array){
	actorReference[key] = { _array, length };
	int i = 0;
}
AActor** GlobalStorage::getArray(int key){
	//if (actorReference.find(key) != actorReference.end())
	return actorReference[key].arr;
	//return nullptr;
}
int GlobalStorage::getArrayLength(int key){
	return actorReference[key].arrayLength;
}