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

#ifndef SMARTBODYGESTURES_H
#define SMARTBODYGESTURES_H

#include "SmartBodyBehaviors.h"


namespace SmartBody
{
class SBGestureMapManager;
}

namespace Ember
{

/**
 * @brief The gesture behavior set. 
 * These are animations using the upper part of the body. Variations can be created depending on the character mood (neutral, sad, or angry). 
 * This class derived from SmartBodyBehaviors.
 *
 * To setup the gestures, we need to group them into gesture maps (neutral, angry, or sad gestures).
 * To add a motion to a map, we use SmartBody::SBGestureMap::addGestureMapping(), and we send as parameter the name of the motion (name of the .sk file), the lexeme, the type, the hand that will be 
 * used (left, right or both), the style and the posture (the animation is applied only if the character is in this posture).
 * Then, when we execute a request like <gesture lexeme="DEICTIC" mode="RIGHT_HAND" type="YOU" sbm::style="mid"/>, an animation corresponding to the given parameters will be played. To specify that 
 * we want to use a motion from the sad gesture map, we can add emotion="sad" to the request. Finally, if the current posture doesn't match the one set in the map, we can set the attribute
 * sbm:additive to "true".
 *
 * If you want a specifical animation to be played, just execute <gesture name="NameOfTheGesture"/>.
 * 
 * @see The SmartBody documentation for additional information.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyGestures: public SmartBodyBehaviors
{

public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyGestures(const std::string& motionPath, const std::string& skeletonRef, SmartBody::SBAssetManager& assetMng, SmartBody::SBRetargetManager& retargetMng, SmartBody::SBGestureMapManager& gestureMapMng);

	/**
	 * @brief Dtor.
	 */
	~SmartBodyGestures();

	/**
	 * @brief Setup the gestures behavior set.
	 */
	bool setup(SmartBody::SBJointMapManager& jointMapManager);

	/**
	 * @brief Sets some of the character attributes to be able to use the gesture maps on him.
	 */
	void applyRetargeting(SmartBody::SBCharacter& character);


private:

	/**
	 * @brief The gesture map manager.
	 */
	SmartBody::SBGestureMapManager& mGestureMapMng;

	/**
	 * @brief Gets the name of all the motions contained in the gestures set.
	 */
	std::vector<std::string> getMotions();

	/**
	 * @brief Creates the maps for the different gesture sets.
	 */
	void mapNeutralGestures();
	void mapAngryGestures();
	void mapSadGestures();
};

}

#endif //SMARTBODYGESTURES_H