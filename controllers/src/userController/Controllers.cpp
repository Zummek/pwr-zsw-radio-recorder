#include "Controllers.h"
#include "AppRadio.h"

struct AvgController Controllers::freqPot;

void Controllers::readAndProcess()
{
  readFreqPot();
  AppRadio::setFrequency(getFormatedFreq());
}

void Controllers::readFreqPot()
{
  freqPot.totalValue -= freqPot.values[freqPot.index];
  freqPot.values[freqPot.index] = analogRead(FREQ_POT_PIN);
  freqPot.totalValue += freqPot.values[freqPot.index++];

  if (freqPot.index >= AVG_OF_N_SAMPLES)
    freqPot.index = 0;

  freqPot.avgValue = freqPot.totalValue / AVG_OF_N_SAMPLES;
}

int Controllers::getFormatedFreq()
{
  int freq = map(freqPot.avgValue, FREQ_POT_MIN, FREQ_POT_MAX, 8750, 10800);

  if (freq < 8750)
    freq = 8750;
  else if (freq > 10800)
    freq = 10800;

  return round10(freq);
}

int Controllers::round10(int num)
{
  return (num + 5) / 10 * 10;
}