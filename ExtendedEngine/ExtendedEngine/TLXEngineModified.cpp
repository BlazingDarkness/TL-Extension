/******************************************************************************
Module:			TLXEngine.cppr

Author:			Laurent Noel
Date created:	6/11/06

Implementation of the CTLXEngine class, a I3DEngine derived class
providing the TL-Engine with a wrapper to the external TL-Xtreme engine

Change history:
V1.16	Created 6/11/06 - LN
******************************************************************************/

#include "StdAfx.h"		// Precompiled headers (Visual Studio only)

#include <algorithm>	// Standard C++ libraries
using namespace std;

#include "TLXEngineModified.h"	// Class declaration

namespace tle			// TL-Engine interface
{
	/*-------------------------------------------------------------------------
	CTLXEngine public member functions
	-------------------------------------------------------------------------*/

	/////////////////////////////////////
	// Construction / Destruction

	// Default constructor
	CTLXEngineMod::CTLXEngineMod()
	{
		TL_FN("CTLXEngine");

		m_pRenderDevice = 0;
		m_pRenderManager = 0;
		m_pSceneManager = 0;
		m_pMeshManager = 0;
		m_pTextureManager = 0;
		m_MeshID = 0;

		m_pInputReceiver = 0;
		m_pWindowReceiver = 0;
		m_pWindow = 0;
		m_pTimer = 0;

		TL_ENDFN;
	}

	// Destructor
	CTLXEngineMod::~CTLXEngineMod()
	{
		TL_FN("~CTLXEngine");

		while (m_Fonts.begin() != m_Fonts.end())
		{
			delete *(m_Fonts.begin());
			m_Fonts.erase(m_Fonts.begin());
		}
		while (m_Cameras.begin() != m_Cameras.end())
		{
			delete *(m_Cameras.begin());
			m_Cameras.erase(m_Cameras.begin());
		}
		while (m_Lights.begin() != m_Lights.end())
		{
			delete *(m_Lights.begin());
			m_Lights.erase(m_Lights.begin());
		}
		while (m_Meshes.begin() != m_Meshes.end())
		{
			delete *(m_Meshes.begin());
			m_Meshes.erase(m_Meshes.begin());
		}

		delete m_pRenderDevice;
		delete m_pWindow;
		delete m_pInputReceiver;

		TL_ENDFN;
	}


	/////////////////////////////////////
	// Main control interface

	// Starts the 3D engine fullscreen with the given dimensions. This can only
	// be done once after creating a 3D engine. Returns false if fullscreen
	// resolution not supported
	bool CTLXEngineMod::StartFullscreen
		(
			const	int	iWidth,
			const	int	iHeight
			)
	{
		TL_FN("StartFullscreen");

		m_pRenderDevice = tlx::CreateRenderDevice(tlx::kDirectX);

		unsigned int iNearestMode = tlx::kiUnspecifiedValue;
		unsigned int iNearestAreaDiff = tlx::kiUnspecifiedValue;
		for (unsigned int iMode = 0; iMode < m_pRenderDevice->GetNumDisplayModes(); ++iMode)
		{
			tlx::SDisplayModeProfile profile;
			m_pRenderDevice->GetDisplayModeProfile(iMode, &profile);
			if (profile.eDeviceType == tlx::kHardware &&
				profile.iTextureFormats != tlx::kUnknownPixelFormat &&
				tlx::BitDepthFromPixelFormat(profile.format.ePixelFormat) == 32)
			{
				unsigned int iAreaDiff =
					tlx::Abs(iWidth * iHeight - profile.format.iWidth * profile.format.iHeight);
				if (iNearestAreaDiff == tlx::kiUnspecifiedValue || iAreaDiff < iNearestAreaDiff)
				{
					iNearestAreaDiff = iAreaDiff;
					iNearestMode = iMode;
				}
			}
		}
		if (iNearestMode == tlx::kiUnspecifiedValue)
		{
			delete m_pRenderDevice;
			return false;
		}

		m_pInputReceiver = new tlx::CInputReceiver;
		m_pWindowReceiver = new tlx::CWindowReceiver(m_pRenderDevice);
		m_pWindow = tlx::CreateOSWindow("TL-Engine (TL-Xtreme)", m_pInputReceiver,
			m_pWindowReceiver);
		m_pTimer = tlx::CreateOSTimer();

		m_pRenderDevice->Start(m_pWindow, iNearestMode, tlx::kHardware, 0);
		m_pRenderManager = m_pRenderDevice->RenderManager();
		m_pMeshManager = m_pRenderDevice->MeshManager();
		m_pTextureManager = m_pRenderDevice->TextureManager();
		m_pSceneManager = tlx::CreateSceneManager();
		m_pWindow->Show();

		tlx::IFont* pDefaultFont = m_pTextureManager->CreateFont("Arial", 12);
		m_pDefaultFont = new CTLXFont(m_pTextureManager, pDefaultFont);
		m_Fonts.push_back(m_pDefaultFont);

		return true;

		TL_ENDFN;
	}

	// Starts the 3D engine in a window of the given dimensions. This can only
	// be done once immediately after creating a 3D engine.
	void CTLXEngineMod::StartWindowed
		(
			const int iWidth,
			const int iHeight
			)
	{
		TL_FN("StartWindowed");

		m_pRenderDevice = tlx::CreateRenderDevice(tlx::kDirectX);

		m_pInputReceiver = new tlx::CInputReceiver;
		m_pWindowReceiver = new tlx::CWindowReceiver(m_pRenderDevice);
		m_pWindow = tlx::CreateOSWindow("TL-Engine (TL-Xtreme)", m_pInputReceiver,
			m_pWindowReceiver, iWidth, iHeight);
		m_pTimer = tlx::CreateOSTimer();

		m_pRenderDevice->Start(m_pWindow, tlx::kWindowedMode, tlx::kHardware, 0);
		m_pRenderManager = m_pRenderDevice->RenderManager();
		m_pMeshManager = m_pRenderDevice->MeshManager();
		m_pTextureManager = m_pRenderDevice->TextureManager();
		m_pSceneManager = tlx::CreateSceneManager();
		m_pWindow->Show();

		tlx::IFont* pDefaultFont = m_pTextureManager->CreateFont("Arial", 12);
		m_pDefaultFont = new CTLXFont(m_pTextureManager, pDefaultFont);
		m_Fonts.push_back(m_pDefaultFont);

		TL_ENDFN;
	}

	// Delete the 3D engine. Must call this before finishing the program
	// to correctly clean up the memory used by the 3D engine.
	void CTLXEngineMod::Delete()
	{
		TL_FN("Delete");

		delete this;

		TL_ENDFN;
	}


	// Request that the 3D engine stops - will ensure IsRunning returns
	// false. Use this function to terminate the main loop of your
	// application from other parts of the code.
	void CTLXEngineMod::Stop()
	{
		TL_FN("Stop");

		if (m_pWindow)
		{
			m_pWindow->Close();
		}

		TL_ENDFN;
	}


	/////////////////////////////////////
	// Properties

	// Returns true if the 3D engine is initialised and running. Will 
	// return false if the system or user has requested the engine to stop.
	// Use this function to control the main loop of your application.
	bool CTLXEngineMod::IsRunning()
	{
		TL_FN("IsRunning");

		return (m_pWindow && m_pWindow->Update());

		TL_ENDFN;
	}

	// Returns true if the engine window/fullscreen is active, i.e. it is the
	// foreground window that the user is working with. If this function
	// returns false, then any keyboard/mouse input should be ignored - it is
	// intended for another window.
	bool CTLXEngineMod::IsActive()
	{
		return m_pWindow->IsActive();
	}


	// Return a handle to the engine window
	int CTLXEngineMod::GetWindow()
	{
		return *reinterpret_cast<const int*>(m_pWindow->GetHandle());
	}


	// Return the width of the engine window/fullscreen.
	int CTLXEngineMod::GetWidth()
	{
		return (m_pRenderDevice->GetSurfaceWidth());
	}

	// Return the height of the engine window/fullscreen.
	int CTLXEngineMod::GetHeight()
	{
		return (m_pRenderDevice->GetSurfaceHeight());
	}


	/////////////////////////////////////
	// Media folder management

	// Add a folder to the list of folders searched for media
	void CTLXEngineMod::AddMediaFolder
		(
			const	string&	sFolder
			)
	{
		TL_FN("AddMediaFolder");

		m_pRenderDevice->AddResourceFolder(sFolder);

		TL_ENDFN;
	}

	// Remove a folder from the list of folders searched for media, returns
	// true on success (folder was found and removed)
	bool CTLXEngineMod::RemoveMediaFolder
		(
			const	string&	sFolder
			)
	{
		TL_FN("RemoveMediaFolder");

		return m_pRenderDevice->RemoveResourceFolder(sFolder);

		TL_ENDFN;
	}

	// Clears the list of folders searched for media
	void CTLXEngineMod::ClearMediaFolders()
	{
		TL_FN("ClearMediaFolders");

		m_pRenderDevice->RemoveAllResourceFolders();

		TL_ENDFN;
	}


	/////////////////////////////////////
	// Mesh creation interface

	// Loads a 3D mesh from a file, returns a pointer to the mesh loaded or
	// 0 for an error. Only loads the mesh data, does not actually create
	// a model in the scene. Use the mesh function CreateModel to create
	// models to place in the scene. Note that different 3D engines support
	// different file types.
	IMesh* CTLXEngineMod::LoadMesh
		(
			const	string& sMeshFileName
			)
	{
		TL_FN("LoadMesh");

		// Create TL-Xtreme mesh
		tlx::IMesh* pTLXMesh;
		if (m_pMeshManager->ImportMesh(sMeshFileName, m_MeshID, &pTLXMesh) != tlx::kSuccess)
		{
			return 0;
		}
		m_MeshID++;
		pTLXMesh->AllocateResources(); //-"-
		m_pRenderManager->SortBuckets(); //-"-

										 // Create TL-Engine mesh from TL-Xtreme mesh - add it to stored list
		CTLXMesh* pMesh = new CTLXMesh(m_pMeshManager, m_pSceneManager,
			m_pRenderManager, pTLXMesh);
		if (!pMesh)
		{
			m_pMeshManager->RemoveMesh(pTLXMesh);
			return 0;
		}
		m_Meshes.push_back(pMesh);

		return pMesh;

		TL_ENDFN;
	}


	// Removes a mesh from the system, also removes any models in the
	// scene that use the mesh.
	void CTLXEngineMod::RemoveMesh
		(
			const	IMesh*	pMesh
			)
	{
		TL_FN("RemoveMesh");

		TL_ASSERT(pMesh, "Attempt to remove null mesh");

		TMeshListIter itMesh = find(m_Meshes.begin(), m_Meshes.end(), pMesh);
		TL_ASSERT(itMesh != m_Meshes.end(), "Mesh not found")
			m_Meshes.erase(itMesh);

		delete pMesh;

		TL_ENDFN;
	}


	/////////////////////////////////////
	// Camera creation interface

	// Creates a camera of a given type (kStatic [default], kFPS, or
	// kTargeted), positioned at a given location. Returns a pointer to
	// the new camera.
	ICamera* CTLXEngineMod::CreateCamera
		(
			const		ECameraType	eCameraType,
			const		float		fX,
			const		float		fY,
			const		float		fZ
			)
	{
		TL_FN("CreateCamera");

		tlx::ICamera* pTLXCamera;
		pTLXCamera = m_pSceneManager->CreateCamera(tlx::CVector3(fX, fY, fZ));
		if (!pTLXCamera)
		{
			return 0;
		}

		float fViewportAspect = static_cast<float>(m_pRenderDevice->GetSurfaceWidth()) /
			m_pRenderDevice->GetSurfaceHeight();
		pTLXCamera->SetFOVMin(tlx::kfPi / 3.4f, fViewportAspect);

		CTLXCamera* pCamera = new CTLXCamera(eCameraType, m_pSceneManager, pTLXCamera);
		if (!pCamera)
		{
			return 0;
		}
		m_Cameras.push_back(pCamera);

		return pCamera;

		TL_ENDFN;
	}

	// Removes a camera from the scene.
	void CTLXEngineMod::RemoveCamera
		(
			const	ICamera* pCamera
			)
	{
		TL_FN("RemoveCamera");

		TL_ASSERT(pCamera, "Attempt to remove null camera");

		TCameraListIter itCamera =
			find(m_Cameras.begin(), m_Cameras.end(), pCamera);
		TL_ASSERT(itCamera != m_Cameras.end(), "Camera not found")
			m_Cameras.erase(itCamera);

		delete pCamera;

		TL_ENDFN;
	}


	/////////////////////////////////////
	// Light interface

	// Set the ambient light colour for the scene. The ambient light colour defaults
	// to white (i.e. everything is fully lit) - this function overrides that setting
	void CTLXEngineMod::SetAmbientLight
		(
			const float fRed,
			const float fGreen,
			const float fBlue
			)
	{
		TL_FN("SetAmbientLight");

		//TODO:

		TL_ENDFN;
	}

	// Creates a point light positioned at a given location and with the given colour (red,
	// green and blue components in range 0.0f to 1.0f). Returns a pointer to the new light.
	ILight* CTLXEngineMod::CreatePointLight
		(
			const float	fX,
			const float	fY,
			const float	fZ,
			const float fRed,
			const float fGreen,
			const float fBlue,
			const float fRadius
			)
	{
		TL_FN("CreatePointLight");

		//TODO:
		//tlx::ILight* pTLXLight;
		//pTLXLight = m_pSceneManager->CreateCamera( CVector3(fX, fY, fZ) );
		//if (!pTLXLight)
		//{
		//	return 0;
		//}

		CTLXLight* pLight = new CTLXLight(kPoint, 0);//pTLXLight );
		if (!pLight)
		{
			return 0;
		}
		m_Lights.push_back(pLight);

		return pLight;

		TL_ENDFN;
	}

	// Removes a light from the scene.
	void CTLXEngineMod::RemoveLight
		(
			const ILight* pLight
			)
	{
		TL_FN("RemoveLight");

		TL_ASSERT(pLight, "Attempt to remove null light");

		TLightListIter itLight =
			find(m_Lights.begin(), m_Lights.end(), pLight);
		TL_ASSERT(itLight != m_Lights.end(), "Camera not found")
			m_Lights.erase(itLight);

		delete pLight;

		TL_ENDFN;
	}


	/////////////////////////////////////
	// Sprite interface

	// Create a sprite in the scene using the given image file. 
	// Returns a pointer to the sprite loaded or 0 for an error.
	// May optionally set the initial position for the sprite
	ISprite* CTLXEngineMod::CreateSprite
		(
			const string& sSpriteName,
			const float	  fX /*= 0.0f*/,
			const float	  fY /*= 0.0f*/,
			const float	  fZ /*= 0.0f*/
			)
	{
		TL_FN("CreateSprite");

		tlx::ISprite* pTLXSprite;
		pTLXSprite = m_pTextureManager->CreateSprite(sSpriteName, tlx::CVector3(fX, fY, fZ));
		if (!pTLXSprite)
		{
			return 0;
		}

		CTLXSprite* pSprite = new CTLXSprite(m_pTextureManager, pTLXSprite);
		if (!pSprite)
		{
			return 0;
		}
		m_Sprites.push_back(pSprite);

		return pSprite;

		TL_ENDFN;
	}

	// Removes a sprite from the scene
	void CTLXEngineMod::RemoveSprite
		(
			const	ISprite* pSprite
			)
	{
		TL_FN("RemoveSprite");

		TL_ASSERT(pSprite, "Attempt to remove null sprite");

		TSpriteListIter itSprite = find(m_Sprites.begin(), m_Sprites.end(), pSprite);
		TL_ASSERT(itSprite != m_Sprites.end(), "Font not found")

			m_Sprites.erase(itSprite);
		delete pSprite;

		TL_ENDFN;
	}


	/////////////////////////////////////
	// Rendering interface

	// Draw everything in the scene from the viewpoint of the camera given.
	// If no camera is supplied, the most recently created camera is used.
	void CTLXEngineMod::DrawScene
		(
			ICamera* pCamera
			)
	{
		TL_FN("DrawScene");

		ICamera* pTempCamera = 0;
		if (!pCamera)
		{
			if (m_Cameras.size() > 0)
			{
				pCamera = m_Cameras.back();
			}
			else
			{
				pCamera = pTempCamera = CreateCamera(kManual, 0, 0, 0);
			}
		}
		CTLXCamera* pTLXCamera;
		pTLXCamera = dynamic_cast<CTLXCamera*>(pCamera);

		// Engine based update - FPS cameras, and scene manager preparation
		if (pTLXCamera->GetType() == kFPS)
		{
			static float time = m_pTimer->GetTime();
			float newTime = m_pTimer->GetTime();
			float frameTime = newTime - time;
			time = newTime;
			float moveSpeed = pTLXCamera->GetMovementSpeed() * frameTime;
			float rotSpeed = pTLXCamera->GetRotationSpeed() * frameTime;

			if (KeyHeld(Key_Up))
			{
				pTLXCamera->MoveLocalZ(moveSpeed);
			}
			if (KeyHeld(Key_Down))
			{
				pTLXCamera->MoveLocalZ(-moveSpeed);
			}
			if (KeyHeld(Key_Right))
			{
				pTLXCamera->MoveLocalX(moveSpeed);
			}
			if (KeyHeld(Key_Left))
			{
				pTLXCamera->MoveLocalX(-moveSpeed);
			}
			pTLXCamera->RotateY(GetMouseMovementX() * rotSpeed);
			if (g_bLegacyTL)
			{
				pTLXCamera->RotateX(GetMouseMovementY() * rotSpeed);
			}
			else
			{
				pTLXCamera->RotateLocalX(GetMouseMovementY() * rotSpeed);
			}
			m_pWindow->SetMousePosition(m_pWindow->GetWidth() / 2, m_pWindow->GetHeight() / 2);
		}
		m_pSceneManager->Update();

		// Draw scene
		m_pRenderDevice->ClearBackBuffer(true, 0xC0C0C0);
		m_pRenderDevice->StartRender();
		m_pRenderManager->Render(pTLXCamera->GetAbsPositionMatrix(),
			pTLXCamera->GetProjectionMatrix());
		m_pTextureManager->RenderSprites();
		for (TFontListIter itFont = m_Fonts.begin(); itFont != m_Fonts.end(); ++itFont)
		{
			(*itFont)->DrawTextRequests();
		}
		m_pRenderDevice->FinishRender();
		m_pRenderDevice->FlipBackBuffer();

		if (pTempCamera)
		{
			RemoveCamera(pTempCamera);
		}

		TL_ENDFN;
	}


	/////////////////////////////////////
	// GUI interface

	// Changes the 3D engine window title - no effect in fullscreen mode
	void CTLXEngineMod::SetWindowCaption
		(
			const string&	sText
			)
	{
		m_pWindow->SetCaption(sText);
	}

	// Returns a pointer to the default (built-in) font
	IFont* CTLXEngineMod::DefaultFont()
	{
		return m_pDefaultFont;
	}

	// Loads a font from a file, returns a pointer to the font loaded or
	// 0 for an error. May optionally pass point size of font
	IFont* CTLXEngineMod::LoadFont
		(
			const string&      sFontName,
			const unsigned int iSize /*= 24*/
			)
	{
		TL_FN("CreateFont");

		tlx::IFont* pTLXFont;
		pTLXFont = m_pTextureManager->CreateFont(sFontName, iSize);
		if (!pTLXFont)
		{
			return 0;
		}

		CTLXFont* pFont = new CTLXFont(m_pTextureManager, pTLXFont);
		if (!pFont)
		{
			return 0;
		}
		m_Fonts.push_back(pFont);

		return pFont;

		TL_ENDFN;
	}

	// Removes a font from the engine
	void CTLXEngineMod::RemoveFont
		(
			const	IFont* pFont
			)
	{
		TL_FN("RemoveFont");

		TL_ASSERT(pFont, "Attempt to remove null font");

		TFontListIter itFont = find(m_Fonts.begin(), m_Fonts.end(), pFont);
		TL_ASSERT(itFont != m_Fonts.end(), "Font not found")

			m_Fonts.erase(itFont);
		delete pFont;

		TL_ENDFN;
	}


	/////////////////////////////////////
	// UI interface

	// Returns true when a given key or button is first pressed down. Use
	// for one-off actions or toggles. Example key codes: Key_A or
	// Mouse_LButton, see input.h for a full list.
	bool CTLXEngineMod::KeyHit
		(
			const EKeyCode eKeyCode
			)
	{
		return m_pInputReceiver->KeyHit(static_cast<tlx::EKeyCode>(eKeyCode));
	}

	// Returns true as long as a given key or button is held down. Use for
	// continuous action or motion. Example key codes: Key_A or
	// Mouse_LButton, see input.h for a full list.
	bool CTLXEngineMod::KeyHeld
		(
			const EKeyCode eKeyCode
			)
	{
		return m_pInputReceiver->KeyHeld(static_cast<tlx::EKeyCode>(eKeyCode));
	}

	// Returns true when any key or button is first pressed down
	bool CTLXEngineMod::AnyKeyHit()
	{
		return m_pInputReceiver->AnyKeyHit();
	}

	// Returns true when any key or button is being held down
	bool CTLXEngineMod::AnyKeyHeld()
	{
		return m_pInputReceiver->AnyKeyHeld();
	}


	// Returns the X position of the mouse (relative to the top-left of the 3D
	// engine window/fullscreen).
	int CTLXEngineMod::GetMouseX()
	{
		return m_pInputReceiver->GetMouseX();
	}

	// Returns the Y position of the mouse (relative to the top-left of the 3D
	// engine window/fullscreen).
	int CTLXEngineMod::GetMouseY()
	{
		return m_pInputReceiver->GetMouseY();
	}

	// Returns the wheel position of the mouse
	float CTLXEngineMod::GetMouseWheel()
	{
		return m_pInputReceiver->GetMouseWheel();
	}


	// Returns the X mouse movement since the last call to this function
	int CTLXEngineMod::GetMouseMovementX()
	{
		return m_pInputReceiver->GetMouseMovementX();
	}

	// Returns the Y mouse movement since the last call to this function
	int CTLXEngineMod::GetMouseMovementY()
	{
		return m_pInputReceiver->GetMouseMovementY();
	}

	// Returns the mouse wheel movement since the last call to this function
	float CTLXEngineMod::GetMouseWheelMovement()
	{
		return m_pInputReceiver->GetMouseWheelMovement();
	}


	// Start capturing the mouse. The mouse cursor will be removed and all
	// mouse movement captured by the 3D engine window. Allows direct mouse 
	// control for games that do not need a mouse cursor.
	void CTLXEngineMod::StartMouseCapture()
	{
		m_pWindow->StartMouseCapture();
	}

	// Stops capturing the mouse. See 'StartMouseCapture'.
	void CTLXEngineMod::StopMouseCapture()
	{
		m_pWindow->StopMouseCapture();
	}


	// Get time passed since last call to this function, returns value in seconds using
	// highest accuracy timer available
	float CTLXEngineMod::Timer()
	{
		return m_pTimer->GetLapTime();
	}

} // namespace tle
