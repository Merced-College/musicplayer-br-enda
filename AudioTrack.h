#ifndef AUDIOTRACK_H
#define AUDIOTRACK_H

#include <Arduino.h>

class AudioTrack {
private:
  int trackNumber;
  String title;

public:
  AudioTrack();
  AudioTrack(int number, const String& title);

  int getTrackNumber() const;
  String getTitle() const;

  void setTrackNumber(int number);
  void setTitle(const String& t);
};

#endif