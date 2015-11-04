#ifndef __CONTROLLER_SUBJECT_H__
#define __CONTROLLER_SUBJECT_H__

#include <memory>
#include <algorithm>

namespace pp
{
	//TODO: make this using templates

	class ControllerSubject;

	class ControllerObserver
	{
	public:
		struct ControllerObserverWeakComp
		{
			explicit ControllerObserverWeakComp(std::shared_ptr<ControllerObserver> i) : n(i) { }
			inline bool operator()(const std::weak_ptr<ControllerObserver> & m) const
			{
				if (auto spt = m.lock()) return (spt == n);
				return false;
			}
		private:
			std::shared_ptr<ControllerObserver> n;
		};

		ControllerObserver() {}
		virtual ~ControllerObserver() {}
		virtual void onControllerParamUpdate(std::weak_ptr<ControllerSubject> sender, const std::string& paramName, const float& paramValue) {}
		virtual void onControllerParamUpdate(std::weak_ptr<ControllerSubject> sender, const std::string& paramName, const int& paramValue) {}
		virtual void onControllerParamUpdate(std::weak_ptr<ControllerSubject> sender, const std::string& paramName, const cv::Point2i& paramValue) {}
	};

	class ControllerSubject : std::enable_shared_from_this<ControllerSubject>
	{
	public:
		ControllerSubject() {}
		virtual ~ControllerSubject() {}

		void addObserver(std::shared_ptr<ControllerObserver> obs)
		{
			mObservers.push_back(obs);
		}

		void removeObserver(std::shared_ptr<ControllerObserver> obs)
		{
			auto it = std::find_if(mObservers.begin(), mObservers.end(), ControllerObserver::ControllerObserverWeakComp(obs));
			if (it != mObservers.end())
				mObservers.erase(it);
		}

	protected:

		virtual void notify(const std::string& paramName, const float& paramValue)
		{
			for (std::weak_ptr<ControllerObserver> wpt : mObservers)
			{
				if (auto spt = wpt.lock())
					spt->onControllerParamUpdate(shared_from_this(), paramName, paramValue);
			}
		}

		virtual void notify(const std::string& paramName, const int& paramValue)
		{
			for (std::weak_ptr<ControllerObserver> wpt : mObservers)
			{
				if (auto spt = wpt.lock())
					spt->onControllerParamUpdate(shared_from_this(), paramName, paramValue);
			}
		}

		virtual void notify(const std::string& paramName, const cv::Point2i& paramValue)
		{
			for (std::weak_ptr<ControllerObserver> wpt : mObservers)
			{
				if (auto spt = wpt.lock())
					spt->onControllerParamUpdate(shared_from_this(), paramName, paramValue);
			}
		}


	private:
		std::vector<std::weak_ptr<ControllerObserver> > mObservers;
	};
}

#endif //__CONTROLLER_SUBJECT_H__