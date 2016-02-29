#pragma once
#include "IParticleEmitter.h"
#include "TLXSceneNode.h"
#include "IUpdateable.h"
#include "ISceneManager.h"
#include "ICamera.h"
#include "CParticle.h"
#include "ExEngine.h"

namespace tle
{
	class CParticleEmitter : virtual public IParticleEmitter, public CTLXSceneNode
	{
	private:
		EmissionType mType;
		float mRate;
		float mTimer;
		bool mPaused;
		tlx::ISceneManager* mpSceneManager;
		tlx::ICamera* mpTLXCamera;
		ExEngine* mpEngine;
		CManagedPool<CParticle>* mpParticlePool;

	public:
		CParticleEmitter(EmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine);

		virtual void Start();
		virtual void Stop();
		virtual void Reset();
		virtual void Update(float delta);

		virtual void SetEmissionType(EmissionType type);
		virtual void SetEmissionRate(float rate);
		virtual void SetParticleLife(float life);

		virtual EmissionType GetEmissionType();
		virtual float GetEmissionRate();
		virtual void GetParticleLife(float life);

		virtual bool IsEmitting();

		~CParticleEmitter();
	};
}