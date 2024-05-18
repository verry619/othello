#include "GuiMainWnd.h"
#include "GuiPainter.h"
#include "resource.h"
#include <windowsx.h>
#include <string>

#define BOARD_ROW_LEN 6
#define BOARD_COL_LEN 6

constexpr int TEXT_BLACK_POS_X = 0;
constexpr int TEXT_BLACK_POS_Y = 0;
constexpr int TEXT_WHITE_POS_X = 550;
constexpr int TEXT_WHITE_POS_Y = 0;

GuiMainWnd::GuiMainWnd(HINSTANCE hInstance, int nCmdShow, GuiMainWndCallbackFuncs callbacks, GuiBoardVV*& vv)
	:m_hInst(hInstance),
	m_nCmdShow(nCmdShow),
	m_callbacks(callbacks),
	m_discVV(vv)
{
	m_pcSettingDialog = new GuiSettingDialog();

	// �O���[�o�������������������
	LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUI, m_szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
}


//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
ATOM GuiMainWnd::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CallbackLauncher_WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUI);
	wcex.lpszClassName = m_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂�
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL GuiMainWnd::InitInstance(void)
{
	m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, this);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, m_nCmdShow);
	UpdateWindow(m_hWnd);

	GuiSettingDialog::InitSetting();

	return TRUE;
}

void GuiMainWnd::DrawBoard(GuiBoardVV*& vv)
{
	DrawBoardForPaint(vv);
	UpdateWindow(m_hWnd);
}

void GuiMainWnd::DrawBoardForPaint(GuiBoardVV*& vv)
{
	unsigned char ucRow = vv->GetRow();
	unsigned char ucCol = vv->GetCol();

	DISC* penBoard = new DISC[ucRow * ucCol];

	for (unsigned char r = 0;r < ucRow;r++)
	{
		for (unsigned char c = 0;c < ucCol;c++)
		{
			penBoard[ucCol * r + c] = vv->GetDisc(r, c);
		}
	}

	InvalidateRect(m_hWnd, NULL, FALSE);

	GuiPainter* pcPainter = new GuiPainter();
	pcPainter->DrawBoard(m_hWnd, m_hInst, ucRow, ucCol, penBoard);
	delete pcPainter;
	delete[] penBoard;
}

void GuiMainWnd::PopupDialog(void)
{
	DialogBox(m_hInst, MAKEINTRESOURCE(IDD_ERR_MSG_INVALID_MOVE), m_hWnd, CallbackLauncher_InvalidDiscPos);
}

LRESULT CALLBACK GuiMainWnd::CallbackLauncher_InvalidDiscPos(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiMainWnd* app;
	if (message == WM_CREATE) {
		app = (GuiMainWnd*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		app = (GuiMainWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
	return app->InvalidDiscPos(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK GuiMainWnd::InvalidDiscPos(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK GuiMainWnd::CallbackLauncher_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiMainWnd* app;

	if (message == WM_CREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		app = reinterpret_cast<GuiMainWnd*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else {
		app = (GuiMainWnd*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if (app)
	{
		return app->WndProc(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI: ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E��`�悷��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK GuiMainWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int x = 0;
	static int y = 0;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case ID_GAME_START:
			m_callbacks.m_callbackGameStart();
			break;
		case ID_GAME_SETTING:
			DialogBoxParam(m_hInst, MAKEINTRESOURCE(IDD_GAME_SETTING), m_hWnd, GuiSettingDialog::CallbackLauncher_GameSetting, reinterpret_cast<LPARAM>(static_cast<void*>(m_pcSettingDialog)));
			break;
		case IDM_EXIT:
			DestroyWindow(m_hWnd);
			break;
		default:
			return DefWindowProc(m_hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		DrawBoardForPaint(m_discVV);
		BoardInfo board = BoardInfo(m_discVV->GetVV());
		unsigned short usBlack = 0;
		unsigned short usWhite = 0;
		(void)board.CountDiscs(usBlack, usWhite);

		HDC hdc = GetDC(hWnd);
		TextOut(hdc, TEXT_BLACK_POS_X, TEXT_BLACK_POS_Y, TEXT("BLACK"), 5);
		TextOut(hdc, TEXT_BLACK_POS_X, TEXT_BLACK_POS_Y + 50, std::to_wstring(usBlack).data(), std::to_wstring(usBlack).length());
		TextOut(hdc, TEXT_WHITE_POS_X, TEXT_WHITE_POS_Y, TEXT("WHITE"), 5);
		TextOut(hdc, TEXT_WHITE_POS_X, TEXT_WHITE_POS_Y + 50, std::to_wstring(usWhite).data(), std::to_wstring(usBlack).length());
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_LBUTTONUP: //�}�E�X���N���b�N
	{
		//lParam����}�E�X���W�����o��
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		DISC_POS enDiscPos;
		if (GuiCalc::ConvDrawPosToDiscPos({ x, y }, enDiscPos, 6, 6))
		{
			m_callbacks.m_callbackPutDisc(enDiscPos);
		}

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}

GAME_SETTING GuiMainWnd::GetCurrentGameSetting(void)
{
	return GuiSettingDialog::GetCurrentGameSetting();
}