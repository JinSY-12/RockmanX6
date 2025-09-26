#include "Stdafx.h"
#include "GameNode.h"
#include "SceneManager.h"

// 초기화'만' 진행 -> 호출 여부에 관계없이
GameNode* SceneManager::_currentScene = nullptr;
GameNode* SceneManager::_loadingScene = nullptr;
GameNode* SceneManager::_readyScene = nullptr;

DWORD CALLBACK loadingThread(LPVOID prc)
{
	// 교체될 씬의 함수 실행
	SceneManager::_readyScene->init();

	// 현재 씬을 교체될 씬으로 변경
	SceneManager::_currentScene = SceneManager::_readyScene;

	// 로딩 씬 해제
	SceneManager::_loadingScene->release();
	SceneManager::_loadingScene = nullptr;
	SceneManager::_readyScene = nullptr;

	return 0;
}

HRESULT SceneManager::init(void)
{
	_currentScene = nullptr;
	_loadingScene = nullptr;
	_readyScene = nullptr;

	return S_OK;
}

void SceneManager::release(void)
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != nullptr)
		{
			if (miSceneList->second == _currentScene)
			{
				miSceneList->second->release();
			}

			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else
		{
			++miSceneList;
		}
	}

	_mSceneList.clear();
}

void SceneManager::update(void)
{
	if (_nextSceneName == "TitleScene")
	{
		changeLoadingScene(_nextSceneName);
	}

	else
	{
		changeScene(_nextSceneName, _nextSceneType);
	}

	if (_currentScene != nullptr)  _currentScene->update();
}

void SceneManager::render(void)
{
	if (_currentScene != nullptr) _currentScene->render();
}

GameNode* SceneManager::addScene(string sceneName, GameNode* scene)
{
	if (!scene) return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

GameNode* SceneManager::addLoadingScene(string loadingSceneName, GameNode* scene)
{
	if (!scene) return nullptr;

	_loadingScene = scene;
	_mLoadingSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}

HRESULT SceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	// find() 함수로 찾지 못하면 end()값을 반환한다.
	if (find == _mSceneList.end()) return E_FAIL;

	// 바꾸려는 씬이 현재 씬이랑 같을 때
	if (find->second == _currentScene) return S_OK;

	// 기존씬이 존재하면 릴리즈 함수를 실행
	if (_currentScene) _currentScene->release();

	// 카메라 페이드인
	CAMERAMANAGER->padeIn(0.8f);

	if (SUCCEEDED(find->second->init()))
	{
		//바꾸려는 씬을 현재씬으로 변경
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::changeScene(string sceneName, int type)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	// find() 함수로 찾지 못하면 end()값을 반환한다.
	if (find == _mSceneList.end()) return E_FAIL;

	// 바꾸려는 씬이 현재 씬이랑 같을 때
	if (find->second == _currentScene) return S_OK;

	// 기존씬이 존재하면 릴리즈 함수를 실행
	if (_currentScene) _currentScene->release();

	// 카메라 페이드인
	CAMERAMANAGER->padeIn(0.8f);

	if (SUCCEEDED(find->second->init(type)))
	{
		//바꾸려는 씬을 현재씬으로 변경
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

void SceneManager::changeScenePadeOut(string sceneName, float padeTime, int nextSceneType)
{
	_nextSceneName = sceneName;
	_nextSceneType = nextSceneType;
	CAMERAMANAGER->padeOut(padeTime);
}

HRESULT SceneManager::changeLoadingScene(string nextSceneName, string loadSceneName)
{
	mapSceneIter sceneFind = _mSceneList.find(nextSceneName);

	// find() 함수로 찾지 못하면 end()값을 반환한다.
	if (sceneFind == _mSceneList.end()) return E_FAIL;

	// 다음 씬을 _readyScene에 할당
	_readyScene = sceneFind->second;

	// 바꾸려는 씬이 현재 씬이랑 같을 때
	if (_readyScene == _currentScene) return S_OK;

	if (_currentScene) _currentScene->release();

	// =================================================================

	mapSceneIter loadSceneFind = _mLoadingSceneList.find(loadSceneName);

	if (loadSceneFind == _mLoadingSceneList.end()) return E_FAIL;

	_loadingScene = loadSceneFind->second;
	_loadingScene->init();

	CAMERAMANAGER->padeIn(0.5f);

	// 로딩 쓰레드 시작
	HANDLE hThread = CreateThread(
		NULL,							// 쓰레드 보안 특성 (보안 설정) 
		0,								// 쓰레드 스택 크기
		loadingThread,					// 쓰레드가 실행할 함수
		NULL,							// 함수에 전달할 인자
		0,								// 쓰레드 생성 플래그
		NULL);							// 생성된 쓰레드의 ID를 저장할 변수

	// 로딩 쓰레드가 시작되면 _currentScene을 _loadingScene으로 변경
	_currentScene = _loadingScene;

	// 쓰레드 핸들을 닫는다
	CloseHandle(hThread);

	return S_OK;
}

bool SceneManager::findScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return false;

	if (find->second == _currentScene) return false;

	if (find->second != nullptr) return true;

	return false;
}

