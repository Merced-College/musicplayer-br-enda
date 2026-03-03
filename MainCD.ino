// Team: Brenda Romero, Hugo Cisneros
// Modular MP3 Controller System
// DFPlayer Mini + Serial menu + Playlist + Search + Manual Sort

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "AudioTrack.h"
#include "Playlist.h"
#include "AudioPlayer.h"

// Uno recommended: DFPlayer TX -> Arduino pin 10 (RX), DFPlayer RX -> Arduino pin 11 (TX)
static const int DF_RX_PIN = 10;
static const int DF_TX_PIN = 11;

SoftwareSerial dfSerial(DF_RX_PIN, DF_TX_PIN);
AudioPlayer player(dfSerial);
Playlist playlist;

String readLine() {
  // Simple line reader for Serial Monitor (set "Newline" recommended)
  if (!Serial.available()) return "";
  return Serial.readStringUntil('\n');
}

void printHelp() {
  Serial.println(F("\nCommands:"));
  Serial.println(F("  help                -> show commands"));
  Serial.println(F("  p <num>             -> play track number (ex: p 3)"));
  Serial.println(F("  n                   -> next track"));
  Serial.println(F("  b                   -> previous track"));
  Serial.println(F("  pause               -> toggle pause/resume"));
  Serial.println(F("  stop                -> stop playback"));
  Serial.println(F("  v <0-30>            -> set volume (ex: v 25)"));
  Serial.println(F("  v+                  -> volume up"));
  Serial.println(F("  v-                  -> volume down"));
  Serial.println(F("  list                -> show playlist"));
  Serial.println(F("  find <keyword>       -> search titles (linear search)"));
  Serial.println(F("  sort                -> sort playlist by title (selection sort)"));
  Serial.println();
}

int parseIntAfterSpace(const String& s) {
  int spaceIndex = s.indexOf(' ');
  if (spaceIndex < 0) return 0;
  String numPart = s.substring(spaceIndex + 1);
  numPart.trim();
  return numPart.toInt();
}

String parseWordAfterSpace(const String& s) {
  int spaceIndex = s.indexOf(' ');
  if (spaceIndex < 0) return "";
  String part = s.substring(spaceIndex + 1);
  part.trim();
  return part;
}

bool playlistHasTrack(int trackNum) {
  for (int i = 0; i < playlist.size(); i++) {
    if (playlist.getAt(i).getTrackNumber() == trackNum) return true;
  }
  return false;
}

void buildPlaylist() {
  playlist.addTrack(AudioTrack(1, "Track 1"));
  playlist.addTrack(AudioTrack(2, "Track 2"));
  playlist.addTrack(AudioTrack(3, "Track 3"));
  playlist.addTrack(AudioTrack(4, "Track 4"));
  playlist.addTrack(AudioTrack(5, "Track 5"));
  playlist.addTrack(AudioTrack(6, "Track 6"));
  playlist.addTrack(AudioTrack(7, "Track 7"));
  playlist.addTrack(AudioTrack(8, "Track 8"));
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Serial.println(F("Modular MP3 Controller starting..."));

  buildPlaylist();
  playlist.listTracks();

  player.begin();
  printHelp();
}

void handleCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  if (cmd == "help") {
    printHelp();
    return;
  }

  if (cmd == "list") {
    playlist.listTracks();
    return;
  }

  if (cmd.startsWith("p ")) {
    int trackNum = parseIntAfterSpace(cmd);

    // Validate track against playlist first (nice + prevents obvious bad play requests)
    if (!playlistHasTrack(trackNum)) {
      Serial.println(F("That track number is not in the playlist. Setting ERROR."));
      // Requirement: invalid track -> ERROR
      player.playTrack(-1); // triggers ERROR via AudioPlayer rules
      return;
    }
    player.playTrack(trackNum);
    return;
  }

  if (cmd == "n") {
    player.nextTrack();
    return;
  }

  if (cmd == "b") {
    player.previousTrack();
    return;
  }

  if (cmd == "pause") {
    player.pause();
    return;
  }

  if (cmd == "stop") {
    player.stop();
    return;
  }

  if (cmd.startsWith("v ")) {
    int level = parseIntAfterSpace(cmd);
    player.setVolume(level);
    return;
  }

  if (cmd == "v+") {
    player.setVolume(player.getVolume() + 1);
    return;
  }

  if (cmd == "v-") {
    player.setVolume(player.getVolume() - 1);
    return;
  }

  if (cmd.startsWith("find ")) {
    String key = parseWordAfterSpace(cmd);
    if (key.length() == 0) {
      Serial.println(F("Usage: find <keyword>"));
      return;
    }
    int foundTrack = playlist.findByTitleKeyword(key);
    if (foundTrack == -1) {
      Serial.println(F("No match found."));
    } else {
      Serial.print(F("Match found: track "));
      Serial.println(foundTrack);
    }
    return;
  }

  if (cmd == "sort") {
    playlist.sortByTitleSelection();
    Serial.println(F("Playlist sorted by title."));
    playlist.listTracks();
    return;
  }

  Serial.println(F("Unknown command. Type 'help' to see options."));
}

void loop() {
  String line = readLine();
  if (line.length() > 0) {
    handleCommand(line);
  }
}