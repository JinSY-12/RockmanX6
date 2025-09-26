#pragma once
#include "GameNode.h"

class MovieScene : public GameNode
{
private:
	struct Dialogue
	{
		string text;
		string bgm;
		string sfx;
	}dialogue;

	GImage* _mainMovie;
	GImage* _subMovie;

	int SceneNum;
	bool subSceneOnOff;
	int testNum;
	int mMainMovieChange;
	int mSubMovieChange;

	int mSubMovieMove;

	bool mChangeable;

	int subLocation;

	typedef vector<Dialogue> vDialogue;
	typedef vector<Dialogue>::iterator viDialogue;

private:
	
	vDialogue _vDialogue;
	viDialogue _viDialogue;
	

public:
	HRESULT init(void);
	HRESULT init(int movieNum);
	void release(void);
	void update(void);
	void render(void);

	void movieReady(void);

	MovieScene() {}
	~MovieScene() {}

};

