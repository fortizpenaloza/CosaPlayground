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

#include "Cosa/Watchdog.hh"
#include "Cosa/Event.hh"
#include "Cosa/Periodic.hh"
#include "Cosa/Socket/Driver/W5100.hh"

#include "ChangePinState.h"

#define TURN_ON 1
#define TURN_OFF 0

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
const char LED_ON[] __PROGMEM = "LED_ON";
ChangePinState led_on(&talkback, LED_ON, Board::LED, TURN_ON);

const char LED_OFF[] __PROGMEM = "LED_OFF";
ChangePinState led_off(&talkback, LED_OFF, Board::LED, TURN_OFF);

class ExecuteNextTalkBackCommand : public Periodic {
public:
  ExecuteNextTalkBackCommand (uint16_t ms) : Periodic(ms) {}
  virtual void run() { 
    talkback.execute_next_command(); 
    trace << Watchdog::millis() << PSTR(": ExecuteNextTalkBackCommand") << endl; 
  }
};

class ExecuteNextTalkBackCommand2 : public Periodic {
public:
  ExecuteNextTalkBackCommand2 (uint16_t ms) : Periodic(ms) {}
  virtual void run() { 
    talkback.execute_next_command(); 
    trace << Watchdog::millis() << PSTR(": ExecuteNextTalkBackCommand2") << endl; 
  }
};

ExecuteNextTalkBackCommand nextCommand(16);
ExecuteNextTalkBackCommand2 nextCommand2(32);

void setup()
{
#ifndef NDEBUG
  uart.begin(9600);
  trace.begin(&uart, PSTR("IoTponics: started"));
#endif

// Watchdog::begin();

  Watchdog::begin(16, SLEEP_MODE_IDLE, Watchdog::push_timeout_events);

  TRACE(ethernet.begin_P(HOSTNAME));
  TRACE(client.begin(ethernet.socket(Socket::TCP)));
}

void loop()
{

//  talkback.execute_next_command();
//  SLEEP(30);
  Event event;
  Event::queue.await(&event);
  event.dispatch();
}
