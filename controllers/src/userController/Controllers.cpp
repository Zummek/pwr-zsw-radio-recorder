#include "Controllers.h"
#include "AppRadio.h"

int Controllers::avgFreqPotVal;
int Controllers::freqPotValesTotal;
int Controllers::freqPotVales[AVG_OF_N_SAMPLES];
int Controllers::freqPotValuesIndex;

void Controllers::readAndProcess()
{
  readFreqPot();
  AppRadio::setFrequency(getFormatedFreq());
}

void Controllers::readFreqPot()
{
  freqPotValesTotal -= freqPotVales[freqPotValuesIndex];
  freqPotVales[freqPotValuesIndex] = analogRead(FREQ_POT_PIN);
  freqPotValesTotal += freqPotVales[freqPotValuesIndex++];

  if (freqPotValuesIndex >= AVG_OF_N_SAMPLES)
    freqPotValuesIndex = 0;

  avgFreqPotVal = freqPotValesTotal / AVG_OF_N_SAMPLES;
}

int Controllers::getFormatedFreq()
{
  int freq = map(avgFreqPotVal, FREQ_POT_MIN, FREQ_POT_MAX, 8750, 10800);

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