// FisheyeEquirectangularDll.cpp : Defines the exported functions for the DLL application.
//

#include "fisheyeEquirectangularDll.h"
#include "fisheyeEquirectangularController.h"

using namespace pp;

FisheyeEquirectangularController* FisheyeEquirectangular::CreateController()
{
	return new FisheyeEquirectangularController();
}

void FisheyeEquirectangular::DestroyController(FisheyeEquirectangularController* controller)
{
	if (controller != nullptr)
	{
		delete controller;
		controller = nullptr;
	}
}

