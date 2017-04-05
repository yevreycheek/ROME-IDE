// автор Александрович Алексей Борисович
//Author: Aleksandrovich Aleksey Borisovich
//Created: x.x.2017
//License: GPL v2

#include <Windows.h>
#include <commctrl.h>
#define ID_EDITCHILD 133

HWND hWnd, button1, button2, hwndEdit; // хранитель окон
HMENU hMenu, hMenu1; // хранитель менюшек 

char buf[1000]; // переменая для хранения адреса файла 
HINSTANCE hIn;
bool fileisOpen;
HANDLE hFile;
DWORD dwBytes = 0; int BufSize = 0;
char *Buffer = { 0 };

TCHAR lpszLatin[] = { "Hello world !" }; // начальный текст в окне 

void GetFileName(); // функция получения имени файла 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // прототип функции обработки сообщений (реализация ниже )
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR); // прототип функции регистрации нашего класса (реализация ниже )

void Draw(HWND hWnd, HWND button, HWND button2) { // функция которая рисует кнопку 
	button = CreateWindow( // создаём первую кнопку 
		"button", // класс 
		"Открыть файл .txt", // надпись внутри кнопки 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // стили окна  
		110, // координата x
		400, // координата y
		200, // ширина
		60, // высота 
		hWnd, // id родителя 
		(HMENU)1001, // сообщение отправляемое кнопкой в данном случае 1001
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), // хз
		NULL); // хз

	button2 = CreateWindow(
		"button", // класс 
		"Батон №2", // надпись внутри кнопки 
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, // стили окна  
		350, // координата x
		400, // координата y
		100, // ширина
		60, // высота 
		hWnd, // id родителя 
		(HMENU)1002, // сообщение отправляемое кнопкой в данном случае 1002
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), // хз
		NULL); // хз




}

void messageButtom(WPARAM wParam) { // обработчик нажатия кнопки 
	if (LOWORD(wParam) == 1001) {

		GetFileName();
		if (fileisOpen == true) {
			hFile = CreateFile(buf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			BufSize = GetFileSize(hFile, NULL); // узнаём размер файла 

			Buffer = new char[BufSize]; // создаём массив символов под размер файла 

			ReadFile(hFile, Buffer, BufSize, &dwBytes, NULL); // считываем файл в массив 
			CloseHandle(hFile); // закрываем файл 

			SetWindowTextA(hwndEdit, Buffer); // выводим массив в редактор 
		}
	}
	else if (LOWORD(wParam) == 1002) {
		MessageBox(hWnd, TEXT("Заебала нах овцэ !"), TEXT("событие"), 0);
	}
	else if (LOWORD(wParam) == 100) {
		MessageBox(hWnd, TEXT("Убери руки от кнопки1 меню! "), TEXT("событие"), 0);
	}
	else if (LOWORD(wParam) == 101) {
		MessageBox(hWnd, TEXT("Убери руки от кнопки2 меню! "), TEXT("событие"), 0);
	}
	else if (LOWORD(wParam) == 99) {
		MessageBox(hWnd, TEXT("Убери руки от кнопки Сервис! "), TEXT("событие"), 0);
	}

}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	LPCTSTR lpzClass = TEXT("My Window Class!"); // тупо имя класса 


	if (!RegMyWindowClass(hInstance, lpzClass)) { // пробуем зарегистрировать класс окна , если нет , то возвращаем 1 
		return 1;
	}


	RECT screen_rect; // переменная размера экрана пользователя 

					  // получаем размер нашего экрана 
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int x = screen_rect.right / 2 - 400; // координаты появления окна (800/2=400)
	int y = screen_rect.bottom / 2 - 300;// координаты появления окна (600/2=300)

	hWnd = CreateWindow(lpzClass, TEXT("Hello"), // название окна 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, 800, 600, NULL, NULL, // стиль и размер окна
		hInstance, NULL);

	// рисуем кнопку 
	Draw(hWnd, button1, button2);


	//рисуем менюшки
	hMenu = CreateMenu(); // создаём меню
	hMenu1 = CreateMenu(); // создаём подменю
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hMenu1, "Файл"); // наполняем его параметрами
	AppendMenu(hMenu, MF_STRING, 99, "Сервис"); // наполняем его параметрами
	AppendMenu(hMenu1, MF_STRING, 1001, "Открыть .. "); // наполняем его параметрами
	AppendMenu(hMenu1, MF_STRING, 101, "кнопка 2"); // наполняем его параметрами
	SetMenu(hWnd, hMenu); // добавляем меню на главное окно 
	SetMenu(hWnd, hMenu1); // добавляем меню на главное окно 
						   //


	if (!hWnd) return 2;
	MSG msg = { 0 };
	int iGetOk = 0;
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (iGetOk == -1) return 3;
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return msg.wParam;

}


ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName) { // функция регистрации нашего класса

	WNDCLASS wcWindowClass = { 0 }; // Создаём класс . все поля этого класса равны нулю

	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW; //задаём стиль окна 
	wcWindowClass.hInstance = hInst; // принимает дискрипт экземпляра приложения из аргументов этой функции 
	wcWindowClass.lpszClassName = lpzClassName; // передаём ему название класса 
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW); // подгружаем курсор 
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE; // задаём цвет окна
	return RegisterClass(&wcWindowClass); // регистрируем сам класс 
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {//  функция обработки сообщений
	switch (message) {
	case WM_CREATE:
		hwndEdit = CreateWindowEx(
			0, "EDIT",   // создаём класс EDIT 
			NULL,         // без TITLE 
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			0, 0, 0, 0,   // set size in WM_SIZE message 
			hWnd,         // parent window 
			(HMENU)ID_EDITCHILD,   // edit control ID 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);        // pointer not needed 

						  // Add text to the window. 
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)lpszLatin);

		return 0;
	case WM_LBUTTONUP:  // реакция на сообщение клика левой клавиши мыши
	case WM_COMMAND:  // реакция на клик по кнопке 
		messageButtom(wParam);// функция обработки сообщения она принимает сами сообщения.
		break;
	case WM_SIZE:
		// Make the edit control the size of the window's client area. 

		MoveWindow(hwndEdit,
			0, 0,                  // starting x- and y-coordinates 
			LOWORD(lParam),        // width of client area 
			350,        // height of client area 
			TRUE);                 // repaint window 
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0); // реакция на сообщение закрытия окна 
		break;
	default:
		// все сообщения не обрабатываемые вами обработает сама винда 

		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}


void GetFileName()
{

	char szFileName[1000] = { 0 };
	OPENFILENAME OpenFileName;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hInstance = hIn;
	ofn.lpstrFilter = ("Выбери текстовый документ .txt\0*.txt\0\0");
	ofn.lpstrTitle = "Заголовок";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = sizeof(szFileName);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (GetOpenFileName(&ofn))
	{
		strcpy_s(buf, szFileName);
		fileisOpen = true;
	}
	else { fileisOpen = false; MessageBox(hWnd, TEXT("Файл не открыт !"), TEXT("событие"), 0); }
}
