/**
 * @file Cosa/INET/HTTP.cpp
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

#ifndef __CHANGEPINSTATE_H__
#define __CHANGEPINSTATE_H__

#include "Cosa/Pins.hh"
#include "Command.h"

class ChangePinState : public Command {
public:
  ChangePinState(ThingSpeak::TalkBack* talkback, const char* string, 
      Board::DigitalPin pin, uint8_t state);

  void execute();

private:
  OutputPin m_pin;
  uint8_t m_state;
};

#endif