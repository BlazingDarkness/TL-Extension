#pragma once
#include "TLXEngineModified.h"
#include "CParticleEmitter.h"
#include "CAnimation.h"
#include <unordered_map>

namespace tle
{
	class ExEngine : public CTLXEngineMod
	{
	private:
		std::unordered_map<string, IMesh*> mMeshMap;
		vector_ptr<CAnimation> mAnimations;

		//Particles & Emitters
		list_ptr<CParticle> mParticles;
		vector_ptr<CParticleEmitter> mEmitters;
		vector_ptr<CParticleEmitter> mDyingEmitters;

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
							const string& particleSprite	= PARTICLE_TEXTURE,				/*Default texture is Transparent.png*/
							const float		emissionRate	= 0.01f,						/*Default spawn rate of the particles*/
							const CVector3& position		= CVector3(0.0f, 0.0f, 0.0f)	/*Default location is the origin*/
						);

		//Remove the particle emitter if it exists
		virtual void RemoveEmitter(IParticleEmitter* emitter);

		//Gives a pointer to a particle object
		virtual CParticle* GetParticle();

		//Adds the particle to the unused particle list
		virtual void ReturnParticle(CParticle* pParticle);

		/***************************************************
						Additional Controls
		****************************************************/

		//Pauses any auto updated entity eg animations and particles
		virtual void PauseAutoUpdates();

		//Unpauses any auto updated entities eg animations and particles
		virtual void UnpauseAutoUpdates();

		/***************************************************
							Destructor
		****************************************************/

		virtual ~ExEngine();
	};
}