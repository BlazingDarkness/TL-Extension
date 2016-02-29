#include "stdafx.h"
#include "CParticleEmitter.h"

namespace tle
{
	CParticleEmitter::CParticleEmitter(EmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine) : CTLXSceneNode(positionNode)
	{
		mType = type;
		mRate = rate;
		mTimer = 0.0f;
		mpSceneManager = sceneManager;
		mpTLXCamera = positionNode;
		mpEngine = engine;
	}

	void CParticleEmitter::Start()
	{

	}

	void CParticleEmitter::Stop()
	{

	}

	void CParticleEmitter::Reset()
	{

	}

	void CParticleEmitter::Update(float delta)
	{

	}

	void CParticleEmitter::SetEmissionType(EmissionType type)
	{

	}

	void CParticleEmitter::SetEmissionRate(float rate)
	{

	}

	EmissionType CParticleEmitter::GetEmissionType()
	{

	}

	float CParticleEmitter::GetEmissionRate()
	{

	}

	bool CParticleEmitter::IsEmitting()
	{

	}

	CParticleEmitter::~CParticleEmitter()
	{
		mpSceneManager->RemoveCamera(mpTLXCamera);
		//mpEngine = engine;
	}
}