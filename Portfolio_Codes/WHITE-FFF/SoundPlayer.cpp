#include "SoundPlayer.hpp"

#include <glm/glm.hpp>

#include "Core/Factory.h"

#include "Core/Logger.hpp"

float SoundPlayer::MasterVolume = 1.0f;
float SoundPlayer::MusicVolume = 1.0f;
float SoundPlayer::SFXVolume = 1.0f;

SoundPlayer::SoundPlayer() {
	//isLooping = false;
	//volumeValue = 1.0f;
	Factory<Component, SoundPlayer>::Add(this);
}

SoundPlayer::~SoundPlayer() {
	Factory<Component, SoundPlayer>::Remove(this);
}

void SoundPlayer::Init() {
	CreateSoundPlayer();
}

void SoundPlayer::CreateSoundPlayer() {
	soundPlayer = createIrrKlangDevice();
}

void SoundPlayer::SetSound(std::string path) {
	sr_soundpath = path;
}

void SoundPlayer::PlaySound() {
	UpdateVolume();

	if(!soundPlayer->getSoundSource(&(sr_soundpath[0]))){
		soundSource = soundPlayer->addSoundSourceFromFile(&(sr_soundpath[0]));
	}
	else {
		soundSource = soundPlayer->getSoundSource(&(sr_soundpath[0]));
	}

	soundVolume = soundPlayer->play2D(soundSource, isLooping, false, true);
}

void SoundPlayer::StopSound() {
	soundPlayer->stopAllSounds();
	ENGINE_INFO("Stop sound");
}

void SoundPlayer::DeleteSoundPlayer() {
	soundPlayer->drop();
}

void SoundPlayer::SetLoop(bool loop) {
	isLooping = loop;
}

void SoundPlayer::UpdateVolume() {
	if (soundVolume != nullptr) {
		switch (sound_type)
		{
		default:
		case SOUND_SFX:
			soundPlayer->setSoundVolume(volumeValue * SoundPlayer::SFXVolume * SoundPlayer::MasterVolume);
			break;
		case SOUND_MUSIC:
			soundPlayer->setSoundVolume(volumeValue * SoundPlayer::MusicVolume * SoundPlayer::MasterVolume);
			break;
		}
	}
}

void SoundPlayer::SetVolume(float value) {
	volumeValue = value;
	UpdateVolume();
}

void SoundPlayer::SetSoundType(int type) {
	sound_type = type;
	UpdateVolume();
}

void SoundPlayer::IncreaseVolume() {
	volumeValue += 0.05f;
	volumeValue = glm::clamp(volumeValue, 0.0f, 1.0f);
	UpdateVolume();
}

void SoundPlayer::DecreaseVolume() {
	volumeValue -= 0.05f;
	volumeValue = glm::clamp(volumeValue, 0.0f, 1.0f);
	UpdateVolume();
}

void SoundPlayer::AdjustVolume(float diff) {
	volumeValue += diff;
	volumeValue = glm::clamp(volumeValue, 0.0f, 1.0f);
	UpdateVolume();
}

void SoundPlayer::UpdateVolumeAll() {
	//update sound
	for (std::pair<int, SoundPlayer*> sound : Factory<Component, SoundPlayer>::getCollection()) {
		sound.second->UpdateVolume();
	}
}
