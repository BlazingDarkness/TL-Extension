#pragma once
#include "CVector3.h"
#include "CMatrix4x4.h"
#include "ExEngine.h"
#include "IManagedResource.h"

namespace tle
{
	struct ParticleData
	{
	public:
		float maxLife;
		float mScale;
		CVector3 mVel;
		CVector3 mAcl;
		IMesh* mpMesh;
		vector<string>* mpTextures;
		CVector3 mOffScreen;
	};

	class CParticle : IManagedResource
	{
	private:
		float mLife;
		CVector3 mVel;
		IModel* mpModel;
		ParticleData* mpData;

	public:
		CParticle();

		virtual void Update(float delta);
		virtual bool IsFinished();
		virtual void Hide();
		virtual void Reset();

		virtual void SetData(ParticleData* data);
		virtual void SetMatrix(float* matrix);

		virtual ParticleData* GetData();
	};
}