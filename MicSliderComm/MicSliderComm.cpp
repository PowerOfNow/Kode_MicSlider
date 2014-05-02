#include "MicSliderComm.h"

MicSliderCommSender::MicSliderCommSender() :
m_sendBuffer()
{
}

void MicSliderCommSender::sendUniqueSignature()
{
  Serial.write(UNIQUE_SIGNATURE_BYTE_ONE);
  Serial.write(UNIQUE_SIGNATURE_BYTE_TWO);
}

void MicSliderCommSender::setControllerOneData(int x, int y)
{
  m_sendBuffer.controllerOne.x = x;
  m_sendBuffer.controllerOne.y = y;
}

void MicSliderCommSender::setControllerTwoData(int x, int y)
{
  m_sendBuffer.controllerTwo.x = x;
  m_sendBuffer.controllerTwo.y = y;
}

void MicSliderCommSender::sendControlData()
{
  sendUniqueSignature();

  unsigned char* ptr = (unsigned char*) &m_sendBuffer;
  for (int i = 0; i < sizeof(packet); ++i, ++ptr) {
    Serial.write(*ptr);
  }
  Serial.flush();
}


MicSliderCommReceiver::MicSliderCommReceiver() :
m_receiveBuffer(), m_bufferPtr(NULL), m_bytesLeft(0), m_state(locatingHeader)
{
}

void MicSliderCommReceiver::receiveControlData()
{
  int inValue = 0;
  int remaining = 0;
  unsigned char curByte = 0;
  //while ((c = fgetc(fd)) != EOF) {
  while (true) { // Keep going until we have read a full packet
    while (Serial.available() > 0) {
      inValue = Serial.read();
      curByte = (unsigned char) (inValue & 0xFF);
      
      switch (m_state) {
      case locatingHeader:
        if (curByte == UNIQUE_SIGNATURE_BYTE_ONE) {
          m_state = firstSignatureByteFound;
        }
        break;
      case firstSignatureByteFound:
        if (curByte == UNIQUE_SIGNATURE_BYTE_TWO) {
          m_state = readData;
          resetBuf();
        }
        else {
          m_state = locatingHeader;
        }
        break;
      
      case readDatafirstSignatureByteFound:
        if (curByte == UNIQUE_SIGNATURE_BYTE_TWO) {
          resetBuf();
          break;
        }
        m_state = readData;
        // No break here.
      case readData:
        if (curByte == UNIQUE_SIGNATURE_BYTE_ONE) {
          m_state = readDatafirstSignatureByteFound;
        }
        remaining = writeByteToBuf(curByte);
        if (remaining == 0) {
          m_state = locatingHeader;
          // A complete packet has been read. Our task is done.
          return;
        }
        break;
      }
    }
    delay(5); // There was no data on the serial port. Wait a few milliseconds.
  }
  // We should never reach this point.
}

void MicSliderCommReceiver::getControllerOneData(int *x, int *y)
{
  *x = m_receiveBuffer.controllerOne.x;
  *y = m_receiveBuffer.controllerOne.y;
}

void MicSliderCommReceiver::getControllerTwoData(int *x, int *y)
{
  *x = m_receiveBuffer.controllerTwo.x;
  *y = m_receiveBuffer.controllerTwo.y;
}

void MicSliderCommReceiver::resetBuf() {
  m_bufferPtr = (unsigned char*) &m_receiveBuffer;
  m_bytesLeft = sizeof(m_receiveBuffer);
}

int MicSliderCommReceiver::writeByteToBuf(unsigned char byte)
{
  if (m_bytesLeft == 0) {
    return -1;
  }

  *m_bufferPtr = byte;
  m_bufferPtr++; m_bytesLeft--;
  return m_bytesLeft;
}