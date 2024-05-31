#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{

}

void CGameStateInit::OnInit()
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0, "Start Initialize...");	// �@�}�l��loading�i�׬�0%

	Sleep(200);
	CAudio::Instance()->Load(0, "music/main.mp3");
	CAudio::Instance()->Play(0, true);
	load_background();

	ShowInitProgress(66, "Initialize...");
	Sleep(200);
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateInit::OnBeginState()
{

}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_RETURN)
	{
		CAudio::Instance()->Stop(0);
		GotoGameState(GAME_STATE_RUN); // ������GAME_STATE_RUN
	}
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void CGameStateInit::OnShow()
{
	background.ShowBitmap();
	draw_text();
}

void CGameStateInit::load_background() {
	background.LoadBitmapByString({ "resources/background.bmp" });
	background.SetTopLeft(0, 0);

	
}


void CGameStateInit::draw_text() {
	CDC *pDC = CDDraw::GetBackCDC();

	/* Print title */
	//CTextDraw::ChangeFontLog(pDC, 36, "�L�n������", RGB(255, 255, 255));
	//CTextDraw::Print(pDC, 180, 90, "Push the Box!!");

	/* Print info */
	CTextDraw::ChangeFontLog(pDC, 24, "�L�n������", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 182, 431, "Press ENTER to start");

	CDDraw::ReleaseBackCDC();
}