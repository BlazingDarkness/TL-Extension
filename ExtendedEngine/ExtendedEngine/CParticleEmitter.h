#pragma once
#include "IParticleEmitter.h"
#include "TLXSceneNode.h"
#include "IUpdateable.h"
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
		EmissionType mType;
		float mRate;
		float mTimer;
		bool mPaused;

		//Particle data

		//Shit
		tlx::ISceneManager* mpSceneManager;
		tlx::ICamera* mpTLXCamera;
		ExEngine* mpEngine;

		//Particle lists
		list_ptr<CParticle> mInactive;
		list_ptr<CParticle> mActive;

	public:
		CParticleEmitter(EmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine);

		/************************************
				  Update Controls
		*************************************/

		//Starts the emitter spawning particles, default off
		virtual void Start();

		//Stops the emitter spawning particles
		virtual void Stop();

		//Removes all the emitter's particles
		virtual void Reset();

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

		virtual void SetEmissionType(EmissionType type);
		virtual void SetEmissionRate(float rate);
		virtual void SetParticleLife(float life);

		/************************************
						Gets
		*************************************/

		virtual EmissionType GetEmissionType();
		virtual float GetEmissionRate();
		virtual void GetParticleLife(float life);

		virtual bool IsEmitting();

		//Destroy all life
		virtual ~CParticleEmitter();
	};
}