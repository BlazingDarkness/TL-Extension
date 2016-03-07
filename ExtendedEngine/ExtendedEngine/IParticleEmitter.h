#pragma once
#include <SceneNode.h>
#include <Mesh.h>
#include <CVector3.h>

namespace tle
{
	enum EmissionType { Sphere, Circle, Cone, Arch, Line };

	struct SParticleData
	{
		IMesh* mMesh;
		string skin;
		tlx::CVector3 Acceleration;
		tlx::CVector3 Velocity;
		float life;
		float spread;
	};

	class IParticleEmitter : virtual public ISceneNode
	{
	public:
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Reset() = 0;

		virtual void SetEmissionType(EmissionType type) = 0;
		virtual void SetEmissionRate(float rate) = 0;

		virtual EmissionType GetEmissionType() = 0;
		virtual float GetEmissionRate() = 0;

		virtual bool IsEmitting() = 0;
	};
}