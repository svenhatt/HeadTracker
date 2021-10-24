/*
 * This file is part of the Head Tracker distribution (https://github.com/dlktdr/headtracker)
 * Copyright (c) 2021 Cliff Blackburn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "soc_flash.h"
#include "io.h"
#include "sense.h"
#include "base64.h"
#include "SBUS/uarte_sbus.h"

#include "trackersettings.h"

TrackerSettings::TrackerSettings()
{
    // Set defaults for Settings
    #define DS(DT, NAME, DEFAULT, MINV, MAXV, ONCH) NAME = DEFAULT;
        SETTINGS
    #undef DS

    // Set Initial Values for Data Variables
    #define DV(DT, NAME, DIV, ROUND) NAME = 0;
        DATA_VARS
    #undef DV

    // Fill all arrays to zero
    #define DA(DT, NAME, SIZE, DIV) memset(NAME, 0, sizeof(DT)*SIZE);
        DATA_ARRAYS
    #undef DA

    // Default data outputs, Pan,Tilt,Roll outputs and Inputs for Graph and Output bars
    senddatavars = 0;
    senddataarray = 0;

    btpairedaddress[0] = 0;

    // Setup button input & ppm output pins, bluetooth
    setButtonPin(buttonpin);
    setPpmInPin(ppminpin);
    setPpmOutPin(ppmoutpin);
    setBlueToothMode(btmode);
}


void TrackerSettings::setRollReversed(bool value)
{
    if(value)
        servoreverse |= HT_ROLL_REVERSE_BIT;
    else
        servoreverse &= (HT_ROLL_REVERSE_BIT ^ 0xFFFF);
}

void TrackerSettings::setPanReversed(bool value)
{
    if(value)
        servoreverse |= HT_PAN_REVERSE_BIT;
    else
        servoreverse &= (HT_PAN_REVERSE_BIT ^ 0xFFFF);
}

void TrackerSettings::setTiltReversed(bool value)
{
    if(value)
        servoreverse |= HT_TILT_REVERSE_BIT;
    else
        servoreverse &= (HT_TILT_REVERSE_BIT ^ 0xFFFF);
}

bool TrackerSettings::isRollReversed()
{
    return (servoreverse & HT_ROLL_REVERSE_BIT);
}

bool TrackerSettings::isTiltReversed()
{
    return (servoreverse & HT_TILT_REVERSE_BIT);
}

bool TrackerSettings::isPanReversed()
{
    return (servoreverse & HT_PAN_REVERSE_BIT);
}


//----------------------------------------------------------------------------------------
// Remappable Buttons + PPM Output Pin + Bluetooth

void TrackerSettings::setButtonPin(int value)
{
    if(value > 1 && value < 14) {
        if(buttonpin > 0)
            pinMode(D_TO_32X_PIN(buttonpin),GPIO_INPUT); // Disable old button pin
        pinMode(D_TO_32X_PIN(value),INPUT_PULLUP);    // Button as Input
        buttonpin = value; // Save

    // Disable the Button Pin
    } else if (value < 0) {
        if(buttonpin > 0)
            pinMode(D_TO_32X_PIN(buttonpin),GPIO_INPUT); // Disable old button pin
        buttonpin = -1;
    }
}


void TrackerSettings::setPWMCh(int pwmno, int pwmch)
{
    if(pwmno >= 0 && pwmno <= 3) {
        if(pwmch > 0 && pwmch <= 16)
            switch(pwmno) {
                case 0:
                    pwm0 = pwmch;
                    break;
                case 1:
                    pwm1 = pwmch;
                    break;
                case 2:
                    pwm2 = pwmch;
                    break;
                case 3:
                    pwm3 = pwmch;
                    break;
            }
    }
}
//---------------------------------
// Bluetooth Settings

void TrackerSettings::setBLEValues(uint16_t vals[BT_CHANNELS])
{
    memcpy(btch,vals,sizeof(uint16_t)*BT_CHANNELS);
}


void TrackerSettings::setPairedBTAddress(const char *ha)
{
    // set to "" for pair to first available
    strncpy(btpairedaddress, ha, sizeof(btpairedaddress));
}

const char * TrackerSettings::pairedBTAddress()
{
    return btpairedaddress;
}

//----------------------------------------------------------------
// Orentation

void TrackerSettings::setOrientation(int rx, int ry, int rz)
{
    rotx = rx;
    roty = ry;
    rotz = rz;
    reset_fusion(); // Cause imu to reset
}

void TrackerSettings::orientRotations(float rot[3])
{
    rot[0] = rotx;
    rot[1] = roty;
    rot[2] = rotz;
}

//----------------------------------------------------------------------------------------
// Data sent the PC, for calibration and info

void TrackerSettings::setBLEAddress(const char *addr)
{
    strcpy(btaddr, addr);
}

void TrackerSettings::setDiscoveredBTHead(const char *addr)
{
    strcpy(btrmt, addr);
}

void TrackerSettings::setPPMInValues(uint16_t vals[16])
{
    for(int i=0;i<16;i++)
        ppmch[i] = vals[i];
}

void TrackerSettings::setSBUSValues(uint16_t vals[16])
{
    for(int i=0;i<16;i++)
        sbusch[i] = vals[i];
}

void TrackerSettings::setChannelOutValues(uint16_t vals[16])
{
    for(int i=0;i<16;i++)
        chout[i] = vals[i];
}

void TrackerSettings::setRawGyro(float x, float y, float z)
{
    gyrox = x;
    gyroy = y;
    gyroz = z;
}

void TrackerSettings::setRawAccel(float x, float y, float z)
{
    accx = x;
    accy = y;
    accz = z;
}

void TrackerSettings::setRawMag(float x, float y, float z)
{
    magx = x;
    magy = y;
    magz = z;
}

void TrackerSettings::setOffGyro(float x, float y, float z)
{
    off_gyrox = x;
    off_gyroy = y;
    off_gyroz = z;
}

void TrackerSettings::setOffAccel(float x, float y, float z)
{
    off_accx = x;
    off_accy = y;
    off_accz = z;
}

void TrackerSettings::setOffMag(float x, float y, float z)
{
    off_magx = x;
    off_magy = y;
    off_magz = z;
}

void TrackerSettings::setRawOrient(float t, float r, float p)
{
    tilt=t;
    roll=r;
    pan=p;
}

void TrackerSettings::setOffOrient(float t, float r, float p)
{
    tiltoff=t;
    rolloff=r;
    panoff=p;
}

void TrackerSettings::setPPMOut(uint16_t t, uint16_t r, uint16_t p)
{
    tiltout=t;
    rollout=r;
    panout=p;
}

void TrackerSettings::setQuaternion(float q[4])
{
    memcpy(quat, q,sizeof(float)*4);
}

//--------------------------------------------------------------------------------------
// Send and receive the data from PC
// Takes the JSON and loads the settings into the local class

void TrackerSettings::loadJSONSettings(DynamicJsonDocument &json)
{
// Channels
    JsonVariant v,v1,v2;

    #define DS(DT, NAME, DEFAULT, MINV, MAXV, ONCH) v = json[#NAME]; if(!v.isNull()) set##NAME(v);
        SETTINGS
    #undef DS

    v = json["btpair"]; if(!v.isNull()) setPairedBTAddress(v);


// Button and Pins
    bool setpins=false;
    int bp=buttonpin;
    int ppmi=ppminpin;
    int ppmo=ppmoutpin;

    v = json["buttonpin"]; if(!v.isNull()) {bp=v; setpins=true;}
    v = json["ppmoutpin"]; if(!v.isNull()) {ppmo=v; setpins=true;}
    v = json["ppminpin"]; if(!v.isNull()) {ppmi=v; setpins=true;}

    // Check and make sure none are the same if they aren't disabled
    if(setpins && (
       (bp   > 0 && (bp == ppmi || bp == ppmo)) ||
       (ppmi > 0 && (ppmi == bp || ppmi == ppmo)) ||
       (ppmo > 0 && (ppmo == bp || ppmo == ppmi)))) {
        serialWriteln("HT: FAULT! Setting Pins, cannot have duplicates");
    } else {
        // Disable all pins first, so no conflicts on change
        setButtonPin(-1);
        setPpmInPin(-1);
        setPpmOutPin(-1);

        // Enable them all
        setButtonPin(bp);
        setPpmInPin(ppmi);
        setPpmOutPin(ppmo);
    }
}

void TrackerSettings::setJSONSettings(DynamicJsonDocument &json)
{
    // Store XMacro values in JSON
    // Type, Name, Default, Min, Max
    #define DS(DT, NAME, DEFAULT, MINV, MAXV) json[ #NAME ] = NAME;
        SETTINGS
    #undef DS

// Misc Settings (Non Min/Max)
    json["btpair"] = btpairedaddress;
}

// Saves current data to flash
void TrackerSettings::saveToEEPROM()
{
    char buffer[TX_RNGBUF_SIZE];
    DynamicJsonDocument json(JSON_BUF_SIZE);
    setJSONSettings(json);
    int len = serializeJson(json,buffer,TX_RNGBUF_SIZE);

    if(socWriteFlash(buffer,len)) {
        serialWriteln("HT: Flash Write Failed");
    } else {
        serialWriteln("HT: Saved to Flash");
    }
}

// Called on startup to read the data from Flash

void TrackerSettings::loadFromEEPROM()
{
    // Load Settings
    DynamicJsonDocument json(JSON_BUF_SIZE);
    DeserializationError de;
    de = deserializeJson(json, get_flashSpace());

    if(de != DeserializationError::Ok)
        serialWriteln("HT: Invalid JSON Data");

    if(json["UUID"] == 837727) {
        serialWriteln("HT: Device has been freshly programmed, no data found");

    } else {
        serialWriteln("HT: Loading settings from flash");
        loadJSONSettings(json);
    }
}

/* Sets if a data item should be included while in data to GUI
 */

void TrackerSettings::setDataItemSend(const char *var, bool enabled)
{
    int id=0;

    // Macro Expansion for Data Variables + Arrays
    #define DV(DT, NAME, DIV, ROUND)\
    if(strcmp(var,#NAME)==0)\
    {\
        enabled==true?senddatavars|=1<<id:senddatavars&=~(1<<id);\
        return;\
    }\
    id++;
        DATA_VARS
    #undef DV

    id=0;

    #define DA(DT, NAME, SIZE, DIV)\
    if(strcmp(var,#NAME)==0)\
    {\
        enabled==true?senddataarray|=1<<id:senddataarray&=~(1<<id);\
        return;\
    }\
    id++;
        DATA_ARRAYS
    #undef DA
}

/* Stops all Data Items from Sending
 */

void TrackerSettings::stopAllData()
{
    senddatavars = 0;
    senddataarray = 0;
}

/* Returns a list of all the Data Variables available in json
 */

void TrackerSettings::setJSONDataList(DynamicJsonDocument &json)
{
    JsonArray array = json.createNestedArray();

    // Macro Expansion for Data Variables + Arrays
    #define DV(DT, NAME, DIV, ROUND)\
        array.add(#NAME);
        DATA_VARS
    #undef DV

    #define DA(DT, NAME, SIZE, DIV)\
        array.add(#NAME);
        DATA_ARRAYS
    #undef DA
}

// Used to transmit raw data back to the GUI
void TrackerSettings::setJSONData(DynamicJsonDocument &json)
{
    static int counter=0;
    // Macro Expansion for Data Variables + Arrays

    // Sends only requested data items
    // Updates only as often as specified, 1 = every cycle
    // Three Decimals is most precision of any data item req as of now.
    // For most items ends up less bytes than base64 encoding everything
    int id=0;
    int itemcount=0;
    #define DV(DT, NAME, DIV, ROUND)\
    if(senddatavars & 1<<id && counter % DIV == 0) {\
        if(ROUND == -1)\
            json[#NAME] = NAME;\
        else\
            json[#NAME] = roundf(((float)NAME * ROUND)) / ROUND;\
        itemcount++;\
    }\
    id++;
        DATA_VARS
    #undef DV

    // Send only requested data arrays, arrays are base64 encoded
    // Variable names prepended by 6 so GUI knows to decode them
    // Suffixed with the 3 character data type
    // Length can be determined with above info

    // If DIVisor less than zero only send on change.
    // If less than -1 update on cycle count or change

    id=0;
    char b64array[500];
    bool sendit=false;

    #define DA(DT, NAME, SIZE, DIV)\
    sendit=false;\
    if(senddataarray & 1<<id) {\
        if(DIV < 0) { \
            if(memcmp(last ## NAME, NAME, sizeof(NAME)) != 0)\
                sendit = true;\
            else if(DIV != -1 && counter % abs(DIV) == 0)\
                sendit = true;\
        } else { \
            if(counter % DIV == 0)\
                sendit = true;\
        }\
        if(sendit) {\
            encode_base64((unsigned char*)NAME, sizeof(DT)*SIZE,(unsigned char*)b64array);\
            json["6" #NAME #DT] = b64array;\
            memcpy(last ## NAME, NAME, sizeof(NAME));\
        }\
    }\
    id++;
        DATA_ARRAYS
    #undef DA

    // Used for reduced data divisor
    counter++;
    if(counter > 500) {
        counter = 0;
    }
}

