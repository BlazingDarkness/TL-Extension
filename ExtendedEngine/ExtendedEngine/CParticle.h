#pragma once
#include "IUsings.h"
#include "IEngine.h"
namespace tle
{
	//Hacky work around to avoid circular reference
	class ExEngine;

	struct ParticleData
	{
	public:
		float mMaxLife;
		float mScale;
		float mAnimationRate; //Only used if there are multiple textures
		CVector3 mVel;
		CVector3 mAcl;
		std::vector<string> mTexture;
	};

	class CParticle
	{
	private:
		float mLife;
		CVector3 mVel;
		IModel* mpModel;
		ParticleData* mpData;
		float mTextureTimer;
		int mTextureIndex;

		ExEngine* mpEngine;

	public:
		//Creates a particle model from the mesh
		CParticle(ParticleData* data, ExEngine* engine);

		/********************************
				  Control stuff
		*********************************/

		//Updates the particle velocity, life, and location
		virtual void Update(float delta);

		//Reset the velocity and health of the particle
		virtual void Reset();

		//Hides the particle at the position specified
		virtual void HideAt(CVector3& position);

		/********************************
					   Sets
		*********************************/

		//Sets the particle's position/rotation matrix
		virtual void SetMatrix(float* matrix);

		/********************************
					   Gets
		*********************************/

		//Checks if the particle is dead
		virtual bool IsDead();

		/********************************
			   Destroyer of worlds
		*********************************/

		virtual ~CParticle();
	};
}