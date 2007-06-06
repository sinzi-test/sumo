/****************************************************************************/
/// @file    RORunningVehicle.cpp
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// A vehicle that has an initial speed (and possibly position)
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


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif
#include <string>
#include <iostream>
#include "ROVehicle.h"
#include "ROVehicleType.h"
#include "RORunningVehicle.h"
#include "RORouteDef.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// used namespaces
// ===========================================================================
using namespace std;


// ===========================================================================
// method definitions
// ===========================================================================
RORunningVehicle::RORunningVehicle(ROVehicleBuilder &vb,
                                   const std::string &id, RORouteDef *route,
                                   SUMOTime time, ROVehicleType *type,
                                   const std::string &lane, SUMOReal pos,
                                   SUMOReal speed,
                                   const RGBColor &col,
                                   int period, int repNo)
        : ROVehicle(vb, id, route, time, type, col, period, repNo),
        _lane(lane), _pos(pos), _speed(speed)
{}


RORunningVehicle::~RORunningVehicle()
{}



void
RORunningVehicle::xmlOut(std::ostream &os) const
{
    os << "<vehicle id=\"" << myID << "\"";
    os << " type=\"" << myType->getID() << "\"";
    os << " route=\"" << myRoute->getID() << "\"";
    os << " depart=\"" << myDepartTime << "\"";
    os << " lane=\"" << _lane << "\"";
    os << " pos=\"" << _pos << "\"";
    os << " speed=\"" << _speed << "\"";
    os << "/>" << endl;
}


ROVehicle *
RORunningVehicle::copy(ROVehicleBuilder &vb,
                       const std::string &id, unsigned int depTime,
                       RORouteDef *newRoute)
{
    return new RORunningVehicle(vb, id, newRoute, depTime, myType, _lane, _pos,
                                _speed, myColor, myRepetitionPeriod, myRepetitionNumber);
}



/****************************************************************************/

