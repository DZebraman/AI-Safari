#pragma once

#include "AutoAgent.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "FlockerBase.generated.h"
//--------FWD Declaration-----------
class AFlockerManagerBase;

UCLASS()
class AISAFARI_API AFlockerBase : public AAutoAgent
{
	GENERATED_BODY()
protected:
	bool bHasManager;
	AFlockerBase ** flockers;
	std::vector<AFlockerBase*> localFlockers;
	std::vector<int> localFlockerID;
	char * ACTORTYPE = "baseFlockers";
	int numFlockers;

	FVector centroid, sepV;
public:	
	// Sets default values for this actor's properties
	AFlockerBase();

	TSubclassOf<AFlockerManagerBase> * manager;

	//Front facing vars
	UPROPERTY(EditAnywhere, Category = "flocking vars")
		float separation;
	UPROPERTY(EditAnywhere, Category = "flocking vars")
		float cohesion;
	UPROPERTY(EditAnywhere, Category = "flocking vars")
		float alignment;
	UPROPERTY(EditAnywhere, Category = "flocking vars")
		float distThreshold;

	float initDistThresh;

	//Getter / Setter ---------------------------
	virtual FVector getPos();
	virtual void setPos(FVector _pos);
	virtual FVector getCentroid();
	virtual void setCentroid(FVector _centroid);

	//Vector Stuff --------------------------
	virtual void calcForces();
	virtual FVector align();
	virtual FVector calcCentroid();
	virtual void findLocal();
	virtual void cullLocal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
};
