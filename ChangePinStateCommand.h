/**
 * @file ChangePinStateCommand.h
 * @version 1.0
 *
 */

#ifndef __CHANGEPINSTATECOMMAND_H__
#define __CHANGEPINSTATECOMMAND_H__

#include "Cosa/Pins.hh"
#include "Cosa/IoT/ThingSpeak.hh"

#define TURN_ON 1
#define TURN_OFF 0

class ChangePinStateCommand : public ThingSpeak::TalkBack::Command {
public:

  /**
    * Construct command.
    */
  ChangePinStateCommand(ThingSpeak::TalkBack* talkback, const char* string, 
      Board::DigitalPin pin, uint8_t state);

  /**
    * Execute command.
    */
  void execute();

private:
  OutputPin m_pin;
  uint8_t m_state;
};

#endif
