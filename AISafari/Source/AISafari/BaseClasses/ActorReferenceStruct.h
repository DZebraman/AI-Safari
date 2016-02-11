
#pragma once
#include <map>

//whenever objects are created, throw a pointer to them here
class GlobalStorage{
private:
	struct actorRefStorage{
		AActor** arr;
		int arrayLength;
	};

	std::map<int, actorRefStorage> actorReference;

	GlobalStorage();
public:
	GlobalStorage* getInstance();
	void setArray(int key, int length, AActor** _array);
	AActor** getArray(int key);
	int getArrayLength(int key);
};


