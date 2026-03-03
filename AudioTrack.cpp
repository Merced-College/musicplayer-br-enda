#include "AudioTrack.h"

AudioTrack::AudioTrack() : trackNumber(0), title("") {}

AudioTrack::AudioTrack(int number, const String& t) : trackNumber(number), title(t) {}

int AudioTrack::getTrackNumber() const { return trackNumber; }
String AudioTrack::getTitle() const { return title; }

void AudioTrack::setTrackNumber(int number) { trackNumber = number; }
void AudioTrack::setTitle(const String& t) { title = t; }