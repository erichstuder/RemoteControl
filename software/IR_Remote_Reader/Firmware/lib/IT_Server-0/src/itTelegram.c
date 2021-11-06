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

#include "itTelegram.h"
#include <string.h>

#define TelegramType_Value 1
#define TelegramType_String 2

#define TelegramStart 0xAA
#define TelegramEnd 0xBB
#define ReplacementMarker 0xCC

#define ItValueType_Int8_Id  1
#define ItValueType_Uint8_Id 2
#define ItValueType_Ulong_Id 3
#define ItValueType_Float_Id 4

static WriteByteToClient_t writeByteToClient;

static ItError_t sendValueTelegramFirstPart(const char* const valueName, unsigned char valueTypeId);
static ItError_t sendValueTelegramSecondPart(uint32_t timestamp);
static ItError_t sendStart(void);
static ItError_t sendEnd(void);
static ItError_t sendContentByte(const unsigned char data);
static ItError_t sendString(const char* const str);
static ItError_t sendTimestamp(uint32_t value);
static ItError_t sendUnsignedLong(uint32_t value);
static ItError_t sendFloat(float value);

void itTelegramInit(WriteByteToClient_t writeByteToClientCallback) {
    writeByteToClient = writeByteToClientCallback;
}

ItError_t itSendValueTelegram_Int8(const char* const valueName, signed char value, uint32_t timestamp) {
    ItError_t err = sendValueTelegramFirstPart(valueName, ItValueType_Int8_Id);
    if (err != ItError_NoError) { return err; }

    err = sendContentByte((unsigned char)value);
    if (err != ItError_NoError) { return err; }

    err = sendValueTelegramSecondPart(timestamp);
    if (err != ItError_NoError) { return err; }

    return ItError_NoError;
}

ItError_t itSendValueTelegram_Uint8(const char* const valueName, unsigned char value, uint32_t timestamp) {
    ItError_t err = sendValueTelegramFirstPart(valueName, ItValueType_Uint8_Id);
    if (err != ItError_NoError) { return err; }

    err = sendContentByte(value);
    if (err != ItError_NoError) { return err; }

    err = sendValueTelegramSecondPart(timestamp);
    if (err != ItError_NoError) { return err; }

    return ItError_NoError;
}

ItError_t itSendValueTelegram_Ulong(const char* const valueName, uint32_t value, uint32_t timestamp) {
    ItError_t err = sendValueTelegramFirstPart(valueName, ItValueType_Ulong_Id);
    if (err != ItError_NoError) { return err; }

    err = sendUnsignedLong(value);
    if (err != ItError_NoError) { return err; }

    err = sendValueTelegramSecondPart(timestamp);
    if (err != ItError_NoError) { return err; }

    return ItError_NoError;
}

ItError_t itSendValueTelegram_Float(const char* const valueName, float value, uint32_t timestamp) {
    ItError_t err = sendValueTelegramFirstPart(valueName, ItValueType_Float_Id);
    if (err != ItError_NoError) { return err; }

    err = sendFloat(value);
    if (err != ItError_NoError) { return err; }

    err = sendValueTelegramSecondPart(timestamp);
    if (err != ItError_NoError) { return err; }

    return ItError_NoError;
}

ItError_t itSendStringTelegram(const char* const str) {
    ItError_t err = sendStart();
    if (err != ItError_NoError) { return err; }

    err = sendContentByte(TelegramType_String);
    if (err != ItError_NoError) { return err; }

    err = sendString(str);
    if (err != ItError_NoError) { return err; }

    err = sendEnd();
    if (err != ItError_NoError) { return err; }

    return ItError_NoError;
}

static ItError_t sendValueTelegramFirstPart(const char* const valueName, unsigned char valueTypeId) {
    ItError_t err = sendStart();
    if (err != ItError_NoError) { return err; }

    err = sendContentByte(TelegramType_Value);
    if (err != ItError_NoError) { return err; }

    err = sendString(valueName);
    if (err != ItError_NoError) { return err; }

    err = sendContentByte(valueTypeId);
    if (err != ItError_NoError) { return err; }

    return ItError_NoError;
}

static ItError_t sendValueTelegramSecondPart(uint32_t timestamp) {
    ItError_t err = sendTimestamp(timestamp);
    if (err != ItError_NoError) { return err; }

    err = sendEnd();
    if (err != ItError_NoError) { return err; }

    return ItError_NoError;
}

static ItError_t sendStart(void) {
    return writeByteToClient(TelegramStart);
}

static ItError_t sendEnd(void) {
    return writeByteToClient(TelegramEnd);
}

static ItError_t sendContentByte(const unsigned char data) {
    ItError_t err;
    switch (data) {
    case TelegramStart:
    case TelegramEnd:
    case ReplacementMarker:
        err = writeByteToClient(ReplacementMarker);
        if (err != ItError_NoError) {
            return err;
        }
        return sendContentByte(data - 1);
        break;
    default:
        return writeByteToClient(data);
        break;
    }
}

static ItError_t sendString(const char* const str) {
    ItError_t err;
    unsigned char n;
    for (n = 0; n < strlen(str)+1; n++) {
        err = sendContentByte(str[n]);
        if (err != ItError_NoError) {
            return err;
        }
    }
    return ItError_NoError;
}

static ItError_t sendTimestamp(uint32_t timestamp) {
    return sendUnsignedLong(timestamp);
}

static ItError_t sendUnsignedLong(uint32_t value) {
    ItError_t err;
    union {
        uint32_t value;
        unsigned char valueBytes[sizeof(uint32_t)];
    } data;
    data.value = value;

    for (unsigned char n = 0; n < sizeof(data.valueBytes); n++) {
        err = sendContentByte(data.valueBytes[n]);
        if (err != ItError_NoError) {
            return err;
        }
    }
    return ItError_NoError;
}

static ItError_t sendFloat(float value) {
    ItError_t err;
    union {
        float value;
        unsigned char valueBytes[sizeof(float)];
    } data;
    data.value = value;

    for (unsigned char n = 0; n < sizeof(data.valueBytes); n++) {
        err = sendContentByte(data.valueBytes[n]);
        if (err != ItError_NoError) {
            return err;
        }
    }
    return ItError_NoError;
}