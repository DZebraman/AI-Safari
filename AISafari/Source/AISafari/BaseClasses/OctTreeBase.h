// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AutoAgent.h"
/**
 * 
 */

class AISAFARI_API OctTreeBase
{
private:
	OctantBase** octants;
public:
	OctTreeBase();
	~OctTreeBase();
};
