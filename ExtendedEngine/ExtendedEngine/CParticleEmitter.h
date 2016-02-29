#pragma once
#include "IParticleEmitter.h"
#include "TLXSceneNode.h"
#include "IUpdateable.h"
#include "ISceneManager.h"
#include "ICamera.h"
#include "ExEngine.h"

namespace tle
{
	class CParticleEmitter : virtual public IParticleEmitter, public CTLXSceneNode, public IUpdateable
	{
	private:
		EmissionType mType;
		float mRate;
		float mTimer;
		tlx::ISceneManager* mpSceneManager;
		tlx::ICamera* mpTLXCamera;
		ExEngine* mpEngine;

	public:
		CParticleEmitter(EmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine);

		virtual void Start();
		virtual void Stop();
		virtual void Reset();
		virtual void Update(float delta);

		virtual void SetEmissionType(EmissionType type);
		virtual void SetEmissionRate(float rate);

		virtual EmissionType GetEmissionType();
		virtual float GetEmissionRate();

		virtual bool IsEmitting();

		~CParticleEmitter();
	};
}