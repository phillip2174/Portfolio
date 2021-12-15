#pragma once
#include "Core/Factory.h"
#include <string>
#include <irrKlang.h>
#include <glm/glm.hpp>

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

namespace Tools { class SoundPlayerEC; }

using namespace irrklang;

enum SOUND_TYPE {
	SOUND_SFX = 0,
	SOUND_MUSIC,
};

class SoundPlayer : public Component {
	friend class Tools::SoundPlayerEC;
private:
	bool isLooping = false;
	float volumeValue = 1.0f;
	/*undetermined*/std::string sr_soundpath;
	int sound_type = SOUND_TYPE::SOUND_SFX;

	ISoundEngine* soundPlayer = nullptr;
	ISoundSource* soundSource = nullptr;
	ISound* soundVolume = nullptr;
	
	//these should be private -------------
	//void CreateSoundPlayer();
	//void UpdateVolume();
	//-------------------------------------

	static float MasterVolume;
	static float MusicVolume;
	static float SFXVolume;

public:
	SoundPlayer();
	~SoundPlayer();

	virtual void Init();
	
	//---------------------------
	void CreateSoundPlayer();
	void UpdateVolume();
	//---------------------------

	float GetVolume() { return volumeValue; }
	int GetSoundType() { return sound_type; }

	void SetSound(std::string path);
	void PlaySound();
	void StopSound();
	void DeleteSoundPlayer();
	void SetLoop(bool loop);
	void SetVolume(float value);
	void SetSoundType(int type);
	void IncreaseVolume();
	void DecreaseVolume();
	void AdjustVolume(float diff);

	static float GetMasterVolume() { return SoundPlayer::MasterVolume; }
	static float GetMusicVolume() { return SoundPlayer::MusicVolume; }
	static float GetSFXVolume() { return SoundPlayer::SFXVolume; }

	static void SetMasterVolume(float vol) { SoundPlayer::MasterVolume = glm::clamp(vol, 0.0f, 1.0f); UpdateVolumeAll(); }
	static void SetMusicVolume(float vol) { SoundPlayer::MusicVolume = glm::clamp(vol, 0.0f, 1.0f); UpdateVolumeAll(); }
	static void SetSFXVolume(float vol) { SoundPlayer::SFXVolume = glm::clamp(vol, 0.0f, 1.0f); UpdateVolumeAll(); }

	static void UpdateVolumeAll();

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			sr_soundpath,
			isLooping,
			volumeValue
		);
	}
};

CEREAL_REGISTER_TYPE(SoundPlayer);
