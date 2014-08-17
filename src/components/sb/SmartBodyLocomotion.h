/*
 Copyright (C) 2014 Céline NOËL <celine.noel.7294@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef SMARTBODYLOCOMOTION_H
#define SMARTBODYLOCOMOTION_H

	#include <string>
	#include <vector>
	#include "SmartBodyBehaviors.h"


namespace SmartBody
{
class SBAnimationBlendManager;
}

namespace Ember
{

/**
 * @brief The locomotion behavior set.
 * This class derived from SmartBodyBehaviors.
 *
 * In addition of loading all the motions implying locomotion, setting up this behavior set allows the creation of SmartBody blends to handle the locomotion more easily.
 * We here use five blends: locomotion (the greatest part of it), starting left and right (translation from idling to moving, left and right referring to the foot), idle_turn (turning while staying
 * in position) and step (only one step movements).
 *
 * A blend is a group of motions, that are hierarchized by parameters (from one to three). The dimension of the blend correspond to the number of parameters used.
 * For each motion of a n-dimension blend, we have to specify n values for the n parameters. If we use a 1D blend, containing a idling motion, a 90° turning motion, and a half turning motion, we can 
 * respectively grant them 0, 90 and 180 values. The parameter will thus be representing the angle of the turn.
 *
 * A blend (having a dimension different from zero) can contain as many motions as wanted. To use it, the bml request should be looking like <blend name="blend_name" x="x_val" />, (precise y and z for
 * 2D and 3D blends). In the previous example, using x=160 will make the character rotate from 160°, and the resulting animation will be a mix between the 90 turning and half turning motions (it will 
 * be closer to the latter), that is to say the two motions the closest in term of parameters. 
 * But for 2D and 3D blends, we will mix at maximum three and four motions together if the parameters do not correspond exactly to one motion. For compatibility reasons (two motions's blend may not
 * render nicely together), we have to precise what group of motions can be mixed together. For 2D blends, we are speaking of triangles, and for 3D blends, the term is tetrahedron. They contains
 * three (resp. four) motions that can be correctly mixed.
 *
 * Finally, in order to blend the motions accurately, we have to set correspondence points: these are time references in the motion progress, that help to superimpose the motions while mixing them.
 * The easiest is to use the moment when the heels touch the floor. 
 *
 * @see The SmartBody documentation for more information.
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyLocomotion : public SmartBodyBehaviors
{

public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyLocomotion(std::string const &motionPath, std::string const &skeletonRef, SmartBody::SBAssetManager& assetMng, SmartBody::SBAnimationBlendManager& blendMng, SmartBody::SBRetargetManager& retargetMng);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyLocomotion();

	/**
	 * @brief Setup the locomotion behavior set.
	 */
	bool setup(SmartBody::SBJointMapManager& jointMapManager);

	/**
	 * @brief Overrides SmartBodyBehaviors::applyRetargeting().
	 * This method does nothing as the retargeting of the locomotion doesn't imply modifying the SBCharacter instance.
	 */
	void applyRetargeting(SmartBody::SBCharacter& character);


private:

	/**
	 * @brief Reference to the animation blend manager.
	 */
	SmartBody::SBAnimationBlendManager& mBlendManager;

	/**
	 * @brief Overrides SmartBodyBehaviors::getMotions().
	 * Gets the name of all the motions contained in the locomotion behavior set.
	 */
	std::vector<std::string> getMotions();

	/**
	 * @brief Gets the name of all the motions involved in the locomotion blend.
	 */
	std::vector<std::string> getMotionsForLocomotionSetup();

	/**
	 * @brief Sets the locomotion behavior up blend by blend.
	 * The values used to set up everything are the ones used in the SmartBody examples.
	 * @see All the python scripts used for setting up / retargeting the behaviors, in SmartBody package for more details.
	 */
	void locomotionSetup();
	void startingSetup();
	void idleTurnSetup();
	void stepSetup();

	/**
	 * @brief Specifies how to go from one of the starting blend to the locomotion blend.
	 */
	void transitionSetup();
};

}

#endif