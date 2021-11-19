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
  return map(volume, VOLUME_POT_MIN, VOLUME_POT_MAX, 0, 15);
}

int Controllers::round10(int num)
{
  return (num + 5) / 10 * 10;
}