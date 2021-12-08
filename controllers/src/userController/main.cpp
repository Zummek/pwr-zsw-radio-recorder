#include "AppRadio.hpp"
#include "Controllers.hpp"
#include "Equalizer.hpp"
#include "Lcd.hpp"
#include "I2CBus.hpp"

#include <Arduino.h>
#include <Wire.h>

void onRDSChange();

void sendCmdToI2CDev(I2CAddresses dstDevice, uint8_t cmd);
void sendCmdToI2CDevWithArg(I2CAddresses dstDevice,
    uint8_t cmd, const char* cmdArg);

bool hasUserStartedRecording = false;

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("User controller is starting..");

    Wire.begin();
    Controllers::init();
    AppRadio::init(&onRDSChange);
    Equalizer::init();
    Lcd::init();

    Serial.println("User controller is ready!");
    Serial.println();
}

void loop()
{
    AppRadio::checkRDS();
    RecAction action = Controllers::readAndProcess();

    switch (action)
    {
    case RecAction::startRecAction:
        sendCmdToI2CDevWithArg(
            I2CAddresses::audioController,
            static_cast<uint8_t>(
                I2CCommands::audioController::startRecording),
            AppRadio::rdsText);
        Lcd::displayRecording("R");
        hasUserStartedRecording = true;
        break;
    case RecAction::stopRecAction:
        if (!hasUserStartedRecording) break;
        sendCmdToI2CDev(
            I2CAddresses::audioController,
            static_cast<uint8_t>(
                I2CCommands::audioController::stopRecording));
        Lcd::displayRecording(" ");
        hasUserStartedRecording = false;
        break;
    default:
        break;
    }
    Equalizer::process();
}

void onRDSChange()
{
    if (hasUserStartedRecording)
    {
        sendCmdToI2CDev(
            I2CAddresses::audioController,
            static_cast<uint8_t>(
                I2CCommands::audioController::stopRecording));
    }
    else
    {
        sendCmdToI2CDev(
            I2CAddresses::audioController,
            static_cast<uint8_t>(
                I2CCommands::audioController::cancelRecording));
    }
    sendCmdToI2CDevWithArg(
        I2CAddresses::audioController,
        static_cast<uint8_t>(
            I2CCommands::audioController::startRecording),
        AppRadio::rdsText);
}

void sendCmdToI2CDev(I2CAddresses dstDevice, uint8_t cmd)
{
    Wire.beginTransmission(
        static_cast<uint8_t>(dstDevice));
    Wire.write((cmd));
    Wire.endTransmission();
}

void sendCmdToI2CDevWithArg(I2CAddresses dstDevice,
    uint8_t cmd, const char* cmdArg)
{
    byte I2CMsg[I2C_MAX_ARG_SIZE + 1];
    I2CMsg[0] = static_cast<uint8_t>(cmd);

    static constexpr uint8_t maxArgLen
        = I2C_MAX_ARG_SIZE - 1;
    uint8_t argLen = strlen(cmdArg);
    if (argLen > maxArgLen) argLen = maxArgLen;
    strncpy(reinterpret_cast<char*>(&I2CMsg[1]),
        cmdArg, argLen);
    I2CMsg[argLen] = '\0';

    Wire.beginTransmission(
        static_cast<uint8_t>(dstDevice));
    Wire.write(I2CMsg, argLen + 1);
    Wire.endTransmission();
}
