#include "I2CBus.hpp"
#include "Recorder.hpp"

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define AUDIO_INPUT_PIN 7
#define SAMPLING_RATE 44100

#define SD_SS_PIN 10

#define MAX_FILE_NAME_LEN I2C_MAX_STR_SIZE + 10
#define MAX_DUP_FILE_NUM 10

void onI2CReceive(int bytesNum);
void readI2CStrToBuff(char* outBuff, int len);
void startRecording(const char* baseFileName);
void stopRecording();
bool getFileName(const char* baseName, char* outFileName);

bool isRecording = false;

void setup()
{
    Serial.begin(115200);
    Serial.println("Audio controller is starting..");

	if (!SD.begin(SD_SS_PIN))
	{
		Serial.println("Error! SD init failed");
		while (true);
	}

	Recorder::setInputPin(AUDIO_INPUT_PIN);
	Recorder::setSampleRate(SAMPLING_RATE);

    Wire.begin(static_cast<uint8_t>(I2CAddresses::audioController));
    Wire.onReceive(onI2CReceive);

    Serial.println("Audio controller is ready!");
    Serial.println();
}

void loop() {}

void onI2CReceive(int bytesNum)
{
    I2CCommands::audioController cmd =
        static_cast<I2CCommands::audioController>(Wire.read());
    char cmdArg[I2C_MAX_STR_SIZE];
    readI2CStrToBuff(cmdArg, bytesNum - 1);

    switch (cmd)
    {
    case I2CCommands::audioController::startRecording:
        startRecording(cmdArg);
        break;
    case I2CCommands::audioController::stopRecording:
        stopRecording();
        break;
    default:
        break;
    }
}

inline void readI2CStrToBuff(char* outBuff, int len)
{
    int buffIndex = 0;
    int buffLimit = I2C_MAX_STR_SIZE - 1;
    if (len < buffLimit) buffLimit = len;

    while (buffIndex < buffLimit)
    {
        outBuff[buffIndex++] = 
            static_cast<byte>(Wire.read());
    }
    outBuff[buffIndex] = '\0';
}

void startRecording(const char* baseFileName)
{
    if (isRecording) stopRecording();
    
    Serial.print("New recording: ");
    Serial.println(baseFileName);

    char fileName[MAX_FILE_NAME_LEN];
    if (!getFileName(baseFileName, fileName))
    {
        Serial.println("Error! File duplicate limit exceeded");
        return;
    }
	File audioFile = SD.open(fileName, FILE_WRITE);
    if (!audioFile)
    {
        Serial.println("Error! Could not open the file");
        return;
    }
    Recorder::startRecording(audioFile);
    isRecording = true;

    Serial.print("Recording into file: ");
    Serial.println(fileName);
}

void stopRecording()
{
    Recorder::stopRecording();
    isRecording = false;

    Serial.println("Recording stopped");
    Serial.println();
}

bool getFileName(const char* baseName, char* outFileName)
{
    int dupIndex = 0;
    snprintf(outFileName, MAX_FILE_NAME_LEN, "%s.wav", baseName);
    do
    {
        if (!SD.exists(outFileName)) return true;
        snprintf(outFileName, MAX_FILE_NAME_LEN, 
            "%s (%d).wav", baseName, ++dupIndex);
    } while (dupIndex <= MAX_DUP_FILE_NUM);
    return false;
}
