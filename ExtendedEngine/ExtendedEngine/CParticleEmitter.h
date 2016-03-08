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

		virtual void SetEmissionType(EEmissionType type);
		virtual void SetEmissionRate(float rate);

		virtual void SetParticleLife(float life);
		virtual void SetParticleSkin(string& skin);
		virtual void SetParticleVelocity(CVector3& vel);
		virtual void SetParticleAcceleration(CVector3& acl);
		virtual void SetParticleScale(float scale);

		/************************************
						Gets
		*************************************/

		virtual EEmissionType GetEmissionType();
		virtual float GetEmissionRate();

		virtual float GetParticleLife();
		virtual string GetParticleSkin();
		virtual CVector3 GetParticleVelocity();
		virtual CVector3 GetParticleAcceleration();
		virtual float GetParticleScale();

		//Returns true is the emitter is emitting particles
		virtual bool IsEmitting();

		//Returns true if one or more of the emitter particles are still active
		virtual bool HasActiveParticles();

		/************************************
				 Goodbye Cruel World
		*************************************/

		virtual ~CParticleEmitter();
	};
}