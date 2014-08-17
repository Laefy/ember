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

#include "SmartBodyMovingAnimation.h"
#include "sb/SBBmlProcessor.h"

#include <OgreVector3.h>


namespace Ember
{

bool SmartBodyMovingAnimation::isMotionLoopOver(Direction direction, const Ogre::Vector3& translation)
{
	//The z-axis concern the forward/backward displacements, whereas the x-axis is for the left/right movements.
	switch (direction) {

		case SmartBodyMovingAnimation::Direction::FORWARD:
			return (translation.z < 0) ? true : false;

		case SmartBodyMovingAnimation::Direction::BACKWARD:
			return (0 < translation.z) ? true : false;

		case SmartBodyMovingAnimation::Direction::LEFT:
			return (translation.x < 0) ? true : false;

		case SmartBodyMovingAnimation::Direction::RIGHT:
			return (0 < translation.x) ? true : false;
	}
}

SmartBodyMovingAnimation::SmartBodyMovingAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng, 
	const std::string& forwardMotion, const std::string& backwardMotion, const std::string& leftMotion, const std::string& rightMotion)
:	SmartBodyAnimation::SmartBodyAnimation(name, assetMng, mMotions), mMotions((int)Direction::DIRECTIONS_COUNT)
{
	mMotions[(int)Direction::FORWARD] = forwardMotion; 
	mMotions[(int)Direction::BACKWARD] = backwardMotion;
	mMotions[(int)Direction::LEFT] = leftMotion;
	mMotions[(int)Direction::RIGHT] = rightMotion;
} 

SmartBodyMovingAnimation::SmartBodyMovingAnimation(const SmartBodyMovingAnimation& animation)
:	SmartBodyAnimation::SmartBodyAnimation(animation.mName, animation.mAssetManager, mMotions), mMotions(animation.mMotions)
{
}

	
SmartBodyMovingAnimation::~SmartBodyMovingAnimation()
{
}

void SmartBodyMovingAnimation::getBmlRequest(std::string& request, Direction direction /*= Direction::FORWARD*/, const std::vector<std::string>& attributes /*= std::vector<std::string>(0)*/) const
{
	//Moving animations always alter the posture attribute in the body element.
	request = "<body posture=\"" + mMotions[(int)direction] + "\"";

	for (auto& attribute : attributes) {
		request += attribute;
	}

	request += "/>";
}

bool SmartBodyMovingAnimation::getBmlRequest(std::string& request, int direction /*= (int)Direction::FORWARD*/, const std::vector<std::string>& attributes /*= std::vector<std::string>(0)*/) const
{
	if (direction < 0 || !(direction < getMotionNumber())) {
		return false;
	}

	getBmlRequest(request, (Direction)direction, attributes);
	return true;
}



SmartBodyMovingAnimationInstance::SmartBodyMovingAnimationInstance(const SmartBodyMovingAnimation& animation, SmartBody::SBBmlProcessor& bmlProcessor, const std::string& character, SmartBodyMovingAnimation::Direction direction /*= SmartBodyMovingAnimation::Direction::FORWARD*/)
:	SmartBodyAnimationInstance::SmartBodyAnimationInstance(animation, bmlProcessor, character), mDirection(direction), mHasDirectionChanged(true)
{
}

SmartBodyMovingAnimationInstance::~SmartBodyMovingAnimationInstance()
{
}

bool SmartBodyMovingAnimationInstance::getBmlRequest(std::string& request) const
{	
	//Convert mReady and mStart to bml attributes.
	std::vector<std::string> times;
	convertTimesToBmlStrings(times);

	return mReference.getBmlRequest(request, (int)mDirection, times);
}

void SmartBodyMovingAnimationInstance::setDirection(SmartBodyMovingAnimation::Direction direction)
{
	mDirection = direction;
	mHasDirectionChanged = true;
}

SmartBodyMovingAnimation::Direction SmartBodyMovingAnimationInstance::getDirection() const
{
	return mDirection;
}

bool SmartBodyMovingAnimationInstance::hasDirectionChanged()
{
	return mHasDirectionChanged;
}

void SmartBodyMovingAnimationInstance::notifyUpdate()
{
	SmartBodyAnimationInstance::notifyUpdate();
	mHasDirectionChanged = false;
}

}