/***************************************************************************
                          MSJunctionControl.C  -  Coordinates
                          Junction-operations. 
                             -------------------
    begin                : Tue, 06 Mar 2001
    copyright            : (C) 2001 by ZAIK http://www.zaik.uni-koeln.de/AFS
    author               : Christian Roessel
    email                : roessel@zpr.uni-koeln.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

namespace 
{
    const char rcsid[] = 
    "$Id$";
} 

// $Log$
// Revision 1.1  2002/10/16 14:48:26  dkrajzew
// ROOT/sumo moved to ROOT/src
//
// Revision 1.1.1.1  2002/04/08 07:21:23  traffic
// new project name
//
// Revision 2.1  2002/02/21 18:45:17  croessel
// Method "printJunctions()" removed.
//
// Revision 2.0  2002/02/14 14:43:15  croessel
// Bringing all files to revision 2.0. This is just cosmetics.
//
// Revision 1.7  2002/02/05 13:51:51  croessel
// GPL-Notice included.
// In *.cpp files also config.h included.
//
// Revision 1.6  2001/12/20 12:01:17  croessel
// Default ctor, copy-ctor and assignment-operator removed.
//
// Revision 1.5  2001/11/21 15:12:22  croessel
// New loop in moveFirstVehicles() to fully integrate the moved vehicles
// into the succeeding lanes.
//
// Revision 1.4  2001/11/15 17:12:13  croessel
// Outcommented the inclusion of the inline *.iC files. Currently not
// needed.
//
// Revision 1.3  2001/11/14 15:47:34  croessel
// Merged the diffs between the .C and .cpp versions. Numerous changes
// in MSLane, MSVehicle and MSJunction.
//
// Revision 1.2  2001/11/14 10:49:06  croessel
// CR-line-end removed.
//
// Revision 1.1  2001/10/24 07:11:38  traffic
// new extension
//
// Revision 1.6  2001/10/23 09:30:40  traffic
// parser bugs removed
//
// Revision 1.5  2001/09/06 15:47:08  croessel
// Numerous changes during debugging session.
//
// Revision 1.4  2001/08/16 13:10:29  traffic
// minor MSVC++-problems solved
//
// Revision 1.3  2001/07/25 12:17:31  traffic
// CC problems with make_pair repaired
//
// Revision 1.2  2001/07/16 12:55:47  croessel
// Changed id type from unsigned int to string. Added string-pointer 
// dictionaries and dictionary methods.
//
// Revision 1.1.1.1  2001/07/11 15:51:13  traffic
// new start
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "MSJunctionControl.h"
#include "MSJunction.h"
#include <algorithm>

using namespace std;


// Init static member.
MSJunctionControl::DictType MSJunctionControl::myDict;


MSJunctionControl::MSJunctionControl(string id, JunctionCont* j) :
    myID(id), myJunctions(j)
{
}


MSJunctionControl::~MSJunctionControl()
{
}

// Why do we separate setting and collecting requests? Because
// main road vehicles may set requests at more than on
// junction. Before we collect, all requests must be set.

void 
MSJunctionControl::moveFirstVehicles()
{
    // Why do we need four loops? The problem is that first vehicles can
    // set requests at more than one junction. Therefore before setting
    // requests, all requests need to be cleared. And before moving all
    // requests must be set. And then, the vehicles have to be integrated
    // into their new lanes.
    for_each ( myJunctions->begin(), myJunctions->end(),
               mem_fun (& MSJunction::clearRequests ) );
    for_each ( myJunctions->begin(), myJunctions->end(),
               mem_fun (& MSJunction::setFirstVehiclesRequests ) );
    for_each ( myJunctions->begin(), myJunctions->end(),
               mem_fun (& MSJunction::moveFirstVehicles ) );
    for_each ( myJunctions->begin(), myJunctions->end(),
               mem_fun (& MSJunction::vehicles2targetLane ) );    
}


bool
MSJunctionControl::dictionary(string id, MSJunctionControl* ptr)
{
    DictType::iterator it = myDict.find(id);
    if (it == myDict.end()) {
        // id not in myDict.
        myDict.insert(DictType::value_type(id, ptr));
        return true;
    }
    return false;
}


MSJunctionControl*
MSJunctionControl::dictionary(string id)
{
    DictType::iterator it = myDict.find(id);
    if (it == myDict.end()) {
        // id not in myDict.
        return 0;
    }
    return it->second;
}

/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

//#ifdef DISABLE_INLINE
//#include "MSJunctionControl.iC"
//#endif

// Local Variables:
// mode:C++
// End:
