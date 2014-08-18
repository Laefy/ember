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

#include "components/ogre/model/ModelHumanoidAttachment.h"
#include "components/ogre/model/ModelRepresentationHumanoid.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/Convert.h"

#include <algorithm>
#include <cmath>

namespace Ember
{

namespace OgreView
{

namespace Model
{

ModelHumanoidAttachment::ModelHumanoidAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider, const std::string& pose /*= ""*/)
:	ModelAttachment(parentEntity, modelRepresentation, nodeProvider, pose)
{
}

ModelHumanoidAttachment::~ModelHumanoidAttachment()
{

}

void ModelHumanoidAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity)
{
	ModelRepresentationHumanoid& representation = dynamic_cast<ModelRepresentationHumanoid&>(mModelRepresentation);

	//If the model is animated through Ogre skeletal animations, we have to use the parent method.
	if (mIgnoreEntityData || representation.isOgreAnimated()) {	

		ModelAttachment::setPosition(position, orientation, velocity);

	} else {

		representation.updateServerPositionAndOrientation(Convert::toOgre(position), Convert::toOgre(orientation));
		ModelAttachment::setPosition(Convert::toWF<WFMath::Point<3>>(representation.getActualPosition()), Convert::toWF(representation.getActualOrientation()), velocity);

	}	
}

bool ModelHumanoidAttachment::isEntityMoving() const
{
	ModelRepresentationHumanoid& representation = dynamic_cast<ModelRepresentationHumanoid&>(mModelRepresentation);
	return getAttachedEntity().isMoving() || representation.isMoving();
}

}
}
}