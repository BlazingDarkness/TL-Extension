#pragma once
#include <SceneNode.h>
#include <Mesh.h>
#include <CVector3.h>

namespace tle
{
	enum EEmissionType { Sphere, Circle, Cone, Arch, Line };

	class IParticleEmitter : virtual public ISceneNode
	{
	public:
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual void Reset() = 0;

		virtual void SetEmissionType(EEmissionType type) = 0;
		virtual void SetEmissionRate(float rate) = 0;

		virtual EEmissionType GetEmissionType() = 0;
		virtual float GetEmissionRate() = 0;

		virtual bool IsEmitting() = 0;
	};
}