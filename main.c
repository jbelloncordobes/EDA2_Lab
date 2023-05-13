#define _UNICODE
#define UNICODE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <Windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <time.h>
#include "headers/handleStack.h"
#include "headers/common.h"
#include "headers/users.h"

#pragma comment(lib, "User32.lib")



// Global Handles
HWND mainWindow;

HWND hFormUsername;
HWND hFormBirthday;
HWND hFormEmail;
HWND hFormLocation;
HWND hFormHobbies;

//HWND hWriteMessage;
HWND hChat;
HWND hMessageBox;
HWND hOperateAsButton;

// Control variables
int active_user;
int serial;

// Interface Control
int w_width;
int w_height;
handleStack* AppStack;

// Friends management
user* friends[MAX_USERS];
int friends_length;
HWND hFriends[MAX_USERS];

// Users management
user* users;
int users_length;
HWND* hUsers;

// wWinMain is the name of the program entry point when using Windows.h. The w before any string or function means
// the string is encoded using UTF-16 or the function encodes the parameters using UTF-16 instead of ANSI
// Parameters:
// - hInstance is the "id" the OS uses to identify the EXE (app) when it is loaded in memory.
// - hPrevInstance is a leftover from 16-bit Windows applications, it is always 0 and unused.
// - pCmdLine contains the command-line arguments passed to the application. In windowed apps the arguments are taken as a string
// instead of an array of strings as with console apps
// - nCmdShow indicates if the main application window is minimized, maximized or shown normally
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Register the window class. Window class is a structure which acts as a blueprint for the window
    // L before a string means it is encoded in UTF-16
    const wchar_t CLASS_NAME[] = L"Main Window Class";

    WNDCLASSW wc = {};

    // pointer to a function that defines most of the behaviour of the window.
    wc.lpfnWndProc   = WindowProc;
    // handle to the application instance (the "id")
    wc.hInstance     = hInstance;
    // string that identifies the window class. Every class name must be unique and not conflict with default class names.
    wc.lpszClassName = CLASS_NAME;

    // How the cursor will be showed in the window
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // RegisterClass registers the window class with the operating system so it can be used in the app
    RegisterClassW(&wc);
    registerFormClass(hInstance);

//    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    // Creating the window. Returns the handle to the new window or NULL if it can't be created
    HWND hwnd = CreateWindowExW(
            // Optional styles (ie transparent window)
            0,
            // It uses the CLASS_NAME we created previously
            CLASS_NAME,
            // The window text, used depending on the type of window
            L"Test Application",
            // The style the window uses. WS_OVERLAPPEDWINDOW includes title bar, border, system menu and minimize and maximize buttons
            WS_OVERLAPPEDWINDOW,
            // The Size and position. CW_USEDEFAULT are the default values
            CW_USEDEFAULT, CW_USEDEFAULT, 1000, 760,
            // The parent window
            NULL,
            // The bar on top some apps have which usually include "File", "Edit", "View", etc. It's called menu
            NULL,
            // Instance handle
            hInstance,
            // Additional application data, usually the State
            NULL);

    // If window couldn't be created returns.
    if (hwnd == NULL)
    {
        return 0;
    }

    mainWindow = hwnd;
    ShowWindow(hwnd, nCmdShow);

    // Captures the events thrown from the Operating System or the user interacting with the application. It is a queue
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

// Defines the function that processes messages sent to the window
// Parameters:
// - hwnd is the handle of the window that receives the message
// - uMsg is the identifier of the message sent
// - wParam and lParam are parameters used to send detailed information (mouse pressed, coordinates of the click, etc.)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg){
        case WM_COMMAND:
            switch (wParam) {
                case MENU_NEW_USER:
                    displayForm(hwnd);
                    break;
                case CHANGE_USER:
                    OperateAs(hwnd);
                    break;
            }
            return 0;
        case WM_CREATE:
            // Global variables initialization
            AppStack = initHandleStack();
            active_user = -1;
            friends_length = 0;
            users_length = 0;
            serial = 0;
            users = NULL;
            hUsers = NULL;
            w_width = CW_USEDEFAULT;
            w_height = CW_USEDEFAULT;

            AddMenu(hwnd);
//            LoadWindow(hwnd);
            return 0;
        case WM_CLOSE:
            //if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
            {
                printf("Closing");
                DestroyWindow(hwnd);
            }
            // Else: User canceled. Do nothing.
            return 0;
        case WM_DESTROY:{
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE: {    // Handle window resizing
            int width = LOWORD(lParam);     // Macro to get the low-order word
            int height = HIWORD(lParam);    // Macro to get the high-order word

            w_width = width;
            w_height = height;

            LoadWindow(hwnd);

//            OnSize(hwnd, (UINT) wParam, width, height);
            return 0;
        }
        case WM_PAINT:
            Paint(hwnd);
            return 0;
        case WM_GETMINMAXINFO:
        {
            // Tamaño mínimo de la pantalla
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
            lpMMI->ptMinTrackSize.x = 800;
            lpMMI->ptMinTrackSize.y = 400;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Paint(HWND hwnd){
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    // CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 250, 0, 500, 700, hwnd, NULL, NULL, NULL);
//    Rectangle(hdc, 250, 850, 0, 500);
    RECT rectangle = {250, 0, 700, 500};
    // rcPaint contains the entire area to update
    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+2));
    FillRect(hdc, &rectangle, (HBRUSH) (COLOR_WINDOW+8));
    EndPaint(hwnd, &ps);
}
// Creo que no hace nada
void OnSize(HWND hwnd, UINT flag, int width, int height) {
    Paint(hwnd);
//    (void)flag;
}

// La barra que se ve arriba
void AddMenu(HWND hwnd){
    HMENU hMenu = CreateMenu();
    HMENU hMenuUsers = CreateMenu();
    HMENU hMenuActions = CreateMenu();

    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuUsers, L"Administración");

    // Users Menu
    AppendMenuW(hMenuUsers, MF_STRING, MENU_NEW_USER, L"Añadir Usuario");

    SetMenu(hwnd, hMenu);
}

void LoadWindow(HWND hwnd){
    // Secciones improvisadas

    HWND currWindow = popHandle(AppStack);
    while(currWindow != NULL){
        DestroyWindow(currWindow);
        currWindow = popHandle(AppStack);
    }

    int width_unit = (w_width)/12;
//    CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 250, 700, hwnd, NULL, NULL, NULL);
//    CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 250, 0, 500, 700, hwnd, NULL, NULL, NULL);
//    CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 750, 0, 250, 700, hwnd, NULL, NULL, NULL);

    HWND separator1 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 2*width_unit, 700, hwnd, NULL, NULL, NULL);
    HWND separator2 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 2*width_unit, 0, 8*width_unit, 700, hwnd, NULL, NULL, NULL);
    HWND separator3 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 10*width_unit, 0, 2*width_unit, 700, hwnd, NULL, NULL, NULL);

//    CreateWindowExW(0, L"static", L"Chat vacío", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 250, 0, 500, 600, hwnd, NULL, NULL, NULL);
//    hWriteMessage = CreateWindowExW(0, L"edit", L"Selecciona un chat y escribe un mensaje", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 250, 600, 500, 100, hwnd, NULL, NULL, NULL);

    CreateWindowExW(0, L"static", L"Chat vacío", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 0, 0, 8*width_unit, 600, separator2, NULL, NULL, NULL);
    hChat = CreateWindowExW(0, L"edit", L"Selecciona un chat y escribe un mensaje", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 0, 600, 8*width_unit, 100, separator2, NULL, NULL, NULL);

//    CreateWindowExW(0, L"static", L"Todos los usuarios", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 250, 16, hwnd, NULL, NULL, NULL);

    CreateWindowExW(0, L"static", L"Todos los usuarios", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 2*width_unit, 16, separator1, NULL, NULL, NULL);


//    CreateWindowExW(0, L"Button", L"Operar como", WS_VISIBLE | WS_CHILD, 774, 24, 100, 30, hwnd, (HMENU) CHANGE_USER, NULL, NULL);
    CreateWindowExW(0, L"Button", L"Operar como", WS_VISIBLE | WS_CHILD, (2*width_unit)/6, 24, (2*(2*width_unit))/3, 30, separator3, (HMENU) CHANGE_USER, NULL, NULL);

    addHandle(AppStack, separator1);
    addHandle(AppStack, separator2);
    addHandle(AppStack, separator3);
//    addHandle(AppStack, &chatWindow);
//    addHandle(AppStack, &chatBox);
//    addHandle(AppStack, &users_title);
//    addHandle(AppStack, &operateas_button);


    // Los botones de los usuarios
    for (int i = 0; i < users_length; i++){
//        hUsers[i] = CreateWindowExW(0, L"Button", users[i].username, WS_VISIBLE | WS_CHILD | SS_CENTER, 8, 24 + i*48 + i*8, 200, 48, separator1,
//                                    (HMENU) OPEN_CHAT, NULL, NULL);
        hUsers[i] = CreateWindowExW(0, L"Button", users[i].username, WS_VISIBLE | WS_CHILD | SS_CENTER, (2*(width_unit))/6, 24 + i*48 + i*8, (4*(width_unit))/3, 48, separator1,
                                   (HMENU) OPEN_CHAT, NULL, NULL);

    }

}

void OperateAs(HWND hwnd){
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Choose a user", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);

    for (int i = 0; i < users_length; i++){
        hUsers[i] = CreateWindowExW(0, L"Button", users[i].username, WS_VISIBLE | WS_CHILD | SS_CENTER, 8, 24 + i*48 + i*8, 200, 48, windowH,
                                    (HMENU) SELECT_USER, NULL, NULL);
    }
}

// // Defines the function that processes messages sent to the dialog (secondary) window
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg){
        case WM_COMMAND:
            switch (wp) {
                case BUTTON_CREATE_USER:
                    if (createUser(hwnd) == TRUE){
                        DestroyWindow(hwnd);
                        LoadWindow(mainWindow);
                    }
                    break;
                case SELECT_USER:{
                    wchar_t username[MAX_LENGTH];
                    GetWindowTextW((HWND) lp, username, MAX_LENGTH);
                    for(int i = 0; i < users_length; i++){
                        if (wcscmp(users[i].username, username) == 0){
                            wchar_t text[MAX_LENGTH + 15] = L"User Selected: ";
                            wcscat(text, users[i].username);

                            MessageBox(hwnd, text, L"Information", MB_OK);
                            active_user = users[i].id;
                            break;
                        }
                    }
                    if (active_user == -1){
                        MessageBox(hwnd, L"Couldn't Select User", L"Information", MB_OK);
                    }
                    DestroyWindow(hwnd);
                }
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

// Registers the dialog/form window class
void registerFormClass(HINSTANCE hInstance){
    const wchar_t CLASS_NAME[] = L"DialogWindow";
    WNDCLASSW wc = {};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc   = DialogProcedure;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
}

// Displays the form to add a new user
void displayForm(HWND hwnd){
    int window_width = 460;
    int window_height = 400;
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"New User", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height, hwnd, NULL, NULL, NULL);

    int paddingx = 24;
    int paddingl = 24;

    int marginsides = 12;
    int marginline = 16;

    int Lheight = 16;
    int Lwidth = 200;

    int Theight = 24;
    int Twidth = 200;

    // Username
    CreateWindowExW(0, L"static", L"Nombre de Usuario", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx, paddingl, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    hFormUsername = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | SS_CENTER, paddingx, paddingl+Lheight, Twidth, Theight, windowH, NULL, NULL, NULL);

    // Email
    CreateWindowExW(0, L"static", L"Email", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx+Lwidth+marginsides, paddingl, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    hFormEmail = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | SS_CENTER, paddingx+Lwidth+marginsides, paddingl+Lheight, Twidth, Theight, windowH, NULL, NULL, NULL);

    // Location
    CreateWindowExW(0, L"static", L"Ubicación", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx, paddingl+Lheight+Theight+marginline, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    hFormLocation = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | SS_CENTER, paddingx, paddingl+(Lheight*2)+Theight+marginline, Twidth, Theight, windowH, NULL, NULL, NULL);

    // Birthday
    CreateWindowExW(0, L"static", L"Cumpleaños (YYYY/MM/DD)", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx+Lwidth+marginsides, paddingl+Lheight+Theight+marginline, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    hFormBirthday = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | SS_CENTER, paddingx+Lwidth+marginsides, paddingl+Lheight*2+Theight+marginline, Twidth, Theight, windowH, NULL, NULL, NULL);

    //Hobbies
    CreateWindowExW(0, L"static", L"Gustos (Gusto1,Gusto2,...,Gusto5)", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx, paddingl+Lheight*2+Theight*2+marginline*2, Lwidth*2+marginsides, Lheight, windowH, NULL, NULL, NULL);
    hFormHobbies = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | SS_CENTER, paddingx, paddingl+Lheight*3+Theight*2+marginline*2, Lwidth*2+marginsides, Theight*2, windowH, NULL, NULL, NULL);

    //Button
    CreateWindowExW(0, L"button", L"Crear", WS_VISIBLE | WS_CHILD | SS_CENTER, window_width/2-100, paddingl+Lheight*3+Theight*4+marginline*4, 200, 24, windowH, (HMENU)BUTTON_CREATE_USER, NULL, NULL);
}

// Checks the form and creates the user
int createUser(HWND hwnd){
    wchar_t username[MAX_LENGTH];
    wchar_t email[MAX_LENGTH];
    wchar_t birthday[MAX_LENGTH];
    wchar_t location[MAX_LENGTH];
    wchar_t hobbies[MAX_LENGTH*5];

    GetWindowTextW(hFormUsername, username, MAX_LENGTH);
    GetWindowTextW(hFormEmail, email, MAX_LENGTH);
    GetWindowTextW(hFormBirthday, birthday, MAX_LENGTH);
    GetWindowTextW(hFormLocation, location, MAX_LENGTH);
    GetWindowTextW(hFormHobbies, hobbies, MAX_LENGTH*5+4);

    for(int i = 0; i < MAX_LENGTH; i++){
        if (email[i] == '@'){
            break;
        }
        if (email[i] == 0) {
            MessageBox(hwnd, L"Email format wrong", L"Information", MB_OK);
            return FALSE;
        }
    }

    if (birthday[4] != '/' || birthday[7] != '/'){
        MessageBox(hwnd, L"Birthday format wrong", L"Information", MB_OK);
        return FALSE;
    }

    users = realloc(users, (users_length + 1) * sizeof(user));

    int count = 0;
    int hobbylength = 0;
    wchar_t hobby[MAX_LENGTH];
    for (int i = 0; i < MAX_LENGTH*5 && count < 5; i++, hobbylength++){
        if(hobbies[i] == ','){
            hobby[hobbylength] = 0;
            wcscpy(users[users_length].hobbies[count], (const wchar_t *) &hobby);
            count +=1;
            hobbylength = 0;
        } else if (hobbies[i] == 0){
            hobby[hobbylength] = 0;
            wcscpy(users[users_length].hobbies[count], (const wchar_t *) &hobby);
            count += 1;
            if (count < 5){
                users = realloc(users, (users_length) * sizeof(user));
                MessageBox(hwnd, L"Hobby format wrong", L"Information", MB_OK);
                return FALSE;
            }
        } else {
            hobby[hobbylength] = hobbies[i];
        }
    }


    hUsers = realloc(hUsers, (users_length + 1) * sizeof(hwnd));

    users[users_length].id = serial;
    wcscpy(users[users_length].username, (const wchar_t *) &username);
    wcscpy(users[users_length].username, (const wchar_t *) &username);
    wcscpy(users[users_length].email, (const wchar_t *) &email);
    wcscpy(users[users_length].birthday, (const wchar_t *) &birthday);
    wcscpy(users[users_length].location, (const wchar_t *) &location);

    serial += 1;
    users_length += 1;

    MessageBox(hwnd, L"User Created", L"Information", MB_OK);

    return TRUE;
}