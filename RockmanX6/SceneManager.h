#pragma once
#include "SingletonBase.h"
#include "PlayerType.h"
#include "BossType.h"

// 전방 선언
class GameNode;

class SceneManager : public SingletonBase<SceneManager>
{
public:
	typedef map<string, GameNode*> mapSceneList;
	typedef map<string, GameNode*>::iterator mapSceneIter;

private:
	static GameNode* _currentScene;
	static GameNode* _loadingScene;
	static GameNode* _readyScene;

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;


	string _nextSceneName;
	int _nextSceneType;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	GameNode* addScene(string sceneName, GameNode* scene);
	GameNode* addLoadingScene(string loadingSceneName, GameNode* scene);

	HRESULT changeScene(string sceneName);
	HRESULT changeScene(string sceneName, int type);
	HRESULT changeScene(string sceneName, PlayerType pType, BossType bType);
	void changeScenePadeOut(string sceneName, float padeTime, int nextSceneType = 0);

	HRESULT changeLoadingScene(string nextSceneName, string loadSceneName = "TitleScene");

	bool findScene(string sceneName);

	// LPVOID : void 포인터
	friend DWORD CALLBACK loadingThread(LPVOID prc);

	SceneManager() {}
	~SceneManager() {}
};

