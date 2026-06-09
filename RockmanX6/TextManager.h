#pragma once
#include "SingletonBase.h"

struct FontSetting
{
	char* fontName;
	int fontSize;
	bool isBold;

	FontSetting() {}
	FontSetting(char* fontName, int fontSize, bool isBold = false) :
		fontName(fontName),
		fontSize(fontSize),
		isBold(isBold)
	{}
};

class TextManager : public SingletonBase<TextManager>
{
public:

	struct Dialogue
	{
		string name;
		wstring text;
		string voice;
		string bgm;
		string sfx;
	}structDialouge;

	typedef map<string, HFONT> mapFontList;
	typedef map<string, HFONT>::iterator mapFontIter;

	int prevLine;
	int charIndex;

	int dialogueIndex;
	int eventIndex;

	int currentEventLine;

	bool movieShowLine;
	bool eventShowLine;

	bool movieWriteFinish;
	bool eventWriteFinish;

	int mMovieNum;
	int mEventNum;

	int mCurrentLine;

	float elapsedTime;
	float typeSpeed;

	string currentVoice;
	string currentBGM;
	string currentSFX;

	RECT mMovieNameArea;
	RECT mMovieTextArea;

	RECT mGameNameArea;
	RECT mGameTextArea;

	wstring mDirect;
	wstring mCharterName;
	wstring mDialogue;
	wstring mVoice;
	wstring mBGM;
	wstring mSFX;

	bool isTalk;

private:
	typedef vector<wstring> vDialogue;
	typedef vector<wstring>::iterator viDialogue;

	typedef map<int, vector<wstring>> mMoiveDialogue;
	typedef map<int, vector<wstring>>::iterator miMovietDialogue;

	typedef vector<wstring> vEvent;
	typedef vector<wstring>::iterator viEvent;

	typedef map<int, vector<wstring>> mEventDialogue;
	typedef map<int, vector<wstring>>::iterator miEventDialogue;

	mapFontList _mFontList;

	HFONT _defaultFont;

	UINT _textAlign;
	COLORREF _textColor;

private:
	vDialogue _vDialogue;
	viDialogue _viDialogue;

	mMoiveDialogue _mMovieDialogue;
	miMovietDialogue _miMovieDialogue;

	vEvent _vEvent;
	viEvent _viEvent;

	mEventDialogue _mEventDialogue;
	miEventDialogue _miEventDialogue;

public:
	HRESULT init(void);
	void release(void);

	void textReset(void);

	void addFontSetting(string settingName, FontSetting setting);
	void addFontSetting(string settingName, FontSetting setting, bool italic);

	HFONT findFont(string settingName);

	void drawText(HDC hdc, int destX, int destY, string printString, string fontName);
	void drawTextColor(HDC hdc, int destX, int destY, string printString, string fontName, COLORREF color);

	void drawMovieName(HDC hdc, int destX, int destY, string fontName);
	void drawEventName(HDC hdc, int destX, int destY, string fontName);

	void drawMovieDialogue(HDC hdc, int destX, int destY, string fontName);
	void drawEventDialogue(HDC hdc, int destX, int destY, string fontName);
	
	void drawTextInRect(HDC hdc, RECT rect, string printString, string settingName, bool isCenter = true);

	void setDefaultFont(HDC hdc);
	void setTextColor(COLORREF color) { _textColor = color; }
	void setTextAlign(UINT textAlign) { _textAlign = textAlign; }

	void ReadMovie();
	void ReadEvent();

	void ReadMovieDialogue(int eventNum, int currentLine);
	void ReadEventDialogue(int eventNum, int currentLine);
	// void ReadEventDialogue(void);

	COLORREF changeFontColor(wstring name);

	inline void ReadMovieEnd(void)
	{
		charIndex = mDialogue.size();
		movieShowLine = true;
	}

	inline void ReadEventEnd(void)
	{
		charIndex = mDialogue.size();
		eventShowLine = true;
	}

	inline bool ReadMovieComplete(void) { return this->movieShowLine; }
	inline bool ReadEventComplete(void) { return this->eventShowLine; }

	bool EventComplete(void);
	bool MovieComplete(void);
	
	void setMovieComplete(bool finish);
	void setEventComplete(bool finish);

	wstring Utf8ToWstring(const std::string& str);
	string WStringToString(const std::wstring& wstr);

	wstring getDirection(void) { return mDirect; }
	bool getIsTalk(void) { return isTalk; }
	wstring getCharterName(void) { return mCharterName; }

	TextManager() {}
	~TextManager() {}

};

