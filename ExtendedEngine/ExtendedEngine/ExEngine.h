#pragma once
#include "IEngine.h"
#include "IUpdateable.h"
#include "TLXEngineModified.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace tle
{
	class ExEngine : public CTLXEngineMod
	{
	private:
		std::unordered_map<string, IMesh*> mMeshMap;
		std::vector<std::unique_ptr<IAnimation>> mAnimations;
	public:
		ExEngine();

		virtual IAnimation* CreateAnimation(const std::vector<string>& frameList, const CVector3& position, const float tickRate, const bool looped);
		virtual IAnimation* CreateAnimation(string& name, string& extension, int amount, const CVector3& position, const float tickRate, const bool looped);
		virtual void RemoveAnimation(IAnimation* pAnimation);

		virtual IMesh* LoadMesh(const string& sMeshFileName);
		virtual void RemoveMesh(const IMesh* pMesh);

	};
}