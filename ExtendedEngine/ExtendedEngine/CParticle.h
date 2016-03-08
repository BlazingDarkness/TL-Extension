#pragma once
#include "IUsings.h"
#include "IEngine.h"
namespace tle
{
	struct ParticleData
	{
	public:
		float mMaxLife;
		float mScale;
		CVector3 mVel;
		CVector3 mAcl;
		string mTexture;
	};

	class CParticle
	{
	private:
		float mLife;
		CVector3 mVel;
		IMesh* mpMesh;
		IModel* mpModel;
		ParticleData* mpData;

	public:
		//Creates a particle model from the mesh
		CParticle(IMesh* quad);

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

		//Sets the particle data and updates the private data
		virtual void SetData(ParticleData* data);

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