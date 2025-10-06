#include "Stdafx.h"
#include "TitleScene.h"

HRESULT TitleScene::init(void)
{
	// �̹��� ���� �ε�
	addImage();
	addSound();
	settingScene();
	
	delayTimer = TIMEMANAGER->getWorldTime();
	
	CAMERAMANAGER->init();

	// Ÿ�̸� üũ
	mSelectTimer = 0.f;
	mMoveTimer = 0.f;

	// Ÿ��Ʋ ����
	SOUNDMANAGER->play("Title", 0.5f);

	// ȭ�� ����
	insert = false;

	// �޴� ����
	buttonSelect = 0;

	return S_OK;
}

void TitleScene::release(void)
{
}

void TitleScene::update(void)
{
	// ������ Ű
	if (KEYMANAGER->isOnceKeyDown('V'))
	{
		SOUNDMANAGER->play("Title", 0.5f);
	}

	// ����Ű
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		// ù Ÿ��Ʋ
		if (insert == false && TIMEMANAGER->getWorldTime() > 1.5)
		{
			// SCENEMANAGER->changeScene("��������", 0);
			// SCENEMANAGER->changeScene("�ƾ�", 0);

			insert = true;
			SOUNDMANAGER->play("ButtonSelect", 0.5f);
			CAMERAMANAGER->padeIn(2.0f);
			mSelectTimer = TIMEMANAGER->getWorldTime();
			mMoveTimer = TIMEMANAGER->getWorldTime();
		}

		// ���� �޴� ���� ������ ���� �ٲ������ �׷��� ���� �Է� Ÿ�̹� ����
		else if(insert == true && TIMEMANAGER->getWorldTime() - mSelectTimer > 1.0)
		{
			mSelectTimer = TIMEMANAGER->getWorldTime();
			
			// �޴� ����
			if (buttonSelect == 0)
			{
				SOUNDMANAGER->play("ButtonSelect", 0.5f);

				// 0 = ��Ʈ�� ��������, 1 = �Ḷũ ��������
				// 0 = ����, 1 = ����, 2 = ����, 3 = ���̵�, 4 = ������, 5 = ��Ƽ��, 6 = �� ����
				SCENEMANAGER->changeScene("��������", 0, 0);

				// �� ü���� -> ��Ʈ�� �ƾ� or ��Ʈ�� ��������
				// SCENEMANAGER->changeScene("�ƾ�", 0);
			}

			else if (buttonSelect == 1)
			{
				SOUNDMANAGER->play("Error", 0.5f);
				// �̱��� ���� ���� ���
			}

			else if (buttonSelect == 2)
			{
				SOUNDMANAGER->play("ButtonSelect", 0.5f);
				// �� ü���� Ȥ�� UI ���
			}
		}
	}
	
	// �޴� ���� �̵� + �̵� ������ �߰�
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && insert == true && TIMEMANAGER->getWorldTime() - mMoveTimer > 0.1)
	{
		button[buttonSelect].dark = !button[buttonSelect].dark;
		
		if (buttonSelect > 1) buttonSelect = 0;
		else buttonSelect++;

		button[buttonSelect].dark = !button[buttonSelect].dark;
		SOUNDMANAGER->play("ButtonMove", 0.5f);
		mMoveTimer = TIMEMANAGER->getWorldTime();
	}

	if (KEYMANAGER->isOnceKeyDown(VK_UP) && insert == true && TIMEMANAGER->getWorldTime() - mMoveTimer > 0.1)
	{
		button[buttonSelect].dark = !button[buttonSelect].dark;

		if (buttonSelect < 1) buttonSelect = 2;
		else buttonSelect--;

		button[buttonSelect].dark = !button[buttonSelect].dark;
		SOUNDMANAGER->play("ButtonMove", 0.5f);
		mMoveTimer = TIMEMANAGER->getWorldTime();
	}

	// Press Button �ִϸ��̼�
	_vButton.at(3)->play(0.7f);
}

void TitleScene::render(void)
{
	_image->render(getMemDC(), 0, 0, 0, 0, _image->getWidth(), _image->getHeight());

	int num = 0; 

	if (insert == false)
		_vButton.at(3)->frameRender(getMemDC(), button[3].x, button[3].y, _vButton.at(3)->getFrameX(), 0);

	else if (insert == true)
	{
		for (_viButton = _vButton.begin(); _viButton != _vButton.end() - 1; _viButton++)
		{
			(*_viButton)->render(getMemDC(), button[num].x, button[num].y, 0 + (*_viButton)->getWidth() / 2 * button[num].dark, 0, (*_viButton)->getWidth() / 2, (*_viButton)->getHeight());
			num++;
		}
	}
}


void TitleScene::addImage(void)
{

	//////////////////////////////////
	// �ֿ켱 �׽�Ʈ �뵵 -> �ߵǸ� �����ؼ� ��������
	//////////////////////////////////

	for (int i = 0; i <= 27; i++)
	{
		string voiceNum;
		string filePath;

		voiceNum = to_string(i);
		filePath = "Resources/Image/Logo/Ready/Ready" + voiceNum + ".bmp";
		IMAGEMANAGER->addImage("Ready"+voiceNum, filePath.c_str(), 320 * 3, 38 * 3, true, MAGENTA);
	}

	// IMAGEMANAGER->addImage("test", "Resources/Image/test.bmp", 320 * 3, 31 * 3);

	//////////////////////////////////
	// ī�޶� ����
	//////////////////////////////////

	// ���̵� ��, �ƿ�
	IMAGEMANAGER->addImage("Black", "Resources/Image/HUD/UI_BlackOut.bmp", WINSIZE_X, WINSIZE_Y);
	IMAGEMANAGER->addImage("White", "Resources/Image/HUD/UI_WhiteOut.bmp", WINSIZE_X, WINSIZE_Y);
	

	//////////////////////////////////
	// Ÿ��Ʋ ȭ�� �̹���
	//////////////////////////////////

	// ���
	IMAGEMANAGER->addImage("Title", "Resources/Image/CutScene/Title.bmp", WINSIZE_X, WINSIZE_Y);

	// ������
	IMAGEMANAGER->addFrameImage("Start", "Resources/Image/Icon/Start.bmp", 288 * 3, 12 * 3, 2, 1, true, MAGENTA);
	IMAGEMANAGER->addImage("GameStart", "Resources/Image/Icon/GameStart.bmp", 174 * 3, 10 * 3, true, MAGENTA);
	IMAGEMANAGER->addImage("Continue", "Resources/Image/Icon/Continue.bmp", 132 * 3, 10 * 3, true, MAGENTA);
	IMAGEMANAGER->addImage("Option", "Resources/Image/Icon/Option.bmp", 102 * 3, 10 * 3, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("Next", "Resources/Image/Icon/Next.bmp", 16 * 2, 16, 2, 1, true, MAGENTA);

	//////////////////////////////////
	// �ƾ� �̹���
	//////////////////////////////////

	//// ���� ��ŸƮ ��Ʈ�� ////
	
	// Ǯ ��ũ��
	IMAGEMANAGER->addImage("Movie1_1", "Resources/Image/CutScene/Movie_1_1.bmp", 512 * 2, 240 * 2, true, MAGENTA);
	IMAGEMANAGER->addImage("Movie1_2", "Resources/Image/CutScene/Movie_1_2.bmp", 512 * 2, 240 * 2, true, MAGENTA);
	IMAGEMANAGER->addImage("Movie1_4", "Resources/Image/CutScene/Movie_1_4.bmp", 512 * 2, 240 * 2, true, MAGENTA);
	IMAGEMANAGER->addImage("Movie1_6", "Resources/Image/CutScene/Movie_1_6.bmp", 512 * 2, 240 * 2, true, MAGENTA);
	IMAGEMANAGER->addImage("Movie1_7", "Resources/Image/CutScene/Movie_1_7.bmp", 512 * 2, 240 * 2, true, MAGENTA);
	IMAGEMANAGER->addImage("Movie1_8", "Resources/Image/CutScene/Movie_1_8.bmp", 512 * 2, 240 * 2, true, MAGENTA);

	// ���� ��ũ��
	IMAGEMANAGER->addImage("Movie1_3", "Resources/Image/CutScene/Movie_1_3.bmp", 288 * 2, 240 * 2, true, MAGENTA);
	IMAGEMANAGER->addImage("Movie1_5", "Resources/Image/CutScene/Movie_1_5.bmp", 288 * 2, 240 * 2, true, MAGENTA);


	//////////////////////////////////
	// �������� �̹���
	//////////////////////////////////
	
	//// ��� �������� ////
	
	// ��Ʈ�� ��������
	IMAGEMANAGER->addImage("Stage_Intro", "Resources/Image/Stage/Stage_Intro.bmp", 6464 * SCALE_FACTOR, 960 * SCALE_FACTOR, true, MAGENTA);
	IMAGEMANAGER->addImage("Pixel_Intro", "Resources/Image/Stage/Stage_Intro_Pixel.bmp", 6464 * SCALE_FACTOR, 960 * SCALE_FACTOR, false, MAGENTA);


	//////////////////////////////////
	// ĳ���� �̹���
	//////////////////////////////////

	//// �ִϸ��̼�////
	
	//////////////////////////////////
	// ����
	//////////////////////////////////
	
	// ���� �̹���

	IMAGEMANAGER->addFrameImage("X_Spawn", "Resources/Image/Player/X/X_Spawn.bmp", 1156 * SCALE_FACTOR, 192 * SCALE_FACTOR, 17, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_Idle", "Resources/Image/Player/X/X_Idle.bmp", 140 * SCALE_FACTOR, 92 * SCALE_FACTOR, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_WalkStart", "Resources/Image/Player/X/X_WalkStart2.bmp", 96 * SCALE_FACTOR, 94 * SCALE_FACTOR, 2, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_WalkLoop", "Resources/Image/Player/X/X_WalkLoop2.bmp", 686 * SCALE_FACTOR, 94 * SCALE_FACTOR, 14, 2, true, MAGENTA);

	IMAGEMANAGER->addFrameImage("X_WalkBurstStart", "Resources/Image/Player/X/X_WalkBurstStart.bmp", 110 * SCALE_FACTOR, 94 * SCALE_FACTOR, 2, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_WalkBurstLoop", "Resources/Image/Player/X/X_WalkBurstLoop.bmp", 798 * SCALE_FACTOR, 94 * SCALE_FACTOR, 14, 2, true, MAGENTA);

	IMAGEMANAGER->addFrameImage("X_StandBurstLoop", "Resources/Image/Player/X/X_StandBurstLoop.bmp", 164 * SCALE_FACTOR, 88 * SCALE_FACTOR, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_StandBurstEnd", "Resources/Image/Player/X/X_StandBurstEnd.bmp", 82 * SCALE_FACTOR, 88 * SCALE_FACTOR, 2, 2, true, MAGENTA);

	IMAGEMANAGER->addFrameImage("X_StandChargeBurst", "Resources/Image/Player/X/X_StandChargeBurst.bmp", 287 * SCALE_FACTOR, 102 * SCALE_FACTOR, 7, 2, true, MAGENTA);


	
	IMAGEMANAGER->addFrameImage("X_JumpUp", "Resources/Image/Player/X/X_JumpUp.bmp", 170 * SCALE_FACTOR, 114 * SCALE_FACTOR, 5, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_FallingDown", "Resources/Image/Player/X/X_FallingDown.bmp", 64 * SCALE_FACTOR, 108 * SCALE_FACTOR, 2, 2, true, MAGENTA);
	
	// ���� ����
	IMAGEMANAGER->addFrameImage("X_Burster1", "Resources/Image/Player/SFX/X/SFX_Burster1_Bullet.bmp", 75 * SCALE_FACTOR, 16 * SCALE_FACTOR, 5, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_Burster2", "Resources/Image/Player/SFX/X/SFX_Burster2_Bullet.bmp", 108 * SCALE_FACTOR, 36 * SCALE_FACTOR, 4, 2, true, MAGENTA);
	IMAGEMANAGER->addFrameImage("X_Burster3", "Resources/Image/Player/SFX/X/SFX_Burster3_Bullet.bmp", 183 * SCALE_FACTOR, 60 * SCALE_FACTOR, 3, 2, true, MAGENTA);

	
	//////////////////////////////////
	// ����
	//////////////////////////////////

	// ���� �̹���
	
	// ���� ����


	//// �ӱ׼�////
	
	// ����
	

	// ����
	

	// �����

	// �ñ׳ʽ�

	// ���۶�

	//////////////////////////////////
	// HUD
	//////////////////////////////////

	IMAGEMANAGER->addImage("HUD_ProgressBar_X", "Resources/Image/HUD/HUD_ProgressBar_X.bmp", 30 * SCALE_FACTOR, 31 * SCALE_FACTOR, true, MAGENTA);
	IMAGEMANAGER->addImage("HUD_ProgressBar_Xweapon", "Resources/Image/HUD/HUD_ProgressBar_Xweapon.bmp", 30 * SCALE_FACTOR, 31 * SCALE_FACTOR, true, MAGENTA);
	
	IMAGEMANAGER->addImage("HUD_HpBar", "Resources/Image/HUD/HUD_HpBar.bmp", 9 * SCALE_FACTOR, 144 * SCALE_FACTOR, true, MAGENTA);
	IMAGEMANAGER->addImage("HUD_HpBarHead", "Resources/Image/HUD/HUD_HpBarHead.bmp", 9 * SCALE_FACTOR, 4 * SCALE_FACTOR, true, MAGENTA);

	IMAGEMANAGER->addImage("HUD_GreenBar", "Resources/Image/HUD/HUD_GreenBar.bmp", 3 * SCALE_FACTOR, 146 * SCALE_FACTOR, true, MAGENTA);
	IMAGEMANAGER->addImage("HUD_RedBar", "Resources/Image/HUD/HUD_RedBar.bmp", 3 * SCALE_FACTOR, 146 * SCALE_FACTOR, true, MAGENTA);
	
}

void TitleScene::addSound(void)
{

	//////////////////////////////////
	// Ÿ��Ʋ ȭ�� ����
	//////////////////////////////////

	// ���
	SOUNDMANAGER->addSound("Title", "Resources/Sound/Menu/Menu_Title.wav", false, false);

	// ������
	SOUNDMANAGER->addSound("ButtonMove", "Resources/Sound/Menu/Menu_ButtonMove.wav", false, false);
	SOUNDMANAGER->addSound("ButtonSelect", "Resources/Sound/Menu/Menu_ButtonSelect.wav", false, false);

	SOUNDMANAGER->addSound("Error", "Resources/Sound/Menu/Menu_Error.wav", false, false);

	//////////////////////////////////
	// �ƾ� ���� - �ƾ� ���̽��� ����!!
	//////////////////////////////////

	// ���� ��ŸƮ ��Ʈ��
	SOUNDMANAGER->addSound("Desert", "Resources/Sound/BGM/BGM_Desert.wav", true, true);
	SOUNDMANAGER->addSound("Laboratory", "Resources/Sound/BGM/BGM_Laboratory.wav", true, true);
	SOUNDMANAGER->addSound("StageSelect", "Resources/Sound/BGM/BGM_StageSelect.wav", true, true);
	SOUNDMANAGER->addSound("VS_D1000", "Resources/Sound/BGM/BGM_VS_D1000.wav", true, true);
	SOUNDMANAGER->addSound("IntroStage", "Resources/Sound/BGM/BGM_IntroStage.wav", true, true);

	SOUNDMANAGER->addSound("Siren", "Resources/Sound/SFX/SFX_Siren.wav", true, false);
	SOUNDMANAGER->addSound("Siren_Loop","Resources/Sound/SFX/SFX_Siren_Loop.wav", true, true);
	SOUNDMANAGER->addSound("Ready", "Resources/Sound/SFX/SFX_Ready.wav", false, false);
	
	// �̺�Ʈ �ѹ��� ���̽� ������ �޾ƿͼ� ���� for�� ���������ڤ�������
	for (int i = 1 ; i < 23 ; i++)
	{
		string voiceNum;
		string filePath;

		voiceNum = to_string(i);
		filePath = "Resources/Sound/Voice/Event/Voice1_" + voiceNum + ".wav";

		SOUNDMANAGER->addSound("Voice1_"+ voiceNum, filePath.c_str(), false, false);
	}

	//////////////////////////////////
	// ĳ���� ���� - �ƾ� ���̽��� �����ϴ�!!
	//////////////////////////////////

	//////////////////////////////////
	// ����
	//////////////////////////////////
	
	// ���̽�
	// SOUNDMANAGER->addSound("");

	// ȿ����
	SOUNDMANAGER->addSound("SFX_X_WarpIn" , "Resources/Sound/SFX/Charcter/X/SFX_X_WarpIn.wav", false, false);
	SOUNDMANAGER->addSound("SFX_X_WarpOut", "Resources/Sound/SFX/Charcter/X/SFX_X_WarpOut.wav", false, false);
	SOUNDMANAGER->addSound("SFX_X_BurstCharge", "Resources/Sound/SFX/Charcter/X/SFX_X_BurstCharge.wav", false, false);
	SOUNDMANAGER->addSound("SFX_X_BurstChargeLoop", "Resources/Sound/SFX/Charcter/X/SFX_X_BurstChargeLoop.wav", false, true);

	SOUNDMANAGER->addSound("SFX_X_Burster1", "Resources/Sound/SFX/Charcter/X/SFX_X_Burster1.wav", false, false);
	SOUNDMANAGER->addSound("SFX_X_Burster2", "Resources/Sound/SFX/Charcter/X/SFX_X_Burster2.wav", false, false);
	SOUNDMANAGER->addSound("SFX_X_Burster3", "Resources/Sound/SFX/Charcter/X/SFX_X_Burster3.wav", false, false);

	SOUNDMANAGER->addSound("SFX_X_ArmorRoom", "Resources/Sound/SFX/Charcter/X/SFX_X_ArmorRoom.wav", false, false);
	SOUNDMANAGER->addSound("SFX_X_BurstCharge", "Resources/Sound/SFX/Charcter/X/SFX_X_BurstCharge.wav", false, false);
	SOUNDMANAGER->addSound("SFX_X_BurstChargeLoop", "Resources/Sound/SFX/Charcter/X/SFX_X_BurstChargeLoop.wav", false, true);

	SOUNDMANAGER->addSound("SFX_X_ArmorRoom", "Resources/Sound/SFX/Charcter/X/SFX_X_ArmorRoom.wav", false, false);

	
	//////////////////////////////////
	// ����
	//////////////////////////////////

	// ���̽�
	

	// ���� ȿ����
	






	//////////////////////////////////
	// �� ����
	//////////////////////////////////



}

void TitleScene::settingScene(void)
{
	// GameStart, Continue, Option ��ư ��ġ
	button[0].dark = false;
	button[0].x = WINSIZE_X / 5 * 2;
	button[0].y = WINSIZE_Y / 2 + 80;

	button[1].dark = true;
	button[1].x = WINSIZE_X / 5 * 2;
	button[1].y = WINSIZE_Y / 2 + 130;

	button[2].dark = true;
	button[2].x = WINSIZE_X / 5 * 2;
	button[2].y = WINSIZE_Y / 2 + 180;

	// Press Button ��ư ��ġ
	button[3].dark = false;
	button[3].x = WINSIZE_X / 10 * 3;
	button[3].y = WINSIZE_Y / 2 + 120;

	// �̹��� ����

	// ��� �̹���
	_image = IMAGEMANAGER->findImage("Title");

	// ��ư �̹��� ����
	_vButton.push_back(IMAGEMANAGER->findImage("GameStart"));
	_vButton.push_back(IMAGEMANAGER->findImage("Continue"));
	_vButton.push_back(IMAGEMANAGER->findImage("Option"));
	_vButton.push_back(IMAGEMANAGER->findImage("Start"));
}


void TitleScene::loadStat(void)
{
}