#pragma once
#include "IParticleEmitter.h"
#include "TLXSceneNode.h"
#include "ISceneManager.h"
#include "ICamera.h"
#include "CParticle.h"

namespace tle
{
	//Hacky work around to avoid circular reference
	class ExEngine;

	class CParticleEmitter : virtual public IParticleEmitter, public CTLXSceneNode
	{
	private:
		//Emitter data
		EEmissionType mType;
		float mRate;
		float mAngle;
		float mTimer;
		bool mPaused;

		//Particle data
		ParticleData mParticleData;

		//Shit
		tlx::ISceneManager* mpSceneManager;
		tlx::ICamera* mpTLXCamera;
		ExEngine* mpEngine;

		//Particle lists
		list_ptr<CParticle> mInactive;
		list_ptr<CParticle> mActive;

	public:
		CParticleEmitter(EEmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine);

		/************************************
				  Update Controls
		*************************************/

		//Starts the emitter spawning particles, default off
		virtual void Start();

		//Stops the emitter spawning particles
		virtual void Stop();

		//Sets all particles to the despawned (inactive) state
		virtual void Reset();

		//Clears local cache of particles
		//Returns all particles back to the engine
		virtual void Clear();

		//Do not call
		//Called from the engine to auto update the particles and emitter
		virtual void Update(float delta);

		//Do not call
		//Called from the engine to orientate the particles
		//To either face or hide behind the camera
		virtual void OrientateParticles(ICamera* camera);
		
		/************************************
						Sets
		*************************************/

		///////////
		//Emitter//

		//Emission type decides what direction the particles are emitted
		virtual void SetEmissionType(EEmissionType type);

		//Set the variance in the direction the particles are emitted
		//Only affects non circular/linear emissions.
		virtual void SetEmissionAngle(float angle);

		//Set the amount of time in seconds between particles being emitted
		//Note: A rate less than 0.001 (>1000 active particles) will be treated an no emission
		virtual void SetEmissionRate(float rate);

		////////////
		//Particle//

		//Set the time in seconds that the particles last
		virtual void SetParticleLife(float life);

		//Set the texture used for the particle's quad model
		virtual void SetParticleSkin(string& skin);

		//Set the base velocity of the particles
		//Does not retroactively change velcoity of existing particles
		virtual void SetParticleVelocity(CVector3& vel);

		//Set the acceleration of the particles
		//Also changes acceleration of existing particles
		virtual void SetParticleAcceleration(CVector3& acl);

		//Set the scale of the particle quad model
		//Does not retroactively change scale of existing particles
		virtual void SetParticleScale(float scale);

		/************************************
						Gets
		*************************************/

		///////////
		//Emitter//

		//Returns the emission area
		virtual EEmissionType GetEmissionType();

		//Returns the angle of variance that the particles are being emitted at
		virtual float GetEmissionAngle();

		//Returns the time in seconds between particle emissions
		virtual float GetEmissionRate();

		//Returns true is the emitter is emitting particles
		virtual bool IsEmitting();

		//Returns true if one or more of the emitter particles are still alive
		virtual bool HasActiveParticles();

		////////////
		//Particle//

		//Returns how long the particles live
		virtual float GetParticleLife();

		//Returns the texture used for the particle's quad model
		virtual string GetParticleSkin();

		//Returns the base velocity of the particles
		virtual CVector3 GetParticleVelocity();

		//Returns the acceleration of the particles
		virtual CVector3 GetParticleAcceleration();

		//Returns the scale of the particle quad model
		virtual float GetParticleScale();

		/************************************
				 Goodbye Cruel World
		*************************************/

		virtual ~CParticleEmitter();
	};
}