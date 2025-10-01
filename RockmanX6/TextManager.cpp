#include "Stdafx.h"
#include "TextManager.h"

HRESULT TextManager::init(void)
{
	AddFontResource("Resources/Font/SB_M.ttf");
	AddFontResource("Resources/Font/Mabinogi.ttf");
	AddFontResource("Resources/Font/DNF_M.ttf");

	_textAlign = TA_LEFT;
	_textColor = RGB(255, 255, 255);
	_defaultFont = CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("Arial"));

	prevLine = -1;
	charIndex = 0;
	showLine = false;
	writeFinish = false;

	mCurrentLine = 0;
	dialogueIndex = 0;
	mMovieNameArea = { 0 , WINSIZE_Y / 3 * 2, 60 , 60};
	mMovieTextArea = { 0 , WINSIZE_Y / 3 * 2 + 40, 500, 400};

	mGameNameArea;
	mGameTextArea;

	ReadEvent();
	ReadDialogue();

	elapsedTime = 0.0f;   // ���� �ð�
	typeSpeed = 0.1f;     // 1���� ��� ������, �� ����


	// �Ϲ� �۲�

	// DNF_M
	addFontSetting("DNF_M_50", FontSetting("���� ���ܵ� Į�� Medium", 50));
	addFontSetting("DNF_M_45", FontSetting("���� ���ܵ� Į�� Medium", 45));
	addFontSetting("DNF_M_40", FontSetting("���� ���ܵ� Į�� Medium", 40));
	addFontSetting("DNF_M_34", FontSetting("���� ���ܵ� Į�� Medium", 34));
	addFontSetting("DNF_M_30", FontSetting("���� ���ܵ� Į�� Medium", 30));
	addFontSetting("DNF_M_24", FontSetting("���� ���ܵ� Į�� Medium", 24));
	addFontSetting("DNF_M_18", FontSetting("���� ���ܵ� Į�� Medium", 18));

	return S_OK;
}

void TextManager::release(void)
{
	RemoveFontResource("Resources/Font/DNF_M.ttf");

	_mFontList.clear();
}

void TextManager::addFontSetting(string settingName, FontSetting setting)
{
	int fontWeight = (setting.isBold) ? FW_BOLD : FW_NORMAL;

	HFONT font = CreateFont
	(
		setting.fontSize,				// ����ũ��
		0,								// ������
		0,								// ��ü ����
		0,								// ���� ����
		fontWeight,						// ���� ����
		0,								// ���� (T / F)
		0,								// ���� (T / F)
		0,								// ��Ҽ� (T / F)
		DEFAULT_CHARSET,				// ���� ����
		0,								// ��� ��Ȯ��
		0,								// Ŭ���� ��Ȯ��
		0,								// ����� ��
		0,								// �ڰ�
		TEXT(setting.fontName)			// ��Ʈ
	);

	_mFontList.insert(make_pair(settingName, font));
}

void TextManager::addFontSetting(string settingName, FontSetting setting, bool italic)
{
	int fontWeight = (setting.isBold) ? FW_BOLD : FW_NORMAL;

	HFONT font = CreateFont
	(
		setting.fontSize,				// ����ũ��
		0,								// ������
		0,								// ��ü ����
		0,								// ���� ����
		fontWeight,						// ���� ����
		italic,								// ���� (T / F)
		0,								// ���� (T / F)
		0,								// ��Ҽ� (T / F)
		DEFAULT_CHARSET,				// ���� ����
		0,								// ��� ��Ȯ��
		0,								// Ŭ���� ��Ȯ��
		0,								// ����� ��
		0,								// �ڰ�
		TEXT(setting.fontName)			// ��Ʈ
	);

	_mFontList.insert(make_pair(settingName, font));
}

HFONT TextManager::findFont(string settingName)
{
	auto key = _mFontList.find(settingName);

	if (key == _mFontList.end())
	{
		cout << "Font Not Found" << endl;
		return NULL;
	}
	else
	{
		return key->second;
	}
}

void TextManager::drawText(HDC hdc, int destX, int destY, string printString, string fontName)
{
	HFONT font = findFont(fontName);

	// ��Ʈ ����
	SelectObject(hdc, font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, _textAlign);
	SetTextColor(hdc, _textColor);

	// �ؽ�Ʈ ���
	TextOut(hdc, destX, destY, printString.c_str(), printString.length());

	// �⺻ ��Ʈ�� ����
	setDefaultFont(hdc);
}

void TextManager::drawTextColor(HDC hdc, int destX, int destY, string printString, string fontName, COLORREF color)
{
	HFONT font = findFont(fontName);

	// ��Ʈ ����
	SelectObject(hdc, font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, _textAlign);
	SetTextColor(hdc, color);

	// �ؽ�Ʈ ���
	TextOut(hdc, destX, destY, printString.c_str(), printString.length());

	// �⺻ ��Ʈ�� ����
	setDefaultFont(hdc);
}

void TextManager::drawName(HDC hdc, int destX, int destY, int eventNum, int currentLine, string fontName)
{
	if (currentLine >= _mEventDialogue[eventNum].size())
	{
		// ���߿� �ε�â���� �ٲٽÿ�
		SOUNDMANAGER->stop(currentVoice);
		SOUNDMANAGER->stop(currentBGM);
		SOUNDMANAGER->stop(currentSFX);

		CAMERAMANAGER->padeOut(3.0f);
		UIMANAGER->changeUiMode();
	}

	HFONT font = findFont(fontName);

	// ��Ʈ ����
	SelectObject(hdc, font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, _textAlign);

	_textColor = changeFontColor(mCharterName.c_str());
	SetTextColor(hdc, _textColor);

	if (UIMANAGER->getIsUiMode() == true)
	{
		TextOutW(hdc, destX, destY, mCharterName.c_str(), mCharterName.length());

		if (currentLine != prevLine)
		{
			// �ؽ�Ʈ ���

			charIndex = 0;

			// Voice ���, ����
			if (mVoice.compare(L"STOP") == 0)
			{
				SOUNDMANAGER->stop(currentVoice);
			}

			else if (mVoice.compare(L""))
			{
				SOUNDMANAGER->stop(currentVoice);
				SOUNDMANAGER->play(WStringToString(mVoice), 0.5f);
				currentVoice = WStringToString(mVoice);
			}

			// ���ڼ� ���� -> ��Ȯ�� ��ġ 0, �ѱ��� ���� 1 = SFX�� ������ ���ڶ� ���ͱ��� ����
			// BGM ���, ����
			if (mBGM.compare(L"STOP") == 0)
			{
				SOUNDMANAGER->stop(currentBGM);
			}

			else if (mBGM.compare(L""))
			{
				SOUNDMANAGER->stop(currentBGM);
				SOUNDMANAGER->play(WStringToString(mBGM), 0.5f);
				currentBGM = WStringToString(mBGM);
			}

			// SFX ���, ����
			if (mSFX.compare(L"STOP") == 0)
			{
				SOUNDMANAGER->stop(currentSFX);
			}

			else if (mSFX.compare(L""))
			{
				SOUNDMANAGER->stop(currentSFX);
				SOUNDMANAGER->play(WStringToString(mSFX), 0.5f);
				currentSFX = WStringToString(mSFX);
			}
			prevLine = currentLine;
		}
	}

	// �⺻ ��Ʈ�� ����
	setDefaultFont(hdc);
}

void TextManager::drawDialogue(HDC hdc, int destX, int destY, int eventNum, int currentLine, string fontName)
{
	HFONT font = findFont(fontName);

	mEventNum = eventNum;
	
	wstring toDisplay = mDialogue.substr(0, showLine ? mDialogue.size() : charIndex);

	// ��Ʈ ����
	SelectObject(hdc, font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, _textAlign);
	SetTextColor(hdc, _textColor);
	
	// �ؽ�Ʈ ���
	TextOutW(hdc, destX, destY, toDisplay.c_str(), toDisplay.length());

	elapsedTime += 0.03f;

	if (mDialogue.size() != charIndex)
	{
		if (elapsedTime >= typeSpeed)
		{
			charIndex++;
			elapsedTime = 0.0f;
		}
	}

	else if (mDialogue.size() == charIndex) showLine = true;

	// �⺻ ��Ʈ�� ����
	setDefaultFont(hdc);

}

void TextManager::drawTextInRect(HDC hdc, RECT rect, string printString, string settingName, bool isCenter)
{
	HFONT font = findFont(settingName);

	// ��Ʈ ����
	SelectObject(hdc, font);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, _textColor);

	// �ؽ�Ʈ ���
	UINT align = (isCenter) ? (DT_CENTER | DT_VCENTER | DT_WORDBREAK) : (DT_LEFT | DT_TOP | DT_WORDBREAK);
	DrawTextA(hdc, printString.c_str(), -1, &rect, align);

	// �⺻ ��Ʈ�� ����
	setDefaultFont(hdc);
}

void TextManager::setDefaultFont(HDC hdc)
{
	// �⺻ ��Ʈ�� ����
	SelectObject(hdc, _defaultFont);

	if (_textColor != RGB(255, 255, 255))
	{
		_textColor = RGB(255, 255, 255);
		SetTextColor(hdc, _textColor);
	}

	if (_textAlign != TA_LEFT)
	{
		_textAlign = TA_LEFT;
		SetTextAlign(hdc, _textAlign);
	}
}

void TextManager::ReadEvent(void)
{
	ifstream file("Resources/Text/Dialogue.txt");

	string line;
	int currentEvent = 0;

	while (getline(file, line))
	{
		if (line.rfind("[Event", 0) == 0)
		{
			size_t pos = line.find(']');

			if (pos != string::npos)
			{
				currentEvent = stoi(line.substr(6, pos - 6));
				_mEventDialogue[currentEvent - 1].clear();
			}
		}

		else if (!line.empty() && currentEvent > 0)
		{
			wstring wline = Utf8ToWstring(line);
			_mEventDialogue[currentEvent - 1].push_back(wline);
		}
	}
	_vDialogue = _mEventDialogue[mEventNum];
}

void TextManager::ReadDialogue(void)
{
	if (dialogueIndex < _vDialogue.size())
	{
		writeFinish = false;
		wstring line = _vDialogue[dialogueIndex++];
		wstring temp;

		size_t pos = line.find(':');
		if (pos != wstring::npos)
		{
			mCharterName = line.substr(0, pos);
			temp = line.substr(pos + 1);

			pos = temp.find('/');
			if (pos != wstring::npos)
			{
				mDialogue = temp.substr(0, pos);
				line = temp.substr(pos + 1);

				pos = line.find('/');
				if (pos != wstring::npos)
				{
					mVoice = line.substr(0, pos);
					temp = line.substr(pos + 1);

					pos = temp.find('/');
					if (pos != wstring::npos)
					{
						mBGM = temp.substr(0, pos);
						mSFX = temp.substr(pos + 1);
					}
					else mSFX = L"";
				}
				else mBGM = L"";
			}
			else mVoice = L"";
		}
		else
		{
			mCharterName = L"";
			mDialogue = line;
		}
		charIndex = 0;
		showLine = false;
	}

	if (dialogueIndex >= _vDialogue.size()) writeFinish = true;
}

COLORREF TextManager::changeFontColor(wstring name)
{
	if (name.compare(L"����Ʈ") == 0) return RGB(80, 60, 185);
	else if (name.compare(L"???") == 0) return RGB(255, 0, 0);
	else if (name.compare(L"�����") == 0) return RGB(255, 255, 0);
	else if (name.compare(L"X") == 0) return RGB(0, 0, 255);
	else if (name.compare(L"�ñ׳ʽ�") == 0) return RGB(5, 6, 30);
	else if (name.compare(L"���۶�") == 0) return RGB(100, 255, 0);

	return RGB(255, 255, 255);
}

wstring TextManager::Utf8ToWstring(const std::string& str)
{
	if (str.empty()) return L"";

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);

	return wstr;
}

string TextManager::WStringToString(const std::wstring& wstr) {
	if (wstr.empty()) return "";
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
	return str;
}