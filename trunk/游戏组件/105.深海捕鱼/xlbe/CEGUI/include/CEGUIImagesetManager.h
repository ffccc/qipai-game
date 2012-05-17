/***********************************************************************
	filename: 	CEGUIImagesetManager.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for ImagesetManager object
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
#ifndef _CEGUIImagesetManager_h_
#define _CEGUIImagesetManager_h_

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
	Class providing a shared library of Imageset objects to the system.

	The ImagesetManager is used to create, access, and destroy Imageset objects.  The idea is that
	the ImagesetManager will function as a central repository for imagery used within the GUI system,
	and that such imagery can be accessed, via a unique name, by any interested party within the system.
*/
class CEGUIEXPORT ImagesetManager : public Singleton<ImagesetManager>
{
public:
	/*!
	\brief
		Constructor for ImagesetManager objects
	*/
	ImagesetManager(void);


	/*!
	\brief
		Destructor for ImagesetManager objects
	*/
	~ImagesetManager(void);


	/*!
	\brief
		Create a Imageset object with the given name and Texture

		The created Imageset will be of limited use, and will require one or more images to be defined for the set.

	\param name
		String object containing the unique name for the Imageset being created.

	\param texture
		Texture object to be associated with the Imageset

	\return
		Pointer to the newly created Imageset object

	\exception AlreadyExistsException	Thrown if an Imageset named \a name is already present in the system.
	*/
	Imageset*	createImageset(const String& name, Texture* texture);

	/*!
	\brief
		Create an Imageset object from the specified file

	\param filename
		String object holding the name of the Imageset definition file which should be used to create the Imageset

    \param resourceGroup
        Resource group identifier to be passed to the resource manager.  NB: This affects the
        imageset xml file only, the texture loaded may have its own group specified in the XML file.

	\return
		Pointer to the newly created Imageset object

	\exception	AlreadyExistsException	Thrown if an Imageset named \a name is already present in the system.
	\exception	FileIOException			Thrown if something goes wrong while processing the file \a filename.
	*/
	Imageset*	createImageset(const String& filename, const String& resourceGroup = "");


    /*!
    \brief
        Create an Imageset object from the specified image file.  The Imageset will initially have a single
        image defined named "full_image" which is an image that represents the entire area of the loaded image.

    \param name
        String object containing the unique name for the Imageset being created.

    \param filename
        String object holding the name of the image file to be loaded.

    \param resourceGroup
        Resource group identifier to be passed to the resource manager when loading the image file.

    \return
        Pointer to the newly created Imageset object

    \exception	AlreadyExistsException	Thrown if an Imageset named \a name is already present in the system.
    \exception	FileIOException			Thrown if something goes wrong while reading the image file \a filename.
    */
    Imageset* createImagesetFromImageFile(const String& name, const String& filename, const String& resourceGroup = "");


	/*!
	\brief
		Destroys the Imageset with the specified name

	\param name
		String object containing the name of the Imageset to be destroyed.  If no such Imageset exists, nothing happens.

	\return
		Nothing.
	*/
	void	destroyImageset(const String& name);

	/*!
	\brief
		Destroys the given Imageset object

	\param imageset
		Pointer to the Imageset to be destroyed.  If no such Imageset exists, nothing happens.

	\return
		Nothing.
	*/
	void	destroyImageset(Imageset* imageset);


	/*!
	\brief
		Destroys all Imageset objects registered in the system

	\return
		Nothing
	*/
	void	destroyAllImagesets(void);


	/*!
	\brief
		Returns a pointer to the Imageset object with the specified name

	\param name
		String object containing the name of the Imageset to return a pointer to

	\return
		Pointer to the requested Imageset object

	\exception	UnknownObjectException	Thrown if no Imageset named \a name is present in within the system
	*/
	Imageset*	getImageset(const String& name) const;


	/*!
	\brief
		Check for the existence of a named Imageset

	\param name
		String object containing the name of the Imageset to look for

	\return
		true if an Imageset named \a name is presently loaded in the system, else false.
	*/
	bool	isImagesetPresent(const String& name) const			{return d_imagesets.find(name) != d_imagesets.end();}


	/*!
	\brief
		Notify the ImagesetManager of the current (usually new) display resolution.

	\param size
		Size object describing the display resolution

	\return
		Nothing
	*/
	void	notifyScreenResolution(const Size& size);


    /*!
    \brief
        Writes a full XML imageset for the specified Imageset to the given OutStream.

    \param imageset
        String holding the name of the Imageset to be written to the stream.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \return
        Nothing.
    */
    void writeImagesetToStream(const String& imageset, OutStream& out_stream) const;


private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	typedef	std::map<String, Imageset*, String::FastLessCompare>		ImagesetRegistry;
	ImagesetRegistry	d_imagesets;

public:
	/*************************************************************************
		Iterator stuff
	*************************************************************************/
	typedef	ConstBaseIterator<ImagesetRegistry>	ImagesetIterator;

	/*!
	\brief
		Return a ImagesetManager::ImagesetIterator object to iterate over the available Imageset objects.
	*/
	ImagesetIterator	getIterator(void) const;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIImageSetManager_h_
