/******************************************************************************
Module:			TLXEngine.h

Author:			Laurent Noel
Date created:	6/11/06

Definition of the CTLXEngine class, a I3DEngine derived class
providing the TL-Engine with a wrapper to the external TL-Xtreme engine

Change history:
v1.16	Created 6/11/06 - LN
******************************************************************************/

#ifndef TL_TLX_ENGINE_H
#define TL_TLX_ENGINE_H

#include <string>			// Standard C++ libraries
#include <list>
using namespace std;

#include <TLX-Engine.h>		// TL-Xtreme engine header file

#include "Common.h"		// Common TL-Engine definitions
#include "IEngine.h"	// Base interface class
#include "TLXCamera.h"	// TL-Xtreme-based cameras
#include "TLXLight.h"	// TL-Xtreme-based lights
#include "TLXMesh.h"	// TL-Xtreme-based meshes
#include "TLXFont.h"	// TL-Xtreme-based fonts
#include "TLXSprite.h"	// TL-Xtreme-based sprites

namespace tle			// TL-Engine interface
{
	// Global toggle for legacy-style TL behaviour
	extern bool g_bLegacyTL;

	/*-------------------------------------------------------------------------
	CTLXEngine declaration
	-------------------------------------------------------------------------*/

	class CTLXEngineMod : public IEngine
	{
		TL_CLASS("CTLXEngine")

			/*-------------------------------------------------------------------------
			CTLXEngine public interface
			-------------------------------------------------------------------------*/
	public:

		/////////////////////////////////////
		// Construction / Destruction

		// Default constructor
		CTLXEngineMod();

		// Destructor
		virtual ~CTLXEngineMod();


		/////////////////////////////////////
		// Main control interface

		// Starts the 3D engine fullscreen with the given dimensions. This can only
		// be done once after creating a 3D engine. Returns false if fullscreen
		// resolution not supported
		bool StartFullscreen
			(
				const	int	iWidth = 1280,
				const	int	iHeight = 1024
				);

		// Starts the 3D engine in a window of the given dimensions. This can only
		// be done once after creating a 3D engine.
		void StartWindowed
			(
				const	int		iWidth = 1280,
				const	int		iHeight = 720
				);

		// Delete the 3D engine. Must call this before finishing the program
		// to correctly clean up the memory used by the 3D engine.
		void Delete();


		// Request that the 3D engine stops - will ensure IsRunning returns
		// false. Use this function to terminate the main loop of your
		// application from other parts of the code.
		void Stop();


		/////////////////////////////////////
		// Properties

		// Returns true if the 3D engine is initialised and running. Will 
		// return false if the system or user has requested the engine to stop.
		// Use this function to control the main loop of your application.
		bool IsRunning();

		// Returns true if the engine window/fullscreen is active, i.e. it is the
		// foreground window that the user is working with. If this function
		// returns false, then any keyboard/mouse input should be ignored - it is
		// intended for another window.
		bool IsActive();


		// Return a handle to the engine window
		int GetWindow();

		// Return the width of the engine window/fullscreen.
		int GetWidth();

		// Return the height of the engine window/fullscreen.
		int GetHeight();


		/////////////////////////////////////
		// Media folder management

		// Add a folder to the list of folders searched for media
		void AddMediaFolder
			(
				const	string&	sFolder
				);

		// Remove a folder from the list of folders searched for media, returns
		// true on success (folder was found and removed)
		bool RemoveMediaFolder
			(
				const	string&	sFolder
				);

		// Clears the list of folders searched for media
		void ClearMediaFolders();


		/////////////////////////////////////
		// Mesh creation interface

		// Loads a 3D mesh from a file, returns a pointer to the mesh loaded or
		// 0 for an error. Only loads the mesh data, does not actually create
		// a model in the scene. Use the mesh function CreateModel to create
		// models to place in the scene. Note that different 3D engines support
		// different file types.
		virtual IMesh* LoadMesh
			(
				const	string&	sMeshFileName
				);

		// Removes a mesh from the system, also removes any models in the
		// scene that use the mesh.
		virtual void RemoveMesh
			(
				const	IMesh*	pMesh
				);


		/////////////////////////////////////
		// Camera creation interface

		// Creates a camera of a given type (kStatic [default], kFPS, or
		// kTargeted), positioned at a given location. Returns a pointer to
		// the new camera.
		ICamera* CreateCamera
			(
				const ECameraType eCameraType = kManual,
				const float       fX = 0.0f,
				const float       fY = 12.0f,
				const float       fZ = -30.0f
				);

		// Removes a camera from the scene.
		void RemoveCamera
			(
				const	ICamera* pCamera
				);


		/////////////////////////////////////
		// Light interface

		// Set the ambient light colour for the scene. The ambient light colour defaults
		// to white (i.e. everything is fully lit) - this function overrides that setting
		void SetAmbientLight
			(
				const float fRed = 1.0f,
				const float fGreen = 1.0f,
				const float fBlue = 1.0f
				);

		// Creates a point light positioned at a given location and with the given colour (red,
		// green and blue components in range 0.0f to 1.0f). Returns a pointer to the new light.
		ILight* CreatePointLight
			(
				const float	fX = 0.0f,
				const float	fY = 0.0f,
				const float	fZ = 0.0f,
				const float fRed = 1.0f,
				const float fGreen = 1.0f,
				const float fBlue = 1.0f,
				const float fRadius = 100.0f
				);

		// Removes a light from the scene.
		void RemoveLight
			(
				const ILight* pLight
				);


		/////////////////////////////////////
		// Sprite interface

		// Create a sprite in the scene using the given image file. 
		// Returns a pointer to the sprite loaded or 0 for an error.
		// May optionally set the initial position for the sprite
		ISprite* CreateSprite
			(
				const string& sSpriteName,
				const float	  fX = 0.0f,
				const float	  fY = 0.0f,
				const float	  fZ = 0.0f
				);

		// Removes a sprite from the scene
		void RemoveSprite
			(
				const	ISprite* pSprite
				);



		/////////////////////////////////////
		// Rendering interface

		// Draw everything in the scene from the viewpoint of the given camera.
		// If no camera is supplied, the most recently created camera is used.
		virtual void DrawScene
			(
				ICamera* pCamera = 0
				);


		/////////////////////////////////////
		// GUI interface

		// Changes the 3D engine window title - no effect in fullscreen mode
		void SetWindowCaption
			(
				const string&	sText
				);

		// Returns a pointer to the default (built-in) font
		IFont* DefaultFont();

		// Loads a font from a file, returns a pointer to the font loaded or
		// 0 for an error.
		IFont* LoadFont
			(
				const string&      sFontName,
				const unsigned int iSize = 24
				);

		// Removes a font from the engine
		void RemoveFont
			(
				const	IFont* pFont
				);


		/////////////////////////////////////
		// UI interface

		// Returns true when a given key or button is first pressed down. Use
		// for one-off actions or toggles. Example key codes: Key_A or
		// Mouse_LButton, see input.h for a full list.
		bool KeyHit
			(
				const	EKeyCode	eKeyCode
				);

		// Returns true as long as a given key or button is held down. Use for
		// continuous action or motion. Example key codes: Key_A or
		// Mouse_LButton, see input.h for a full list.
		bool KeyHeld
			(
				const	EKeyCode	eKeyCode
				);

		// Returns true when any key or button is first pressed down
		bool AnyKeyHit();

		// Returns true when any key or button is being held down
		bool AnyKeyHeld();


		// Returns the X position of the mouse (relative to the top-left of the
		// 3D engine window/fullscreen).
		int GetMouseX();

		// Returns the Y position of the mouse (relative to the top-left of the 
		// 3D engine window/fullscreen).
		int GetMouseY();

		// Returns the wheel position of the mouse
		float GetMouseWheel();


		// Returns the X mouse movement since the last call to this function
		int GetMouseMovementX();

		// Returns the Y mouse movement since the last call to this function
		int GetMouseMovementY();

		// Returns the mouse wheel movement since the last call to this function
		float GetMouseWheelMovement();


		// Start capturing the mouse. The mouse cursor will be removed and all
		// mouse movement captured by the 3D engine window. Allows direct mouse 
		// control for games that do not need a mouse cursor.
		void StartMouseCapture();

		// Stops capturing the mouse. See 'StartMouseCapture'.
		void StopMouseCapture();


		// Get time passed since last call to this function, returns value in seconds using
		// highest accuracy timer available
		virtual float Timer();


		/*-------------------------------------------------------------------------
		CTLXEngine private interface
		-------------------------------------------------------------------------*/
	protected:

		/////////////////////////////////////
		// Media folder support

		// Finds the given file in the current folder or the media folder list.
		// Returns the full path to the file, or an empty string on failure
		string FindMediaFile
			(
				const	string&	sFileName
				);


		/////////////////////////////////////
		// Private data

		// TL-Xtreme interface and manager classes
		tlx::IRenderDevice*   m_pRenderDevice;
		tlx::IRenderManager*  m_pRenderManager;
		tlx::ISceneManager*   m_pSceneManager;
		tlx::IMeshManager*    m_pMeshManager;
		tlx::ITextureManager* m_pTextureManager;
		TUInt32               m_MeshID;

		// TL-Xtreme I/O, GUI etc.
		tlx::CInputReceiver*  m_pInputReceiver;
		tlx::CWindowReceiver* m_pWindowReceiver;
		tlx::IOSWindow*       m_pWindow;
		tlx::IOSTimer*        m_pTimer;


		// Resource container definitions
		typedef list<CTLXMesh*>       TMeshList;
		typedef TMeshList::iterator   TMeshListIter;
		typedef list<CTLXCamera*>     TCameraList;
		typedef TCameraList::iterator TCameraListIter;
		typedef list<CTLXLight*>      TLightList;
		typedef TLightList::iterator  TLightListIter;
		typedef list<CTLXFont*>       TFontList;
		typedef TFontList::iterator   TFontListIter;
		typedef list<CTLXSprite*>     TSpriteList;
		typedef TSpriteList::iterator TSpriteListIter;

		// Resource containers
		TMeshList   m_Meshes;
		TCameraList m_Cameras;
		TLightList  m_Lights;
		TFontList   m_Fonts;
		TSpriteList m_Sprites;

		// Default font object, pointer to entry in font list above
		CTLXFont*   m_pDefaultFont;

	}; // class CTLXEngine


} // namespace tle


#endif // TL_TLX_ENGINE_H
