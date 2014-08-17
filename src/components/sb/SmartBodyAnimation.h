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

#ifndef SMARTBODYANIMATION_H
#define SMARTBODYANIMATION_H

#include <string>
#include <vector>


namespace SmartBody
{
class SBMotion;
class SBAssetManager;
class SBBmlProcessor;
}

namespace Ember
{

/**
 * @brief This class is used to represent an animation: each of them is identified by a name, and is composed of several motions.
 * 
 * An SmartBodyAnimation should not be created directly. It is better to instantiate SmartBodyStaticAnimation, SmartBodyMovingAnimation, SmartBodyIntuitiveAnimation and SmartBodyGestureAnimation. 
 * In addition, the manipulation of this object should be handled by the SmartBodyAnimationManager. For each motion that composes an animation, there is a corresponding bml request that can be executed, 
 * using the bml processor. A SmartBodyAnimation is a global object, each of them represent the concept of the animation. To bound a SmartBodyAnimation to a character, in order to say, right now, the 
 * character is animated with motions from this animation state, you need to create a SmartBodyAnimationInstance, which will refer to the SmartBodyAnimation.
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 *
 */
class SmartBodyAnimation
{
public:

	/**
	 * @brief The different name an animation can take.
	 */
	enum class Name 
	{
		WALKING, RUNNING,			//MovingAnimations.
		LOCOMOTION,					//IntuitiveAnimations.
		STANDING,					//StaticAnimations.
		WAITING,					//Gestures.

		ANIMATIONS_COUNT			//Gives the number of supported animations.
	};

	/**
	 * @brief The different subsequent classes.
	 * This is used by the SmartBodyAnimationManager to identify what type of SmartBodyAnimationInstance it must create.
	 */
	enum class Type
	{
		STATIC, MOVING, INTUITIVE, GESTURE, 
		UNDEFINED								//Undefined is used when invalid animation name (ANIMATIONS_COUNT for example) is send to getType().
	};

	/**
	 * @brief Gets what type of class has to be created for the given animations.
	 */
	static Type getType(Name animationName);


	/**
	 * @brief Ctor.
	 */
	SmartBodyAnimation(SmartBodyAnimation::Name name, SmartBody::SBAssetManager& assetManager, const std::vector<std::string>& motions);

	/**
	 * @brief Dtor.
	 */
	virtual ~SmartBodyAnimation();

	/**
	 * @brief Returns the name of the animation.
	 */
	SmartBodyAnimation::Name getName() const;

	/**
	 * @brief Gets the request that is to be sent to the bml processor.
	 *
	 * @param attributes: the different attributes that will be appended to the request ("start", "ready", "x", "y", "z", etc.).
	 * @return false if the given index is invalid.
	 */
	virtual bool getBmlRequest(std::string& request, int motionIndex, const std::vector<std::string>& attributes) const = 0;

	/**
	 * @brief Returns the duration in seconds of the given motion.
	 *
	 * @param motionIndex: must be set between 0 and mMotions.size().
	 * @return A negative value if something went wrong.
	 */
	virtual float getMotionDuration(int motionIndex) const;

	/**
	 * @brief Returns the total number of motions contained in this set.
	 */
	virtual int getMotionNumber() const;


protected:

	/**
	 * @brief The name for this animation.
	 */
	Name mName;

	/**
	 * @brief A reference on the asset manager. 
	 * It is needed to get the duration of a motion.
	 */
	SmartBody::SBAssetManager& mAssetManager;

	/**
	 * @brief A reference on the motions names.
	 */
	const std::vector<std::string>& mMotionNames;

};


/**
 * @brief An instance for a SmartBodyAnimation.
 *
 * Whereas a SmartBodyAnimation is used to describe an animation in general, this class is used into SmartBodyRepresentation to bound it to an animation. Everytime we want to animate a character with 
 * SmartBody, an object from this class has to be created by the SmartBodyAnimationManager and attributed to the character.
 * In the same way as SmartBodyAnimation, SmartBodyAnimationInstance should be instantiated through children classes (MovindAnimationInstance, IntuitiveAnimationInstance, StaticAnimationInstance, 
 * GestureAnimationInstance). Each of them contains information about how the animation is being played on the representation it is linked to.
 * The execute method allow you to animate the character with the motion attended and parameterized by this instance. 
 *
 * @author Céline NOEL <celine.noel.7294@gmail.com>
 */
class SmartBodyAnimationInstance
{
public:

	/**
	 * @brief Ctor.
	 */
	SmartBodyAnimationInstance(const SmartBodyAnimation& animation, SmartBody::SBBmlProcessor& bmlProcessor, const std::string& character);

	/**
	 * @brief Dtor.
	 */
	virtual ~SmartBodyAnimationInstance();

	/** 
	 * @brief Returns a const reference over the corresponding SmartBodyAnimation.
	 */
	const SmartBodyAnimation& getReference() const;

	/**
	 * @brief Returns the number of motions constituing mReference.
	 */ 
	int getMotionNumber() const;

	/** 
	 * @brief Specify the time to wait before beginning the new motion.
	 * @param specify: set to false if the animation should start as soon as the previous one is finished, to true if you want to specify it yourself.
	 */
	void specifyStartTime(bool specify, float time = 0.0f);

	/** 
	 * @brief Specify the time necessary to move from the previous animation to the new one (the start time has to be specified too, or the behaviour could not be the one expected).
	 * @param specify: set to false if the duration of the blend should be handled by SmartBody, to true if you want to specify it yourself.
	 */
	void specifyReadyTime(bool specify, float time = 0.0f);

	/**
	 * @brief Sends the bml request to SmartBody, that will play the active motion with the specified parameters.
	 */
	void execute(const std::string& characterName);


protected:

	/**
	 * @brief A reference on the corresponding SmartBodyMovingAnimation.
	 */
	const SmartBodyAnimation& mReference;

	/** 
	 * @brief The bml processor, used to send or interrupt requests that animate characters.
	 */
	SmartBody::SBBmlProcessor& mBmlProcessor;

	/**
	 * @brief The name of the character this animation instance is bound to.
	 */
	std::string mCharacter;	

	/**
	 * @brief The time when the animation starts.
	 */
	float mStartTime;

	/**
	 * @brief States that mStartTime has been specified.
	 */
	bool mHasStartTime;

	/**
	 * @brief The time taken by the animation to be fully blended.
	 */
	float mReadyTime;

	/**
	 * @brief States that mReadyTime has been specified.
	 */
	float mHasReadyTime;

	/**
	 * @brief Gets the bml request parts corresponding to start and ready attributes.
	 */
	void convertTimesToBmlStrings(std::vector<std::string>& times) const;

	/**
	 * @brief Returns the bml request corresponding to this instance.
	 */
	virtual bool getBmlRequest(std::string& request) const = 0;

	/** 
	 * @brief The identifier of the last request sent (can be useful if you need to interrupt it).
	 */
	std::string mLastRequestId;

	/**
	 * @brief Called just after a bml request has been sent, in order to reinitialize some values (mHasStartTime, mHasReadyTime).
	 */
	virtual void notifyUpdate();

};

}

#endif
