/***********************************************************************
	filename:	openglrenderer.h
	created:	9/4/2004
	author:		Mark Strom
				mwstrom@gmail.com

	purpose:	Interface to Renderer implemented via Opengl
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
#ifndef _openglrenderer_h_
#define _openglrenderer_h_

#include "CEGUIBase.h"

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef OPENGL_GUIRENDERER_EXPORTS
#       define OPENGL_GUIRENDERER_API __declspec(dllexport)
#   else
#       define OPENGL_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define OPENGL_GUIRENDERER_API
#endif


#if defined(_WIN32)//  All this taken from glut.h
#  ifndef APIENTRY
#   define GLUT_APIENTRY_DEFINED
#   if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__) || defined(__GNUC__)
#    define APIENTRY    __stdcall
#   else
#    define APIENTRY
#   endif
#  endif
/* XXX This is from Win32's <winnt.h> */
#  ifndef CALLBACK
#   if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS) || defined(__LCC__) || defined(__GNUC__)
#    define CALLBACK __stdcall
#   else
#    define CALLBACK
#   endif
#  endif
/* XXX Hack for lcc compiler.  It doesn't support __declspec(dllimport), just __stdcall. */
#  if defined( __LCC__ )
#   undef WINGDIAPI
#   define WINGDIAPI __stdcall
#  else
/* XXX This is from Win32's <wingdi.h> and <winnt.h> */
#   ifndef WINGDIAPI
#    define GLUT_WINGDIAPI_DEFINED
#    define WINGDIAPI __declspec(dllimport)
#   endif
#  endif
/* XXX This is from Win32's <ctype.h> */
#  ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#   define _WCHAR_T_DEFINED
#  endif
# endif //win32 end glut.h stuff

/* XXX Hack for finding headers in Apple's OpenGL framework. */
#if defined( __APPLE__ )
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else /* __APPLE__ */
#include <GL/gl.h>
#include <GL/glu.h>
#endif /* __APPLE__ */
#include <list>
#include <set>

#include "CEGUIRenderer.h"
#include "CEGUITexture.h"


// #if defined(_WIN32)
// #  if defined(_DEBUG)
// #     pragma comment(lib, "CEGUIBase_d.lib")
// #  else
// #     pragma comment(lib, "CEGUIBase.lib")
// #  endif
// #endif

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

#define OGLRENDERER_VBUFF_CAPACITY	4096


// Start of CEGUI namespace section
namespace CEGUI
{
  /*************************************************************************
	Forward refs
  *************************************************************************/
  class OpenGLTexture;
  class ImageCodec;
  class DynamicModule;
  
  /*!
    \brief
    Renderer class to interface with OpenGL
  */
  class OPENGL_GUIRENDERER_API OpenGLRenderer : public Renderer
  {
  public:
    /*!
      \brief
      Constructor for OpenGL Renderer object

      \param max_quads
      obsolete.  Set to 0.
      
      \param codec
      A pointer to a user provided image codec. The renderer
      does not take ownership of the codec object. 
    */
    OpenGLRenderer(uint max_quads, ImageCodec* codec = 0);
    /*!
      \brief
      Constructor for OpenGL Renderer object 
      
      \param max_quads
      obsolete. Set to 0.
      
      \param width
      width of the CEGUI viewport.
      
      \param height 
      height of the CEGUI viewport.

      \param codec
      A pointer to a user provided image codec. The renderer
      does not take ownership of the codec object. 
    */
    OpenGLRenderer(uint max_quads,int width, int height, ImageCodec* codec  = 0);


    /*!
      \brief
      Destructor for OpenGLRenderer objects
    */
    virtual ~OpenGLRenderer(void);

    // add's a quad to the list to be rendered
    virtual	void	addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode);

    // perform final rendering for all queued renderable quads.
    virtual	void	doRender(void);

    // clear the queue
    virtual	void	clearRenderList(void);


    /*!
      \brief
      Enable or disable the queuing of quads from this point on.

      This only affects queuing.  If queuing is turned off, any calls to addQuad will cause the quad to be rendered directly.  Note that
      disabling queuing will not cause currently queued quads to be rendered, nor is the queue cleared - at any time the queue can still
      be drawn by calling doRender, and the list can be cleared by calling clearRenderList.  Re-enabling the queue causes subsequent quads
      to be added as if queuing had never been disabled.

      \param setting
      true to enable queuing, or false to disable queuing (see notes above).

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

    /*!
      \brief
      Return whether queuing is enabled.

      \return
      true if queuing is enabled, false if queuing is disabled.
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
      Set the size of the display in pixels.

      If your viewport size changes, you can call this function with the new size
      in pixels to update the rendering area.

      \note
      This method will cause the EventDisplaySizeChanged event to fire if the
      display size has changed.

      \param sz
      Size object describing the size of the display.

      \return
      Nothing.
    */
    void	setDisplaySize(const Size& sz);


    /*!
      \brief
      Grabs all the loaded textures from Texture RAM and stores them in a local data buffer.
      This function invalidates all textures, and restoreTextures must be called before any
      CEGUI rendering is done for predictable results
    */
    void grabTextures(void);


    /*!
      \brief
      Restores all the loaded textures from the local data buffers previously created by 'grabTextures'
    */
    void restoreTextures(void);

    /*! 
      \brief 
      Retrieve the image codec used internaly 
    */
    ImageCodec& getImageCodec(void);
    
       
    /*! 
      \brief 
      Set the image codec to use for loading textures 
    */
    void setImageCodec(const String& codecName);

    /*!
     \brief  
     Set the image codec to use from an existing image codec.
     
     In this case the renderer does not take the ownership of the 
     image codec object. 

     \param codec a pointer to an image codec object 
     */
    void setImageCodec(ImageCodec* codec);
     
     
    /*!
      \brief 
      Set the name of the default image codec to be used 
    */
    static void setDefaultImageCodecName(const String& codecName);
    
    /*!
      \brief 
      Get the name of the default image codec 
    */
    static const String& getDefaultImageCodecName();
    

  private:
    /************************************************************************
		Implementation Constants
    ************************************************************************/
    static const int			VERTEX_PER_QUAD;							//!< number of vertices per quad
    static const int			VERTEX_PER_TRIANGLE;						//!< number of vertices for a triangle
    static const int			VERTEXBUFFER_CAPACITY;						//!< capacity of the allocated vertex buffer
    
    /*************************************************************************
		Implementation Structs & classes
    *************************************************************************/
    struct MyQuad
    {
      float tex[2];
      uint32 color;
      float vertex[3];
    };

    /*!
      \brief
      structure holding details about a quad to be drawn
    */
    struct QuadInfo
    {
      GLuint		texid;  
      Rect		position;
      float		z;
      Rect		texPosition;
      uint32		topLeftCol;
      uint32		topRightCol;
      uint32		bottomLeftCol;
      uint32		bottomRightCol;

      QuadSplitMode   splitMode;

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

    // restore states
    void  exitPerFrameStates(void);

    // render whatever is in the vertex buffer
    void	renderVBuffer(void);

    // sort quads list according to texture
    void	sortQuads(void);

    // render a quad directly to the display
    void	renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode);

    // convert colour value to whatever the OpenGL system is expecting.
    uint32	colourToOGL(const colour& col) const;

    // set the module ID string
    void setModuleIdentifierString();
  
    // setup image codec 
    void setupImageCodec(const String& codecName);
    
    // cleanup image codec 
    void cleanupImageCodec();
    
    /*************************************************************************
	    Implementation Data
    *************************************************************************/
    typedef std::multiset<QuadInfo>		QuadList;
    QuadList d_quadlist;

    Rect		d_display_area;

    MyQuad		myBuff[OGLRENDERER_VBUFF_CAPACITY];

    bool		d_queueing;			//!< setting for queuing control.
    uint		d_currTexture;		//!< Currently bound texture.
    int			d_bufferPos;		//!< index into buffer where next vertex should be put.
    bool		d_sorted;			//!< true when data in quad list is sorted.

    std::list<OpenGLTexture*>	d_texturelist;		//!< List used to track textures.
    GLint       d_maxTextureSize;		//!< Holds maximum supported texture size (in pixels).
    
    ImageCodec* d_imageCodec;           //!< Holds a pointer to the image codec to use.
    DynamicModule* d_imageCodecModule; //!< Holds a pointer to the image codec module. If d_imageCodecModule is 0 we are not owner of the image codec object 
    
    static String d_defaultImageCodecName; //!< Holds the name of the default codec to use 
    
    
  };

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _openglrenderer_h_
