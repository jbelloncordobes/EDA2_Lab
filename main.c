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
#include "headers/user_arrays.h"
#include "headers/PostQueue.h"
#include "headers/posts.h"
#include "headers/WordDictionary.h"

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
WNDPROC ButtonProc;

// Control variables
user* active_user;
int serial;

// Interface Control
int w_width;
int w_height;
handleStack *AppStack;

// Friends management
int friends_length;
HWND hFriends;

// Users management
UserList users;
WDict WordDictionary;


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
    wc.lpfnWndProc = WindowProc;
    // handle to the application instance (the "id")
    wc.hInstance = hInstance;
    // string that identifies the window class. Every class name must be unique and not conflict with default class names.
    wc.lpszClassName = CLASS_NAME;

    // How the cursor will be showed in the window
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // RegisterClass registers the window class with the operating system so it can be used in the app
    RegisterClassW(&wc);
    registerFormClass(hInstance);

    // Creating the window. Returns the handle to the new window or NULL if it can't be created
    HWND hwnd = CreateWindowExW(
            // Optional styles (ie transparent window)
            0,
            // It uses the CLASS_NAME we created previously
            CLASS_NAME,
            // The window text, used depending on the type of window
            L"Red Social",
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
    if (hwnd == NULL) {
        return 0;
    }

    mainWindow = hwnd;
    ShowWindow(hwnd, nCmdShow);

    // Captures the events thrown from the Operating System or the user interacting with the application. It is a queue
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
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
    switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case MENU_NEW_USER:
                    displayForm(hwnd);
                    break;
                case USER_DETAILS:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    userDetails(mainWindow);
                    break;
                case POST_MESSAGE:
                    if (active_user == NULL){
                        break;
                    } else {
//                        wchar_t posttext[POST_LENGTH];
//                        GetWindowTextW(hMessageBox, posttext, POST_LENGTH);
//                        Post* newpost = createPost(posttext);
//                        addToQueue(&active_user->posts, newpost);
//                        SetWindowTextW(hMessageBox, L"");
//                        // RedrawWindow(NULL, NULL, NULL, RDW_UPDATENOW);
//                        LoadWindow(mainWindow);
                        sendMessage(mainWindow);
                    }
                    break;
                case MENU_CHANGE_USER:
                    operateAs(hwnd, users);
                    break;
                case OPEN_SEND_FR:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    sendFRModal(hwnd, users, *active_user);
                    break;
                case OPEN_RECEIVED_FR:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    receivedFRModal(hwnd, active_user);
                    break;
                case OPEN_SENT_FR:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    sentFRModal(hwnd, active_user);
                    break;
            }
            return 0;
        case WM_CREATE:
            // Global variables initialization
            AppStack = initHandleStack();
            // Se comprobará la id del usuario activo antes que nada, no hace falta inicializar más atributos
            active_user = NULL;
            friends_length = 0;
            serial = 0;
            users.first = NULL;
            users.last = NULL;
            users.size = 0;
            hMessageBox = NULL;
            w_width = CW_USEDEFAULT;
            w_height = CW_USEDEFAULT;
            initDict(&WordDictionary, 10000);
            // Leer csvs cuando estén

            AddMenu(hwnd);
            return 0;
        case WM_CLOSE:
            // if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
        }
            // Else: User canceled. Do nothing.
            return 0;
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE: {    // Handle window resizing
            int width = LOWORD(lParam);     // Macro to get the low-order word
            int height = HIWORD(lParam);    // Macro to get the high-order word

            w_width = width;
            w_height = height;

            LoadWindow(hwnd);
            return 0;
        }
        case WM_PAINT:
            Paint(hwnd);
            return 0;
        case WM_GETMINMAXINFO: {
            // Tamaño mínimo de la pantalla
            LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
            lpMMI->ptMinTrackSize.x = 800;
            lpMMI->ptMinTrackSize.y = 400;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Paint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
//    RECT rectangle = {250, 0, 700, 500};
    // rcPaint contains the entire area to update
    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 2));
//    FillRect(hdc, &rectangle, (HBRUSH) (COLOR_WINDOW + 8));
    EndPaint(hwnd, &ps);
}

// La barra que se ve arriba
void AddMenu(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hMenuUsers = CreateMenu();
    HMENU hMenuActions = CreateMenu();

    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR) hMenuUsers, L"Administración");

    // Users Menu
    AppendMenuW(hMenuUsers, MF_STRING, MENU_NEW_USER, L"Añadir Usuario");
    AppendMenuW(hMenuUsers, MF_STRING, MENU_CHANGE_USER, L"Operar como usuario");

    SetMenu(hwnd, hMenu);
}

void LoadWindow(HWND hwnd) {
    // Guarda el texto que haya escrito el usuario
    wchar_t messageboxText[POST_LENGTH] = L"";
    if (hMessageBox != NULL){
        GetWindowTextW(hMessageBox, messageboxText ,POST_LENGTH);
    }

    // Destruye las ventanas del stack (toda la pantalla) para recargarlas en su nueva posición.
    HWND currWindow = popHandle(AppStack);
    while (currWindow != NULL) {
        DestroyWindow(currWindow);
        currWindow = popHandle(AppStack);
    }

    int width_unit = ((w_width) / 12);
    int height_unit = ((w_height) / 24);

    // Separadores

    HWND separator1 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER , 0, 0, 2 * width_unit, w_height,
                                      hwnd, NULL, NULL, NULL);
    HWND separator2 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 2 * width_unit, 0,
                                      8 * width_unit, w_height, hwnd, NULL, NULL, NULL);
    HWND separator3 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 10 * width_unit, 0,
                                      2 * width_unit, w_height, hwnd, NULL, NULL, NULL);

    // Parte central - chat
    HWND chat;
    if (active_user == NULL){
        chat = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL, 2*width_unit, 0, 8 * width_unit,
                                    (7*w_height)/8, hwnd, NULL, NULL, NULL);
    } else {
        chat = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL, 2*width_unit, 0, 8 * width_unit,
                               (7*w_height)/8, hwnd, NULL, NULL, NULL);

        wchar_t existing_text[TEXT_LENGTH];

        resetQueue(&active_user->posts);
        Post* currpost = dequeue(&active_user->posts);
        while(currpost != NULL){
            GetWindowTextW(chat, existing_text, TEXT_LENGTH);
            wcscat(existing_text, currpost->message);
            wcscat(existing_text, L"\r\n------------------------\r\n");
            SetWindowTextW(chat, existing_text);
            currpost = dequeue(&active_user->posts);
        }
    }


    hMessageBox = CreateWindowExW(0, L"edit", messageboxText,
                            WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_LEFT, 2 * width_unit, (7*w_height)/8, 6 * width_unit, (w_height/8)-2, hwnd,
                            NULL, NULL, NULL);

    HWND messagebox_button = CreateWindowExW(0, L"Button", L"Send",
                                      WS_VISIBLE | WS_CHILD , (2 * width_unit) + (6 * width_unit) + width_unit/3, (7*w_height)/8 + w_height/32, width_unit, (w_height/16)-2, hwnd,
                                      (HMENU) POST_MESSAGE, NULL, NULL);


    // Botón parte derecha (-)
//    HWND operateas = CreateWindowExW(0, L"Button", L"Operar como", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24,
//                    (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) CHANGE_USER, NULL, NULL);
    HWND operateas = CreateWindowExW(0, L"Button", L"Ver detalles", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24,
                                     (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) USER_DETAILS, NULL, NULL);

    // Botón parte derecha (Agregar amigos)
    HWND add_friends = CreateWindowExW(0, L"Button", L"Agregar amigos", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24+30+12,
                                           (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) OPEN_SEND_FR, NULL, NULL);

    // Botón parte derecha (Solicitudes de amistad)
    HWND friend_requests = CreateWindowExW(0, L"Button", L"Solicitudes recibidas", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24+2*30+2*12,
                                           (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) OPEN_RECEIVED_FR, NULL, NULL);

    // Botón parte derecha (Peticiones de amistad enviadas)
    HWND friend_requests_sent = CreateWindowExW(0, L"Button", L"Solicitudes enviadas", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24+3*30+3*12,
                                           (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) OPEN_SENT_FR, NULL, NULL);

    // Título parte izquierda (usuarios)
    HWND users_title = CreateWindowExW(0, L"static", L"Usuarios", WS_VISIBLE | WS_CHILD | SS_CENTER, 2, 8, (2 * width_unit) - 16, 16,
                                       hwnd, NULL, NULL, NULL);

    // Agregados los handles al stack de la aplicación.
    addHandle(AppStack, separator1);
    addHandle(AppStack, separator2);
    addHandle(AppStack, separator3);
    addHandle(AppStack, chat);
    addHandle(AppStack, hMessageBox);
    addHandle(AppStack, messagebox_button);
    addHandle(AppStack, users_title);
    addHandle(AppStack, operateas);
    addHandle(AppStack, add_friends);
    addHandle(AppStack, friend_requests);
    addHandle(AppStack, friend_requests_sent);

    // La lista de usuarios
    UserNode *curruser = users.first;
    wchar_t name[MAX_LENGTH+5];
    wchar_t character = '#';
    int id;
    int i = 0;
    while (curruser != NULL) {
        id = curruser->User->id;
        if (active_user != NULL){
            if (active_user->id == id){
                curruser = curruser->next;
                continue;
            }
            if (search_User(active_user->friends, id) == TRUE){
                curruser = curruser->next;
                continue;
            }
        }

        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", curruser->User->username, character, id);

        HWND user = CreateWindowExW(0, L"static", name, WS_VISIBLE | WS_CHILD | SS_CENTER,
                            2, 36 + i * 16 + i * 4, (2 * width_unit) - 16, 16, hwnd,
                            NULL, NULL, NULL);
        addHandle(AppStack, user);
        curruser = curruser->next;
        i++;
    }
    int final_elements = i + 1;

    // Título parte izquierda (amigos)
    HWND friends_title = CreateWindowExW(0, L"static", L"Amigos", WS_VISIBLE | WS_CHILD | SS_CENTER, 2, 36 + 8 + final_elements * 16 + final_elements * 4,
                                         (2 * width_unit) - 16, 16, hwnd, NULL, NULL, NULL);

    addHandle(AppStack, friends_title);
    if (active_user == NULL) return;
    if (active_user->friends.size == 0){
        return;
    }
    i = 0;
    user* currfriend;
    while (i < active_user->friends.size) {
        currfriend = active_user->friends.users[i];
        id = currfriend->id;

        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", currfriend->username, character, id);

        HWND friend = CreateWindowExW(0, L"static", name, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                      2, (36 + 8 + final_elements * 16 + final_elements * 4) + (i * 16 + i * 4) + 24, (2 * width_unit) - 16, 16, hwnd,
                                    NULL, NULL, NULL);
        addHandle(AppStack, friend);
        i++;
    }

}

// Defines the function that processes messages sent to the dialog (secondary) window
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_COMMAND:
            switch (wp) {
                case BUTTON_CREATE_USER:
                    if (createUser(hwnd) == TRUE) {
                        DestroyWindow(hwnd);
                        LoadWindow(mainWindow);
                    }
                    break;
                case SELECT_USER: {
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo seleccionar al usuario", L"Información", MB_OK);
                    } else {
                        active_user = selected_user;
                        wchar_t text[MAX_LENGTH + 22] = L"Usuario Seleccionado: ";
                        wcscat(text, selected_user->username);
                        MessageBox(hwnd, text, L"Information", MB_OK);
                    }
                    DestroyWindow(hwnd);
                    LoadWindow(mainWindow);
                }
                break;
                case FR_BUTTON: {
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo enviar la solicitud de amistad", L"Error", MB_ICONEXCLAMATION);
                    } else {
                        wchar_t text[MAX_LENGTH + 32] = L"¿Enviar solicitud de amistad a ";
                        wcscat(text, selected_user->username);
                        wcscat(text, L"?");
                        if (MessageBox(hwnd, text, L"¿Estás seguro?", MB_OKCANCEL) == IDOK){
                            sendFriendRequest(active_user, selected_user);
                        }
                    }
                    DestroyWindow(hwnd);
                }
                break;
                case RFR_BUTTON:{
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo aceptar la solicitud de amistad", L"Error", MB_ICONEXCLAMATION);
                    } else {
                        wchar_t text[MAX_LENGTH + 24] = L"¿Aceptar solicitud de ";
                        wcscat(text, selected_user->username);
                        wcscat(text, L"?");
                        if (MessageBox(hwnd, text, L"¿Estás seguro?", MB_OKCANCEL) == IDOK){
                            acceptFriendRequest(active_user, selected_user);
                            LoadWindow(mainWindow);
                        } else {
                            rejectFriendRequest(active_user, selected_user);
                        }
                    }
                    DestroyWindow(hwnd);
                }
                break;
                case SFR_BUTTON:{
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo rechazar la solicitud de amistad", L"Error", MB_ICONEXCLAMATION);
                    } else {
                        wchar_t text[MAX_LENGTH + 25] = L"¿Cancelar solicitud de ";
                        wcscat(text, selected_user->username);
                        wcscat(text, L"?");
                        if (MessageBox(hwnd, text, L"¿Estás seguro?", MB_OKCANCEL) == IDOK){
                            rejectFriendRequest(selected_user, active_user);
                        }
                    }
                    DestroyWindow(hwnd);
                }
                break;
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
void registerFormClass(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"DialogWindow";
    WNDCLASSW wc = {};

    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.lpfnWndProc = DialogProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
}

// Displays the form to add a new user
void displayForm(HWND hwnd) {
    int window_width = 460;
    int window_height = 400;
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Nuevo Usuario", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                   CW_USEDEFAULT, window_width, window_height, hwnd, NULL, NULL, NULL);

    int paddingx = 24;
    int paddingl = 24;

    int marginsides = 12;
    int marginline = 16;

    int Lheight = 16;
    int Lwidth = 200;

    int Theight = 24;
    int Twidth = 200;

    // Username
    CreateWindowExW(0, L"static", L"Nombre de Usuario", WS_VISIBLE | WS_CHILD | SS_CENTER , paddingx, paddingl, Lwidth,
                    Lheight, windowH, NULL, NULL, NULL);
    hFormUsername = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_AUTOHSCROLL,
                                    paddingx, paddingl + Lheight, Twidth, Theight, windowH, NULL, NULL, NULL);

    // Email
    CreateWindowExW(0, L"static", L"Email", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx + Lwidth + marginsides,
                    paddingl, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    hFormEmail = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_AUTOHSCROLL,
                                 paddingx + Lwidth + marginsides, paddingl + Lheight, Twidth, Theight, windowH, NULL,
                                 NULL, NULL);

    // Location
    CreateWindowExW(0, L"static", L"Ubicación", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight + Theight + marginline, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    hFormLocation = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_AUTOHSCROLL,
                                    paddingx, paddingl + (Lheight * 2) + Theight + marginline, Twidth, Theight, windowH,
                                    NULL, NULL, NULL);

    // Birthday
    CreateWindowExW(0, L"static", L"Cumpleaños (DD/MM/YYYY)", WS_VISIBLE | WS_CHILD | SS_CENTER | ES_AUTOHSCROLL,
                    paddingx + Lwidth + marginsides, paddingl + Lheight + Theight + marginline, Lwidth, Lheight,
                    windowH, NULL, NULL, NULL);
    hFormBirthday = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                                    paddingx + Lwidth + marginsides, paddingl + Lheight * 2 + Theight + marginline,
                                    Twidth, Theight, windowH, NULL, NULL, NULL);

    //Hobbies
    CreateWindowExW(0, L"static", L"Gustos (Gusto1,Gusto2,...,Gusto5)", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight * 2 + Theight * 2 + marginline * 2, Lwidth * 2 + marginsides, Lheight, windowH,
                    NULL, NULL, NULL);
    hFormHobbies = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | SS_CENTER,
                                   paddingx, paddingl + Lheight * 3 + Theight * 2 + marginline * 2,
                                   Lwidth * 2 + marginsides, Theight * 2, windowH, NULL, NULL, NULL);

    //Button
    CreateWindowExW(0, L"button", L"Crear", WS_VISIBLE | WS_CHILD | SS_CENTER, window_width / 2 - 100,
                    paddingl + Lheight * 3 + Theight * 4 + marginline * 4, 200, 24, windowH, (HMENU) BUTTON_CREATE_USER,
                    NULL, NULL);
}

void userDetails(HWND hwnd){
    int window_width = 460;
    int window_height = 400;
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Detalles del Usuario", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                   CW_USEDEFAULT, window_width, window_height, hwnd, NULL, NULL, NULL);

    int paddingx = 24;
    int paddingl = 24;

    int marginsides = 12;
    int marginline = 16;

    int Lheight = 16;
    int Lwidth = 200;

    int Theight = 40;
    int Twidth = 200;

    // Username
    CreateWindowExW(0, L"static", L"Nombre de Usuario", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx, paddingl, Lwidth,
                    Lheight, windowH, NULL, NULL, NULL);
    CreateWindowExW(0, L"static", active_user->username, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                    paddingx, paddingl + Lheight, Twidth, Theight, windowH, NULL, NULL, NULL);

    // Email
    CreateWindowExW(0, L"static", L"Email", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx + Lwidth + marginsides,
                    paddingl, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    CreateWindowExW(0, L"static", active_user->email, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                 paddingx + Lwidth + marginsides, paddingl + Lheight, Twidth, Theight, windowH, NULL,
                                 NULL, NULL);

    // Location
    CreateWindowExW(0, L"static", L"Ubicación", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight + Theight + marginline, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    CreateWindowExW(0, L"static", active_user->location, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                    paddingx, paddingl + (Lheight * 2) + Theight + marginline, Twidth, Theight, windowH,
                                    NULL, NULL, NULL);

    // Birthday
    CreateWindowExW(0, L"static", L"Cumpleaños", WS_VISIBLE | WS_CHILD | SS_CENTER,
                    paddingx + Lwidth + marginsides, paddingl + Lheight + Theight + marginline, Lwidth, Lheight,
                    windowH, NULL, NULL, NULL);
    CreateWindowExW(0, L"static", active_user->birthday, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                    paddingx + Lwidth + marginsides, paddingl + Lheight * 2 + Theight + marginline,
                                    Twidth, Theight, windowH, NULL, NULL, NULL);

    //Hobbies
    CreateWindowExW(0, L"static", L"Gustos", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight * 2 + Theight * 2 + marginline * 2, Lwidth * 2 + marginsides, Lheight, windowH,
                    NULL, NULL, NULL);
    wchar_t hobbies[MAX_HOBBIES*MAX_LENGTH+4];
    hobbies[0] = 0;
    for (int i = 0; i < 5; i++){
        wcscat(hobbies, active_user->hobbies[i]);
        if (i == 4){
            break;
        }
        wcscat(hobbies, L"\n");
    }

    CreateWindowExW(0, L"static", hobbies, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                   paddingx, paddingl + Lheight * 3 + Theight * 2 + marginline * 2,
                                   Lwidth * 2 + marginsides, Theight * 2, windowH, NULL, NULL, NULL);

}

// Checks the form and creates the user
int createUser(HWND hwnd) {
    wchar_t username[MAX_LENGTH];
    wchar_t email[MAX_LENGTH];
    wchar_t birthday[MAX_LENGTH];
    wchar_t location[MAX_LENGTH];
    wchar_t hobbies[MAX_LENGTH * 5];

    GetWindowTextW(hFormUsername, username, MAX_LENGTH);
    GetWindowTextW(hFormEmail, email, MAX_LENGTH);
    GetWindowTextW(hFormBirthday, birthday, MAX_LENGTH);
    GetWindowTextW(hFormLocation, location, MAX_LENGTH);
    GetWindowTextW(hFormHobbies, hobbies, MAX_LENGTH * 5 + 4);

    for (int i = 0; i < MAX_LENGTH; i++) {
        if (email[i] == '@') {
            break;
        }
        if (email[i] == 0) {
            MessageBox(hwnd, L"Formato de mail incorrecto", L"Information", MB_OK);
            return FALSE;
        }
    }

    int year;
    int month;
    int day;

    if (swscanf(birthday, L"%d/%d/%d", &day, &month, &year) < 3) {
        MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Information", MB_OK);
        return FALSE;
    }

    struct tm time = getCurrentDate();

    if (year > time.tm_year + 1900 || year < 1900) {
        MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Information", MB_OK);
        return FALSE;
    }

    if (month > 12 || month < 1) {
        MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Information", MB_OK);
        return FALSE;
    }

    if (day > 28) {
        if (month == 2) {
            if ((year % 4 && (year % 100 != 0 || year % 400 == 0)) || day > 29) {
                MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Information", MB_OK);
                return FALSE;
            }
        }
    }
    if (day == 31) {
        if (month == 2 || month == 4 || month == 6 || month == 9 || month == 11) {
            MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Information", MB_OK);
            return FALSE;
        }
    }


    wchar_t hobby1[MAX_LENGTH];
    wchar_t hobby2[MAX_LENGTH];
    wchar_t hobby3[MAX_LENGTH];
    wchar_t hobby4[MAX_LENGTH];
    wchar_t hobby5[MAX_LENGTH];
    int source = swscanf_s(hobbies, L"%[^,],%[^,],%[^,],%[^,],%[^\0]", hobby1, MAX_LENGTH, hobby2, MAX_LENGTH, hobby3, MAX_LENGTH, hobby4, MAX_LENGTH, hobby5, MAX_LENGTH);
    if (source < 5) {
        MessageBox(hwnd, L"Formato de gustos incorrecto/Menos de 5 gustos introducidos", L"Information", MB_OK);
        return FALSE;
    }
    user *newuser = malloc(sizeof(user));

    newuser->id = serial;
    wcscpy(newuser->username, (const wchar_t *) &username);
    wcscpy(newuser->email, (const wchar_t *) &email);
    wcscpy(newuser->birthday, (const wchar_t *) &birthday);
    wcscpy(newuser->location, (const wchar_t *) &location);

    wcscpy(newuser->hobbies[0], (const wchar_t *) &hobby1);
    wcscpy(newuser->hobbies[1], (const wchar_t *) &hobby2);
    wcscpy(newuser->hobbies[2], (const wchar_t *) &hobby3);
    wcscpy(newuser->hobbies[3], (const wchar_t *) &hobby4);
    wcscpy(newuser->hobbies[4], (const wchar_t *) &hobby5);

    newuser->friend_requests_sent.size = 0;
    newuser->friend_requests_sent.users = NULL;
    newuser->friend_requests_received.size = 0;
    newuser->friend_requests_received.users = NULL;
    newuser->friends.size = 0;
    newuser->friends.users = NULL;
    initQueue(&newuser->posts);

    addUser(&users, newuser);
    UserNode *newnode = malloc(sizeof(UserNode));
    newnode->User = newuser;

    serial += 1;
    users.size += 1;

    MessageBox(hwnd, L"User Created", L"Information", MB_OK);

    return TRUE;
}


// Crea un Post y lo agrega a la cola del usuario
void sendMessage(HWND hwnd){
    // Usuario no seleccionado
    if (active_user==NULL){
        return;
    }

    wchar_t messageboxText[POST_LENGTH] = L"";
    GetWindowTextW(hMessageBox, messageboxText ,POST_LENGTH);



    // Mensaje vacío
    if (wcscmp(messageboxText, L"") == 0){
        return;
    }
    Post* newpost = createPost(messageboxText);
    addToQueue(&active_user->posts, newpost);
    SetWindowTextW(hMessageBox, L"");

    // wchar_t buffer[MAX_WORD_LENGTH];
    // wcscpy(buffer, wcstok(messageboxText, (const wchar_t *) L" "));

//    wchar_t* buffer;
//    wchar_t* token = wcstok_s(messageboxText, (const wchar_t *) L" ", &buffer);

    wchar_t* token = wcstok(messageboxText, (const wchar_t *) L"\t\r\n\v\f ");
    DictNode* currnode;

    while (token != NULL) {
        //wcscpy(buffer, wcstok(NULL, (const wchar_t *) L" "));
        currnode = searchDict(&WordDictionary, token);
        if (currnode == NULL){
            wprintf(L"Word doesn't exist %ls\n", token);
            addToDict(&WordDictionary, token);
        } else {
            wprintf(L"Word does exist %ls\n", token);
            currnode->count += 1;
        }
        token = wcstok(NULL, (const wchar_t *) L"\t\r\n\v\f ");
        //token = wcstok_s(NULL, (const wchar_t *) L" ", &buffer);
    }

    LoadWindow(mainWindow);
}


















