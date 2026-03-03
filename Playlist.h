#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Arduino.h>
#include "AudioTrack.h"

class Playlist {
private:
  static const int MAX_TRACKS = 20;
  AudioTrack tracks[MAX_TRACKS];
  int count;

public:
  Playlist();

  bool addTrack(const AudioTrack& track);
  void listTracks() const;

  // Linear search by keyword (partial match). Returns track number or -1.
  int findByTitleKeyword(const String& keyword) const;

  // Manual selection sort by title (no std::sort)
  void sortByTitleSelection();
  int size() const;

  AudioTrack getAt(int index) const;
};

#endif