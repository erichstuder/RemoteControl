/* IT - Internal Tracer
 * Copyright (C) 2019 Erich Studer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "itCommand.h"

#define LoggedSignalCapacity 10
struct {
	ItSignal_t* signal[LoggedSignalCapacity];
	unsigned char nextFreeIndex;
	unsigned char count;
} loggedSignals;

static const char* const LogCommandPrefix = "log ";
static const char* const UnlogCommandPrefix = "unlog ";

static ItSignal_t* signals;
static unsigned char signalCount;

static SendCommandResult_t sendResult;

static bool hasResetCommandFormat(const char* const command);

static bool hasLogCommandFormat(const char* const command);
static ItError_t handleLogCommand(const char* const command);
static ItError_t addSignalToLog(ItSignal_t* signal);

static bool hasUnlogCommandFormat(const char* const command);
static ItError_t handleUnlogCommand(const char* const command);
static ItError_t removeFromLoggedSignals(ItSignal_t* signal);
static void removeIndexFromLoggedSignals(unsigned char index);

static bool hasSetCommandFormat(const char* const command);
static ItError_t handleSetCommand(const char* const command);

static bool hasRequestCommandFormat(const char* const command);
static ItError_t handleRequestCommand(const char* const command);
static ItError_t readSignalValue(ItSignal_t* signal);

void itCommandInit(ItSignal_t* itSignals, unsigned char itSignalCount, SendCommandResult_t sendCommandResult) {
	signals = itSignals;
	signalCount = itSignalCount;
	sendResult = sendCommandResult;

	loggedSignals.nextFreeIndex = 0;
	loggedSignals.count = 0;
}

static ItError_t parseCommand_Implementation(const char* const command) {
	if (hasResetCommandFormat(command))
	{
		loggedSignals.nextFreeIndex = 0;
		loggedSignals.count = 0;
		return ItError_NoError;
	}
	else if (hasLogCommandFormat(command)) {
		return handleLogCommand(command);
	}
	else if (hasUnlogCommandFormat(command)) {
		return handleUnlogCommand(command);
	}
	else if (hasSetCommandFormat(command)) {
		return handleSetCommand(command);
	}
	else if (hasRequestCommandFormat(command))	{
		return handleRequestCommand(command);
	}
	else {
		return ItError_InvalidCommand;
	}
}
ItError_t (*parseCommand) (const char* const command) = parseCommand_Implementation;

static bool hasResetCommandFormat(const char* const command) {
	return strcmp(command, "reset") == 0;
}

static bool hasLogCommandFormat(const char* const command) {
	return strstr(command, LogCommandPrefix) == command;
}

static ItError_t handleLogCommand(const char* const command) {
	unsigned char n;
	for (n = 0; n < signalCount; n++) {
		ItSignal_t* signalPtr = &(signals[n]);
		const unsigned char LogCommandPrefixLength = strlen(LogCommandPrefix);
		if(strcmp(signalPtr->name, command + LogCommandPrefixLength) != 0) {
			continue;
		}
		if(signalPtr->getter == NULL){
			return ItError_NoGetter;
		}
		return addSignalToLog(signalPtr);
	}
	return ItError_InvalidCommand;
}

static ItError_t addSignalToLog(ItSignal_t* signal) {
	if (loggedSignals.nextFreeIndex >= LoggedSignalCapacity) {
		return ItError_MaximumOfLoggedSignalsReached;
	}
	loggedSignals.signal[loggedSignals.nextFreeIndex] = signal;
	loggedSignals.nextFreeIndex++;
	loggedSignals.count++;
	return ItError_NoError;
}

static bool hasUnlogCommandFormat(const char* const command) {
	return strstr(command, UnlogCommandPrefix) == command;
}

static ItError_t handleUnlogCommand(const char* const command) {
	unsigned char n;
	for (n = 0; n < signalCount; n++) {
		ItSignal_t* signalPtr = &(signals[n]);
		const unsigned char UnlogCommandPrefixLength = strlen(UnlogCommandPrefix);
		if (strcmp(signalPtr->name, command + UnlogCommandPrefixLength) == 0) {
			return removeFromLoggedSignals(signalPtr);
		}
	}
	return ItError_InvalidCommand;
}

static ItError_t removeFromLoggedSignals(ItSignal_t* signal) {
	unsigned char signalIndex;
	for (signalIndex = 0; signalIndex < loggedSignals.count; signalIndex++) {
		if (loggedSignals.signal[signalIndex] == signal) {
			removeIndexFromLoggedSignals(signalIndex);
			return ItError_NoError;
		}
	}
	return ItError_InvalidCommand;
}

static void removeIndexFromLoggedSignals(unsigned char index) {
	unsigned char signalIndex;
	for (signalIndex = index; signalIndex < loggedSignals.count-1; signalIndex++) {
		loggedSignals.signal[signalIndex] = loggedSignals.signal[signalIndex + 1];
	}
	loggedSignals.nextFreeIndex--;
	loggedSignals.count--;
}

static bool hasSetCommandFormat(const char* const command) {
	unsigned char index = 0;
	unsigned char spaceCount = 0;
	while (command[index] != '\0') {
		if (command[index] == ' ') {
			spaceCount++;
		}
		index++;
	}
	return spaceCount == 1;
}

static ItError_t handleSetCommand(const char* const command) {
	bool signalFound = false;
	const char* signalName = "";
	unsigned char signalIndex;
	for (signalIndex = 0; signalIndex < signalCount; signalIndex++) {
		signalName = signals[signalIndex].name;
		if (strstr(command, signalName) == command) {
			signalFound = true;
			break;
		}
	}
	if (!signalFound) {
		return ItError_InvalidCommand;
	}

	if (command[strlen(signalName)] != ' ') {
		return ItError_InvalidCommand;
	}

	const char* startPosition = strchr(command, ' ') + 1;
	if (*startPosition == '\0') {
		return ItError_InvalidCommand;
	}

	char* charAfterNumber;
	double value = strtod(startPosition, &charAfterNumber);
	if (*charAfterNumber != '\0') {
		return ItError_InvalidCommand;
	}

	void (*setter)(void) = signals[signalIndex].setter;
	if(setter == NULL){
		return ItError_NoSetter;
	}
	switch (signals[signalIndex].valueType) {
	case ItValueType_Int8:
		((void (*) (signed char))setter)((signed char)value);
		break;
	case ItValueType_Uint8:
		((void (*) (unsigned char))setter)((unsigned char)value);
		break;
	case ItValueType_Ulong:
		((void (*) (unsigned long))setter)((unsigned long)value);
		break;
	case ItValueType_Float:
		((void (*) (float))setter)((float)value);
		break;
	default:
		return ItError_InvalidValueType;
	}
	
	if(signals[signalIndex].getter != NULL){
		return readSignalValue(&(signals[signalIndex]));
	}
	return ItError_NoError;
}

static bool hasRequestCommandFormat(const char* const command) {
	return !hasLogCommandFormat(command) && !hasSetCommandFormat(command);
}

static ItError_t handleRequestCommand(const char* const command) {
	unsigned char n;
	for (n = 0; n < signalCount; n++) {
		ItSignal_t signal = signals[n];
		if (strcmp(command, signal.name) == 0) {
			return readSignalValue(&signal);
		}
	}
	return ItError_InvalidCommand;
}

ItError_t logSignals_Implementation(void) {
	unsigned char n;
	for (n = 0; n < loggedSignals.count; n++) {
		ItError_t err = readSignalValue(loggedSignals.signal[n]);
		if (err != ItError_NoError) {
			return err;
		}
	}
	return ItError_NoError;
}
ItError_t (*logSignals) (void) = logSignals_Implementation;

static ItError_t readSignalValue(ItSignal_t* signal) {
	void (*getter)(void) = signal->getter;
	if(getter == NULL){
		return ItError_NoGetter;
	}

	ItCommandResult_t result;
	result.name = signal->name;
	switch (signal->valueType) {
	case ItValueType_Int8:
		result.valueType = ItValueType_Int8;
		result.resultInt8 = ((signed char (*) (void)) getter)();
		break;
	case ItValueType_Uint8:
		result.valueType = ItValueType_Uint8;
		result.resultUint8 = ((unsigned char (*) (void)) getter)();
		break;
	case ItValueType_Ulong:
		result.valueType = ItValueType_Ulong;
		result.resultUlong = ((unsigned long (*) (void)) getter)();
		break;
	case ItValueType_Float:
		result.valueType = ItValueType_Float;
		result.resultFloat = ((float (*) (void)) getter)();
		break;
	default:
		return ItError_InvalidValueType;
	}
	return sendResult(&result);
}