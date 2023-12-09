#pragma once
#include <json.h>

class GameObject
{
public:
	GameObject();

	//Set true if we want this cass to replicate the data 
	bool bReplicates;
	bool bReplicateTransform;

	virtual void Init();

protected: 

	//Json root value. We should add here all the information we want to send by network
	Json::Value localRootData;
};