#include "stdafx.h"
#include "CParticleEmitter.h"

namespace tle
{
	CParticleEmitter::CParticleEmitter(EmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine) : CTLXSceneNode(positionNode)
	{
		mType = type;
		mRate = rate;
		mTimer = 0.0f;
		mPaused = false;
		mpSceneManager = sceneManager;
		mpTLXCamera = positionNode;
		mpEngine = engine;
		mpParticlePool = mpEngine->CreateManagedPool<CParticle>(50);
	}

	void CParticleEmitter::Start()
	{
		mPaused = false;
	}

	void CParticleEmitter::Stop()
	{
		mPaused = true;
	}

	void CParticleEmitter::Reset()
	{
		mpParticlePool->Reset();
	}

	void CParticleEmitter::Update(float delta)
	{
		mTimer += delta;
		while (mTimer > mRate)
		{
			mTimer -= mRate;
			CParticle* p = mpParticlePool->Spawn();

			float matrix[16];
			GetMatrix(&matrix[0]);
			p->SetMatrix(&(matrix[0]));
		}
	}

	void CParticleEmitter::SetEmissionType(EmissionType type)
	{
		mType = type;
	}

	void CParticleEmitter::SetEmissionRate(float rate)
	{
		mRate = rate;
	}

	EmissionType CParticleEmitter::GetEmissionType()
	{
		return mType;
	}

	float CParticleEmitter::GetEmissionRate()
	{
		return mRate;
	}

	bool CParticleEmitter::IsEmitting()
	{
		return !mPaused;
	}

	CParticleEmitter::~CParticleEmitter()
	{
		mpSceneManager->RemoveCamera(mpTLXCamera);
		//mpEngine = engine;
	}
}