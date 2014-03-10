/**
 * @file Command.hh
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Cosa/IoT/ThingSpeak.hh"
 
class Command : public ThingSpeak::TalkBack::Command {
public:
  /**
    * Construct command.
    */
  Command(ThingSpeak::TalkBack* talkback, const char* string);

  /**
    * Execute command.
    */
  virtual void execute() {}
};

#endif