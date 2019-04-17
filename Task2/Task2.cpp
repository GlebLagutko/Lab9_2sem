#include "stdafx.h"
#include "WinApiModuleh.h"
#include "Colors.h"
#include <string>

class Application
{
	static bool mouseClicked;
	static bool rectangleDragged;
	static const int rowsCount = 2;
	static const int columnsCount = 2;
	static const int fieldCount = 2;
	static const int paddingX = 50;
	static const int paddingY = 50;
	static const int border = 25;
	static const int textFieldHeight = 25;
	static const int squareSize = 50;
	static const int fieldHeight = columnsCount * (squareSize + border) + border;
	static const int fieldWidth = rowsCount * (squareSize + border) + border;
	static const int fieldSpacing = 75;
	static bool rectangles[fieldCount][rowsCount * columnsCount];
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
		{
			auto mouseX = LOWORD(lParam);
			auto mouseY = HIWORD(lParam);
			mouseClicked = true;
			InvalidateRect(hWnd, nullptr, true);

			for (int i = 0; i < fieldCount; i++)
			{
				for (int j = 0; j < rowsCount; j++)
				{
					for (int k = 0; k < columnsCount; k++)
					{
						if (rectangles[i][j * rowsCount + k])
						{
							if (mouseX >= paddingX + i * (fieldWidth + fieldSpacing) + border + k * (border + squareSize)
								&& mouseY >= paddingY + textFieldHeight + j * (border + squareSize)
								&& mouseX <= paddingX + i * (fieldWidth + fieldSpacing) + border + squareSize + k * (border + squareSize)
								&& mouseY <= paddingY + textFieldHeight + j * (border + squareSize) + squareSize)
							{
								rectangles[i][j * rowsCount + k] = false;
								rectangleDragged = true;
							}
						}
					}
				}
			}
			if (rectangleDragged)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				POINT positionLeftTop = { rect.left, rect.top };
				ClientToScreen(hWnd, &positionLeftTop);
				POINT positionRightBottom = { rect.right, rect.bottom };
				ClientToScreen(hWnd, &positionRightBottom);
				RECT rectCapture;
				SetRect(&rectCapture, positionLeftTop.x, positionLeftTop.y, positionRightBottom.x - squareSize, positionRightBottom.y - squareSize);
				ClipCursor(&rectCapture);
			}
			break;
		}

		case WM_LBUTTONUP:
		{
			auto mouseX = LOWORD(lParam);
			auto mouseY = HIWORD(lParam);
			mouseClicked = false;
			InvalidateRect(hWnd, nullptr, true);
			ClipCursor(nullptr);
			if (rectangleDragged)
			{
				rectangleDragged = false;
				int fieldIndex = mouseX < paddingX + fieldWidth + fieldSpacing + border / 2 ? 0 : 1;
				for (int i = 0; i < rowsCount * columnsCount; i++)
				{
					if (!rectangles[fieldIndex][i])
					{
						rectangles[fieldIndex][i] = true;
						break;
					}
				}
			}
			break;
		}
		case WM_CREATE:
		{
			OnCreate(hWnd);
			return true;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps);
			OnPaint(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
		}

		case WM_MOUSEMOVE:
			OnMouseMove(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

private:
	static void OnCreate(HWND hWnd)
	{
		rectangles[0][0] = true;
		rectangles[0][1] = true;
		rectangles[0][2] = true;
		rectangles[0][3] = true;

		rectangles[1][0] = false;
		rectangles[1][1] = false;
		rectangles[1][2] = false;
		rectangles[1][3] = false;
	}

	static void SetRectFields(RECT* rect, int x, int y, int width, int height)
	{
		SetRect(rect, x, y, x + width, y + height);
	}

	static void OnMouseMove(HWND hWnd)
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(hWnd, &mousePosition);

		InvalidateRect(hWnd, nullptr, true);
	}

	static void OnPaint(HWND hWnd, HDC hdc)
	{
		auto fieldRectBrush = CreateSolidBrush(Color::DeepSkyBlue);
		auto objectRectBrush = CreateSolidBrush(Color::Sienna1);
		auto highlitedObjectRectBrush = CreateSolidBrush(Color::Firebrick);

		for (int i = 0; i < 2; i++)
		{
			RECT rect = { 135 + i * 250, 25, 200 + i * 250, 50 };
			int quantity = 0;
			for (int j = 0; j < 4; j++)
			{
				if (rectangles[i][j]) quantity++;
			}
			auto text = std::to_wstring(quantity);
			DrawTextW(hdc, text.c_str(), text.size(), &rect, NULL);
		}

		SelectObject(hdc, fieldRectBrush);
		for (int i = 0; i < fieldCount; i++)
		{
			RECT Field = { 50 + i * 250, 50, 225 + i * 250, 225 };
			DrawRectangle(hdc, Field);
		}

		for (int i = 0; i < fieldCount; i++)
		{
			for (int j = 0; j < rowsCount; j++)
			{
				for (int k = 0; k < columnsCount; k++)
				{
					if (rectangles[i][j * rowsCount + k])
					{
						SelectObject(hdc, objectRectBrush);
						RECT rect = { 50 + i * 250 + 25 + k * 75, 75 + j * 75 , 50 + i * 250 + 25 + 50 + k * 75, 75 + j * 75 + 50 };
						DrawRectangle(hdc, rect);
					}
				}
			}
		}

		if (rectangleDragged)
		{
			bool inField = false;
			POINT mousePosition;
			GetCursorPos(&mousePosition);
			ScreenToClient(hWnd, &mousePosition);
			for (int i = 0; i < 2; i++)
			{
				if (mousePosition.x >= 50 + i * 250 && mousePosition.x <= 175 + i * 250 && mousePosition.y >= 50 && mousePosition.y <= 175)
				{
					inField = true;
				}
			}
			SelectObject(hdc, !inField ? highlitedObjectRectBrush : objectRectBrush);
			RECT rect = { mousePosition.x, mousePosition.y, 50 + mousePosition.x, 50 + mousePosition.y };
			DrawRectangle(hdc, rect);
		}

		DeleteObject(fieldRectBrush);
		DeleteObject(objectRectBrush);
		DeleteObject(highlitedObjectRectBrush);
	}


	static void DrawRectangle(const HDC& hdc, const RECT& rect)
	{
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
};

bool Application::rectangles[Application::fieldCount][Application::columnsCount * Application::rowsCount];
bool Application::mouseClicked = false;
bool Application::rectangleDragged = false;

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto winApiModule = WinApiModule(
		L"Lab9",
		L"Lab9",
		hInstance,
		Application::WndProc);

	return winApiModule.Run(nCmdShow);
}