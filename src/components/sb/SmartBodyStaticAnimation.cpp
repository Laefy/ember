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

#include "SmartBodyStaticAnimation.h"
#include "SmartBodyGestureAnimation.h"
#include "sb/SBBmlProcessor.h"

namespace Ember
{

SmartBodyStaticAnimation::SmartBodyStaticAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetMng,
	const std::vector<std::string>& postures, SmartBodyGestureAnimation const *gestures /*= nullptr*/)
:	SmartBodyAnimation::SmartBodyAnimation(name, assetMng, mPostures), mPostures(postures), mGestures(gestures)
{
}


SmartBodyStaticAnimation::SmartBodyStaticAnimation(const SmartBodyStaticAnimation& animation)
:	SmartBodyAnimation::SmartBodyAnimation(animation.mName, animation.mAssetManager, mPostures), mPostures(animation.mPostures), mGestures(animation.mGestures)
{
}
	
SmartBodyStaticAnimation::~SmartBodyStaticAnimation()
{
}

bool SmartBodyStaticAnimation::getBmlRequest(std::string& request, int postureIndex, const std::vector<std::string>& attributes) const
{
	if (postureIndex < 0 || !(postureIndex < getMotionNumber())) {
		return false;
	}

	request = "<body posture=\"" + mPostures[postureIndex] + "\"";

	for (auto& attribute : attributes) {
		request += attribute;
	}

	request += "/>";

	return true;
}

bool SmartBodyStaticAnimation::getGestureBmlRequest(std::string& request, int gestureIndex) const
{
	if (!mGestures) {
		return false;
	}

	return mGestures->getBmlRequest(request, gestureIndex, std::vector<std::string>(0));
}

SmartBodyGestureAnimation const* SmartBodyStaticAnimation::getGestures() const
{
	return mGestures;
}

int SmartBodyStaticAnimation::getGestureMotionNumber() const
{
	if (!mGestures) {
		return 0;
	}

	return mGestures->getMotionNumber();
}



SmartBodyStaticAnimationInstance::SmartBodyStaticAnimationInstance(const SmartBodyStaticAnimation& reference, SmartBody::SBBmlProcessor& bmlProcessor, const std::string& character, int postureIndex /*= 0*/)
:	SmartBodyAnimationInstance::SmartBodyAnimationInstance(reference, bmlProcessor, character), mPosture(postureIndex), mGesture(-1),
	mTimeSincePostureChange(0.0f), mTimeSinceGestureEnd(0.0f)
{
}
	
SmartBodyStaticAnimationInstance::~SmartBodyStaticAnimationInstance()
{
}

bool SmartBodyStaticAnimationInstance::getBmlRequest(std::string& request) const
{
	std::vector<std::string> times;
	convertTimesToBmlStrings(times);

	return mReference.getBmlRequest(request, mPosture, times);
}

float SmartBodyStaticAnimationInstance::getTimeSincePostureChange()
{
	return mTimeSincePostureChange;
}

bool SmartBodyStaticAnimationInstance::changePosture(int postureIndex)
{
	if (postureIndex < 0 || !(postureIndex < mReference.getMotionNumber())) {
		return false;
	}

	mPosture = postureIndex;
	mTimeSincePostureChange = 0;

	return true;
}

bool SmartBodyStaticAnimationInstance::getGestureBmlRequest(std::string& request) const
{
	return dynamic_cast<const SmartBodyStaticAnimation&>(mReference).getGestureBmlRequest(request, mGesture);
}

float SmartBodyStaticAnimationInstance::getTimeSinceGestureEnd()
{
	return mTimeSinceGestureEnd;
}

bool SmartBodyStaticAnimationInstance::playGesture(int gestureIndex)
{
	SmartBodyGestureAnimation const* gestures = dynamic_cast<const SmartBodyStaticAnimation&>(mReference).getGestures();
	
	if (!gestures || gestureIndex < 0 || !(gestureIndex < gestures->getMotionNumber())) {
		return false;
	}

	float duration = gestures->getMotionDuration(gestureIndex);

	if (duration < 0) {
		//If something went wrong, we return false.
		return false;
	}

	mGesture = gestureIndex;
	mTimeSinceGestureEnd = -duration;

	return true;
}

int SmartBodyStaticAnimationInstance::getGestureMotionNumber() const
{
	return dynamic_cast<const SmartBodyStaticAnimation&>(mReference).getGestureMotionNumber();
}

void SmartBodyStaticAnimationInstance::updateTimers(float timeSlice)
{
	mTimeSincePostureChange += timeSlice;
	mTimeSinceGestureEnd += timeSlice;
}

void SmartBodyStaticAnimationInstance::executeGesture(const std::string& characterName) 
{
	std::string request;
	getGestureBmlRequest(request);
	mBmlProcessor.execBML(characterName, request);
}

}