#pragma once

#include "ble.h"

extern uint16_t chanoverrides;

class BTParaRmt : public BTFunction {
public:
    //static void fff6Written(BLEDevice central, BLECharacteristic characteristic);
    //static void overrideWritten(BLEDevice central, BLECharacteristic characteristic);

    BTParaRmt();
    ~BTParaRmt();
    char *address() {return _address;}
    void execute();
    int mode() {return BTPARARMT;}
    bool isConnected() {return bleconnected;}
    uint16_t getChannel(int chn);
    void sendButtonData(char bd);
    void setChannel(int channel, const uint16_t value);

private:
    int decodeBLEData(uint8_t *buffer, int len, uint16_t *channelvals);

    bool scanning = false;

    /*BLEDevice peripheral;
    BLECharacteristic fff6;
    BLECharacteristic butpress;
    BLECharacteristic overridech;
*/
    /*Timer sbusupdate;
    Ticker ioTick;*/

    char _address[20];
};
