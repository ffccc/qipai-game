/***********************************************************************
	filename: 	CEGUISchemeManager.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines the interface to the SchemeManager class
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
#ifndef _CEGUISchemeManager_h_
#define _CEGUISchemeManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIIteratorBase.h"
#include <map>


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	A class that manages the creation of, access to, and destruction of GUI Scheme objects
*/
class CEGUIEXPORT SchemeManager : public Singleton<SchemeManager>
{
public:
	/*!
	\brief
		Constructor for SchemeManager objects
	*/
	SchemeManager(void);


	/*!
	\brief
		Destructor for SchemeManager objects
	*/
	~SchemeManager(void);


	/*!
	\brief
		Loads a scheme

	\param scheme_filename
		String object that holds the filename of the scheme to be loaded

    \param resourceGroup
        Resource group identifier to be passed to the resource manager.  NB: This
        affects loading of the scheme xml file only, scheme resources may specify
        their own groups.

	\return
		Pointer to an object representing the loaded Scheme.
	*/
	Scheme*	loadScheme(const String& scheme_filename, const String& resourceGroup = "");


	/*!
	\brief
		Unloads all data referenced in a scheme.  If any object is using some resource which is listed in the scheme, this function
		will effectively pull the rug out from under those objects.  This should be used with extreme caution, or not at all.

	\param scheme_name
		String object specifying the name of the Scheme to be unloaded.
	*/
	void	unloadScheme(const String& scheme_name);


	/*!
	\brief
		Returns true if the named Scheme is present in the system (though the resources for the scheme may or may not be loaded)

	\param scheme_name
		String object specifying the name of the Scheme to check for.

	\return
		true if the scheme is loaded, false if it is not.
	*/
	bool	isSchemePresent(const String& scheme_name) const		{return (d_schemes.find(scheme_name) != d_schemes.end());}


	/*!
	\brief
		Returns a pointer to the Scheme object with the specified name.

	\param name
		String object holding the name of the Scheme to be returned.

	\return
		Pointer to the Scheme named \a name.

	\exception UnknownObjectException	thrown if no Scheme named \a name is present in the system
	*/
	Scheme*	getScheme(const String& name) const;


	/*!
	\brief
		Unload all schemes currently defined within the system.

	\note
		Calling this method has the potential to be very dangerous; if any of the
		data that forms part of the scheme is still in use, you can expect
		fireworks shortly after!

	\return
		Nothing.
	*/
	void	unloadAllSchemes(void);

private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef	std::map<String, Scheme*, String::FastLessCompare> SchemeRegistry;
	SchemeRegistry	d_schemes;			//!< Collection that tracks the loaded Schemes.


public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	ConstBaseIterator<SchemeRegistry>	SchemeIterator;

	/*!
	\brief
		Return a SchemeManager::SchemeIterator object to iterate over the available schemes.
	*/
	SchemeIterator	getIterator(void) const;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUISchemeManager_h_
