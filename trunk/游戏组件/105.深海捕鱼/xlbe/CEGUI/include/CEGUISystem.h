/***********************************************************************
	filename: 	CEGUISystem.h
	created:	20/2/2004
	author:		Paul D Turner

	purpose:	Defines interface for main GUI system class
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
#ifndef _CEGUISystem_h_
#define _CEGUISystem_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIRenderer.h"
#include "CEGUIMouseCursor.h"
#include "CEGUIInputEvent.h"
#include "CEGUIResourceProvider.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
//! Implementation struct that tracks and controls multiclick for mouse buttons.
struct MouseClickTrackerImpl;


/*!
\brief
	The System class is the CEGUI class that provides access to all other elements in this system.

	This object must be created by the client application.  The System object requires that you pass it
	an initialised Renderer object which it can use to interface to whatever rendering system will be
	used to display the GUI imagery.
*/
class CEGUIEXPORT System : public Singleton<System>, public EventSet
{
public:
	static const String EventNamespace;				//!< Namespace for global events

	/*************************************************************************
		Constants
	*************************************************************************/
	static const double		DefaultSingleClickTimeout;		//!< Default timeout for generation of single click events.
	static const double		DefaultMultiClickTimeout;		//!< Default timeout for generation of multi-click events.
	static const Size		DefaultMultiClickAreaSize;		//!< Default allowable mouse movement for multi-click event generation.

	// event names
	static const String EventGUISheetChanged;				//!< Name of event fired whenever the GUI sheet is changed.
	static const String EventSingleClickTimeoutChanged;	//!< Name of event fired when the single-click timeout is changed.
	static const String EventMultiClickTimeoutChanged;	//!< Name of event fired when the multi-click timeout is changed.
	static const String EventMultiClickAreaSizeChanged;	//!< Name of event fired when the size of the multi-click tolerance area is changed.
	static const String EventDefaultFontChanged;			//!< Name of event fired when the default font changes.
	static const String EventDefaultMouseCursorChanged;	//!< Name of event fired when the default mouse cursor changes.
	static const String EventMouseMoveScalingChanged;		//!< Name of event fired when the mouse move scaling factor changes.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
    /*!
    \brief
        Construct a new System object

    \param renderer
        Pointer to the valid Renderer object that will be used to render GUI
        imagery.

    \param resourceProvider
        Pointer to a ResourceProvider object, or NULL to use whichever default
        the Renderer provides.

    \param xmlParser
        Pointer to a valid XMLParser object to be used when parsing XML files,
        or NULL to use a default parser.

    \param scriptModule
        Pointer to a ScriptModule object.  may be NULL for none.

    \param configFile
        String object containing the name of a configuration file to use.

    \param logFile
        String object containing the name to use for the log file.
    */
    System(Renderer* renderer, ResourceProvider* resourceProvider = 0, XMLParser* xmlParser = 0, ScriptModule* scriptModule = 0, const String& configFile = "", const String& logFile = "CEGUI.log");

	/*!
	\brief
		Destructor for System objects.
	*/
	~System(void);


	/*!
	\brief
		Return a pointer to the Renderer object being used by the system

	\return
		Pointer to the Renderer object used by the system.
	*/
	Renderer*	getRenderer(void) const			{return d_renderer;}


	/*!
	\brief
		Return singleton System object

	\return
		Singleton System object
	*/
	static	System&	getSingleton(void);


	/*!
	\brief
		Return pointer to singleton System object

	\return
		Pointer to singleton System object
	*/
	static	System*	getSingletonPtr(void);


	/*!
	\brief
		Set the default font to be used by the system

	\param name
		String object containing the name of the font to be used as the system default.

	\return
		Nothing.
	*/
	void	setDefaultFont(const String& name);


	/*!
	\brief
		Set the default font to be used by the system

	\param font
		Pointer to the font to be used as the system default.

	\return
		Nothing.
	*/
	void	setDefaultFont(Font* font);


	/*!
	\brief
		Return a pointer to the default Font for the GUI system

	\return
		Pointer to a Font object that is the default font in the system.
	*/
	Font*	getDefaultFont(void) const				{return d_defaultFont;}


	/*!
	\brief
		Causes a full re-draw next time renderGUI() is called

	\return
		Nothing
	*/
	void	signalRedraw()		{d_gui_redraw = true;}


	/*!
	\brief
		Return a boolean value to indicate whether a full re-draw is requested next time renderGUI() is called.

	\return
		true if a re-draw has been requested
	*/
	bool	isRedrawRequested() const		{return d_gui_redraw;}


	/*!
	\brief
		Render the GUI

		Depending upon the internal state, this may either re-use rendering from last time, or trigger a full re-draw from all elements.

	\return
		Nothing
	*/
	void	renderGUI(void);


	/*!
	\brief
		Set the active GUI sheet (root) window.

	\param sheet
		Pointer to a Window object that will become the new GUI 'root'

	\return
		Pointer to the window that was previously set as the GUI root.
	*/
	Window*	setGUISheet(Window* sheet);


	/*!
	\brief
		Return a pointer to the active GUI sheet (root) window.

	\return
		Pointer to the window object that has been set as the GUI root element.
	*/
	Window*	getGUISheet(void) const		{return d_activeSheet;}


	/*!
	\brief
		Return the current timeout for generation of single-click events.

		A single-click is defined here as a button being pressed and then released.

	\return
		double value equal to the current single-click timeout value.
	*/
	double	getSingleClickTimeout(void) const		{return d_click_timeout;}


	/*!
	\brief
		Return the current timeout for generation of multi-click events.

		A multi-click event is a double-click, or a triple-click.  The value returned
		here is the maximum allowable time between mouse button down events for which
		a multi-click event will be generated.

	\return
		double value equal to the current multi-click timeout value.
	*/
	double	getMultiClickTimeout(void) const		{return d_dblclick_timeout;}


	/*!
	\brief
		Return the size of the allowable mouse movement tolerance used when generating multi-click events.

		This size defines an area with the mouse at the centre.  The mouse must stay within the tolerance defined
		for a multi-click (double click, or triple click) event to be generated.

	\return
		Size object describing the current multi-click tolerance area size.
	*/
	const Size&	getMultiClickToleranceAreaSize(void) const		{return d_dblclick_size;}


	/*!
	\brief
		Set the timeout used for generation of single-click events.

		A single-click is defined here as a button being pressed and then
        released.

	\param timeout
		double value equal to the single-click timeout value to be used from now
        onwards.

    \note
        A timeout value of 0 indicates infinity and so no timeout occurrs; as
        long as the mouse is in the prescribed area, a mouse button 'clicked'
        event will therefore always be raised.

	\return
		Nothing.
	*/
	void	setSingleClickTimeout(double timeout);


	/*!
	\brief
		Set the timeout to be used for the generation of multi-click events.

		A multi-click event is a double-click, or a triple-click.  The value
        returned here is the maximum allowable time between mouse button down
        events for which a multi-click event will be generated.

	\param timeout
		double value equal to the multi-click timeout value to be used from now
        onwards.

    \note
        A timeout value of 0 indicates infinity and so no timeout occurrs; as
        long as the mouse is in the prescribed area, an appropriate mouse button
        event will therefore always be raised.

	\return
		Nothing.
	*/
	void setMultiClickTimeout(double timeout);


	/*!
	\brief
		Set the size of the allowable mouse movement tolerance used when generating multi-click events.

		This size defines an area with the mouse at the centre.  The mouse must stay within the tolerance defined
		for a multi-click (double click, or triple click) event to be generated.

	\param sz
		Size object describing the multi-click tolerance area size to be used.

	\return
		Nothing.
	*/
	void setMultiClickToleranceAreaSize(const Size&	sz);


	/*!
	\brief
		Return the currently set default mouse cursor image

	\return
		Pointer to the current default image used for the mouse cursor.  May return NULL if default cursor has not been set,
		or has intentionally been set to NULL - which results in a blank default cursor.
	*/
	const Image*	getDefaultMouseCursor(void) const	{return d_defaultMouseCursor;}


	/*!
	\brief
		Set the image to be used as the default mouse cursor.

	\param image
		Pointer to an image object that is to be used as the default mouse cursor.  To have no cursor rendered by default, you
		can specify NULL here.

	\return
		Nothing.
	*/
	void	setDefaultMouseCursor(const Image* image);


	/*!
	\brief
		Set the image to be used as the default mouse cursor.

	\param image
		One of the MouseCursorImage enumerated values.

	\return
		Nothing.
	*/
	void	setDefaultMouseCursor(MouseCursorImage image)		{setDefaultMouseCursor((const Image*)image);}


	/*!
	\brief
		Set the image to be used as the default mouse cursor.

	\param imageset
		String object that contains the name of the Imageset  that contains the image to be used.

	\param image_name
		String object that contains the name of the Image on \a imageset that is to be used.

	\return
		Nothing.

	\exception UnknownObjectException	thrown if \a imageset is not known, or if \a imageset contains no Image named \a image_name.
	*/
	void	setDefaultMouseCursor(const String& imageset, const String& image_name);


	/*!
	\brief
		Return the Window object that the mouse is presently within

	\return
		Pointer to the Window object that currently contains the mouse cursor, or NULL if none.
	*/
	Window*	getWindowContainingMouse(void) const	{return d_wndWithMouse;}


	/*!
	\brief
		Return a pointer to the ScriptModule being used for scripting within the GUI system.

	\return
		Pointer to a ScriptModule based object.
	*/
	ScriptModule*	getScriptingModule(void) const;

    /*!
    \brief
        Set the ScriptModule to be used for scripting within the GUI system.

    \param scriptModule
        Pointer to a ScriptModule based object, or 0 for none (be careful!)

    \return
        Nothing
    */
    void setScriptingModule(ScriptModule* scriptModule);

	/*!
	\brief
		Return a pointer to the ResourceProvider being used within the GUI system.

	\return
		Pointer to a ResourceProvider based object.
	*/
	ResourceProvider* getResourceProvider(void) const;

	/*!
	\brief
		Execute a script file if possible.

	\param filename
		String object holding the filename of the script file that is to be executed

	\param resourceGroup
		Resource group identifier to be passed to the ResourceProvider when loading the script file.
	*/
	void	executeScriptFile(const String& filename, const String& resourceGroup = "") const;


	/*!
	\brief
		Execute a scripted global function if possible.  The function should not take any parameters and should return an integer.

	\param function_name
		String object holding the name of the function, in the global script environment, that
		is to be executed.

	\return
		The integer value returned from the script function.
	*/
	int		executeScriptGlobal(const String& function_name) const;


    /*!
    \brief
        If possible, execute script code contained in the given CEGUI::String object.

    \param str
        String object holding the valid script code that should be executed.

    \return
        Nothing.
    */
    void executeScriptString(const String& str) const;


    /*!
	\brief
		return the current mouse movement scaling factor.

	\return
		float value that is equal to the currently set mouse movement scaling factor.  Defaults to 1.0f.
	*/
	float	getMouseMoveScaling(void) const;


	/*!
	\brief
		Set the current mouse movement scaling factor

	\param scaling
		float value specifying the scaling to be applied to mouse movement inputs.

	\return
		nothing.
	*/
	void	setMouseMoveScaling(float scaling);


	/*!
	\brief
		Internal method used to inform the System object whenever a window is destroyed, so that System can perform any required
		housekeeping.

	\note
		This method is not intended for client code usage.  If you use this method anything can, and probably will, go wrong!
	*/
	void	notifyWindowDestroyed(const Window* window);


    /*!
    \brief
        Return the current system keys value.

    \return
        uint value representing a combination of the SystemKey bits.
    */
    uint    getSystemKeys(void) const   { return d_sysKeys; }

    /*!
    \brief
        Set a new XML parser module to be used.

        The current XMLParser will be cleaned up and, if owned by the system,
        also deleted, as will any dynamically loaded module associated with the
        XMLParser object.  The newly created XMLParser object, and the
        associated module will be owned by the system.

    \param parserName
        String object describing the name of the XML parser module to be used.
    */
    void setXMLParser(const String& parserName);

    /*!
    \brief
        Sets the XMLParser object to be used by the system.

        The current XMLParser will be cleaned up and, if owned by the system,
        also deleted, as will any dynamically loaded module associated with the
        XMLParser object.

        If the argument passed in the \a parser parameter is 0, the system will
        cleanup any existing parser as described above, and revert to using
        the parser provided by the default module (see getDefaultXMLParserName
        and setDefaultXMLParserName).

    \param parser
        Pointer to the XMLParser object to be used by the system, or 0 to cause
        the system to initialise a default parser.
    */
    void setXMLParser(XMLParser* parser);

    /*!
    \brief
        Return the XMLParser object.
     */
    XMLParser* getXMLParser(void) const     { return d_xmlParser; }


    /*!
    \brief
        Set the system default Tooltip object.  This value may be NULL to indicate that no default Tooltip will be
        available.

    \param tooltip
        Pointer to a valid Tooltip based object which should be used as the default tooltip for the system, or NULL to
        indicate that no system default tooltip is required.  Note that when passing a pointer to a Tooltip object,
        ownership of the Tooltip does not pass to System.

    \return
        Nothing.
    */
    void setDefaultTooltip(Tooltip* tooltip);

    /*!
    \brief
        Set the system default Tooltip to be used by specifying a Window type.

        System will internally attempt to create an instance of the specified window type (which must be
        derived from the base Tooltip class).  If the Tooltip creation fails, the error is logged and no
        system default Tooltip will be available.

    \param tooltipType
        String object holding the name of the Tooltip based Window type which should be used as the Tooltip for
        the system default.

    \return
        Nothing.
    */
    void setDefaultTooltip(const String& tooltipType);

    /*!
    \brief
        return a poiter to the system default tooltip.  May return 0.

    \return
        Pointer to the current system default tooltip.  May return 0 if
        no system default tooltip is available.
     */
    Tooltip* getDefaultTooltip(void) const  { return d_defaultTooltip; }


	/*!
	\brief
		Internal method to directly set the current modal target.

	\note
		This method is called internally by Window, and must be used by client code.
		Doing so will most likely not have the expected results.
	*/
	void setModalTarget(Window* target)		{d_modalTarget = target;}

	/*!
	\brief
		Return a pointer to the Window that is currently the modal target.

	\return
		Pointer to the current modal target. NULL if there is no modal target.
	*/
	Window* getModalTarget(void) const		{return d_modalTarget;}

    /*!
    \brief
        Static member to set the name of the default XML parser module that
        should be used.

        If you want to modify the default parser from the one compiled in, you
        need to call this static member prior to instantiating the main
        CEGUI::System object.

        Note that calling this member to change the name of the default module
        after CEGUI::System, and therefore the default xml parser, has been
        created will have no real effect - the default parser name will be
        updated, though no actual changes to the xml parser module will occur.

        The built-in options for this are:
         - XercesParser
         - ExpatParser
         - LibxmlParser
         - TinyXMLParser

        Whether these are actually available, depends upon how you built the
        system.  If you have some custom parser, you can provide the name of
        that here to have it used as the default, though note that the
        final filename of the parser module should be of the form:

        [prefix]CEGUI[parserName][suffix]

        where:
        - [prefix] is some optional prefix; like 'lib' on linux.
        - CEGUI is a required prefix.
        - [parserName] is the name of the parser, as supplied to this function.
        - [suffix] is the filename suffix, like .dll or .so

        Final module filenames are, thus, of the form:
        - CEGUIXercesParser.dll
        - libCEGUIXercesParser.so

    \param parserName
        String describing the name of the xml parser module to be used as the
        default.

    \return
        Nothing.
    */
    static void setDefaultXMLParserName(const String& parserName);

    /*!
    \brief
        Return the name of the currently set default xml parser module.

    \return
        String holding the currently set default xml parser name.  Note that if
        this name has been changed after instantiating the system, the name
        returned may not actually correspond to the module in use.
    */
    static const String getDefaultXMLParserName();
    
    /*!
    \brief
        Perform updates with regards to the window that contains the mouse
        cursor, firing any required MouseEnters / MouseLeaves events.
    
    \note
        The CEGUI system components call this member as a matter of course,
        in most cases there will be no need for user / client code to call this
        member directly.
    
    \return
        - true if the window containing the mouse had changed.
        - false if the window containing the mouse had not changed.
    */
    bool updateWindowContainingMouse();

	/*************************************************************************
		Input injection interface
	*************************************************************************/
	/*!
	\brief
		Method that injects a mouse movement event into the system

	\param delta_x
		amount the mouse moved on the x axis.

	\param delta_y
		amount the mouse moved on the y axis.

	\return
		- true if the input was processed by the gui system.
		- false if the input was not processed by the gui system.
	*/
	bool	injectMouseMove(float delta_x, float delta_y);


	/*!
	\brief
		Method that injects that the mouse has left the application window

	\return
		- true if the generated mouse move event was handled.
		- false if the generated mouse move event was not handled.
	*/
	bool	injectMouseLeaves(void);


	/*!
	\brief
		Method that injects a mouse button down event into the system.

	\param button
		One of the MouseButton values indicating which button was pressed.

	\return
		- true if the input was processed by the gui system.
		- false if the input was not processed by the gui system.
	*/
	bool	injectMouseButtonDown(MouseButton button);


	/*!
	\brief
		Method that injects a mouse button up event into the system.

	\param button
		One of the MouseButton values indicating which button was released.

	\return
		- true if the input was processed by the gui system.
		- false if the input was not processed by the gui system.
	*/
	bool	injectMouseButtonUp(MouseButton button);


	/*!
	\brief
		Method that injects a key down event into the system.

	\param key_code
		uint value indicating which key was pressed.

	\return
		- true if the input was processed by the gui system.
		- false if the input was not processed by the gui system.
	*/
	bool	injectKeyDown(uint key_code);


	/*!
	\brief
		Method that injects a key up event into the system.

	\param key_code
		uint value indicating which key was released.

	\return
		- true if the input was processed by the gui system.
		- false if the input was not processed by the gui system.
	*/
	bool	injectKeyUp(uint key_code);


	/*!
	\brief
		Method that injects a typed character event into the system.

	\param code_point
		Unicode code point of the character that was typed.

	\return
		- true if the input was processed by the gui system.
		- false if the input was not processed by the gui system.
	*/
	bool	injectChar(utf32 code_point);


	/*!
	\brief
		Method that injects a mouse-wheel / scroll-wheel event into the system.

	\param delta
		float value representing the amount the wheel moved.

	\return
		- true if the input was processed by the gui system.
		- false if the input was not processed by the gui system.
	*/
	bool	injectMouseWheelChange(float delta);


	/*!
	\brief
		Method that injects a new position for the mouse cursor.

	\param x_pos
		New absolute pixel position of the mouse cursor on the x axis.

	\param y_pos
		New absolute pixel position of the mouse cursoe in the y axis.

	\return
		- true if the generated mouse move event was handled.
		- false if the generated mouse move event was not handled.
	*/
	bool	injectMousePosition(float x_pos, float y_pos);


	/*!
	\brief
		Method to inject time pulses into the system.

	\param timeElapsed
		float value indicating the amount of time passed, in seconds, since the last time this method was called.

	\return
		Currently, this method always returns true.
	*/
	bool	injectTimePulse(float timeElapsed);


private:
    // unimplemented constructors / assignment
    System(const System& obj);
    System& operator=(const System& obj);

	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const char	CEGUIConfigSchemaName[];			//!< Filename of the XML schema used for validating Config files.


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Given Point \a pt, return a pointer to the Window that should receive a mouse input if \a pt is the mouse location.

	\param pt
		Point object describing a screen location in pixels.

	\return
		Pointer to a Window object that should receive mouse input with the system in its current state and the mouse at location \a pt.
	*/
	Window*	getTargetWindow(const Point& pt) const;


	/*!
	\brief
		Return a pointer to the Window that should receive keyboard input considering the current modal target.

	\return
		Pointer to a Window object that should receive the keyboard input.
	*/
	Window* getKeyboardTargetWindow(void) const;


	/*!
	\brief
		Return a pointer to the next window that is to receive the input if the given Window did not use it.

	\param w
		Pointer to the Window that just received the input.

	\return
		Pointer to the next window to receive the input.
	*/
	Window* getNextTargetWindow(Window* w) const;


	/*!
	\brief
		Translate a MouseButton value into the corresponding SystemKey value

	\param btn
		MouseButton value describing the value to be converted

	\return
		SystemKey value that corresponds to the same button as \a btn
	*/
	SystemKey	mouseButtonToSyskey(MouseButton btn) const;


	/*!
	\brief
		Translate a Key::Scan value into the corresponding SystemKey value.

		This takes key direction into account, since we map two keys onto one value.

	\param key
		Key::Scan value describing the value to be converted

	\param direction
		true if the key is being pressed, false if the key is being released.

	\return
		SystemKey value that corresponds to the same key as \a key, or 0 if key was not a system key.
	*/
	SystemKey	keyCodeToSyskey(Key::Scan key, bool direction);


	/*!
	\brief
		Handler method for display size change notifications
	*/
	bool	handleDisplaySizeChange(const EventArgs& e);

    //! output the standard log header
    void outputLogHeader();

    //! adds factories for all the basic window types
    void addStandardWindowFactories();

    //! create the other core system singleton objects (except the logger)
    void createSingletons();

    //! cleanup the core system singleton objects
    void destroySingletons();

    //! handle creation and initialisation of the XML parser.
    void setupXMLParser();

    //! handle cleanup of the XML parser
    void cleanupXMLParser();

    //! common function used for injection of mouse positions and movements
    bool mouseMoveInjection_impl(MouseEventArgs& ma);

	/*************************************************************************
		Handlers for System events
	*************************************************************************/
	/*!
	\brief
		Handler called when the main system GUI Sheet (or root window) is changed.

		\a e is a WindowEventArgs with 'window' set to the old root window.
	*/
	void	onGUISheetChanged(WindowEventArgs& e);


	/*!
	\brief
		Handler called when the single-click timeout value is changed.
	*/
	void	onSingleClickTimeoutChanged(EventArgs& e);


	/*!
	\brief
		Handler called when the multi-click timeout value is changed.
	*/
	void	onMultiClickTimeoutChanged(EventArgs& e);


	/*!
	\brief
		Handler called when the size of the multi-click tolerance area is changed.
	*/
	void	onMultiClickAreaSizeChanged(EventArgs& e);


	/*!
	\brief
		Handler called when the default system font is changed.
	*/
	void	onDefaultFontChanged(EventArgs& e);


	/*!
	\brief
		Handler called when the default system mouse cursor image is changed.
	*/
	void	onDefaultMouseCursorChanged(EventArgs& e);


	/*!
	\brief
		Handler called when the mouse movement scaling factor is changed.
	*/
	void	onMouseMoveScalingChanged(EventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Renderer*	d_renderer;			//!< Holds the pointer to the Renderer object given to us in the constructor
    ResourceProvider* d_resourceProvider;      //!< Holds the pointer to the ResourceProvider object given to us by the renderer or the System constructor.
	Font*		d_defaultFont;		//!< Holds a pointer to the default GUI font.
	bool		d_gui_redraw;		//!< True if GUI should be re-drawn, false if render should re-use last times queue.

	Window*		d_wndWithMouse;		//!< Pointer to the window that currently contains the mouse.
	Window*		d_activeSheet;		//!< The active GUI sheet (root window)
	Window*		d_modalTarget;		//!< Pointer to the window that is the current modal target. NULL is there is no modal target.

	String d_strVersion;    //!< CEGUI version

	uint		d_sysKeys;			//!< Current set of system keys pressed (in mk1 these were passed in, here we track these ourself).
	bool		d_lshift;			//!< Tracks state of left shift.
	bool		d_rshift;			//!< Tracks state of right shift.
	bool		d_lctrl;			//!< Tracks state of left control.
	bool		d_rctrl;			//!< Tracks state of right control.
	bool		d_lalt;				//!< Tracks state of left alt.
	bool		d_ralt;				//!< Tracks state of right alt.

	double		d_click_timeout;	//!< Timeout value, in seconds, used to generate a single-click (button down then up)
	double		d_dblclick_timeout;	//!< Timeout value, in seconds, used to generate multi-click events (botton down, then up, then down, and so on).
	Size		d_dblclick_size;	//!< Size of area the mouse can move and still make multi-clicks.

	MouseClickTrackerImpl* const	d_clickTrackerPimpl;		//!< Tracks mouse button click generation.

	// mouse cursor related
	const Image*	d_defaultMouseCursor;		//!< Image to be used as the default mouse cursor.

	// scripting
	ScriptModule*	d_scriptModule;			//!< Points to the scripting support module.
	String			d_termScriptName;		//!< Name of the script to run upon system shutdown.

	float	d_mouseScalingFactor;			//!< Scaling applied to mouse movement inputs.

    XMLParser*  d_xmlParser;        //!< XMLParser object we use to process xml files.
    bool        d_ourXmlParser;     //!< true when we created the xml parser.
    DynamicModule* d_parserModule;  //! pointer to parser module.

    Tooltip* d_defaultTooltip;      //!< System default tooltip object.
    bool     d_weOwnTooltip;        //!< true if System created the custom Tooltip.

    //!< Holds the connection to Renderer::EventDisplaySizeChanged so we can unsubscribe before we die.
    Event::Connection d_rendererCon;

    static String   d_defaultXMLParserName; //!< Holds name of default XMLParser
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUISystem_h_
