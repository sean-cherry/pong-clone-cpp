#include "utilities.cpp"
#include <Windows.h>
//#include <Utilapiset.h>

global_variable bool running = true;

struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};

global_variable Render_State renderstate;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY: {running = false;
	}break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		renderstate.width = rect.right - rect.left;
		renderstate.height = rect.bottom - rect.top;
		int size = renderstate.width * renderstate.height * sizeof(unsigned int);

		if (renderstate.memory) VirtualFree(renderstate.memory, 0, MEM_RELEASE);
		renderstate.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderstate.bitmap_info.bmiHeader.biSize = sizeof(renderstate.bitmap_info.bmiHeader);
		renderstate.bitmap_info.bmiHeader.biWidth = renderstate.width;
		renderstate.bitmap_info.bmiHeader.biHeight = renderstate.height;
		renderstate.bitmap_info.bmiHeader.biPlanes = 1;
		renderstate.bitmap_info.bmiHeader.biBitCount = 32;
		renderstate.bitmap_info.bmiHeader.biCompression = BI_RGB;
	} break;
	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
				return result;
	}

	int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
		//create window class
		WNDCLASS window_class = {};
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpszClassName = L"Game Window Class";
		window_class.lpfnWndProc = window_callback;
		//register class
		RegisterClass(&window_class);
		//create window
		HWND window = CreateWindow(window_class.lpszClassName, L"My First Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
		HDC hdc = GetDC(window);

		Input input = {};

		float delta_time = 0.0166666f;
		LARGE_INTEGER frame_begin_time;
		QueryPerformanceCounter(&frame_begin_time);

		float performance_frequency;
		{
			LARGE_INTEGER perf;
			QueryPerformanceFrequency(&perf);
			performance_frequency = (float)perf.QuadPart;
		}

		while (running) {
			//Input
			MSG message;

			for (int i = 0; i < BUTTON_COUNT; i++)
				input.buttons[i].changed = false;

			while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
				switch ((message.message)) {
					case WM_KEYUP:
					case WM_KEYDOWN: {
						u32 vk_code = (u32)message.wParam;
						bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk: {\
	input.buttons[b].changed = is_down != input.buttons[b].is_down;\
	input.buttons[b].is_down = is_down;\
}break;

						switch (vk_code) {
							process_button(BUTTON_UP, VK_UP);
							process_button(BUTTON_DOWN, VK_DOWN);
							process_button(BUTTON_W, 'W');
							process_button(BUTTON_S, 'S');
						}
					}break;

					default: {
						TranslateMessage(&message);
						DispatchMessage(&message);
					}
				}
					
			}
			//Simulate
			simulate_game(&input, delta_time);
			
			//Render
			StretchDIBits(hdc, 0, 0, renderstate.width, renderstate.height, 0, 0, renderstate.width, renderstate.height, renderstate.memory, &renderstate.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
			

			LARGE_INTEGER frame_end_time;
			QueryPerformanceCounter(&frame_end_time);
			delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart)/performance_frequency;
			frame_begin_time = frame_end_time;
		}
		return 0;
	}