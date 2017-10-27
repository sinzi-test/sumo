/****************************************************************************/
/// @file    MSCFModel_IDM.cpp
/// @author  Tobias Mayer
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @date    Thu, 03 Sep 2009
/// @version $Id$
///
// The Intelligent Driver Model (IDM) car-following model
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2001-2017 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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

#include <microsim/cfmodels/MSCFModel_IDM_plat.h>
#include <microsim/MSVehicle.h>
#include <microsim/MSLane.h>
#include <utils/common/RandHelper.h>
#include <utils/common/SUMOTime.h>


// ===========================================================================
// method definitions
// ===========================================================================
MSCFModel_IDM_plat::MSCFModel_IDM_plat(const MSVehicleType* vtype,
                             double accel, double decel, double emergencyDecel,
                             double headwayTime, double delta,
                             double internalStepping) : 
    MSCFModel(vtype, accel, decel, emergencyDecel, decel, headwayTime), myDelta(delta),
    myAdaptationFactor(1.), myAdaptationTime(0.),
    myIterations(MAX2(1, int(TS / internalStepping + .5))),
    myTwoSqrtAccelDecel(double(2 * sqrt(accel * decel))) {
}


MSCFModel_IDM_plat::MSCFModel_IDM_plat(const MSVehicleType* vtype,
                             double accel, double decel, double emergencyDecel,
                             double headwayTime,
                             double adaptationFactor, double adaptationTime,
                             double internalStepping) : 
    MSCFModel(vtype, accel, decel, emergencyDecel, decel, headwayTime), myDelta(4.),
    myAdaptationFactor(adaptationFactor), myAdaptationTime(adaptationTime),
    myIterations(MAX2(1, int(TS / internalStepping + .5))),
    myTwoSqrtAccelDecel(double(2 * sqrt(accel * decel))) {
}


MSCFModel_IDM_plat::~MSCFModel_IDM_plat() {}


double
MSCFModel_IDM_plat::moveHelper(MSVehicle* const veh, double vPos) {
    ego=veh;
    setlead();
    setleader();	
    const double vNext = MSCFModel::moveHelper(veh, vPos);
    if (myAdaptationFactor != 1.) {
        VehicleVariables* vars = (VehicleVariables*)veh->getCarFollowVariables();
        vars->levelOfService += (vNext / veh->getLane()->getVehicleMaxSpeed(veh) - vars->levelOfService) / myAdaptationTime * TS;
    }
    return vNext;
}


double
MSCFModel_IDM_plat::followSpeed(const MSVehicle* const veh, double speed, double gap2pred, double predSpeed, double /*predMaxDecel*/) const {
    return _v(veh, gap2pred, speed, predSpeed, veh->getLane()->getVehicleMaxSpeed(veh));
}


double
MSCFModel_IDM_plat::stopSpeed(const MSVehicle* const veh, const double speed, double gap2pred) const {
    if (gap2pred < 0.01) {
        return 0;
    }
    return _v(veh, gap2pred, speed, 0, veh->getLane()->getVehicleMaxSpeed(veh), false);
}

double
MSCFModel_IDM_plat::interactionGap(const MSVehicle* const veh, double vL) const {
    const double acc = myAccel * (1. - pow(veh->getSpeed() / veh->getLane()->getVehicleMaxSpeed(veh), myDelta));
    const double desacc = MAX3(lead->getAcceleration(), acc, leader->getAcceleration());
    const double vNext = veh->getSpeed()+ desacc;
    const double gap = (vNext - vL) * (veh->getSpeed() + vL) / (2 * myDecel) + vL;

    // Don't allow timeHeadWay < deltaT situations.
    return MAX2(gap, SPEED2DIST(vNext));
}


double
MSCFModel_IDM_plat::_v(const MSVehicle* const veh, const double gap2pred, const double egoSpeed,
                  const double predSpeed, const double desSpeed, const bool respectMinGap) const {
// this is more or less based on http://www.vwi.tu-dresden.de/~treiber/MicroApplet/IDM.html
// and http://arxiv.org/abs/cond-mat/0304337
// we assume however constant speed for the leader
    double headwayTime = myHeadwayTime;
    if (myAdaptationFactor != 1.) {
        const VehicleVariables* vars = (VehicleVariables*)veh->getCarFollowVariables();
        headwayTime *= myAdaptationFactor + vars->levelOfService * (1. - myAdaptationFactor);
    }
    double newSpeed = egoSpeed;
    double gap = gap2pred;
    for (int i = 0; i < myIterations; i++) {
        const double delta_v = newSpeed - predSpeed;
        double s = MAX2(0., newSpeed * headwayTime + newSpeed * delta_v / myTwoSqrtAccelDecel);
        if (respectMinGap) {
            s += myType->getMinGap();
        }
        const double acc = myAccel * (1. - pow(newSpeed / desSpeed, myDelta) - (s * s) / (gap * gap));
        newSpeed += ACCEL2SPEED(acc) / myIterations;
        //TODO use more realistic position update which takes accelerated motion into account
        gap -= MAX2(0., SPEED2DIST(newSpeed - predSpeed) / myIterations);
    }
//    return MAX2(getSpeedAfterMaxDecel(egoSpeed), newSpeed);
    return MAX2(0., newSpeed);
}




void MSCFModel_IDM_plat::setlead()
{
	std::pair<const MSVehicle*, double> veh_gap;
	const MSVehicle* veh;
	double gap;
	veh_gap=ego->getLeader();	
	std::tie(veh, gap)=veh_gap;

	lead=veh;
	if (!lead){
		lead=ego;
		leader=ego;
		}
}

void MSCFModel_IDM_plat::setleader()
{

	std::pair<const MSVehicle*, double> veh_gap;
	const MSVehicle* veh;
	double gap;
	veh_gap=ego->getLeader();	
	std::tie(veh, gap)=veh_gap;

	while(gap!=NULL){
		veh_gap=veh->getLeader();
		std::tie(veh, gap)=veh_gap;
	}
	
	leader=veh;

}



MSCFModel*
MSCFModel_IDM_plat::duplicate(const MSVehicleType* vtype) const {
    return new MSCFModel_IDM_plat(vtype, myAccel, myDecel, myEmergencyDecel, myHeadwayTime, myDelta, TS / myIterations);
}