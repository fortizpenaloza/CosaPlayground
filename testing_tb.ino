/**
 * @file testing_iot.ino
 * @version 1.0
 *
 */

#include "Cosa/Alarm.hh"
#include "Cosa/Event.hh"
#include "Cosa/RTC.hh"
#include "Cosa/Watchdog.hh"

#include "Cosa/Memory.h"
#include "Cosa/Trace.hh"
#include "Cosa/IOStream/Driver/UART.hh"

#include "Cosa/Socket/Driver/W5100.hh"
#include "Cosa/IoT/ThingSpeak.hh"

#include "ChangePinStateCommand.h"


// Ethernet controller
static const char HOSTNAME[] __PROGMEM = "IoTponics";
W5100 ethernet;


// Thingspeak client
static const char KEY[] __PROGMEM = "MI05AL3BJWVQAFZ6";
uint16_t TALKBACK_ID = 210;
ThingSpeak::Client client;
ThingSpeak::TalkBack talkback(&client, KEY, TALKBACK_ID);

// Create commands
const char LED_ON_COMMAND[] __PROGMEM = "TURN_ON";
ChangePinStateCommand led_on(&talkback, LED_ON_COMMAND, Board::LED, 1);

const char LED_OFF_COMMAND[] __PROGMEM = "TURN_OFF";
ChangePinStateCommand led_off(&talkback, LED_OFF_COMMAND, Board::LED, 0);

class ExecuteNextTalkBackCommand : public Alarm 
{
  public:
    ExecuteNextTalkBackCommand (uint16_t period) : Alarm(period) {}
    virtual void run();
};
 
void 
ExecuteNextTalkBackCommand::run()
{
  trace << time() << PSTR(" : Execute next TalkBack command") << endl;
}

class PostToThingSpeak : public Alarm 
{
  public:
    PostToThingSpeak(uint16_t period) : Alarm(period) {}
    virtual void run();
};
 
void 
PostToThingSpeak::run()
{
  trace << time() << PSTR(" : Post to ThinkgSpeak") << endl;
}

Alarm::Scheduler scheduler;
ExecuteNextTalkBackCommand every_fifteen_seconds(15);
PostToThingSpeak every_minute(60);

void setup()
{
  uart.begin(9600);
  trace.begin(&uart, PSTR("IoTponics: started"));

  TRACE(free_memory());

  Watchdog::begin(16, SLEEP_MODE_IDLE, Watchdog::push_timeout_events);
  RTC::begin();

  TRACE(ethernet.begin_P(HOSTNAME));
  TRACE(client.begin(ethernet.socket(Socket::TCP)));

  every_fifteen_seconds.enable();
  every_minute.enable();
}

void loop()
{
  Event event;
  Event::queue.await(&event);
  event.dispatch();
  
  TRACE(talkback.execute_next_command()); 
}
