//#include <MBE/Audio/SoundManager.h>
//
//using namespace mbe;
//
//float SoundManager::listenerZ = 300.f;
//
//unsigned short int SoundManager::totalNumberOfSounds = 0u;
//const unsigned short int SoundManager::maxNumberOfSounds = 255u; // Internal limit set by SFML / OpenAL
//
//SoundManager::SoundManager(const SoundBufferHolder<AudioData::ID> & soundHolder, const FilePathDictionary & musicDictionary) :
//	soundHolder(soundHolder),
//	musicFilePathDictionary(musicDictionary)
//{
//	// Listener points towards the screen (default in SFML)
//	sf::Listener::setDirection(0.f, 0.f, -1.f);
//}
//
//SoundManager::~SoundManager()
//{
//	for (auto & pair : soundDictionary)
//		pair.second.stop();
//	for (auto & pair : musicDictionary)
//		pair.second->stop();
//}
//
//typename SoundManager::ID SoundManager::PlaySound(const AudioData::ID & audioSourceId, AudioData::Type audioType)
//{
//	return this->PlaySound(audioSourceId, this->GetListenerPosition(), audioType);
//}
//
//typename SoundManager::ID SoundManager::PlaySound(const AudioData & audioData)
//{
//	return this->PlaySound(audioData, this->GetListenerPosition());
//}
//
//typename SoundManager::ID SoundManager::PlaySound(const AudioData::ID & audioSourceId, const sf::Vector2f & position, AudioData::Type audioType)
//{
//	AudioData soundData = AudioData::Default;
//	soundData.SetAudioSourceID(audioSourceId);
//	soundData.audioType = audioType;
//
//	return this->PlaySound(soundData, position);
//}
//
//typename SoundManager::ID SoundManager::PlaySound(const AudioData & audioData, const sf::Vector2f & position)
//{
//	// The audio id is unique accross all mbe::SoundManagers
//	// It is increased every time a new sound is played
//	static ID audioId = 0u;
//
//	// The total number of sf::Sound and sf::Music instances must be kept below 256
//	if (totalNumberOfSounds >= maxNumberOfSounds)
//		throw std::runtime_error("SoundManager: Playing this sound would acceed the max number of sounds");
//
//	if (audioData.audioType == AudioData::Type::Sound)
//	{
//		// Create and insert the sound
//		auto & sound = soundDictionary.insert(std::make_pair(audioId, sf::Sound())).first->second;
//
//		// Store the audioSourceId for this sound
//		audioDataIdDictionary.insert(std::make_pair(audioId, audioData.GetAudioSourceID()));
//
//		// Set the sound parameters using the values specified in the sound data object
//		sound.setBuffer(soundHolder[audioData.GetAudioSourceID()]);
//		sound.setPosition({ position.x, -position.y, 0.f });
//		sound.setAttenuation(audioData.attenuation);
//		sound.setVolume(audioData.volume);
//		sound.setPitch(audioData.pitch);
//		sound.setPlayingOffset(audioData.playingOffset);
//		sound.setLoop(audioData.looped);
//
//		// Calculate and set the minimum 3D distance
//		sound.setMinDistance(Convert2DTo3DMinDistance(audioData.minDistance));
//
//		// Play the sound
//		sound.play();
//		
//		// Increment the audioId
//		audioId++;
//	}
//	else if (audioData.audioType == AudioData::Type::Music)
//	{
//		// Create and insert the music
//		//auto & music = *musicDictionary.insert(std::make_pair(audioId, std::make_unique<sf::Music>())).first->second;
//		auto musicPtr = std::make_unique<sf::Music>();
//		auto & music = *musicPtr;
//		
//		// Store the audioSourceId for this music
//		audioDataIdDictionary.insert(std::make_pair(audioId, audioData.GetAudioSourceID()));
//
//		// Set the music parameters using the values specified in the sound data object
//		music.openFromFile(musicFilePathDictionary[audioData.GetAudioSourceID()]);
//		music.setPosition({ position.x, -position.y, 0.f });
//		music.setAttenuation(audioData.attenuation);
//		music.setVolume(audioData.volume);
//		music.setPitch(audioData.pitch);
//		music.setPlayingOffset(audioData.playingOffset);
//		music.setLoop(audioData.looped);
//
//		// Calculate and set the minimum 3D distance
//		music.setMinDistance(Convert2DTo3DMinDistance(audioData.minDistance));
//
//		// Start streaming the music
//		music.play();
//
//		// Insert the music
//		musicDictionary.insert(std::make_pair(audioId, std::move(musicPtr)));
//
//		// Increment the audioId
//		audioId++;
//	}
//	else
//	{
//		throw std::runtime_error("SoundManager: Unidentifiable sound type");
//	}
//
//	// Increment the total number of sounds
//	totalNumberOfSounds++;
//
//	return audioId - 1;
//}
//
//void SoundManager::SetPaused(ID audioId, bool value)
//{
//	ApplyAudioFunction<void>(audioId, [value](sf::Sound & sound) {
//		if (value ==  true)
//		{
//			if (sound.getStatus() == sf::Sound::Status::Playing)
//				sound.pause();
//		}
//		else
//		{
//			if (sound.getStatus() == sf::Sound::Status::Paused)
//				sound.play();
//		}
//	}, [value](sf::Music & music) {
//		if (value == true)
//		{
//			if (music.getStatus() == sf::Music::Status::Playing)
//				music.pause();
//		}
//		else
//		{
//			if (music.getStatus() == sf::Music::Status::Paused)
//				music.play();
//		}
//	});
//}
//
//void SoundManager::StopSound(ID audioId)
//{
//	ApplyAudioFunction<void>(audioId, [](sf::Sound & sound) { sound.stop(); }, [](sf::Music & music) { music.stop(); });
//}
//
//void SoundManager::UpdateSound(ID audioId, const AudioData & audioData)
//{
//	this->UpdateSound(audioId, audioData, this->GetListenerPosition());
//}
//
//void SoundManager::UpdateSound(ID audioId, const AudioData & audioData, const sf::Vector2f & position)
//{
//	ApplyAudioFunction<void>(audioId, [&](sf::Sound & sound) {
//		sound.setPosition({ position.x, -position.y, 0.f });
//		sound.setAttenuation(audioData.attenuation);
//		sound.setVolume(audioData.volume);
//		sound.setPitch(audioData.pitch);
//		sound.setLoop(audioData.looped);
//	}, [&](sf::Music & music) {
//		music.setPosition({ position.x, -position.y, 0.f });
//		music.setAttenuation(audioData.attenuation);
//		music.setVolume(audioData.volume);
//		music.setPitch(audioData.pitch);
//		music.setLoop(audioData.looped);
//	});
//}
//
//AudioData::AudioStatus SoundManager::GetAudioStatus(ID audioId)
//{
//	return ApplyAudioFunction<AudioData::AudioStatus>(audioId, [](sf::Sound & sound) {
//		switch (sound.getStatus())
//		{
//		case sf::Sound::Status::Playing:
//			return AudioData::AudioStatus::Playing;
//		case sf::Sound::Status::Paused:
//			return AudioData::AudioStatus::Paused;
//		default:
//			return AudioData::AudioStatus::Stopped;
//		}
//	}, [](sf::Music & music) {
//		switch (music.getStatus())
//		{
//		case sf::Music::Status::Playing:
//			return AudioData::AudioStatus::Playing;
//		case sf::Music::Status::Paused:
//			return AudioData::AudioStatus::Paused;
//		default:
//			return AudioData::AudioStatus::Stopped;
//		}
//	});
//}
//
//AudioData SoundManager::GetAudioData(ID audioId)
//{
//	return ApplyAudioFunction<AudioData>(audioId, [this, &audioId](sf::Sound & sound)
//	{
//		AudioData audioData = AudioData::Default;
//		// Should be guaranteed to not throw since the audioDataIdDictionary is updated when the sound and music dictionaries are
//		audioData.SetAudioSourceID(audioDataIdDictionary.find(audioId)->second);
//		audioData.attenuation = sound.getAttenuation();
//		audioData.volume = sound.getVolume();
//		audioData.pitch = sound.getPitch();
//		audioData.playingOffset = sound.getPlayingOffset();
//		audioData.looped = sound.getLoop();
//		audioData.minDistance = Convert3DTo2DMinDistance(sound.getMinDistance());
//		return audioData;
//	}, [this, &audioId](sf::Music & music) {
//		AudioData audioData = AudioData::Default;
//		// Should be guaranteed to not throw since the audioDataIdDictionary is updated when the sound and music dictionaries are
//		audioData.SetAudioSourceID(audioDataIdDictionary.find(audioId)->second);
//		audioData.attenuation = music.getAttenuation();
//		audioData.volume = music.getVolume();
//		audioData.pitch = music.getPitch();
//		audioData.playingOffset = music.getPlayingOffset();
//		audioData.looped = music.getLoop();
//		audioData.minDistance = Convert3DTo2DMinDistance(music.getMinDistance());
//		return audioData;
//	});
//}
//
//sf::Vector2f SoundManager::GetAudioPosition(ID audioId)
//{
//	return ApplyAudioFunction<sf::Vector2f>(audioId, [](sf::Sound & sound) {
//		return sf::Vector2f(sound.getPosition().x, -sound.getPosition().y);
//	}, [](sf::Music & music) {
//		return sf::Vector2f(music.getPosition().x, -music.getPosition().y);
//	});
//}
//
//bool SoundManager::DoesExists(ID audioId)
//{
//	auto soundItr = soundDictionary.find(audioId);
//	if (soundItr == soundDictionary.end())
//	{
//		auto musicItr = musicDictionary.find(audioId);
//		if (musicItr == musicDictionary.end())
//			return false;
//		return false;
//	}
//	return true;
//}
//
//void SoundManager::RemoveStoppedSounds()
//{
//	// Remove and delete all stopped sf::Sound instances
//	for (auto it = soundDictionary.cbegin(); it != soundDictionary.cend(); )
//	{
//		const auto & sound = it->second;
//		if (sound.getStatus() == sf::Sound::Status::Stopped)
//		{
//			// Erase the audio source id
//			audioDataIdDictionary.erase(it->first);
//
//			// Delete the sound and decrease the number of sound counter
//			it = soundDictionary.erase(it);
//			totalNumberOfSounds--;
//		}
//		else
//			++it;
//	}
//
//	// Remove and delete all stopped sf::Music instances
//	for (auto it = musicDictionary.cbegin(); it != musicDictionary.cend(); )
//	{
//		const auto & music = *it->second;
//		if (music.getStatus() == sf::Music::Status::Stopped)
//		{
//			// Erase the audio source id
//			audioDataIdDictionary.erase(it->first);
//
//			// Delete the music and decrease the number of sound counter
//			it = musicDictionary.erase(it);
//			totalNumberOfSounds--;
//		}
//		else
//			++it;
//	}
//}
//
//void SoundManager::SetListenerPosition(const sf::Vector2f & position)
//{
//	sf::Listener::setPosition(position.x, -position.y, listenerZ);
//}
//
//sf::Vector2f SoundManager::GetListenerPosition()
//{
//	sf::Vector3f position = sf::Listener::getPosition();
//	return sf::Vector2f(position.x, -position.y);
//}
//
//float SoundManager::Convert2DTo3DMinDistance(float minDistance2d)
//{
//	// Pythagoras
//	return std::sqrt(std::pow(minDistance2d, 2) + std::pow(listenerZ, 2));
//}
//
//float SoundManager::Convert3DTo2DMinDistance(float minDistance3d)
//{
//	// Inverse function
//	return std::sqrt(std::pow(minDistance3d, 2) - std::pow(listenerZ, 2));
//}
//
//template <typename TReturnType>
//TReturnType SoundManager::ApplyAudioFunction(ID audioId, std::function<TReturnType(sf::Sound&)> soundFunction, std::function<TReturnType(sf::Music&)> musicFunction)
//{
//	auto soundItr = soundDictionary.find(audioId);
//	if (soundItr == soundDictionary.end())
//	{
//		auto musicItr = musicDictionary.find(audioId);
//		if (musicItr == musicDictionary.cend())
//			throw std::runtime_error("SoundManager: No sound exists under this id: " + std::to_string(audioId));
//		else
//			return musicFunction(*musicItr->second);
//	}
//	else
//		return soundFunction(soundItr->second);
//}