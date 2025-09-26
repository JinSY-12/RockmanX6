#include "Stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager() : _system(nullptr)
{
}

HRESULT SoundManager::init()
{
	// 사운드 시스템 생성
	System_Create(&_system);

	// 시스템 초기화
	FMOD_RESULT fr = _system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);
	if (fr == FMOD_ERR_OUTPUT_INIT)
	{
		_system->release();

		System_Create(&_system);
		_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		fr = _system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);
	}

	menuSound = 0.5f;
	voiceSound = 0.5f;
	bgmSound = 0.5f;

	return S_OK;
}

void SoundManager::release()
{
	mapSoundIter soundIter = _mSoundList.begin();

	for (int i = 0; i < _mActiveChannels.size(); ++i)
	{
		_mActiveChannels[i].chaneel->stop();
	}

	for (; soundIter != _mSoundList.end(); ++soundIter)
	{
		soundIter->second->release();
	}

	// 시스템 해제
	if (_system != nullptr)
	{
		_system->release();
		_system->close();
	}
}

void SoundManager::update()
{
	_system->update();

	bool isPaused;
	bool isPlaying;

	for (int i = 0; i < _mActiveChannels.size(); ++i)
	{
		_mActiveChannels[i].chaneel->getPaused(&isPaused);
		if (isPaused)return;

		_mActiveChannels[i].chaneel->isPlaying(&isPlaying);
		if (isPlaying == false)
		{
			_mActiveChannels[i].chaneel->stop();
			_mActiveChannels.erase(_mActiveChannels.begin() + i);
			--i;
		}
	}
}

void SoundManager::addSound(string strKey, const char* fileName, bool bgm, bool loop)
{
	mapSoundIter iter = _mSoundList.find(strKey);
	if (iter != _mSoundList.end()) return;

	Sound* sound = nullptr;

	if (loop)
	{
		if (bgm)
		{
			// 통로를 만들어주기 떄문에 createStream
			// 1 : 1 매칭
			// createStream(파일명, 사운드를 열기 위한 방식, 성공 여부 반환, 주소값)
			_system->createStream(fileName, FMOD_LOOP_NORMAL, NULL, &sound);
		}
		else
		{
			// 안전성은 떨어지지만 동시에 여러 개의 음원 재생 가능
			_system->createSound(fileName, FMOD_LOOP_NORMAL, NULL, &sound);
		}
	}
	else
	{
		if (bgm)
		{
			_system->createStream(fileName, FMOD_DEFAULT, NULL, &sound);
		}
		else
		{
			_system->createSound(fileName, FMOD_DEFAULT, NULL, &sound);
		}
	}

	if (sound != nullptr)
	{
		_mSoundList.insert(make_pair(strKey, sound));
	}
}

void SoundManager::play(string strKey, float volume)
{
	//일시정지 되어 있는 사운드 인지 판단
	for (int i = 0; i < _mActiveChannels.size(); ++i)
	{
		if (_mActiveChannels[i].soundName == strKey)
		{
			bool isPaused;
			_mActiveChannels[i].chaneel->getPaused(&isPaused);
			if (isPaused)
			{
				_mActiveChannels[i].chaneel->setPaused(false);
				return;
			}
		}
	}

	if (_mActiveChannels.size() >= TOTALSOUNDBUFFER) return;

	//해당 이름의 사운드가 없다면 return 
	mapSoundIter iter = _mSoundList.find(strKey);
	if (iter == _mSoundList.end()) return;

	Channel* channel;
	Sound* sound = iter->second;

	_system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	channel->setVolume(volume);

	ChannelInfo channelInfo;
	channelInfo.chaneel = channel;
	channelInfo.soundName = strKey;
	_mActiveChannels.push_back(channelInfo);
}

void SoundManager::stop(string strKey)
{
	for (int i = 0; i < _mActiveChannels.size(); ++i)
	{
		if (_mActiveChannels[i].soundName == strKey)
		{
			_mActiveChannels[i].chaneel->stop();
			_mActiveChannels.erase(_mActiveChannels.begin() + i);
			break;
		}
	}
}

void SoundManager::pause(string strKey)
{
	for (int i = 0; i < _mActiveChannels.size(); ++i)
	{
		if (_mActiveChannels[i].soundName == strKey)
		{
			_mActiveChannels[i].chaneel->setPaused(true);
			break;
		}
	}
}