/*
 * nes-usb-adapter: USB adapter for the original NES controller
 * Copyright (C) 2016 Josh Gadeken
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see {http://www.gnu.org/licenses/}.
*/

#define DEBUG false
#define TIMER false

#define CLOCK_PIN 0
#define LATCH_PIN 1
#define DATA_PIN 2
#define LED_PIN 11

byte switchData = 0;
byte prevSwitchData = 0;
byte changedSwitches = 0;


#if TIMER
elapsedMillis sincePrint;
unsigned long int loopCount = 0;
#endif


byte shift_in() {
	byte data = 0;

	for (int i = 7; i >= 0; i--) {
		digitalWrite(CLOCK_PIN, LOW);
		delayMicroseconds(2);
		if(digitalRead(DATA_PIN) == HIGH) {
			bitSet(data, i);
		}

		digitalWrite(CLOCK_PIN, HIGH);
	}

	return data;
}


void setup() {
	#if DEBUG || TIMER
	Serial.begin(115200);
	#endif
	pinMode(CLOCK_PIN, OUTPUT);
	pinMode(LATCH_PIN, OUTPUT);
	pinMode(DATA_PIN, INPUT);
	pinMode(LED_PIN, OUTPUT);

	digitalWrite(CLOCK_PIN, HIGH);
	digitalWrite(LATCH_PIN, HIGH);

	Joystick.useManualSend(true);
	#if DEBUG
	digitalWrite(LED_PIN, LOW);
	#else
	digitalWrite(LED_PIN, HIGH);
	#endif
}


void loop() {
	#if TIMER
	loopCount++;
	#endif
	digitalWrite(LATCH_PIN, HIGH);
	delayMicroseconds(5);
	digitalWrite(LATCH_PIN, LOW);

	switchData = ~shift_in();
	#if DEBUG
	if(switchData != 0) {
		digitalWrite(LED_PIN, HIGH);
	}
	else {
		digitalWrite(LED_PIN, LOW);
	}
	#endif

	if(switchData != prevSwitchData) {
		changedSwitches = switchData^prevSwitchData;
		#if DEBUG
		Serial.print(" prevSwitchData: ");
		Serial.println(prevSwitchData, BIN);
		Serial.print("     switchData: ");
		Serial.println(switchData, BIN);
		Serial.print("changedSwitches: ");
		Serial.println(changedSwitches, BIN);
		#endif
		for(int i = 0; i < 8; i++) {
			if(bitRead(changedSwitches, i) == 1) {
				#if DEBUG
				Serial.println("");
				Serial.print("bit ");
				Serial.print(i, DEC);
				Serial.print(": ");
				Serial.println(bitRead(switchData, i), DEC);
				#endif
				Joystick.button(i+1, bitRead(switchData, i));
			}
		}

		Joystick.send_now();
		#if DEBUG
		Serial.println("----------");
		#endif
		prevSwitchData = switchData;
	}

	#if TIMER
	if(sincePrint >= 1000) {
		Serial.print("cycles per second: ");
		Serial.println(loopCount, DEC);
		loopCount = 0;
		sincePrint = 0;
	}
	#endif
}
