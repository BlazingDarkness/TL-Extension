#include "stdafx.h"

//#include <algorithm>
#include <iostream>

#include "ExEngine.h"
#include "CAnimation.h"

namespace tle
{
	ExEngine::ExEngine() : CTLXEngineMod()
	{

	}

	IMesh* ExEngine::LoadMesh(const string& sMeshFileName)
	{
		auto mesh = mMeshMap.find(sMeshFileName);
		if (mesh != mMeshMap.end())
		{
			return mesh->second;
		}
		else
		{
			IMesh* newMesh = CTLXEngineMod::LoadMesh(sMeshFileName);
			mMeshMap.insert(std::pair<string, IMesh*>{sMeshFileName, newMesh});
			return newMesh;
		}
	}

	void ExEngine::RemoveMesh(const IMesh* pMesh)
	{
		for (auto mesh = mMeshMap.begin(); mesh != mMeshMap.end(); mesh++)
		{
			if (pMesh == mesh->second)
			{
				CTLXEngineMod::RemoveMesh(mesh->second);
				mMeshMap.erase(mesh);
				return;
			}
		}
	}

	IAnimation* ExEngine::CreateAnimation(const std::vector<string>& frameList, const CVector3& position, const float tickRate, const bool looped)
	{
		SpriteVector sprites;
		for (auto it = frameList.begin(); it != frameList.end(); it++)
		{
			ISprite* newSprite = CreateSprite(*it, position.x, position.y, position.z);

			//Couldn't load sprite
			if (!newSprite)
			{
				std::cout << "Could not load image file \"" + (*it) + "\" Aborting creation of IAnimation.";

				//Remove sprites
				for (auto sprite = sprites.begin(); sprite != sprites.end(); sprite++)
				{
					RemoveSprite(*sprite);
				}
				return 0;
			}
		}
		IAnimation* animation = new CAnimation(this, sprites, position, tickRate, looped);
		mAnimations.push_back(unique_ptr<IAnimation>(animation));
		return animation;
	}

	IAnimation* ExEngine::CreateAnimation(string& name, string& extension, int amount, const CVector3& position, const float tickRate, const bool looped)
	{
		SpriteVector sprites;
		for (int i = 0; i < amount; i++)
		{
			string spriteFile = name + to_string(i) + extension;
			ISprite* newSprite = CreateSprite(spriteFile, position.x, position.y, position.z);

			//Couldn't load sprite
			if (!newSprite)
			{
				std::cout << "Could not load image file \"" + spriteFile + "\" Aborting creation of IAnimation.";

				//Remove sprites
				for (auto sprite = sprites.begin(); sprite != sprites.end(); sprite++)
				{
					RemoveSprite(*sprite);
				}
				return 0;
			}
		}
		IAnimation* animation = new CAnimation(this, sprites, position, tickRate, looped);
		mAnimations.push_back(unique_ptr<IAnimation>(animation));
		return animation;
	}

	void ExEngine::RemoveAnimation(IAnimation* pAnimation)
	{
		for (auto animation = mAnimations.begin(); animation != mAnimations.end(); animation++)
		{
			if ((*animation).get() == pAnimation)
			{
				mAnimations.erase(animation);
				return;
			}
		}
	}

	CVector3 ExEngine::GetOffScreenPos()
	{
		if (m_Cameras.size() > 0)
			return CVector3(m_Cameras.back()->GetX(),
							m_Cameras.back()->GetY(),
							m_Cameras.back()->GetZ());
		else
			return CVector3(0.0f, 0.0f, 0.0f);
	}
}