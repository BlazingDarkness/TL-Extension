#pragma once
#include "IEngine.h"
#include "TLXEngineModified.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace tle
{
	class ExEngine : public CTLXEngineMod
	{
	private:
		std::unordered_map<string, IMesh*> mMeshMap;
		std::vector<std::unique_ptr<IAnimation>> mAnimations;
	public:
		ExEngine();

		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(const std::vector<string>& frameList,
							const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
							const float tickRate = 0.1f,
							const bool looped = true
							);

		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(string& name, string& extension, int amount,
							const CVector3& position	= CVector3(0.0f, 0.0f, 0.0f),
							const float		tickRate	= 0.1f,
							const bool		loope		= true
							);

		//Attempts to remove the animation
		virtual void RemoveAnimation(IAnimation* pAnimation);

		virtual IMesh* LoadMesh(const string& sMeshFileName);
		virtual void RemoveMesh(const IMesh* pMesh);

		/*template <class T>
		virtual CManagedPool<T>* CreateManagedPool(int size)
		{
			CManagedPool<T>* newPool = new CManagedPool<T>(size);
			mUpdateables.push_back(unique_ptr<CManagedPool<T>>(newPool));
			return newPool;
		}

		template <class T>
		void RemoveManagedPool(CManagedPool<T>* pool)
		{
			for (auto updateable = mUpdateables.begin(); updateable != mUpdateables.end(); updateable++)
			{
				if ((*updateable).get() == pool)
				{
					mUpdateables.erase(updateable);
					return;
				}
			}
		}*/

	};
}