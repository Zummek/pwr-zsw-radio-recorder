#include "Recorder.hpp"
#include "WavFile.hpp"

#include <Arduino.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFF_SIZE 256

#define ENABLE_NESTED_INTS() sei()

#define POWER_RED_REG PRR
#define POWER_RED_TIMER1 _BV(PRTIM1)
#define POWER_RED_ADC _BV(PRADC)

#define TIMER1_CTRL_REG_A TCCR1A
#define TIMER1_CTRL_REG_B TCCR1B
#define TIMER1_COMP_REG_A OCR1A
#define TIMER1_IN_CAP_REG ICR1
#define TIMER1_INT_MASK_REG TIMSK1
#define TIMER1_INT_REG TIFR1

#define TIMER1_CLOCK_MASK (_BV(CS12) | _BV(CS11) | (_BV(CS10)))
#define TIMER1_CLOCK_NO_CLOCK 0
#define TIMER1_CLOCK_PRESC_X1 _BV(CS10)

#define TIMER1_MODE_MASK_A (_BV(WGM11) | _BV(WGM10))
#define TIMER1_MODE_FAST_PWM_ISR_A _BV(WGM11)
#define TIMER1_MODE_MASK_B (_BV(WGM13) | _BV(WGM12))
#define TIMER1_MODE_FAST_PWM_ISR_B (_BV(WGM13) & _BV(WGM12))

#define TIMER1_OUT_MODE_MASK_A (_BV(COM1A1) | _BV(COM1A0))
#define TIMER1_OUT_MODE_OFF_A 0
#define TIMER1_OUT_MODE_TOGGLE_NO_INV_A (_BV(COM1A1))

#define TIMER1_INT_COMPA_ENABLE _BV(OCIE1A)

#define TIMER1_INT_COMPB_FLAG _BV(OCF1B)

#ifdef AUDIO_PASSTHROUGH
#define TIMER1_OUT_PIN_A 9
#endif

#define ADC_CTRL_REG_A ADCSRA
#define ADC_CTRL_REG_B ADCSRB
#define ADC_IN_REG ADMUX
#define ADC_DATA_REG_H ADCH

#define ADC_ENABLE _BV(ADEN)

#define ADC_REF_VOLT_MASK (_BV(REFS1) | _BV(REFS0))
#define ADC_REF_VOLT_AVCC _BV(REFS0)

#define ADC_PIN_4_BIT_MASK 0x0F
#define ADC_PIN_3_BIT_MASK 0x07

#define ADC_PIN_HAS_6TH_BIT defined(ADCSRB) && defined(MUX5)
#define ADC_PIN_6TH_BIT (_BV(MUX5))
#define ADC_PIN_MOVE_TO_6TH_BIT(VAL) (VAL << MUX5);

#define ADC_LEFT_ALIGN _BV(ADLAR)

#define ADC_TRIGGER_ENABLE _BV(ADATE)
#define ADC_TRIGGER_MASK (_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0))
#define ADC_TRIGGER_TIMER1_COMPB (_BV(ADTS2) | _BV(ADTS0))

#define ADC_INT_ENABLE _BV(ADIE)

void setUpTimer(uint16_t sampleRate);
void stopTimer();
void setUpADC(uint8_t inputPin, uint16_t sampleRate);
void stopADC();

void setADCSource(uint8_t pin);
void setADCFreq(uint16_t sampleRate);

uint8_t srcPin;
uint16_t outSampleRate;

WavFile* outFile = nullptr;
volatile byte buffs[2][BUFF_SIZE];
volatile bool buffReady[2];
volatile size_t buffIndex;
volatile size_t sampleIndex;

void Recorder::setInputPin(uint8_t pin)
{
    srcPin = pin;
}

void Recorder::setSampleRate(uint16_t sampleRate)
{
    outSampleRate = sampleRate;
}

bool Recorder::startRecording(File& outputFile)
{
    if (outFile) return false;
    outFile = new WavFile(outputFile, outSampleRate);
    setUpADC(srcPin, outSampleRate);
    setUpTimer(outSampleRate);
    return true;
}

void Recorder::stopRecording()
{
    if (!outFile) return;
    delete outFile;
    stopTimer();
    stopADC();
}

ISR(TIMER1_COMPA_vect)
{
    TIMER1_INT_MASK_REG &= ~TIMER1_INT_COMPA_ENABLE;
    ENABLE_NESTED_INTS();
    size_t otherBuffIndex = !buffIndex;
    if (buffReady[otherBuffIndex])
    {
        outFile->write(const_cast<const byte*>(buffs[otherBuffIndex]), BUFF_SIZE);
        buffReady[otherBuffIndex] = false;
    }
    TIMER1_INT_MASK_REG |= TIMER1_INT_COMPA_ENABLE;
}

ISR(ADC_vect)
{
    byte currentSample = ADC_DATA_REG_H;
    buffs[buffIndex][sampleIndex] = currentSample;

#ifdef AUDIO_PASSTHROUGH
    TIMER1_COMP_REG_A = currentSample;
#endif

    ++sampleIndex;
    if (sampleIndex >= BUFF_SIZE)
    {
        buffReady[buffIndex] = true;
        buffIndex = !buffIndex;
        sampleIndex = 0;
    }

    TIMER1_INT_REG |= TIMER1_INT_COMPB_FLAG;
}

inline void setUpTimer(uint16_t sampleRate)
{
    POWER_RED_REG &= ~POWER_RED_TIMER1;

    TIMER1_CTRL_REG_A = 
        (TIMER1_CTRL_REG_A & ~TIMER1_MODE_MASK_A)
        | TIMER1_MODE_FAST_PWM_ISR_A;
    TIMER1_CTRL_REG_B = 
        (TIMER1_CTRL_REG_B & ~TIMER1_MODE_MASK_B)
        | TIMER1_MODE_FAST_PWM_ISR_B;
    uint16_t timer1Top = (F_CPU / sampleRate) - 1;
    TIMER1_IN_CAP_REG = timer1Top;

#ifdef AUDIO_PASSTHROUGH
    pinMode(TIMER1_OUT_PIN_A, OUTPUT);

    TIMER1_CTRL_REG_A = 
        (TIMER1_CTRL_REG_A & ~TIMER1_OUT_MODE_MASK_A)
        | TIMER1_OUT_MODE_TOGGLE_NO_INV_A;
#endif

    TIMER1_INT_MASK_REG |= TIMER1_INT_COMPA_ENABLE;
    
    TIMER1_CTRL_REG_B = 
        (TIMER1_CTRL_REG_B & ~TIMER1_CLOCK_MASK)
        | TIMER1_CLOCK_PRESC_X1;
}

inline void stopTimer()
{
#ifdef AUDIO_PASSTHROUGH
    TIMER1_CTRL_REG_A = 
        (TIMER1_CTRL_REG_A & ~TIMER1_OUT_MODE_MASK_A)
        | TIMER1_OUT_MODE_OFF_A;
#endif
    
    TIMER1_INT_MASK_REG &= ~TIMER1_INT_COMPA_ENABLE;

    TIMER1_CTRL_REG_B = 
        (TIMER1_CTRL_REG_B & ~TIMER1_CLOCK_MASK)
        | TIMER1_CLOCK_NO_CLOCK;
}

inline void setUpADC(uint8_t inputPin, uint16_t sampleRate)
{
    POWER_RED_REG &= ~POWER_RED_ADC;

    ADC_CTRL_REG_A |= ADC_ENABLE;

    setADCFreq(sampleRate);

    ADC_IN_REG = 
        (ADC_IN_REG & ~ADC_REF_VOLT_MASK)
        | ADC_REF_VOLT_AVCC;
    setADCSource(inputPin);

    ADC_IN_REG |= ADC_LEFT_ALIGN;
    
    ADC_CTRL_REG_B =
        (ADC_CTRL_REG_B & ~ADC_TRIGGER_MASK)
        | ADC_TRIGGER_TIMER1_COMPB;
    ADC_CTRL_REG_A |= ADC_TRIGGER_ENABLE;

    ADC_CTRL_REG_A |= ADC_INT_ENABLE;
}

inline void stopADC()
{
    ADC_CTRL_REG_A &= ~ADC_INT_ENABLE;

    ADC_CTRL_REG_A &= ~ADC_ENABLE;
}

inline void setADCSource(uint8_t pin)
{
    // Code taken from Arduino's analogRead()
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18;
#endif
	pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54;
#elif defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18;
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
	if (pin >= 24) pin -= 24;
#else
	if (pin >= 14) pin -= 14;
#endif

#if ADC_PIN_HAS_6TH_BIT
	ADC_CTRL_REG_B = 
        (ADC_CTRL_REG_B & ~ADC_PIN_6TH_BIT) 
        | ADC_PIN_MOVE_TO_6TH_BIT((pin >> 3) & 0x01);
#endif
    
	ADC_IN_REG = 
        (ADC_IN_REG & ~ADC_PIN_4_BIT_MASK) 
        | (pin & ADC_PIN_3_BIT_MASK);
}

inline void setADCFreq(uint16_t sampleRate)
{
    static const uint8_t prescVals[] = {
        128, 64, 32, 16, 8, 4, 2
    };
    static constexpr uint8_t valCount = 
        sizeof(prescVals) / sizeof(prescVals[0]);

    uint8_t valIndex = 0;
    for (; valIndex < valCount; ++valIndex)
    {
        if (F_CPU / prescVals[valIndex] / 14 > sampleRate)
        {
            break;
        }
    }

    static const uint8_t prescBits[] = {
        0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1
    };
    ADC_CTRL_REG_A &= ~(prescBits[0]);
    ADC_CTRL_REG_A |= prescBits[valIndex];
}
