#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

enum class PlayerState { STOPPED, PLAYING, PAUSED, ERROR };

class AudioPlayer {
private:
  SoftwareSerial& dfSerial;
  DFRobotDFPlayerMini dfPlayer;

  PlayerState currentState;
  int currentTrack;   // -1 means none selected
  int volumeLevel;    // 0-30

  void setState(PlayerState newState);
  void printState() const;

public:
  AudioPlayer(SoftwareSerial& serial);

  bool begin(); // init DFPlayer
  PlayerState getState() const;

  // Required control functions
  void playTrack(int trackNumber);
  void pause();
  void stop();
  void nextTrack();
  void previousTrack();
  void setVolume(int level);

  int getCurrentTrack() const;
  int getVolume() const;
};

#endif