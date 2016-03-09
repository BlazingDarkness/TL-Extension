#include "stdafx.h"
#include "CParticle.h"
#include <iostream>

namespace tle
{
	int CParticle::count = 0;

	//Creates a particle model from the mesh
	CParticle::CParticle(IMesh* mesh)
	{
		mpMesh = mesh;
		mpModel = mpMesh->CreateModel();

		mLife = 0.0f;
		mVel = CVector3(0.0f, 0.0f, 0.0f);
		mpData = 0;
		count++;
	}

	/********************************
			  Control stuff
	*********************************/

	//Updates the particle velocity, life, and location
	void CParticle::Update(float delta)
	{
		mLife -= delta;
		
		if (mpData)
		{
			mVel += mpData->mAcl;
		}

		mpModel->MoveLocal(mVel.x, mVel.y, mVel.z);
	}

	//Reset the velocity and health of the particle
	void CParticle::Reset()
	{
		if (mpData)
		{
			mLife = mpData->mMaxLife;
			mVel = mpData->mVel;
		}
		else
		{
			mLife = 0.0f;
			mVel = CVector3(0.0f, 0.0f, 0.0f);
		}
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

	//Sets the particle data and updates the private data
	void CParticle::SetData(ParticleData* data)
	{
		mpData = data;
		if (mpData)
		{
			mLife = mpData->mMaxLife;
			mVel = mpData->mVel;
			mpModel->SetSkin(mpData->mTexture);
			mpModel->ResetScale();
			mpModel->Scale(mpData->mScale);
		}
	}

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
		mpMesh->RemoveModel(mpModel);
		count--;
	}
}