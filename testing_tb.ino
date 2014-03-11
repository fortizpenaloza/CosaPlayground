/**
 * @file CosaThingSpeakTalkBack.ino
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
 * @section Description
 * ThingSpeak talkback demonstration.
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/Alarm.hh"
#include "Cosa/Event.hh"
#include "Cosa/RTC.hh"
#include "Cosa/Watchdog.hh"

#include "Cosa/Memory.h"

#include "Cosa/Socket/Driver/W5100.hh"
#include "Cosa/IoT/ThingSpeak.hh"

#include "ChangePinStateCommand.h"


#ifndef NDEBUG
#include "Cosa/Trace.hh"
#include "Cosa/IOStream/Driver/UART.hh"
#else
#define TRACE(x) x
#endif

// Ethernet controller
static const char HOSTNAME[] __PROGMEM = "IoTponics";
W5100 ethernet;


// Thingspeak client
static const char KEY[] __PROGMEM = "MI05AL3BJWVQAFZ6";
uint16_t TALKBACK_ID = 210;
ThingSpeak::Client client;
ThingSpeak::TalkBack talkback(&client, KEY, TALKBACK_ID);

// Create commands
const char LED_ON_COMMAND[] __PROGMEM = "TURN_LED_OFF";
ChangePinStateCommand led_on(&talkback, LED_ON_COMMAND, Board::LED, TURN_ON);

const char LED_OFF_COMMAND[] __PROGMEM = "TURN_LED_ON";
ChangePinStateCommand led_off(&talkback, LED_OFF_COMMAND, Board::LED, TURN_OFF);

class ExecuteNextTalkBackCommand : public Alarm 
{
  public:
    ExecuteNextTalkBackCommand (uint16_t period) : Alarm(period) {}
    virtual void run();
};
 
void 
ExecuteNextTalkBackCommand::run()
{
  TRACE(talkback.execute_next_command()); 
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
#ifndef NDEBUG
  uart.begin(9600);
  trace.begin(&uart, PSTR("IoTponics: started"));
#endif

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
}
