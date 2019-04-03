#include "stdafx.h"
#include "WinApiModule.h"
#include "Color.h"
#include <string>
#include <string_view>
#include <array>

using namespace std;

bool paused = false;
int counter = -1;

class Application
{
	static const int MinDialogWidth = 300;
	static const int MinDialogHeight = 250;

	static const int Timer1ID = 1;
	static const int Timer5ID = 2;

	static const int Timer1Interval = 1000;
	static const int Timer15Interval = 6000;

	static const int Timer1MaxValue = 5;
	static const int Timer2MaxValue = 3;

	static int _timer1Ticks;
	static int _timer15Ticks;

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
		{
			OnCreate(hWnd);
			return true;
		}

		case WM_GETMINMAXINFO:
		{
			OnGetMinMaxInfo((LPMINMAXINFO)lParam);
			return TRUE;
		}

		case WM_TIMER:
		{
			OnTimer1(hWnd, (int)wParam);
			return true;
		}

		case WM_KEYDOWN:
		{
			if (wParam == VK_SPACE)
			{
				if (paused == false)
					paused = true;
				else
					paused = false;
			}
			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps);
			OnPaint(hWnd, hdc);
			EndPaint(hWnd, &ps);
			break;
		}


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
		SetTimer(hWnd, Timer1ID, Timer1Interval, nullptr);
		SetTimer(hWnd, Timer5ID, Timer15Interval, TimerProc);
	}

	static void OnGetMinMaxInfo(LPMINMAXINFO minMaxInfo)
	{
		minMaxInfo->ptMinTrackSize.x = MinDialogWidth;
		minMaxInfo->ptMinTrackSize.y = MinDialogHeight;
	}

	static void OnTimer1(HWND hWnd, int timerID)
	{
		if (timerID != Timer1ID) return;
		if(!paused)
		++_timer1Ticks;

		if (_timer1Ticks > Timer1MaxValue)
		{
			_timer1Ticks = 0;
		}

		InvalidateRect(hWnd, nullptr, true);
	}
	
	


	static void CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR idEvent, DWORD time)
	{
		if (idEvent != Timer5ID) return;
		if (!paused)
			++_timer15Ticks;
		

		if (_timer15Ticks > Timer2MaxValue )
		{
			_timer15Ticks = 0;
		}

		InvalidateRect(hWnd, nullptr, true);
	}

	static void OnPaint(HWND hWnd, HDC hdc)
	{

		auto clientRect = GetClientRect(hWnd);
		auto width = (clientRect.right - clientRect.left) / 2;
		auto height = (clientRect.bottom - clientRect.top) / 2;
		auto scale = 100;
		if(!paused)
		{
			if (_timer1Ticks == 0)
			{
				if (counter < 4)
					counter++;
				else
					counter = 0;
			}
		}

		switch (counter)
			{
			case 0:
			{
				DrawEllipse(hdc, width, height, scale, 255, 255, 0);
				height += 100;
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += -200;
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += 100;
				DrawTimerTicksText(hdc, width, height, scale);
				break;
			}
			case 1:
			{
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += 100;
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += -200;
				DrawEllipse(hdc, width, height, scale, 255, 0, 0);
				height += 100;
				DrawTimerTicksText(hdc, width, height, scale);
				break;
			}
			case 2:
			{
				DrawEllipse(hdc, width, height, scale, 255, 255, 0);
				height += 100;
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += -200;
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += 100;
				DrawTimerTicksText(hdc, width, height, scale);

				break;
			}
			case 3:
			{
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += 100;
				DrawEllipse(hdc, width, height, scale, 0, 255, 0);
				height += -200;
				DrawEllipse(hdc, width, height, scale, 128, 128, 128);
				height += 100;
				DrawTimerTicksText(hdc, width, height, scale);
				break;
			}
			
		}
	}


	static RECT GetClientRect(HWND hWnd)
	{
		RECT rect;
		::GetClientRect(hWnd, &rect);
		return rect;
	}

	static void DrawLines(HDC hdc, long width, long height)
	{
		auto defaultPen = SelectObject(hdc, CreatePen(PS_DASHDOTDOT, 1, Color::SteelBlue2));

		MoveToEx(hdc, 0, height / 2, nullptr);
		LineTo(hdc, width, height / 2);

		MoveToEx(hdc, width / 2, 0, nullptr);
		LineTo(hdc, width / 2, height);

		SelectObjectAndDeletePrevious(hdc, defaultPen);
	}

	static void DrawRectangle(HDC hdc, long width, long height, long scale)
	{
		auto rectangleColor = (_timer15Ticks != 0) ? Color::Firebrick : Color::Sienna1;
		auto defaultBrush = SelectObject(hdc, CreateSolidBrush(rectangleColor));
		auto defaultPen = SelectObject(hdc, CreatePen(PS_SOLID, 2, Color::Black));

		DrawRectangle(hdc, CalculateShapeRect(width / 4, height / 4, 0.85 * scale, 0.95 * scale));

		SelectObjectAndDeletePrevious(hdc, defaultBrush);
		SelectObjectAndDeletePrevious(hdc, defaultPen);
	}

	static void DrawEllipse(HDC hdc, long width, long height, long scale,int r , int g , int b)
	{
		auto defaultBrush = SelectObject(hdc, CreateSolidBrush(RGB(r,g,b)));

		DrawEllipse(hdc, CalculateShapeRect(width , height , 0.95 * scale, 0.85 * scale));

		SelectObjectAndDeletePrevious(hdc, defaultBrush);
	}

	static void DrawRoundRectangle(HDC hdc, long width, long height, long scale)
	{
		auto penBrush = LOGBRUSH{};
		penBrush.lbColor = Color::Turquoise3;
		auto defaultPen = SelectObject(hdc, ExtCreatePen(PS_GEOMETRIC | PS_DASH, 3, &penBrush, 0, nullptr));

		DrawRoundRectangle(hdc, CalculateShapeRect(width , height, 0.95 * scale, 0.95 * scale), 0.15 * scale, 0.20 * scale);

		SelectObjectAndDeletePrevious(hdc, defaultPen);
	}

	static void DrawTimerTicksText(HDC hdc, long width, long height, long scale)
	{
		auto defaultFont = SelectObject(hdc, CreateFont(L"Consolas", scale / 2, FW_BOLD));
		SetTextColor(hdc, Color::SlateBlue);
		SetBkMode(hdc, TRANSPARENT);

		auto textRectangle = RECT{ width - scale, height - scale, width + scale, height  + scale};
		auto text = std::to_wstring(_timer1Ticks);
		DrawTextW(hdc, text.c_str(), -1, &textRectangle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		SelectObjectAndDeletePrevious(hdc, defaultFont);
	}

	static HFONT CreateFont(std::wstring name, int size, int weight)
	{
		auto lf = LOGFONT{};
		wcscpy_s(lf.lfFaceName, name.data());
		lf.lfHeight = size;
		lf.lfWeight = weight;
		lf.lfCharSet = DEFAULT_CHARSET;
		return CreateFontIndirect(&lf);
	}

	static void DrawTriangle(HDC hdc, long width, long height, long scale)
	{
		auto defaultBrush = SelectObject(hdc, CreateSolidBrush(Color::SpringGreen3));
		auto defaultPen = SelectObject(hdc, CreatePen(PS_NULL, 0, Color::Black));

		auto points = InitializeTrianglePoints(CalculateShapeRect(width * 3 / 4, height * 3 / 4, 0.85 * scale, 0.85 * scale));
		Polygon(hdc, points.data(), points.size());

		SelectObjectAndDeletePrevious(hdc, defaultBrush);
		SelectObjectAndDeletePrevious(hdc, defaultPen);
	}

	static std::array<POINT, 3> InitializeTrianglePoints(RECT triangleRect)
	{
		return
		{
			POINT { (triangleRect.left + triangleRect.right) / 2, triangleRect.top },
			POINT { triangleRect.right, triangleRect.bottom },
			POINT { triangleRect.left, triangleRect.bottom },
		};
	}

	static RECT CalculateShapeRect(int xCenter, int yCenter, double width, double heigth)
	{
		auto x = xCenter - (int)width / 2;
		auto y = yCenter - (int)heigth / 2;
		return RECT{ x, y, x + (int)width, y + (int)heigth };
	}

	static void SelectObjectAndDeletePrevious(HDC hDeviceContext, HGDIOBJ gdiObject)
	{
		auto previousGdiObject = SelectObject(hDeviceContext, gdiObject);
		DeleteObject(previousGdiObject);
	}

	static void DrawRectangle(HDC hDeviceContext, RECT rect)
	{
		Rectangle(hDeviceContext, rect.left, rect.top, rect.right, rect.bottom);
	}

	static void DrawRoundRectangle(HDC hDeviceContext, RECT rect, double roundWidth, double roundHeigth)
	{
		RoundRect(hDeviceContext, rect.left, rect.top, rect.right, rect.bottom, (int)roundWidth, (int)roundHeigth);
	}

	static void DrawEllipse(HDC hDeviceContext, RECT rect)
	{
		Ellipse(hDeviceContext, rect.left, rect.top, rect.right, rect.bottom);
	}
};

int Application::_timer1Ticks = 0;
int Application::_timer15Ticks = 0;

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto winApiModule = WinApiModule(
		L"9_Timer_and_Drawing",
		L"9. Таймер и рисование",
		hInstance,
		Application::WndProc);

	return winApiModule.Run(nCmdShow);
}