#include "stdafx.h"
#include "CParticleEmitter.h"
#include "ExEngine.h"

namespace tle
{
	CParticleEmitter::CParticleEmitter(EEmissionType type, float rate, tlx::ICamera* positionNode, tlx::ISceneManager* sceneManager, ExEngine* engine) : CTLXSceneNode(positionNode)
	{
		mType = type;

		mParticleData.mMaxLife = 0.0f;
		mParticleData.mScale = 1.0f;
		mParticleData.mAnimationRate = 1.0f;
		mParticleData.mVel = CVector3(0.0f, 0.0f, 0.0f);
		mParticleData.mAcl = CVector3(0.0f, 0.0f, 0.0f);
		mParticleData.mTexture = vector<string>(1, PARTICLE_TEXTURE);

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
		mActive.clear();

		//Return all inactive particles
		mInactive.clear();
	}

	//Do not call
	//Called from the engine to auto update the particles and emitter
	void CParticleEmitter::Update(float delta)
	{
		//Update spawning if not paused
		//Also don't spawn anything if the spawn rate is faster than the limit
		if (!mPaused && mRate > FASTEST_EMISSION_RATE)
		{
			mTimer += delta;
			while (mTimer > mRate)
			{
				do {
					mTimer -= mRate;
				} while (mTimer > mParticleData.mMaxLife); //No point in making a particle that is already dead

				CParticle* particle;
				float matrix[16];
				GetMatrix(matrix);
				matrix[12] = GetX();
				matrix[13] = GetY();
				matrix[14] = GetZ();

				//Get an unused particle
				if (mInactive.size() > 0)
				{
					particle = mInactive.back().release();
					mInactive.pop_back();
					particle->Reset();
				}
				else
				{
					particle = new CParticle(&mParticleData, mpEngine);
				}

				//Set particle data and location
				particle->SetMatrix(matrix);

				//Move it by the amount of time passed since it should of been created
				particle->Update(mTimer);

				mActive.push_back(std::unique_ptr<CParticle>(particle));
			}
		}

		//Update particles and remove those that are dead
		for (auto particle = mActive.begin(); particle != mActive.end(); /*Only iterate if no erase occurs*/)
		{
			(*particle)->Update(delta);
			if ((*particle)->IsDead())
			{
				mInactive.push_back( move(*particle) );
				particle = mActive.erase(particle);
			}
			else
			{
				++particle;
			}
		}
	}

	//Do not call
	//Called from the engine to orientate the particles
	//To either face or hide behind the camera
	void CParticleEmitter::OrientateParticles(ICamera* camera)
	{
		//Todo

		//Well s***, I've made a mistake here.
		//Particles move on the local axis, so rotating them to
		//face the camera will change their direction
		//
		//Solution would be to use a dummy model to move the particle
		//therefore allowing the attached quad model to be rotated
		//freely without affecting movement

		float matrix[16];

		camera->GetMatrix(matrix);

		CVector3 pos((matrix[0] * -100.0f) + camera->GetX(),
					 (matrix[1] * -100.0f) + camera->GetY(),
					 (matrix[2] * -100.0f) + camera->GetZ());

		for (auto particle = mInactive.begin(); particle != mInactive.end(); ++particle)
		{
			(*particle)->HideAt(pos);
		}
	}

	/************************************
					Sets
	*************************************/

	void CParticleEmitter::SetEmissionType(EEmissionType type)
	{
		mType = type;
	}

	void CParticleEmitter::SetEmissionAngle(float angle)
	{
		mAngle = angle;
	}

	void CParticleEmitter::SetEmissionRate(float rate)
	{
		mRate = rate;
	}

	void CParticleEmitter::SetParticleLife(float life)
	{
		mParticleData.mMaxLife = life;
		mParticleData.mAnimationRate = life / static_cast<int>(mParticleData.mTexture.size());
	}

	void CParticleEmitter::SetParticleSkin(const string& skin)
	{
		mParticleData.mTexture.clear();
		mParticleData.mTexture.push_back(skin);
		mParticleData.mAnimationRate = mParticleData.mMaxLife;
	}

	//Set the texture used for the particle's quad model
	void CParticleEmitter::SetParticleSkin(const std::vector<string>& skin)
	{
		mParticleData.mAnimationRate = mParticleData.mMaxLife / static_cast<int>(skin.size());
		mParticleData.mTexture = skin;
	}
	
	void CParticleEmitter::SetParticleVelocity(const CVector3& vel)
	{
		mParticleData.mVel = vel;
	}
	
	void CParticleEmitter::SetParticleAcceleration(const CVector3& acl)
	{
		mParticleData.mAcl = acl;
	}
	
	void CParticleEmitter::SetParticleScale(float scale)
	{
		mParticleData.mScale = scale;
	}

	/************************************
					Gets
	*************************************/

	EEmissionType CParticleEmitter::GetEmissionType()
	{
		return mType;
	}

	float CParticleEmitter::GetEmissionAngle()
	{
		return mAngle;
	}

	float CParticleEmitter::GetEmissionRate()
	{
		return mRate;
	}

	float CParticleEmitter::GetParticleLife()
	{
		return mParticleData.mMaxLife;
	}

	string CParticleEmitter::GetParticleSkin()
	{
		//Assumes that there is always at least one texture
		return mParticleData.mTexture[0];
	}

	CVector3 CParticleEmitter::GetParticleVelocity()
	{
		return mParticleData.mVel;
	}

	CVector3 CParticleEmitter::GetParticleAcceleration()
	{
		return mParticleData.mAcl;
	}

	float CParticleEmitter::GetParticleScale()
	{
		return mParticleData.mScale;
	}

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