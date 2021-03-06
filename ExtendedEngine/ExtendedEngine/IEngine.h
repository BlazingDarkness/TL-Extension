#pragma once
#include "TL-Engine.h"
#include "IAnimation.h"
#include "IParticleEmitter.h"
#include "ISound.h"
#include "IMusic.h"
#include "ILoadScreen.h"
#include "IUsings.h"

namespace tle
{
	class IEngine : public I3DEngine
	{
	public:
		/***************************************************
							New functions
		****************************************************/

		///////////
		//Loading//

		//Loads the mesh file into the engine
		//If an amount is specified it will create a number of models and store them for later use
		//The models will be created and the texture set the what has been provided
		//A texture of "" will use the default model's texture
		virtual void Load(const string& sMesh, const int amount = 0, const string& texture = "") = 0;

		//Adds the mesh to a load queue but does not load it
		//Also adds the amount and texture to of the mesh to be loaded
		//The models and mesh will be loaded when "LoadQueuedObjects" is called
		virtual void AddToLoadQueue(const string& sMesh, const int amount = 0, const string& texture = "") = 0;

		//Adds a pointer to a model pointer to the queue
		//A model of the mesh with the given texture will be created and stored into
		//the model pointer when "LoadQueuedObjects" is called
		virtual void AddToLoadQueue(IModel** model, const string& sMesh, const string& texture = "") = 0;

		//Loads all the meshes that had been added to the load queue
		//If an amount or/and texture was also specified then it will create that
		//many models from the mesh with the indicated texture
		//The load screen is optional and allows the engine to call the load screen's update function
		//After every few objects have been loaded to show progress
		virtual void LoadQueuedObjects(ILoadScreen* loadScreen) = 0;

		///////////////
		//Model Cache//

		//Returns an instance of the mesh from the cache that already has the provided texture
		//if one already exists, otherwise it creates a model with the texture
		//A texture of "" will use the model's default texture 
		virtual IModel* GetModel(IMesh* pMesh, const string& texture = "") = 0;

		//Stores the model in the model cache for later use
		//Use the texture param to specify if the mesh has been given a different texture
		//A texture of "" will assume the model has the default texture
		virtual void CacheModel(IModel* pModel, const string& texture = "") = 0;

		/////////////
		//Animation//

		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(const std::vector<string>& frameList,
								const CVector3& position	= CVector3(0.0f, 0.0f, 0.0f),	/*Default location is the origin*/
								const float		tickRate	= 0.1f,							/*Default rate is 10 frames per second*/
								const bool		looped		= true							/*Set to loop the frames by default*/
							) = 0;

		//Creates an animation at the given location
		//Runs through the frames at a tick rate
		//Return 0 if failed to load any of the animation frames
		virtual IAnimation* CreateAnimation(string& name, string& extension, int amount,
								const CVector3& position	= CVector3(0.0f, 0.0f, 0.0f),	/*Default location is the origin*/
								const float		tickRate	= 0.1f,							/*Default rate is 10 frames per second*/
								const bool		looped		= true							/*Set to loop the frames by default*/
							) = 0;

		//Remove the animation if it exists
		virtual void RemoveAnimation(IAnimation* pAnimation) = 0;

		////////////////////
		//Particle Emitter//

		//Create a particle emitter at the given location
		virtual IParticleEmitter* CreateEmitter(EEmissionType type,
								const string&	particleSprite	= PARTICLE_TEXTURE,				/*Default texture is Transparent.png*/
								const float		emissionRate	= 0.01f,						/*Default spawn rate of the particles*/
								const CVector3& position		= CVector3(0.0f, 0.0f, 0.0f)	/*Default location is the origin*/
							) = 0;

		//Create a particle emitter at the given location
		virtual IParticleEmitter* CreateEmitter(EEmissionType type,
								const std::vector<string>&	particleSprite,						/*Animated texture*/
								const float		emissionRate	= 0.01f,						/*Default spawn rate of the particles*/
								const CVector3& position		= CVector3(0.0f, 0.0f, 0.0f)	/*Default location is the origin*/
							) = 0;

		//Remove the particle emitter if it exists
		virtual void RemoveEmitter(IParticleEmitter* emitter) = 0;

		/////////
		//Sound//

		//Creates a sound object from the given file
		//Returns 0 if the sound could not be loaded
		virtual ISound* CreateSound(const std::string& soundFile) = 0;

		//Removes the sound if it exists
		virtual void RemoveSound(ISound* pSound) = 0;

		/////////
		//Music//

		//Creates a music object from the given file
		//Returns 0 if the file could not be loaded
		virtual IMusic* CreateMusic(const std::string& musicFile) = 0;

		//Removes the music if it exists
		virtual void RemoveMusic(IMusic* pMusic) = 0;

		/***************************************************
						Additional Controls
		****************************************************/

		//Pauses any auto updated entity eg animations and particles
		virtual void PauseAutoUpdates() = 0;

		//Unpauses any auto updated entities eg animations and particles
		virtual void UnpauseAutoUpdates() = 0;

		//Destroys all models and only models in the cache
		virtual void ClearModelCache() = 0;

		//Destroys all meshes and therefore all models and particle emitters
		virtual void ClearMeshCache() = 0;

		//Destroys all music
		virtual void ClearMusic() = 0;

		//Destroys all sounds
		virtual void ClearSounds() = 0;

		//Sets the volume for the specific volume modifier
		virtual void SetVolume(float volume, SoundType type) = 0;

		//Gets the volume for the specific volume modifier
		virtual float GetVolume(SoundType type) = 0;
	};
}