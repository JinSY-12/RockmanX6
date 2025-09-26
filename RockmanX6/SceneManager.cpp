#include "Stdafx.h"
#include "GameNode.h"
#include "SceneManager.h"

// �ʱ�ȭ'��' ���� -> ȣ�� ���ο� �������
GameNode* SceneManager::_currentScene = nullptr;
GameNode* SceneManager::_loadingScene = nullptr;
GameNode* SceneManager::_readyScene = nullptr;

DWORD CALLBACK loadingThread(LPVOID prc)
{
	// ��ü�� ���� �Լ� ����
	SceneManager::_readyScene->init();

	// ���� ���� ��ü�� ������ ����
	SceneManager::_currentScene = SceneManager::_readyScene;

	// �ε� �� ����
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

	// find() �Լ��� ã�� ���ϸ� end()���� ��ȯ�Ѵ�.
	if (find == _mSceneList.end()) return E_FAIL;

	// �ٲٷ��� ���� ���� ���̶� ���� ��
	if (find->second == _currentScene) return S_OK;

	// �������� �����ϸ� ������ �Լ��� ����
	if (_currentScene) _currentScene->release();

	// ī�޶� ���̵���
	CAMERAMANAGER->padeIn(0.8f);

	if (SUCCEEDED(find->second->init()))
	{
		//�ٲٷ��� ���� ��������� ����
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::changeScene(string sceneName, int type)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	// find() �Լ��� ã�� ���ϸ� end()���� ��ȯ�Ѵ�.
	if (find == _mSceneList.end()) return E_FAIL;

	// �ٲٷ��� ���� ���� ���̶� ���� ��
	if (find->second == _currentScene) return S_OK;

	// �������� �����ϸ� ������ �Լ��� ����
	if (_currentScene) _currentScene->release();

	// ī�޶� ���̵���
	CAMERAMANAGER->padeIn(0.8f);

	if (SUCCEEDED(find->second->init(type)))
	{
		//�ٲٷ��� ���� ��������� ����
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

	// find() �Լ��� ã�� ���ϸ� end()���� ��ȯ�Ѵ�.
	if (sceneFind == _mSceneList.end()) return E_FAIL;

	// ���� ���� _readyScene�� �Ҵ�
	_readyScene = sceneFind->second;

	// �ٲٷ��� ���� ���� ���̶� ���� ��
	if (_readyScene == _currentScene) return S_OK;

	if (_currentScene) _currentScene->release();

	// =================================================================

	mapSceneIter loadSceneFind = _mLoadingSceneList.find(loadSceneName);

	if (loadSceneFind == _mLoadingSceneList.end()) return E_FAIL;

	_loadingScene = loadSceneFind->second;
	_loadingScene->init();

	CAMERAMANAGER->padeIn(0.5f);

	// �ε� ������ ����
	HANDLE hThread = CreateThread(
		NULL,							// ������ ���� Ư�� (���� ����) 
		0,								// ������ ���� ũ��
		loadingThread,					// �����尡 ������ �Լ�
		NULL,							// �Լ��� ������ ����
		0,								// ������ ���� �÷���
		NULL);							// ������ �������� ID�� ������ ����

	// �ε� �����尡 ���۵Ǹ� _currentScene�� _loadingScene���� ����
	_currentScene = _loadingScene;

	// ������ �ڵ��� �ݴ´�
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

