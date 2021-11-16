#include "Controllers.h"
#include "AppRadio.h"

struct AvgController Controllers::freqPot;
struct AvgController Controllers::volumePot;

void Controllers::readAndProcess()
{
  readFreqPot();
  AppRadio::setFrequency(getFormatedFreq());
  readVolumePot();
  AppRadio::setVolume(getFormatedVolume());
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

void Controllers::readVolumePot()
{
  volumePot.totalValue -= volumePot.values[volumePot.index];
  volumePot.values[volumePot.index] = analogRead(VOLUME_POT_PIN);
  volumePot.totalValue += volumePot.values[volumePot.index++];

  if (volumePot.index >= AVG_OF_N_SAMPLES)
    volumePot.index = 0;

  volumePot.avgValue = volumePot.totalValue / AVG_OF_N_SAMPLES;
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

int Controllers::getFormatedVolume()
{
  return map(volumePot.avgValue, VOLUME_POT_MIN, VOLUME_POT_MAX, 0, 15);
}

int Controllers::round10(int num)
{
  return (num + 5) / 10 * 10;
}