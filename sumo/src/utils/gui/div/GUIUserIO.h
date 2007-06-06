/****************************************************************************/
/// @file    GUIUserIO.h
/// @author  Daniel Krajzewicz
/// @date    Sept 2006
/// @version $Id$
///
// Some OS-dependant functions to ease cliboard manipulation
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// copyright : (C) 2001-2007
//  by DLR (http://www.dlr.de/) and ZAIK (http://www.zaik.uni-koeln.de/AFS)
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef GUIUserIO_h
#define GUIUserIO_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <fx.h>


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GUIUserIO
 * @brief Some OS-dependant functions to ease cliboard manipulation
 */
class GUIUserIO
{
public:
    /** @brief Copies the given text to clipboard
     *
     * !!! This method is only implemented for MS Windows
     */
    static void copyToClipboard(FXApp *app, const std::string &text);

};


#endif

/****************************************************************************/

