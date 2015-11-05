// FisheyeEquirectangularSample.cpp : Defines the entry point for the console application.
//

#include "FisheyeEquirectangularDll.h"
#include "fisheyeWorkView.h"

int main(int argc, char* argv[])
{
	auto mController = std::shared_ptr<pp::FisheyeEquirectangularController>(pp::FisheyeEquirectangular::CreateController());
	auto mView = std::shared_ptr<pp::FisheyeWorkView>(pp::FisheyeEquirectangular::CreateWorkView());

	mController->open("D:/main/Workspace/PanoramicProcessing/fisheye_to_equirectangular/resources/sigma8-sample_full.jpg");

	mView->bind(mController);

	mView->draw();

	_sleep(1000);
	mController->setParamRadius(500);
	mView->draw();

	_sleep(1000);
	mController->setParamRadius(400);
	mView->draw();

	_sleep(1000);
	mController->setParamRadius(600);
	mView->draw();

	return 0;
}

