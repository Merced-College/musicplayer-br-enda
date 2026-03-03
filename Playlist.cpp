#include "Playlist.h"

Playlist::Playlist() : count(0) {}

bool Playlist::addTrack(const AudioTrack& track) {
  if (count >= MAX_TRACKS) return false;
  tracks[count] = track;
  count++;
  return true;
}

void Playlist::listTracks() const {
  Serial.println(F("---- Playlist ----"));
  for (int i = 0; i < count; i++) {
    Serial.print(tracks[i].getTrackNumber());
    Serial.print(F(": "));
    Serial.println(tracks[i].getTitle());
  }
  Serial.println(F("------------------"));
}

int Playlist::findByTitleKeyword(const String& keyword) const {
  String key = keyword;
  key.toLowerCase();

  for (int i = 0; i < count; i++) {
    String t = tracks[i].getTitle();
    t.toLowerCase();
    if (t.indexOf(key) >= 0) {
      return tracks[i].getTrackNumber();
    }
  }
  return -1;
}

void Playlist::sortByTitleSelection() {
  // Selection sort: O(n^2)
  for (int i = 0; i < count - 1; i++) {
    int minIndex = i;
    for (int j = i + 1; j < count; j++) {
      if (tracks[j].getTitle() < tracks[minIndex].getTitle()) {
        minIndex = j;
      }
    }
    if (minIndex != i) {
      AudioTrack temp = tracks[i];
      tracks[i] = tracks[minIndex];
      tracks[minIndex] = temp;
    }
  }
}

int Playlist::size() const { return count; }

AudioTrack Playlist::getAt(int index) const {
  if (index < 0 || index >= count) return AudioTrack();
  return tracks[index];
}