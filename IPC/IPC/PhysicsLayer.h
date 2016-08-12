#pragma once
#include "LayerStructure.h"

class PhysicsLayer : public LayerStructure
{
public:
	PhysicsLayer();
	~PhysicsLayer();

	BOOL	Receive();
	BOOL	Send(unsigned char* ppayload, int frameLength);
};

