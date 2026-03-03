#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(SoftwareSerial& serial)
  : dfSerial(serial), currentState(PlayerState::STOPPED), currentTrack(-1), volumeLevel(20) {}

bool AudioPlayer::begin() {
  dfSerial.begin(9600);

  if (!dfPlayer.begin(dfSerial)) {
    Serial.println(F("DFPlayer init failed. Check wiring + SD card."));
    setState(PlayerState::ERROR);
    return false;
  }

  dfPlayer.volume(volumeLevel);
  Serial.println(F("DFPlayer ready."));
  setState(PlayerState::STOPPED);
  return true;
}

PlayerState AudioPlayer::getState() const { return currentState; }

void AudioPlayer::setState(PlayerState newState) {
  if (newState == currentState) return;
  currentState = newState;
  printState();
}

void AudioPlayer::printState() const {
  Serial.print(F("STATE -> "));
  switch (currentState) {
    case PlayerState::STOPPED: Serial.println(F("STOPPED")); break;
    case PlayerState::PLAYING: Serial.println(F("PLAYING")); break;
    case PlayerState::PAUSED:  Serial.println(F("PAUSED"));  break;
    case PlayerState::ERROR:   Serial.println(F("ERROR"));   break;
  }
}

void AudioPlayer::playTrack(int trackNumber) {
  if (trackNumber <= 0) {
    Serial.println(F("Invalid track number. Must be >= 1."));
    setState(PlayerState::ERROR);
    return;
  }

  // Basic rule: invalid requested track -> ERROR
  // (We don't know max track count from SD here; playlist validation can happen in controller.)
  currentTrack = trackNumber;
  dfPlayer.play(trackNumber);
  Serial.print(F("Playing track "));
  Serial.println(trackNumber);
  setState(PlayerState::PLAYING);
}

void AudioPlayer::pause() {
  // Rule: cannot pause if STOPPED
  if (currentState == PlayerState::STOPPED) {
    Serial.println(F("Cannot pause: player is STOPPED."));
    return;
  }

  // Toggle pause/resume
  if (currentState == PlayerState::PLAYING) {
    dfPlayer.pause();
    Serial.println(F("Paused."));
    setState(PlayerState::PAUSED);
  } else if (currentState == PlayerState::PAUSED) {
    dfPlayer.start();
    Serial.println(F("Resumed."));
    setState(PlayerState::PLAYING);
  } else {
    Serial.println(F("Pause ignored due to ERROR state."));
  }
}

void AudioPlayer::stop() {
  dfPlayer.stop();
  Serial.println(F("Stopped."));
  setState(PlayerState::STOPPED);
}

void AudioPlayer::nextTrack() {
  // Rule: cannot next/prev if no track selected
  if (currentTrack == -1) {
    Serial.println(F("No track selected yet. Use: p <num>"));
    return;
  }
  dfPlayer.next();
  currentTrack++;
  Serial.println(F("Next track."));
  setState(PlayerState::PLAYING);
}

void AudioPlayer::previousTrack() {
  if (currentTrack == -1) {
    Serial.println(F("No track selected yet. Use: p <num>"));
    return;
  }
  dfPlayer.previous();
  if (currentTrack > 1) currentTrack--;
  Serial.println(F("Previous track."));
  setState(PlayerState::PLAYING);
}

void AudioPlayer::setVolume(int level) {
  // Rule: volume 0-30 only
  if (level < 0) level = 0;
  if (level > 30) level = 30;

  volumeLevel = level;
  dfPlayer.volume(volumeLevel);
  Serial.print(F("Volume set to "));
  Serial.println(volumeLevel);
}

int AudioPlayer::getCurrentTrack() const { return currentTrack; }
int AudioPlayer::getVolume() const { return volumeLevel; }