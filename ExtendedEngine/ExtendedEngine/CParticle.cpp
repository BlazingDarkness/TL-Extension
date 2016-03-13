#include "stdafx.h"
#include "CParticle.h"
#include "ExEngine.h"
#include <iostream>

namespace tle
{
	//Creates a particle model from the mesh
	CParticle::CParticle(ParticleData* data, ExEngine* engine)
	{
		mpEngine = engine;
		mpData = data;
		
		mTextureIndex = 0;
		mTextureTimer = 0.0f;

		mpModel = engine->GetParticleModel(mpData->mTexture[mTextureIndex]);
		mpModel->ResetScale();
		mpModel->Scale(mpData->mScale);

		mLife = mpData->mMaxLife;
		mVel = mpData->mVel;
	}

	/********************************
			  Control stuff
	*********************************/

	//Updates the particle velocity, life, and location
	void CParticle::Update(float delta)
	{
		mLife -= delta;
		
		if (mLife <= 0.0f) return;

		mVel += mpData->mAcl;
		mTextureTimer += delta;

		while (mTextureTimer > mpData->mAnimationRate)
		{
			float matrix[16];

			mpModel->GetMatrix(matrix);
			mpEngine->ReturnParticleModel(mpModel, mpData->mTexture[mTextureIndex]);

			mTextureTimer -= mpData->mAnimationRate;
			mTextureIndex++;

			mpModel = mpEngine->GetParticleModel(mpData->mTexture[mTextureIndex]);
			mpModel->SetMatrix(matrix);
			mpModel->ResetScale();
			mpModel->Scale(mpData->mScale);
		}
		
		mpModel->MoveLocal(mVel.x, mVel.y, mVel.z);
	}

	//Reset the velocity and health of the particle
	void CParticle::Reset()
	{
		mpEngine->ReturnParticleModel(mpModel, mpData->mTexture[mTextureIndex]);

		mTextureIndex = 0;
		mTextureTimer = 0.0f;

		mpModel = mpEngine->GetParticleModel(mpData->mTexture[mTextureIndex]);
		mpModel->ResetScale();
		mpModel->Scale(mpData->mScale);

		mLife = mpData->mMaxLife;
		mVel = mpData->mVel;

	}

	//Hides the particle at the position specified
	void CParticle::HideAt(CVector3& position)
	{
		mpModel->SetPosition(position.x,
								position.y,
								position.z);
	}

	/********************************
				   Sets
	*********************************/

	//Sets the particle's position/rotation matrix
	void CParticle::SetMatrix(float* matrix)
	{
		mpModel->SetMatrix(matrix);
		//std::cout << "Particle Created at: (" << matrix[12] << ',' << matrix[13] << ',' << matrix[14] << ')' << std::endl;
	}

	/********************************
				   Gets
	*********************************/

	//Checks if the particle is dead
	bool CParticle::IsDead()
	{
		return mLife < 0.0f;
	}

	/********************************
			Destroyer of worlds
	*********************************/

	CParticle::~CParticle()
	{
		//If there's a model then return it
		if (mpModel)
		{
			//Get the relavent texture for the model
			string texture = PARTICLE_TEXTURE;
			if (static_cast<int>(mpData->mTexture.size()) > mTextureIndex)
			{
				texture = mpData->mTexture[mTextureIndex];
			}
			mpEngine->ReturnParticleModel(mpModel, texture);
		}
	}
}