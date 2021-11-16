#include "Controllers.h"

Ewma Controllers::freqFilter(0.05);
int Controllers::frequency;
Ewma Controllers::volumeFilter(0.5);
int Controllers::volume;

void Controllers::readAndProcess()
{
  readFrequency();
  AppRadio::setFrequency(getFormatedFreq());
  readVolume();
  AppRadio::setVolume(getFormatedVolume());
}

void Controllers::readFrequency()
{
  frequency = freqFilter.filter(analogRead(FREQ_POT_PIN));
}

void Controllers::readVolume()
{
  volume = volumeFilter.filter(analogRead(VOLUME_POT_PIN));
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
  int freq = map(frequency, FREQ_POT_MIN, FREQ_POT_MAX, 8750, 10800);

  if (freq < 8750)
    freq = 8750;
  else if (freq > 10800)
    freq = 10800;

  return round10(freq);
}

int Controllers::getFormatedVolume()
{
  <<<<<< < HEAD
    return map(volume, VOLUME_POT_MIN, VOLUME_POT_MAX, 0, 15);
  ====== =
    return map(volumePot.avgValue, VOLUME_POT_MIN, VOLUME_POT_MAX, 0, 15);
  >>>>>> > f3def9c(Add volume controller)
}

int Controllers::round10(int num)
{
  return (num + 5) / 10 * 10;
}