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

	bool showLine;
	bool writeFinish;
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
		
	wstring mCharterName;
	wstring mDialogue;
	wstring mVoice;
	wstring mBGM;
	wstring mSFX;

private:

	typedef vector<wstring> vDialogue;
	typedef vector<wstring>::iterator viDialogue;

	typedef map<int, vector<wstring>> mEventDialogue;
	typedef map<int, vector<wstring>>::iterator miEventDialogue;

	mapFontList _mFontList;

	HFONT _defaultFont;

	UINT _textAlign;
	COLORREF _textColor;

private:

	vDialogue _vDialogue;
	viDialogue _viDialogue;

	mEventDialogue _mEventDialogue;
	miEventDialogue _miEventDialogue;

public:
	HRESULT init(void);
	void release(void);

	void addFontSetting(string settingName, FontSetting setting);
	void addFontSetting(string settingName, FontSetting setting, bool italic);

	HFONT findFont(string settingName);

	void drawText(HDC hdc, int destX, int destY, string printString, string settingName);
	void drawText(HDC hdc, int destX, int destY, int eventNum, int currentLine, string settingName);
	void drawName(HDC hdc, int destX, int destY, int eventNum, int currentLine, string settingName);

	void drawTextInRect(HDC hdc, RECT rect, string printString, string settingName, bool isCenter = true);

	void setDefaultFont(HDC hdc);
	void setTextColor(COLORREF color) { _textColor = color; }
	void setTextAlign(UINT textAlign) { _textAlign = textAlign; }

	void ReadEvent(void);
	void ReadDialogue(void);

	COLORREF changeFontColor(wstring name);

	inline void ReadEnd(void) { this->showLine = true; }
	inline bool ReadComplete(void) { return this->showLine; }

	wstring Utf8ToWstring(const std::string& str);
	string TextManager::WStringToString(const std::wstring& wstr);

	TextManager() {}
	~TextManager() {}

};

