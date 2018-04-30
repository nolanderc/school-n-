#include "Window.h"


std::vector<Window*> Window::windows;

// Return id's in increasing order
int getNextID() {
	static int CURRENT_ID = 0;
	int nextID = CURRENT_ID;
	CURRENT_ID++;
	return nextID;
}


Window::Window()
{
}

Window::Window(int width, int height, std::string windowName)
{
	// Hämta detta programs HINSTANCE
	HINSTANCE hInstance = GetModuleHandle(NULL);

	this->size.cx = width;
	this->size.cy = height;

	WNDCLASSEX WndClsEx = createClassEx(hInstance, windowName.c_str());

	// Registrera WndClsEx i Operativsystemet
	if (!RegisterClassEx(&WndClsEx)) {
		MessageBox(NULL, "Failed to register host window!", "ERROR", MB_ICONERROR | MB_OK);
		return;
	}

	// Beskriv fönstrets stil
	this->style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER;

	// Justera fönstrets storlek
	SIZE adjustedSize = adjustSize(width, height, style);

	// Skapa ett nytt fönster
	handle = CreateWindow(windowName.c_str(), windowName.c_str(),
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		adjustedSize.cx, adjustedSize.cy,
		NULL, NULL, hInstance, NULL);

	if (!handle) {
		MessageBox(NULL, "Failed to create host window!", "ERROR", MB_ICONERROR | MB_OK);
		return;
	}

	// Centrera fönstret
	this->setSize(width, height);

	id = getNextID();
	windows.push_back(this);

	this->dc = GetDC(this->handle);
	this->backBuffer = CreateCompatibleBitmap(this->dc, this->size.cx, this->size.cy);
}

void Window::show(int nCmdShow)
{
	ShowWindow(handle, nCmdShow);
	UpdateWindow(handle);

	open = true;
}

int Window::getID()
{
	return id;
}

bool Window::isOpen()
{
	return open;
}

void Window::close()
{
	this->open = false;
}

Vector2i Window::getSize()
{
	Vector2i size(this->size.cx, this->size.cy);
	return size;
}

void Window::setSize(int width, int height)
{
	SIZE adjusted = adjustSize(width, height, this->style);

	SIZE screenSize;
	screenSize.cx = GetSystemMetrics(SM_CXSCREEN);
	screenSize.cy = GetSystemMetrics(SM_CYSCREEN);

	// Centerara fönstret
	int newX = (screenSize.cx - adjusted.cx) / 2;
	int newY = (screenSize.cy - adjusted.cy) / 2;

	SetWindowPos(this->handle, NULL, newX, newY, adjusted.cx, adjusted.cy, NULL);
}

void Window::setTitle(std::string title)
{
	SetWindowText(this->handle, title.c_str());
}

POINT Window::getMousePosition()
{
	static POINT position;

	POINT screenPosition;
	if (GetCursorPos(&screenPosition)) {
		if (ScreenToClient(this->handle, &screenPosition)) {
			position = screenPosition;
		}
	}

	return position;
}

bool Window::isKeyDown(int key)
{
	int keyCount = this->pressedKeys.size();
	for (int i = 0; i < keyCount; i++) {
		if (this->pressedKeys[i] == key) {
			return true;
		}
	}

	return false;
}

WindowsBitmap* Window::createCompatibleBitmap(Vector2i size)
{
	return new WindowsBitmap(CreateCompatibleBitmap(this->dc, size.x, size.y), {size.x, size.y});
}


WindowsRenderer Window::getNewRenderer()
{
	HDC backDC = CreateCompatibleDC(this->dc);

	SelectObject(backDC, this->backBuffer);
	return WindowsRenderer(backDC, this->size);
}

void Window::submitRenderer(WindowsRenderer & renderer)
{
	renderer.blitResult(this->dc, 0, 0, this->size.cx, this->size.cy);
	DeleteDC(renderer.releaseDC());
}


void Window::pollMessages()
{
	// Hantera alla anlända meddelanden
	MSG msg;
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) {
			open = false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::setEventHandler(WindowEventHandler * handler)
{
	this->eventHandler = handler;
}

HWND Window::getHandle()
{
	return handle;
}

LRESULT Window::windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int windowCount = windows.size();
	for (int i = 0; i < windowCount; i++)
	{
		Window* window = windows[i];

		// Hantera enbart meddelanden ifån fönster vi känner till
		if (window->handle == hWnd) {
			switch (msg) {
				// Stäng fönstret
			case WM_DESTROY:
				PostQuitMessage(0);
				return DefWindowProc(hWnd, msg, wParam, lParam);

				// Hantera tangentbordet
			case WM_KEYDOWN:
				if (!window->isKeyDown(wParam)) {
					window->eventHandler->keyPressed(wParam);
					window->pressedKeys.push_back(wParam);
				}
				break;

			case WM_KEYUP: {
				int keyCount = window->pressedKeys.size();
				for (int i = 0; i < keyCount; i++) {
					if (window->pressedKeys[i] == wParam) {
						window->pressedKeys.erase(window->pressedKeys.begin() + i);
						i--;
						keyCount--;
					}
				}

				window->eventHandler->keyReleased(wParam);
			}
			break;

				// Hantera musknappar
			case WM_LBUTTONDOWN: window->eventHandler->mousePressed(MouseButton::MOUSE_LEFT, LOWORD(lParam), HIWORD(lParam)); break;
			case WM_MBUTTONDOWN: window->eventHandler->mousePressed(MouseButton::MOUSE_MIDDLE, LOWORD(lParam), HIWORD(lParam)); break;
			case WM_RBUTTONDOWN: window->eventHandler->mousePressed(MouseButton::MOUSE_RIGHT, LOWORD(lParam), HIWORD(lParam)); break;

			case WM_LBUTTONUP: window->eventHandler->mouseReleased(MouseButton::MOUSE_LEFT, LOWORD(lParam), HIWORD(lParam)); break;
			case WM_MBUTTONUP: window->eventHandler->mouseReleased(MouseButton::MOUSE_MIDDLE, LOWORD(lParam), HIWORD(lParam)); break;
			case WM_RBUTTONUP: window->eventHandler->mouseReleased(MouseButton::MOUSE_RIGHT, LOWORD(lParam), HIWORD(lParam)); break;

				// Hantera muspekaren
			case WM_MOUSEMOVE: {
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				if (x <= window->size.cx && y <= window->size.cy && (x != window->mousePosition.x || y != window->mousePosition.y)) {
					window->eventHandler->mouseMoved(x, y);

					window->mousePosition.x = x;
					window->mousePosition.y = y;
				}
				} break;


			case WM_MOUSEWHEEL: {
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);
				
				window->eventHandler->mouseScrolled(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA, x, y);

				} break;


				// Fönstret ändrade storlek
			case WM_SIZE:
				window->size.cx = LOWORD(lParam);
				window->size.cy = HIWORD(lParam);

				DeleteObject(window->backBuffer);
				window->backBuffer = CreateCompatibleBitmap(window->dc, window->size.cx, window->size.cy);

				if (window->eventHandler) {
					window->eventHandler->sizeChanged(window->size.cx, window->size.cy);
				}
				break;


				// Hanter omritningar
			case WM_PAINT:
				if (window->eventHandler) {
					window->eventHandler->repaint();
				}
				return DefWindowProc(hWnd, msg, wParam, lParam);

			default:
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

WNDCLASSEX Window::createClassEx(HINSTANCE hInstance, LPCSTR className)
{
	WNDCLASSEX WndClsEx;
	WndClsEx.cbSize = sizeof(WNDCLASSEX);
	WndClsEx.style = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc = Window::windowProcedure;
	WndClsEx.cbClsExtra = 0;
	WndClsEx.cbWndExtra = 0;
	WndClsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClsEx.lpszMenuName = NULL;
	WndClsEx.lpszClassName = className;
	WndClsEx.hInstance = hInstance;
	WndClsEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	return WndClsEx;
}

SIZE Window::adjustSize(int width, int height, DWORD style)
{
	// Fönstrets form
	RECT windowRect;
	windowRect.left = 0;
	windowRect.right = width;
	windowRect.top = 0;
	windowRect.bottom = height;

	// Justera formen baserat på fönstrets stil
	AdjustWindowRect(&windowRect, style, false);

	SIZE adjusted = { width, height };

	if (width != CW_USEDEFAULT)
		adjusted.cx = windowRect.right - windowRect.left;
	if (height != CW_USEDEFAULT)
		adjusted.cy = windowRect.bottom - windowRect.top;

	return adjusted;
}
