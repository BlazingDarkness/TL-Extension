#pragma once
#include <memory>
#include <list>
//#include "ExEngine.h"
#include "IUpdateable.h"

namespace tle
{
	//Forward declare, hacky?
	//class ExEngine;

	template<class T>
	class CManagedPool : virtual public IUpdateable
	{
	public:
		//Wrapped constructor
		typedef T* (*WConStrct)();
	protected:
		std::list<std::unique_ptr<T>> mInactive;
		std::list<std::unique_ptr<T>> mActive;
		WConStrct mpConstructor = 0;
		bool mDynamic; //creates new elements if mInactive is empty when Spawn is called

	public:
		CManagedPool(bool dynamic = false)
		{
			mDynamic = dynamic;
		}

		CManagedPool(WConStrct constructor, bool dynamic = true)
		{
			mpConstructor = constructor;
			mDynamic = dynamic;
		}

		CManagedPool(int initSize)
		{
			for (int i = 0; i < initSize; i++)
			{
				if(mpConstructor)
					mInactive.push_back(std::unique_ptr<T>(mpConstructor));
				else
					mInactive.push_back(std::unique_ptr<T>(new T));
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
				if (mDynamic)
				{
					if (mpConstructor)
						mInactive.push_back(std::unique_ptr<T>(mpConstructor));
					else
						mInactive.push_back(std::unique_ptr<T>(new T));
				}
				else
				{
					//Error
					return 0;
				}
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