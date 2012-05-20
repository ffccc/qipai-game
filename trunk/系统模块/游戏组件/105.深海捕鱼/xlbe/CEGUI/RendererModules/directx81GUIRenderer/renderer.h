/***********************************************************************
	filename: 	renderer.h
	created:	15/3/2004
	author:		Paul D Turner
	
	purpose:	Interface for DirectX 8.1 Renderer class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
/*************************************************************************
	This file contains code that is specific to Win32 and DirectX
*************************************************************************/
#ifndef _DirectX81GUIRenderer_h_
#define _DirectX81GUIRenderer_h_

#if !defined(CEGUI_STATIC)
	#ifdef DIRECTX81_GUIRENDERER_EXPORTS
	#define DIRECTX81_GUIRENDERER_API __declspec(dllexport)
	#else
	#define DIRECTX81_GUIRENDERER_API __declspec(dllimport)
	#endif
#else
	#define DIRECTX81_GUIRENDERER_API
#endif

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include <d3d8.h>
#include <list>
#include <set>


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Forward refs
*************************************************************************/
class DirectX81Texture;

/*!
\brief
	Renderer class to interface with Microsoft DirectX 8.1
*/
class DIRECTX81_GUIRENDERER_API DirectX81Renderer : public Renderer
{
public:
	/*!
	\brief
		Constructor for Direct3D 8.1 Renderer object

	\param device
		Pointer to the IDirect3DDevice8 interface object that will be used for all rendering

	\param max_quads
		Obsolete.  Set to 0.
	*/
	DirectX81Renderer(LPDIRECT3DDEVICE8 device, uint max_quads = 0);


	/*!
	\brief
		Constructor for Direct3D 8.1 Renderer object
		
	\param device
		Pointer to the IDirect3DDevice8 interface object that will be used for all rendering

	\param sz
		Size object describing the initial size of the display (the dimensions should be >0)
	*/
	DirectX81Renderer(LPDIRECT3DDEVICE8 device, const Size& sz);


	/*!
	\brief
		Destructor for DirectX81Renderer objects
	*/
	virtual ~DirectX81Renderer(void);

	// add's a quad to the list to be rendered
	virtual	void	addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode);

	// perform final rendering for all queued renderable quads.
	virtual	void	doRender(void);

	// clear the queue
	virtual	void	clearRenderList(void);


	/*!
	\brief
		Enable or disable the queueing of quads from this point on.

		This only affects queueing.  If queueing is turned off, any calls to addQuad will cause the quad to be rendered directly.  Note that
		disabling queueing will not cause currently queued quads to be rendered, nor is the queue cleared - at any time the queue can still
		be drawn by calling doRender, and the list can be cleared by calling clearRenderList.  Re-enabling the queue causes subsequent quads
		to be added as if queueing had never been disabled.

	\param setting
		true to enable queueing, or false to disable queueing (see notes above).

	\return
		Nothing
	*/
	virtual void	setQueueingEnabled(bool setting)		{d_queueing = setting;}


	// create an empty texture
	virtual	Texture*	createTexture(void);

	// create a texture and load it with the specified file.
	virtual	Texture*	createTexture(const String& filename, const String& resourceGroup);

	// create a texture and set it to the specified size
	virtual	Texture*	createTexture(float size);

	// destroy the given texture
	virtual	void		destroyTexture(Texture* texture);

	// destroy all textures still active
	virtual void		destroyAllTextures(void);

	// return ptr to device
	LPDIRECT3DDEVICE8	getDevice(void) const		{return d_device;}


	/*!
	\brief
		Return whether queueing is enabled.

	\return
		true if queueing is enabled, false if queueing is disabled.
	*/
	virtual bool	isQueueingEnabled(void) const	{return d_queueing;}


	/*!
	\brief
	Return the current width of the display in pixels

	\return
	float value equal to the current width of the display in pixels.
	*/
	virtual float	getWidth(void) const		{return d_display_area.getWidth();}


	/*!
	\brief
	Return the current height of the display in pixels

	\return
	float value equal to the current height of the display in pixels.
	*/
	virtual float	getHeight(void) const		{return d_display_area.getHeight();}


	/*!
	\brief
	Return the size of the display in pixels

	\return
	Size object describing the dimensions of the current display.
	*/
	virtual Size	getSize(void) const			{return d_display_area.getSize();}


	/*!
	\brief
	Return a Rect describing the screen

	\return
	A Rect object that describes the screen area.  Typically, the top-left values are always 0, and the size of the area described is
	equal to the screen resolution.
	*/
	virtual Rect	getRect(void) const			{return d_display_area;}


	/*!
	\brief
		Return the maximum texture size available

	\return
		Size of the maximum supported texture in pixels (textures are always assumed to be square)
	*/
	virtual	uint	getMaxTextureSize(void) const		{return d_maxTextureSize;}


	/*!
	\brief
		Return the horizontal display resolution dpi

	\return
		horizontal resolution of the display in dpi.
	*/
	virtual	uint	getHorzScreenDPI(void) const	{return 96;}


	/*!
	\brief
		Return the vertical display resolution dpi

	\return
		vertical resolution of the display in dpi.
	*/
	virtual	uint	getVertScreenDPI(void) const	{return 96;}


	/*!
	\brief
		Direct3D support method that must be called prior to a Reset call on the
		Direct3DDevice; this is required so that the GUI renderer can release any
		unmanaged D3D resources as needed for the device reset to succeed.
	*/
	virtual	void	preD3DReset(void);


	/*!
	\brief
		Direct3D support method that must be called after a Reset call on the
		Direct3DDevice; this is required so that the GUI renderer can rebuild any
		unmanaged D3D resources after the device has been reset.
	*/
	virtual	void	postD3DReset(void);


	/*!
	\brief
		Set the size of the display in pixels.

		This method is important if your D3D 8 device is a pure device; since
		with a pure device the system is unable to determine the size of the
		view port, so you must manually tell it the size by using this method.

	\note
		This method will cause the EventDisplaySizeChanged event to fire if the
		display size has changed.

	\param sz
		Size object describing the size of the display.

	\return
		Nothing.
	*/
	void	setDisplaySize(const Size& sz);


private:
	/************************************************************************
		Implementation Constants
	************************************************************************/
	static const int			VERTEX_PER_QUAD;							//!< number of vertices per quad
	static const int			VERTEX_PER_TRIANGLE;						//!< number of vertices for a triangle
	static const int			VERTEXBUFFER_CAPACITY;						//!< capacity of the allocated vertex buffer
	static const ulong			VERTEX_FVF;									//!< FVF specifier constant

	/*************************************************************************
	    Implementation Structs & classes
	*************************************************************************/
	/*!
	\brief
		FVF structure used for all vertices.
	*/
	struct QuadVertex {
		FLOAT x, y, z, rhw;		//!< The transformed position for the vertex.
		DWORD diffuse;			//!< colour of the vertex
		float tu1, tv1;			//!< texture coordinates
	};

	/*!
	\brief
		structure holding details about a quad to be drawn
	*/
	struct QuadInfo
	{
		LPDIRECT3DTEXTURE8	texture;
		Rect				position;
		float				z;
		Rect				texPosition;
		ulong				topLeftCol;
		ulong				topRightCol;
		ulong				bottomLeftCol;
		ulong				bottomRightCol;

        QuadSplitMode       splitMode;

		bool operator<(const QuadInfo& other) const
		{
			// this is intentionally reversed.
			return z > other.z;
		}
	};


	/*************************************************************************
	    Implementation Methods
	*************************************************************************/
	// setup states etc
	void	initPerFrameStates(void);

	// renders whatever is in the vertex buffer
	void	renderVBuffer(void);

	// sort quads list according to texture
	void	sortQuads(void);

	// render a quad directly to the display
	void	renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode);

	// return size of device view port (if possible)
	Size	getViewportSize(void);

	// method to do work of constructor
	void	constructor_impl(LPDIRECT3DDEVICE8 device, const Size& display_size);


	/*************************************************************************
	    Implementation Data
	*************************************************************************/
	Rect				d_display_area;

	typedef std::multiset<QuadInfo>		QuadList;
	QuadList d_quadlist;
	bool	d_queueing;		//!< setting for queueing control.

	LPDIRECT3DDEVICE8		d_device;			//!< Base Direct3DDevice8 interface that we use for rendering
	LPDIRECT3DTEXTURE8		d_currTexture;		//!< currently set texture;
	LPDIRECT3DVERTEXBUFFER8	d_buffer;			//!< vertex buffer to queue sprite rendering
	int						d_bufferPos;		//!< index into buffer where next vertex should be put.

	std::list<DirectX81Texture*>	d_texturelist;		//!< List used to track textures.

	uint	d_maxTextureSize;		//!< Holds maximum supported texture size (in pixels).
	bool	d_canGetVPSize;			//!< true if we should try to extract the view port size ourselves.
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _DirectX81GUIRenderer_h_
