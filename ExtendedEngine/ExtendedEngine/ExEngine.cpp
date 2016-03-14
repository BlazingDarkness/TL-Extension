#include "stdafx.h"

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

		mParticleMesh = 0;
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
		for (auto mesh = mMeshMap.begin(); mesh != mMeshMap.end(); ++mesh)
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
		if (pCamera)
		{
			//Make particle emitters's particles face the camera
			for (auto emitter = mEmitters.begin(); emitter != mEmitters.end(); ++emitter)
			{
				(*emitter)->OrientateParticles(pCamera);
			}
			for (auto emitter = mDyingEmitters.begin(); emitter != mDyingEmitters.end(); ++emitter)
			{
				(*emitter)->OrientateParticles(pCamera);
			}

			//Hide unused particles
			float matrix[16];

			pCamera->GetMatrix(matrix);

			CVector3 pos((matrix[0] * -100.0f) + pCamera->GetX(),
				(matrix[1] * -100.0f) + pCamera->GetY(),
				(matrix[2] * -100.0f) + pCamera->GetZ());

			/*for (auto particleList = mParticleModels.begin(); particleList != mParticleModels.end(); ++particleList)
			{
				for (auto particle = particleList->second.begin(); particle != particleList->second.end(); ++particle)
				{
					(*particle)->SetPosition(pos.x, pos.y, pos.z);
				}
			}*/

			for (auto modelList = mModelCache.begin(); modelList != mModelCache.end(); ++modelList)
			{
				for (auto model = modelList->second.begin(); model != modelList->second.end(); ++model)
				{
					(*model)->SetPosition(pos.x, pos.y, pos.z);
				}
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
			//Update animations
			for (auto animation = mAnimations.begin(); animation != mAnimations.end(); ++animation)
			{
				(*animation)->Update(frameTime);
			}

			//Update emitters
			for (auto emitter = mEmitters.begin(); emitter != mEmitters.end(); ++emitter)
			{
				(*emitter)->Update(frameTime);
			}

			//Update dying emitters
			for (auto emitter = mDyingEmitters.begin(); emitter != mDyingEmitters.end(); /*Only increment if no erase occurs*/)
			{
				(*emitter)->Update(frameTime);

				//Erase if dead
				if (!(*emitter)->HasActiveParticles())
				{
					emitter = mDyingEmitters.erase(emitter);
				}
				else
				{
					++emitter;
				}
			}
		}

		return frameTime;
	}

	/***************************************************
						New functions
	****************************************************/

	///////////////
	//Model Cache//

	//Loads the mesh file into the engine
	//If an amount is specified it will create a number of models and store them for later use
	//The models will be created and the texture set the what has been provided
	//A texture of "" will use the default model's texture
	void ExEngine::Preload(const string& sMesh, const int amount, const string& texture)
	{
		//Load mesh
		IMesh* mesh = ExEngine::LoadMesh(sMesh);
		ModelKey mKey = ModelKey(mesh, texture);

		//Return if loading mesh failed or no models to create
		if (amount <= 0 || !mesh) return;
		
		//Get list
		auto mapItr = mModelCache.find(mKey);

		//If list already exists
		if (mapItr == mModelCache.end())
		{
			ModelList modelList;

			for (int i = 0; i < amount; ++i)
			{
				IModel* model = mesh->CreateModel();

				//Set the texture if not default
				if (texture != kDefaultTexture) model->SetSkin(texture);

				modelList.push_back(model);
			}

			mModelCache.insert(ModelKeyList(mKey, modelList));
		}
		else //If list already exists
		{
			for (int i = 0; i < amount; ++i)
			{
				IModel* model = mesh->CreateModel();

				//Set the texture if not default
				if (texture != kDefaultTexture) model->SetSkin(texture);

				mapItr->second.push_back(model);
			}
		}
	}

	//Returns an instance of the mesh from the cache that already has the provided texture
	//if one already exists, otherwise it creates a model with the texture
	//A texture of "" will use the model's default texture 
	IModel* ExEngine::GetModel(IMesh* pMesh, const string& texture)
	{
		ModelKey mKey = ModelKey(pMesh, texture);

		//Get list
		auto mapItr = mModelCache.find(mKey);

		//Returns a model from the model cache
		if (mapItr != mModelCache.end() && static_cast<int>(mapItr->second.size()) > 0)
		{
			IModel* model = mapItr->second.back();
			mapItr->second.pop_back();
			return model;
		}
		else //Create a new model if there are none in the cache
		{
			IModel* model = pMesh->CreateModel();

			//Set the texture if not default
			if (texture != kDefaultTexture) model->SetSkin(texture);

			return model;
		}
	}

	//Stores the model in the model cache for later use
	//Use the texture param to specify if the mesh has been given a different texture
	//A texture of "" will assume the model has the default texture
	void ExEngine::CacheModel(IModel* pModel, const string& texture)
	{
		ModelKey mKey = ModelKey(pModel->GetMesh(), texture);

		//Get list
		auto mapItr = mModelCache.find(mKey);

		//Add model to the list
		if (mapItr != mModelCache.end())
		{
			mapItr->second.push_back(pModel);
		}
		else //Create a new list if none exists
		{
			ModelList modelList;
			modelList.push_back(pModel);
			mModelCache.insert(ModelKeyList(mKey, modelList));
		}
	}

	/////////////
	//Animation//

	//Creates an animation at the given location
	//Runs through the frames at a tick rate
	//Return 0 if failed to load any of the animation frames
	IAnimation* ExEngine::CreateAnimation(const std::vector<string>& frameList, const CVector3& position, const float tickRate, const bool looped)
	{
		SpriteVector sprites;
		for (auto it = frameList.begin(); it != frameList.end(); ++it)
		{
			ISprite* newSprite = CreateSprite(*it, position.x, position.y, position.z);

			//Couldn't load sprite
			if (!newSprite)
			{
				std::cout << "Could not load image file \"" + (*it) + "\" Aborting creation of IAnimation.";

				//Remove sprites
				for (auto sprite = sprites.begin(); sprite != sprites.end(); ++sprite)
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
		for (int i = 0; i < amount; ++i)
		{
			string spriteFile = name + to_string(i) + extension;
			ISprite* newSprite = CreateSprite(spriteFile, position.x, position.y, position.z);

			//Couldn't load sprite
			if (!newSprite)
			{
				std::cout << "Could not load image file \"" + spriteFile + "\" Aborting creation of IAnimation.";

				//Remove sprites
				for (auto sprite = sprites.begin(); sprite != sprites.end(); ++sprite)
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
		for (auto animation = mAnimations.begin(); animation != mAnimations.end(); ++animation)
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
	IParticleEmitter* ExEngine::CreateEmitter(EEmissionType type, const string& particleSprite, const float emissionRate, const CVector3& position)
	{
		tlx::ICamera* node = m_pSceneManager->CreateCamera();

		CParticleEmitter* emitter = new CParticleEmitter(type, emissionRate, node, m_pSceneManager, this);
		emitter->SetPosition(position.x, position.y, position.z);
		emitter->SetParticleSkin(particleSprite);

		mEmitters.push_back(std::unique_ptr<CParticleEmitter>(emitter));

		return emitter;
	}

	//Create a particle emitter at the given location
	IParticleEmitter* ExEngine::CreateEmitter(EEmissionType type, const std::vector<string>& particleSprite, const float emissionRate, const CVector3& position)
	{
		tlx::ICamera* node = m_pSceneManager->CreateCamera();

		CParticleEmitter* emitter = new CParticleEmitter(type, emissionRate, node, m_pSceneManager, this);
		emitter->SetPosition(position.x, position.y, position.z);
		emitter->SetParticleSkin(particleSprite);

		mEmitters.push_back(std::unique_ptr<CParticleEmitter>(emitter));

		return emitter;
	}

	//Remove the particle emitter if it exists
	void ExEngine::RemoveEmitter(IParticleEmitter* emitter)
	{
		for (auto it = mEmitters.begin(); it != mEmitters.end(); ++it)
		{
			if (emitter == it->get())
			{
				if ((*it)->HasActiveParticles())
				{
					mDyingEmitters.push_back(move(*it));
				}
				mEmitters.erase(it);
				return;
			}
		}
	}
	
	//Gives a pointer to a particle model (quad) that already has the given texture
	IModel* ExEngine::GetParticleModel(const string& texture)
	{
		if (!mParticleMesh) mParticleMesh = ExEngine::LoadMesh(PARTICLE_MODEL);
		return GetModel(mParticleMesh, texture);
	}

	//Adds an unused particle model to the cache
	void ExEngine::ReturnParticleModel(IModel* model, const string& texture)
	{
		CacheModel(model, texture);
	}

	/***************************************************
					Additional Controls
	****************************************************/

	//Pauses any auto updated entity eg animations and particles
	void ExEngine::PauseAutoUpdates()
	{
		mAutoUpdate = false;
	}

	//Unpauses any auto updated entities eg animations and particles
	void ExEngine::UnpauseAutoUpdates()
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
		mDyingEmitters.clear();

		//Must be after the emitters but before meshes
		for (auto modelList = mModelCache.begin(); modelList != mModelCache.end(); ++modelList)
		{
			for (auto model = modelList->second.begin(); model != modelList->second.end(); ++model)
			{
				(*model)->GetMesh()->RemoveModel(*model);
			}
		}

		//Must be done last
		for (auto it = mMeshMap.begin(); it != mMeshMap.end(); ++it)
		{
			CTLXEngineMod::RemoveMesh((*it).second);
		}
		mMeshMap.clear();
	}
}