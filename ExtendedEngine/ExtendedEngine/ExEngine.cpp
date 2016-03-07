#include "stdafx.h"

//#include <algorithm>
#include <iostream>

#include "ExEngine.h"

namespace tle
{
	////////////////////////////////////////////////////////////////////////////////
	//Constructor


	/***************************************************
					Overriden functions
	****************************************************/

	//Attempts to Loads the specificed mesh
	//Returns 0 if mesh can't be found
	ExEngine::ExEngine() : CTLXEngineMod()
	{
		mAutoUpdate = true;
	}

	//Removes the mesh if found, all models of the mesh will also be deleted
	IMesh* ExEngine::LoadMesh(const string& sMeshFileName)
	{
		auto mesh = mMeshMap.find(sMeshFileName);
		if (mesh != mMeshMap.end())
		{
			return mesh->second;
		}
		else
		{
			//No mesh is found of that name so attempt to load it
			IMesh* newMesh = CTLXEngineMod::LoadMesh(sMeshFileName);
			mMeshMap.insert(std::pair<string, IMesh*>{sMeshFileName, newMesh});
			return newMesh;
		}
	}

	// Draw everything in the scene from the viewpoint of the given camera.
	// If no camera is supplied, the most recently created camera is used.
	void ExEngine::RemoveMesh(const IMesh* pMesh)
	{
		for (auto mesh = mMeshMap.begin(); mesh != mMeshMap.end(); mesh++)
		{
			if (pMesh == mesh->second)
			{
				CTLXEngineMod::RemoveMesh(mesh->second);
				mMeshMap.erase(mesh);
				return;
			}
		}
	}

	// Draw everything in the scene from the viewpoint of the given camera.
	// If no camera is supplied, the most recently created camera is used.
	void ExEngine::DrawScene(ICamera* pCamera)
	{
		//If no specific camera and a camera exists then select the most recently created camera
		if (!pCamera && m_Cameras.size() > 0)
		{
			pCamera = m_Cameras.back();
		}

		//If a camera is still not selected then skip the updating of the particle locations/orientation
		if (!pCamera)
		{
			//Todo - Make particle emitters's particles face the camera
			for (auto emitter = mEmitters.begin(); emitter != mEmitters.end(); emitter++)
			{
				(*emitter)->OrientateParticles(pCamera);
			}
		}

		CTLXEngineMod::DrawScene(pCamera);
	}

	// Get time passed since last call to this function, returns value in seconds using
	// highest accuracy timer available
	// Calls update function on all automaticly updated objects if mAutoUpdate is true
	float ExEngine::Timer()
	{
		float frameTime = CTLXEngineMod::Timer();

		if (mAutoUpdate)
		{
			for (auto animation = mAnimations.begin(); animation != mAnimations.end(); animation++)
			{
				(*animation)->Update(frameTime);
			}

			for (auto emitter = mEmitters.begin(); emitter != mEmitters.end(); emitter++)
			{
				(*emitter)->Update(frameTime);
			}
		}

		return frameTime;
	}

	/***************************************************
						New functions
	****************************************************/

	/////////////
	//Animation//

	//Creates an animation at the given location
	//Runs through the frames at a tick rate
	//Return 0 if failed to load any of the animation frames
	IAnimation* ExEngine::CreateAnimation(const std::vector<string>& frameList, const CVector3& position, const float tickRate, const bool looped)
	{
		SpriteVector sprites;
		for (auto it = frameList.begin(); it != frameList.end(); it++)
		{
			ISprite* newSprite = CreateSprite(*it, position.x, position.y, position.z);

			//Couldn't load sprite
			if (!newSprite)
			{
				std::cout << "Could not load image file \"" + (*it) + "\" Aborting creation of IAnimation.";

				//Remove sprites
				for (auto sprite = sprites.begin(); sprite != sprites.end(); sprite++)
				{
					CTLXEngineMod::RemoveSprite(*sprite);
				}
				return 0;
			}
		}
		CAnimation* animation = new CAnimation(this, sprites, position, tickRate, looped);
		mAnimations.push_back(unique_ptr<CAnimation>(animation));
		return animation;
	}

	//Creates an animation at the given location
	//Runs through the frames at a tick rate
	//Return 0 if failed to load any of the animation frames
	IAnimation* ExEngine::CreateAnimation(string& name, string& extension, int amount, const CVector3& position, const float tickRate, const bool looped)
	{
		SpriteVector sprites;
		for (int i = 0; i < amount; i++)
		{
			string spriteFile = name + to_string(i) + extension;
			ISprite* newSprite = CreateSprite(spriteFile, position.x, position.y, position.z);

			//Couldn't load sprite
			if (!newSprite)
			{
				std::cout << "Could not load image file \"" + spriteFile + "\" Aborting creation of IAnimation.";

				//Remove sprites
				for (auto sprite = sprites.begin(); sprite != sprites.end(); sprite++)
				{
					RemoveSprite(*sprite);
				}
				return 0;
			}
		}
		CAnimation* animation = new CAnimation(this, sprites, position, tickRate, looped);
		mAnimations.push_back(unique_ptr<CAnimation>(animation));
		return animation;
	}

	//Removes the animation if found
	void ExEngine::RemoveAnimation(IAnimation* pAnimation)
	{
		for (auto animation = mAnimations.begin(); animation != mAnimations.end(); animation++)
		{
			if ((*animation).get() == pAnimation)
			{
				mAnimations.erase(animation);
				return;
			}
		}
	}

	////////////////////
	//Particle Emitter//

	//Create a particle emitter at the given location
	IParticleEmitter* ExEngine::CreateEmitter()
	{
		//Todo
		return 0;
	}

	//Remove the particle emitter if it exists
	void ExEngine::RemoveEmitter(IParticleEmitter* emitter)
	{
		for (auto it = mEmitters.begin(); it != mEmitters.end(); it++)
		{
			if (emitter == it->get())
			{
				mEmitters.erase(it);
				return;
			}
		}
	}

	/***************************************************
					Additional Controls
	****************************************************/

	//Pauses any auto updated entity eg animations and particles
	void ExEngine::PauseAnimations()
	{
		mAutoUpdate = false;
	}

	//Unpauses any auto updated entities eg animations and particles
	void ExEngine::UnpauseAnimations()
	{
		mAutoUpdate = true;
	}

	/***************************************************
						Destructor
	****************************************************/

	//Ensure all memory is released
	ExEngine::~ExEngine()
	{
		mAnimations.clear();
		mEmitters.clear();

		for (auto it = mMeshMap.begin(); it != mMeshMap.end(); it++)
		{
			CTLXEngineMod::RemoveMesh((*it).second);
		}
		mMeshMap.clear();
	}
}