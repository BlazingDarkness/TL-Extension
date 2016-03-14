#pragma once
#include "TLXEngineModified.h"
#include "CParticleEmitter.h"
#include "CAnimation.h"
#include <unordered_map>

namespace tle
{
	const string kDefaultTexture = "";

	class ExEngine : public CTLXEngineMod
	{
	private:
		//Declare model cache types
		using ModelKey = std::pair<IMesh*, string>;
		using ModelList = std::list<IModel*>;
		using ModelKeyList = std::pair<ModelKey, ModelList>;

		//Hasher for ModelKey
		struct ModelKeyHasher
		{
			std::size_t operator()(const ModelKey& k) const
			{
				using std::size_t;
				using std::hash;
				using std::string;

				return (hash<uintptr_t>()(reinterpret_cast<uintptr_t>(k.first)) ^ (hash<string>()(k.second) << 1));
			}
		};

		std::unordered_map<string, IMesh*> mMeshMap;
		std::unordered_map<ModelKey, ModelList, ModelKeyHasher> mModelCache;
		vector_ptr<CAnimation> mAnimations;

		//Particles & Emitters
		vector_ptr<CParticleEmitter> mEmitters;
		vector_ptr<CParticleEmitter> mDyingEmitters;
		IMesh* mParticleMesh;

		bool mAutoUpdate;

	public:
		ExEngine();

		/***************************************************
						Overriden functions
		****************************************************/

		//Attempts to Loads the specificed mesh
		//Returns 0 if mesh can't be found
		virtual IMesh* LoadMesh(const string& sMeshFileName);

		//Removes the mesh if found, all models of the mesh will also be deleted
		virtual void RemoveMesh(const IMesh* pMesh);

		// Draw everything in the scene from the viewpoint of the given camera.
		// If no camera is supplied, the most recently created camera is used.
		virtual void DrawScene(ICamera* pCamera = 0);

		// Get time passed since last call to this function, returns value in seconds using
		// highest accuracy timer available
		// Calls update function on all automaticly updated objects if mAutoUpdate is true
		virtual float Timer();

		/***************************************************
							New functions
		****************************************************/

		///////////////
		//Model Cache//

		//Loads the mesh file into the engine
		//If an amount is specified it will create a number of models and store them for later use
		//The models will be created and the texture set the what has been provided
		//A texture of "" will use the default model's texture
		virtual void Preload(const string& sMesh, const int amount = 0, const string& texture = kDefaultTexture);

		//Returns an instance of the mesh from the cache that already has the provided texture
		//if one already exists, otherwise it creates a model with the texture
		//A texture of "" will use the model's default texture 
		virtual IModel* GetModel(IMesh* pMesh, const string& texture = kDefaultTexture);

		//Stores the model in the model cache for later use
		//Use the texture param to specify if the mesh has been given a different texture
		//A texture of "" will assume the model has the default texture
		virtual void CacheModel(IModel* pModel, const string& texture = kDefaultTexture);

		/////////////
		//Animation//

		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(const std::vector<string>& frameList,
							const CVector3& position	= CVector3(0.0f, 0.0f, 0.0f),	/*Default location is the origin*/
							const float		tickRate	= 0.1f,							/*Default rate is 10 frames per second*/
							const bool		looped		= true							/*Set to loop the frames by default*/
						);

		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(string& name, string& extension, int amount,
							const CVector3& position	= CVector3(0.0f, 0.0f, 0.0f),	/*Default location is the origin*/
							const float		tickRate	= 0.1f,							/*Default rate is 10 frames per second*/
							const bool		looped		= true							/*Set to loop the frames by default*/
						);

		//Remove the animation if it exists
		virtual void RemoveAnimation(IAnimation* pAnimation);

		////////////////////
		//Particle Emitter//

		//Create a particle emitter at the given location
		virtual IParticleEmitter* CreateEmitter(EEmissionType type,
							const string&	particleSprite	= PARTICLE_TEXTURE,				/*Default texture is Transparent.png*/
							const float		emissionRate	= 0.01f,						/*Default spawn rate of the particles*/
							const CVector3& position		= CVector3(0.0f, 0.0f, 0.0f)	/*Default location is the origin*/
						);

		//Create a particle emitter at the given location
		virtual IParticleEmitter* CreateEmitter(EEmissionType type,
							const std::vector<string>&	particleSprite,						/*Animated texture*/
							const float		emissionRate	= 0.01f,						/*Default spawn rate of the particles*/
							const CVector3& position		= CVector3(0.0f, 0.0f, 0.0f)	/*Default location is the origin*/
						);

		//Remove the particle emitter if it exists
		virtual void RemoveEmitter(IParticleEmitter* emitter);

		//Gives a pointer to a particle model (quad) that already has the given texture
		virtual IModel* GetParticleModel(const string& texture);

		//Adds an unused particle model to the cache
		virtual void ReturnParticleModel(IModel* model, const string& texture);

		/***************************************************
						Additional Controls
		****************************************************/

		//Pauses any auto updated entity eg animations and particles
		virtual void PauseAutoUpdates();

		//Unpauses any auto updated entities eg animations and particles
		virtual void UnpauseAutoUpdates();

		//Destroys all models and only models in the cache
		virtual void ClearModelCache();

		//Destroys all meshes and therefore all models and particle emitters
		virtual void ClearMeshCache();

		/***************************************************
							Destructor
		****************************************************/

		virtual ~ExEngine();
	};
}