/**
 * @file ChangePinStateCommand.cpp
 * @version 1.0
 *
 */

#include "ChangePinStateCommand.h"

ChangePinStateCommand::ChangePinStateCommand(ThingSpeak::TalkBack* talkback, const char* string,
 Board::DigitalPin pin, uint8_t state) :
    ThingSpeak::TalkBack::Command(talkback, string),
    m_pin(pin),
    m_state(state)
{}

void 
ChangePinStateCommand::execute() 
{ 
  m_pin.set(m_state); 
}
