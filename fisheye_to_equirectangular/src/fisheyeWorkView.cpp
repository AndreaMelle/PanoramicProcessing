#include "fisheyeWorkView.h"

using namespace pp;

FisheyeWorkView::FisheyeWorkView()
	: mGuiDownsample(2)
	, mRadiusColor(cv::Scalar(0, 0, 255.0))
	, mCenterColor(cv::Scalar(0, 0, 255.0))
{

}

FisheyeWorkView::~FisheyeWorkView()
{
	mViz.release();
}

void FisheyeWorkView::update()
{
	if (auto spt = mFisheyeController.lock())
	{
		const cv::Mat& src = spt->getSource();

		cv::Point2i center = spt->getParamCenter();
		int radius = spt->getParamRadius();

		cv::resize(src, mViz, src.size() / mGuiDownsample);
		cv::Point vizCenter = (cv::Point(src.cols / 2, src.rows / 2) + center) / mGuiDownsample;
		cv::circle(mViz, vizCenter, radius / mGuiDownsample, mRadiusColor, 1);
		cv::circle(mViz, vizCenter, 3, mCenterColor, -1);
	}
}

void FisheyeWorkView::draw()
{
	if (mViz.rows > 0 && mViz.cols > 0)
	{
		cv::imshow("fisheye", mViz);
		cv::waitKey(1000);
	}
}

void FisheyeWorkView::bind(std::shared_ptr<FisheyeEquirectangularController> inFisheyeController)
{
	mFisheyeController = inFisheyeController;
	inFisheyeController->addObserver(shared_from_this());
	this->update();
}

void FisheyeWorkView::unbind(std::shared_ptr<FisheyeEquirectangularController> inFisheyeController)
{
	inFisheyeController->removeObserver(shared_from_this());
}

void FisheyeWorkView::onControllerSourceUpdate(std::weak_ptr<ControllerSubject> sender)
{
	this->update();
}

void FisheyeWorkView::onControllerParamUpdate(std::weak_ptr<ControllerSubject> sender, const std::string& paramName, const int& paramValue)
{
	if (paramName == "radius")
		this->update();
}

void FisheyeWorkView::onControllerParamUpdate(std::weak_ptr<ControllerSubject> sender, const std::string& paramName, const cv::Point2i& paramValue)
{
	if (paramName == "center")
		this->update();
}