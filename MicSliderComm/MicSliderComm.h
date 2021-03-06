#ifndef MIC_SLIDER_COMM_H
#define MIC_SLIDER_COMM_H

#include "Arduino.h"

#define UNIQUE_SIGNATURE_BYTE_ONE 0xFF
#define UNIQUE_SIGNATURE_BYTE_TWO 0xFE

// Max/min limits for controller one position
#define C_ONE_MIN_X_POS 1000
#define C_ONE_MAX_X_POS 2000
#define C_ONE_MIN_Y_POS 1000
#define C_ONE_MAX_Y_POS 2000

// Max/min limits for controller two position
#define C_TWO_MIN_X_POS 1000
#define C_TWO_MAX_X_POS 2000
#define C_TWO_MIN_Y_POS 1000
#define C_TWO_MAX_Y_POS 2000

typedef struct controlDataStruct
{
  int x;
  int y;

  controlDataStruct() {
    this->x = 0;
    this->y = 0;
  }
} controlData, *pcontrolData;


typedef struct packetStruct
{
  controlData controllerOne;
  controlData controllerTwo;

  packetStruct() : controllerOne(), controllerTwo()
  {
  }
} packet;

class MicSliderCommSender
{
  public:
    MicSliderCommSender();
    void sendControlData();

    void setControllerOneData(int x, int y);
    void setControllerTwoData(int x, int y);
    
  private:
    packet m_sendBuffer;

    void sendUniqueSignature();
};


class MicSliderCommReceiver
{
  public:
    MicSliderCommReceiver();
    void receiveControlData();

    void getControllerOneData(int *x, int *y);
    void getControllerTwoData(int *x, int *y);
    
  private:
    typedef enum { locatingHeader = 0, firstSignatureByteFound, readData, readDatafirstSignatureByteFound } state;

    void resetBuf();
    int writeByteToBuf(unsigned char byte);

    state           m_state;
    packet          m_receiveBuffer;
    int             m_bytesLeft;
    unsigned char  *m_bufferPtr;
};



#endif