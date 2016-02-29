#include "stdafx.h"
#include "CParticle.h"

namespace tle
{
	CParticle::CParticle()
	{
	}

	void CParticle::Update(float delta)
	{
		mVel += mpData->mAcl;
		mpModel->MoveLocal(mVel.x, mVel.y, mVel.z);
	}

	bool CParticle::IsFinished()
	{
		return mLife < 0.0f;
	}

	void CParticle::Hide()
	{
		mpModel->SetPosition(mpData->mOffScreen.x,
							 mpData->mOffScreen.y,
							 mpData->mOffScreen.z);
	}

	void CParticle::Reset()
	{
		if (mpData)
		{
			mLife = mpData->maxLife;
			mVel = mpData->mVel;
			if (mpData->mpTextures)
			{
				if (mpData->mpTextures->size())
				{
					mpModel->SetSkin((*(mpData->mpTextures))[0]);
				}
			}
		}
	}

	void CParticle::SetData(ParticleData* data)
	{
		if (!mpData && data)
		{
			mpData = data;
			mpModel = mpData->mpMesh->CreateModel();
			mpModel->Scale(mpData->mScale);
			mLife = mpData->maxLife;
			mVel = mpData->mVel;
		}
	}

	void CParticle::SetMatrix(float* matrix)
	{
		if (mpModel) mpModel->SetMatrix(matrix);
	}

	ParticleData* CParticle::GetData()
	{
		return mpData;
	}
}