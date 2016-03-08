#include "stdafx.h"
#include "CParticleEmitter.h"
#include "ExEngine.h"

namespace tle
{
	CParticleEmitter::CParticleEmitter(EEmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine) : CTLXSceneNode(positionNode)
	{
		mType = type;

		mParticleData.maxLife = 0.0f;
		mParticleData.mScale = 1.0f;
		mParticleData.mVel = CVector3(0.0f, 0.0f, 0.0f);
		mParticleData.mAcl = CVector3(0.0f, 0.0f, 0.0f);
		mParticleData.mTexture = PARTICLE_TEXTURE;

		mRate = rate;
		mTimer = 0.0f;
		mPaused = false;

		mpSceneManager = sceneManager;
		mpTLXCamera = positionNode;
		mpEngine = engine;
	}

	/************************************
			  Update Controls
	*************************************/

	//Starts the emitter spawning particles, default off
	void CParticleEmitter::Start()
	{
		mPaused = false;
	}

	//Stops the emitter spawning particles
	void CParticleEmitter::Stop()
	{
		mPaused = true;
	}

	//Sets all particles to the despawned (inactive) state
	void CParticleEmitter::Reset()
	{
		//Todo
	}

	//Clears local cache of particles
	//Returns all particles back to the engine
	void CParticleEmitter::Clear()
	{
		//Return all active particles
		while (mActive.size() > 0)
		{
			CParticle* particle = mActive.back().release();
			mActive.pop_back();
			particle->SetData(0);
			mpEngine->ReturnParticle(particle);
		}

		//Return all inactive particles
		while (mInactive.size() > 0)
		{
			CParticle* particle = mInactive.back().release();
			mInactive.pop_back();
			particle->SetData(0);
			mpEngine->ReturnParticle(particle);
		}
	}

	//Do not call
	//Called from the engine to auto update the particles and emitter
	void CParticleEmitter::Update(float delta)
	{
		mTimer += delta;
		while (mTimer > mRate)
		{
			mTimer -= mRate;

			CParticle* particle;
			float matrix[16];
			GetMatrix(matrix);

			//Get an unused particle
			if (mInactive.size() > 0)
			{
				particle = mInactive.back().release();
				mInactive.pop_back();
			}
			else
			{
				particle = mpEngine->GetParticle();
			}

			//Set particle data and location
			particle->SetMatrix(matrix);
			particle->SetData(&mParticleData);

			mActive.push_back(std::unique_ptr<CParticle>(particle));
		}
	}

	//Do not call
	//Called from the engine to orientate the particles
	//To either face or hide behind the camera
	void CParticleEmitter::OrientateParticles(ICamera* camera)
	{
		//Todo
	}

	/************************************
					Sets
	*************************************/

	void CParticleEmitter::SetEmissionType(EEmissionType type)
	{
		mType = type;
	}

	void CParticleEmitter::SetEmissionRate(float rate)
	{
		mRate = rate;
	}

	void CParticleEmitter::SetParticleLife(float life)
	{
		mParticleData.mMaxLife = life;
	}

	void CParticleEmitter::SetParticleSkin(string& skin)
	{
		mParticleData.mTexture = skin;
	}
	
	void CParticleEmitter::SetParticleVelocity(CVector3& vel)
	{
		mParticleData.mVel = vel;
	}
	
	void CParticleEmitter::SetParticleAcceleration(CVector3& acl)
	{
		mParticleData.mAcl = acl;
	}
	
	void CParticleEmitter::SetParticleScale(float scale)
	{
		mParticleData.mScale = scale;
	}

	EEmissionType CParticleEmitter::GetEmissionType()
	{
		return mType;
	}

	float CParticleEmitter::GetEmissionRate()
	{
		return mRate;
	}

	float CParticleEmitter::GetParticleLife();
	string CParticleEmitter::GetParticleSkin();
	CVector3 CParticleEmitter::GetParticleVelocity();
	CVector3 CParticleEmitter::GetParticleAcceleration();
	float CParticleEmitter::GetParticleScale();

	//Returns true is the emitter is emitting particles
	bool CParticleEmitter::IsEmitting()
	{
		return !mPaused;
	}

	//Returns true if one or more of the emitter particles are still active
	bool CParticleEmitter::HasActiveParticles()
	{
		return mActive.size() > 0;
	}

	/************************************
			 Goodbye Cruel World
	*************************************/

	CParticleEmitter::~CParticleEmitter()
	{
		mpSceneManager->RemoveCamera(mpTLXCamera);
		Clear();
	}
}