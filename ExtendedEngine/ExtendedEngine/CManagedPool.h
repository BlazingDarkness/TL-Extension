#pragma once
#include <memory>
#include <list>
#include "ExEngine.h"

namespace tle
{
	//Forward declare, hacky?
	class ExEngine;

	template<class T>
	class CManagedPool
	{
	protected:
		ExEngine* mpEngine;
		std::list<std::unique_ptr<T>> mInactive;
		std::list<std::unique_ptr<T>> mActive;

	public:
		CManagedPool(int initSize, ExEngine* engine)
		{
			mpEngine = engine;
			for (int i = 0; i < initSize; i++)
			{
				mInactive.push_back(std::unique_ptr<T>(new T(mpEngine)));
			}
		}

		void Update(float delta)
		{
			for (auto element = mActive.begin(); element != mActive.end();)
			{
				(*element)->Update(delta);

				if ((*element)->IsFinished())
				{
					(*element)->Hide();
					mInactive.push_back(move(*element));
					element = mActive.erase(element);
				}
				else
				{
					element++;
				}
			}
		}

		T* Spawn()
		{
			if (mInactive.empty())
			{
				mInactive.push_back(std::unique_ptr<T>(new T(mpEngine)));
			}
			mInactive.back()->Reset();
			mActive.push_back(move(mInactive.back()));
			return mActive.back().get();
		}

		void Reset()
		{
			for (auto element = mActive.begin(); element != mActive.end(); element++)
			{
				(*element)->Reset();
				mInactive.push_back(move(*element));
			}
			mActive.clear();
		}

		~CManagedPool()
		{
			mActive.clear();
			mInactive.clear();
		}
	};
}