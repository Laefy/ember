/*
	Copyright (C) 2002  Tim Enderling
    Copyright (C) 2005	Erik Hjortsberg

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LoggingService.h"
#include "ErisLogReciever.h"


// Include system headers here
#include <string>
#include <list>
#include <cstdio>
#include <sstream>

#include <sigc++/object_slot.h>

namespace Ember {


bool LoggingService::start ()
{
	setRunning(true);
	return true;
}

LoggingService::LoggingService () : Service("Logging")
{
	//set service properties
	
	mErisLogReciever = std::unique_ptr<ErisLogReciever>(new ErisLogReciever());
}


LoggingService::~LoggingService ()
{

}


}
