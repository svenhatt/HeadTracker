#pragma once

#define ARDUINOJSON_USE_DOUBLE 0
#include <include/arduinojsonwrp.h>
#include <zephyr.h>
#include "PPM/PPMOut.h"
#include "PPM/PPMIn.h"
#include "bluetooth/btparahead.h"
#include "bluetooth/btpararmt.h"
#include "serial.h"

// Defaults, Mins/Maxes
#define MIN_PIN -1
#define MAX_PIN 13
#define MIN_PWM 988
#define MAX_PWM 2012
#define DEF_MIN_PWM 1050
#define DEF_MAX_PWM 1950
#define MINMAX_RNG 242
#define MIN_CNT (((MAX_PWM-MIN_PWM)/2)+MIN_PWM-MINMAX_RNG)
#define MAX_CNT (((MAX_PWM-MIN_PWM)/2)+MIN_PWM+MINMAX_RNG)
#define HT_TILT_REVERSE_BIT 0x01
#define HT_ROLL_REVERSE_BIT 0x02
#define HT_PAN_REVERSE_BIT 0x04
#define CHANNEL_COUNT 16
#define DEF_PPM_CHANNELS 8
#define DEF_PPM_FRAME 22500
#define PPM_MAX_FRAME 40000
#define PPM_MIN_FRAME 12500
#define PPM_MIN_FRAMESYNC 4000 // Not adjustable
#define DEF_PPM_SYNC 350
#define PPM_MAX_SYNC 800
#define PPM_MIN_SYNC 100
#define DEF_BOARD_ROT_X 0
#define DEF_BOARD_ROT_Y 0
#define DEF_BOARD_ROT_Z 0
#define DEF_BUTTON_LONG_PRESS true
#define DEF_BUTTON_PIN 2 // Chosen because it's beside ground
#define DEF_PPM_OUT_PIN 10 // Random choice
#define DEF_PPM_IN_PIN -1
#define PPM_CENTER 1500
#define SBUS_CENTER 992
#define SBUS_SCALE 1.6f
#define MIN_GAIN 0.0
#define MAX_GAIN 35.0
#define DEF_GAIN 5.0
#define DEF_BT_MODE BTDISABLE // Bluetooth Disabled
#define DEF_RST_PPM -1
#define DEF_TILT_CH 1
#define DEF_ROLL_CH 2
#define DEF_PAN_CH 3
#define DEF_LP_PAN 75
#define DEF_LP_TLTRLL 75
#define DEF_PWM_A0_CH -1
#define DEF_PWM_A1_CH -1
#define DEF_PWM_A2_CH -1
#define DEF_PWM_A3_CH -1
#define DEF_SBUS_IN_INV false
#define DEF_SBUS_OUT_INV false
#define MIN_SBUS_RATE 30
#define MAX_SBUS_RATE 150
#define DEF_SBUS_RATE 60
#define DEF_ALG_A4_CH -1
#define DEF_ALG_A5_CH -1
#define DEF_ALG_A6_CH -1
#define DEF_ALG_A7_CH -1
#define DEF_ALG_GAIN 310.00f
#define DEF_ALG_OFFSET 0
#define DEF_AUX_CH0 -1
#define DEF_AUX_CH1 -1
#define DEF_AUX_CH2 -1
#define DEF_AUX_FUNC 0

// Variables to be sent back to GUI if enabled
// Datatype, Name, UpdateDivisor, RoundTo
#define DATA_VARS\
    DV(float,magx,      1,1000)\
    DV(float,magy,      1,1000)\
    DV(float,magz,      1,1000)\
    DV(float,gyrox,     1,1000)\
    DV(float,gyroy,     1,1000)\
    DV(float,gyroz,     1,1000)\
    DV(float,accx,      1,1000)\
    DV(float,accy,      1,1000)\
    DV(float,accz,      1,1000)\
    DV(float,off_magx,  5,1000)\
    DV(float,off_magy,  5,1000)\
    DV(float,off_magz,  5,1000)\
    DV(float,off_gyrox, 5,1000)\
    DV(float,off_gyroy, 5,1000)\
    DV(float,off_gyroz, 5,1000)\
    DV(float,off_accx,  5,1000)\
    DV(float,off_accy,  5,1000)\
    DV(float,off_accz,  5,1000)\
    DV(float,tilt,      5,1000)\
    DV(float,roll,      5,1000)\
    DV(float,pan,       5,1000)\
    DV(float,tiltoff,   1,1000)\
    DV(float,rolloff,   1,1000)\
    DV(float,panoff,    1,1000)\
    DV(uint16_t,tiltout,1,-1)\
    DV(uint16_t,rollout,1,-1)\
    DV(uint16_t,panout, 1,-1)\
    DV(bool,isCalibrated,2,-1)\
    DV(bool,btcon,      10,-1)\
    DV(bool,isSense,      10,-1)\
    DV(bool,trpenabled,  10,-1)\
    DV(uint8_t, cpuuse, 1,-1)

// To shorten names, as these are sent to the GUI for decoding
#define s8 int8_t
#define u8  uint8_t
#define u16 uint16_t
#define s16 int16_t
#define u32 uint32_t
#define s32 int32_t
#define flt float
#define chr char

#define FLOAT_MINIMUM -1000000.0f
#define FLOAT_MAXIMUM 1000000.0f
#define MIN_CHANNEL -1
#define MAX_CHANNEL CHANNEL_COUNT+1

// Arrays to be sent back to GUI if enabled, Base64 Encoded
// Datatype, Name, Size, UpdateDivisor
#define DATA_ARRAYS\
    DA(u16, chout, 16, 1)\
    DA(u16, btch, BT_CHANNELS, 1)\
    DA(u16, ppmch, 16, 1)\
    DA(u16, sbusch, 16, 1)\
    DA(flt, quat,4, 1)\
    DA(chr, btaddr,18, 20)\
    DA(chr, btrmt,18, -100)

// Basic Settings with a Min/Max
// Type, name, default, min, max
#define SETTINGS\
    DS(u16, tlt_min, DEF_MIN_PWM, MIN_PWM, MAX_PWM)\
    DS(u16, tlt_max, DEF_MAX_PWM, MIN_PWM, MAX_PWM)\
    DS(u16, tlt_cnt, PPM_CENTER,  MIN_PWM, MAX_PWM)\
    DS(u16, rll_min, DEF_MIN_PWM, MIN_PWM, MAX_PWM)\
    DS(u16, rll_max, DEF_MAX_PWM, MIN_PWM, MAX_PWM)\
    DS(u16, rll_cnt, PPM_CENTER,  MIN_PWM, MAX_PWM)\
    DS(u16, pan_min, DEF_MIN_PWM, MIN_PWM, MAX_PWM)\
    DS(u16, pan_max, DEF_MAX_PWM, MIN_PWM, MAX_PWM)\
    DS(u16, pan_cnt, PPM_CENTER,  MIN_PWM, MAX_PWM)\
    DS(float ,tlt_gain, DEF_GAIN, MIN_GAIN, MAX_GAIN)\
    DS(float, rll_gain, DEF_GAIN, MIN_GAIN, MAX_GAIN)\
    DS(float, pan_gain, DEF_GAIN, MIN_GAIN, MAX_GAIN)\
    DS(s8, tltch, DEF_TILT_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, rllch, DEF_ROLL_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, panch, DEF_PAN_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(float, magxoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, magyoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, magzoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, accxoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, accyoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, acczoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, gyrxoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, gyryoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, gyrzoff, 0, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(int, rotx, 0, -360, 360)\
    DS(int, roty, 0, -360, 360)\
    DS(int, rotz, 0, -360, 360)\
    DS(u16, servoreverse, 0, 0, 0x07)\
    DS(u8, lppan, DEF_LP_PAN, 0, 100)\
    DS(u8, lptiltroll, DEF_LP_TLTRLL, 0, 0)\
    DS(u8, buttonpin, DEF_BUTTON_PIN, MIN_PIN, MAX_PIN)\
    DS(u8, ppmoutpin, DEF_PPM_OUT_PIN, MIN_PIN, MAX_PIN)\
    DS(u8, ppminpin, DEF_PPM_IN_PIN, MIN_PIN, MAX_PIN)\
    DS(u8, butlngps, DEF_BUTTON_LONG_PRESS, 0, 1)\
    DS(u8, ppmoutinvert, false, 0, 1)\
    DS(u8, ppmininvert, false, 0, 1)\
    DS(u8, btmode, DEF_BT_MODE, BTDISABLE, BTCOUNT)\
    DS(u8, rstonwave, false, 0, 1)\
    DS(u8, rstppm, DEF_RST_PPM, 0, 1)\
    DS(u16, ppmfrm, DEF_PPM_FRAME, 0, 1)\
    DS(u16, ppmsync, DEF_PPM_SYNC, 0, 1)\
    DS(s8, ppmchcnt, DEF_PPM_CHANNELS, 0, 1)\
    DS(s8, an4ch, DEF_ALG_A4_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, an5ch, DEF_ALG_A5_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, an6ch, DEF_ALG_A6_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, an7ch, DEF_ALG_A6_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(float, an4gain, DEF_ALG_GAIN, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, an5gain, DEF_ALG_GAIN, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, an6gain, DEF_ALG_GAIN, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, an7gain, DEF_ALG_GAIN, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, an4off, DEF_ALG_OFFSET, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, an5off, DEF_ALG_OFFSET, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, an6off, DEF_ALG_OFFSET, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, an7off, DEF_ALG_OFFSET, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(s8, aux0ch, DEF_AUX_CH0, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, aux1ch, DEF_AUX_CH1, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, aux2ch, DEF_AUX_CH2, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, aux0func, DEF_AUX_FUNC, -1, AUX_COUNT)\
    DS(s8, aux1func, DEF_AUX_FUNC, -1, AUX_COUNT)\
    DS(s8, aux2func, DEF_AUX_FUNC, -1, AUX_COUNT)\
    DS(u8, sboutinv, DEF_SBUS_OUT_INV, 0, 1)\
    DS(u8, sbininv, DEF_SBUS_IN_INV, 0, 1)\
    DS(u8, sbrate, DEF_SBUS_RATE,MIN_SBUS_RATE, MAX_SBUS_RATE)\
    DS(s8, pwm0, DEF_PWM_A0_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, pwm1, DEF_PWM_A1_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, pwm2, DEF_PWM_A2_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(s8, pwm3, DEF_PWM_A3_CH, MIN_CHANNEL, MAX_CHANNEL)\
    DS(float, so00, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so01, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so02, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so10, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so11, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so12, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so20, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so21, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\
    DS(float, so22, 0.0f, FLOAT_MINIMUM, FLOAT_MAXIMUM)\


// Global Config Values
class TrackerSettings
{
public:
    enum {AUX_DISABLE=-1,
        AUX_GYRX, // 0
        AUX_GYRY, // 1
        AUX_GYRZ, // 2
        AUX_ACCELX, // 3
        AUX_ACCELY, // 4
        AUX_ACCELZ, // 5
        AUX_ACCELZO, // 6
        AUX_BT_RSSI, // 7
        AUX_COUNT};

    static constexpr int MAX_DATA_VARS = 30;

    TrackerSettings();

    // Create getter functions
    #define DS(DT, NAME, DEFAULT, MINV, MAXV) DT get##NAME() {return NAME;}
        SETTINGS
    #undef DS

    // Create setter functions
    #define DS(DT, NAME, DEFAULT, MINV, MAXV) void set##NAME( DT V )\
    {\
        if(V < MINV)\
         V = MINV;\
        else if(V > MAXV)\
         V = MAXV;\
         NAME = V;\
         }
        SETTINGS
    #undef DS

    void setRollReversed(bool value);
    void setPanReversed(bool Value);
    void setTiltReversed(bool Value);
    bool isRollReversed();
    bool isTiltReversed();
    bool isPanReversed();

    void setTRPEnabled(bool v) {trpenabled = v;}

    int resetCntPPM() const;
    void setResetCntPPM(int value);

    bool resetOnWave() const;
    void setResetOnWave(bool value);

    void gyroOffset(float &x, float &y, float &z) const;
    void setGyroOffset(float x,float y, float z);

    void accOffset(float &x, float &y, float &z) const;
    void setAccOffset(float x,float y, float z);

    void magOffset(float &x, float &y, float &z) const;
    void setMagOffset(float x,float y, float z);

    bool isBlueToothConnected() {return btcon;}
    void setBlueToothConnected(bool con) {btcon = con;}

    void setPairedBTAddress(const char *ha);
    const char* pairedBTAddress();

    void setOrientation(int rx, int ry, int rz);
    void orientRotations(float rot[3]);

    void magSiOffset(float v[]) {
        float magsioff[9] = {so00, so01, so02, so10, so11, so12, so20, so21, so22};
        memcpy(v,magsioff,sizeof(magsioff));
        }
    void setMagSiOffset(float v[]) {
        so00 = v[0];
        so01 = v[1];
        so02 = v[2];
        so10 = v[3];
        so11 = v[4];
        so12 = v[5];
        so20 = v[6];
        so21 = v[7];
        so22 = v[8];
        }

// PWM Channels
    void setPWMCh(int pwmno, int pwmch);
    int PWMCh(int pwmno) {
         switch(pwmno) {
             case 0:
                return pwm0;
                break;
             case 1:
                return pwm1;
                break;
             case 2:
                return pwm2;
                break;
             case 3:
                return pwm3;
                break;
         }
        return -1;
        }

    void loadJSONSettings(DynamicJsonDocument &json);
    void setJSONSettings(DynamicJsonDocument &json);

    void saveToEEPROM();
    void loadFromEEPROM();

// Setting of data to be returned to the GUI
    void setRawGyro(float x, float y, float z);
    void setRawAccel(float x, float y, float z);
    void setRawMag(float x, float y, float z);
    void setRawOrient(float t, float r, float p);
    void setOffGyro(float x, float y, float z);
    void setOffAccel(float x, float y, float z);
    void setOffMag(float x, float y, float z);
    void setOffOrient(float t, float r, float p);
    void setPPMOut(uint16_t t, uint16_t r, uint16_t p);
    void setJSONData(DynamicJsonDocument &json);
    void setBLEAddress(const char *addr);
    void setDiscoveredBTHead(const char* addr);
    void setBLEValues(uint16_t vals[BT_CHANNELS]);
    void setSenseboard(bool sense);
    void setSBUSValues(uint16_t vals[16]);
    void setPPMInValues(uint16_t vals[16]);
    void setChannelOutValues(uint16_t vals[16]);
    void setQuaternion(float q[4]);
    void setDataItemSend(const char *var, bool enabled);
    void stopAllData();
    void setJSONDataList(DynamicJsonDocument &json);

private:
    // Bit map of data to send to GUI, max 64 items
    uint64_t senddatavars;
    uint32_t senddataarray;

    // BT Address for remote mode to pair with
    char btpairedaddress[17];

    // Device Settings from X Macro
    #define DS(DT, NAME, DEFAULT, MINV, MAXV) DT NAME;
        SETTINGS
    #undef DS

    // Define Data Variables from X Macro
    #define DV(DT, NAME, DIV, ROUND) DT NAME;
        DATA_VARS
    #undef DV

    // Define Data Arrays from X Macro
    #define DA(DT, NAME, SIZE, DIV) DT NAME[SIZE];
        DATA_ARRAYS
    #undef DA

    // Define Last Arrays from X Macro
    #define DA(DT, NAME, SIZE, DIV) DT last ## NAME [SIZE];
        DATA_ARRAYS
    #undef DA
};

extern TrackerSettings trkset;