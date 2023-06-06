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

// VARIABLES GLOBALES

// -------- Global Handles --------
// Handles del form de crear usuarios.
HWND mainWindow;
HWND hFormUsername;
HWND hFormBirthday;
HWND hFormEmail;
HWND hFormLocation;
HWND hFormHobbies;

// Handle de la caja para escribir posts.
HWND hMessageBox;
// --------------------------------

// Variables de control
user* active_user;
int serial;
WDict WordDictionary;

// Control de la interfaz
int w_width;
int w_height;
handleStack *AppStack;

// Users management
UserList users;


/// wWinMain is the name of the program entry point when using Windows.h. The w before any string or function means
/// the string is encoded using UTF-16 or the function encodes the parameters using UTF-16 instead of ANSI
/// Parameters:
/// - hInstance is the "id" the OS uses to identify the EXE (app) when it is loaded in memory.
/// - hPrevInstance is a leftover from 16-bit Windows applications, it is always 0 and unused.
/// - pCmdLine contains the command-line arguments passed to the application. In windowed apps the arguments are taken as a string
/// instead of an array of strings as with console apps
/// - nCmdShow indicates if the main application window is minimized, maximized or shown normally
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

    // Asigna la ventana creada a la variable global referente a la ventana principal
    mainWindow = hwnd;
    ShowWindow(hwnd, nCmdShow);

    // Captures the events thrown from the Operating System or the user interacting with the application. It is a queue,
    // so there can be cases where more messages are received than can be processed, and it will clear them one by one.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

/// Defines the function that processes messages sent to the window
/// Parameters:
/// - hwnd is the handle of the window that receives the message
/// - uMsg is the identifier of the message sent
/// - wParam and lParam are parameters used to send detailed Información (mouse pressed, coordinates of the click, etc.)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        // Boton pulsado
        case WM_COMMAND:
            // ID de la acción. En este caso lParam contiene el handle del botón
            switch (LOWORD(wParam)) {
                // Administración -> Añadir usuario
                case MENU_NEW_USER:
                    displayForm(hwnd);
                    break;
                // Ver detalles
                case USER_DETAILS:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    userDetails(mainWindow);
                    break;
                // Send
                case POST_MESSAGE:
                    if (active_user == NULL){
                        break;
                    } else {
                        sendMessage(mainWindow);
                    }
                    break;
                // Administración -> Operar como usuario
                case MENU_CHANGE_USER:
                    operateAs(hwnd, users);
                    break;
                // Agregar amigos
                case OPEN_SEND_FR:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    sendFRModal(hwnd, users, *active_user);
                    break;
                // Solicitudes recibidas
                case OPEN_RECEIVED_FR:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    receivedFRModal(hwnd, active_user);
                    break;
                // Solicitudes enviadas
                case OPEN_SENT_FR:
                    if (active_user == NULL){
                        MessageBoxW(hwnd, L"No se está operando como ningún usuario", L"Usuario no seleccionado", MB_ICONEXCLAMATION);
                        break;
                    }
                    sentFRModal(hwnd, active_user);
                    break;
            }
            break;
        // Ventana creada. Solo se ejecuta al principio de la aplicación
        case WM_CREATE:
            // Inicialización de variables globales
            AppStack = initHandleStack();
            active_user = NULL;
            serial = 0;
            users.first = NULL;
            users.last = NULL;
            users.size = 0;
            hMessageBox = NULL;
            w_width = CW_USEDEFAULT;
            w_height = CW_USEDEFAULT;
            initDict(&WordDictionary, DICT_SIZE);


            AddMenu(hwnd);
            break;
        // Ventana cerrada
        case WM_CLOSE:
            // Envia señal WM_DESTROY
            DestroyWindow(hwnd);
            break;
        // Ventana destruida
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        // Cambios de tamaño de la ventana. Se llama también al crear la ventana
        case WM_SIZE: {
            int width = LOWORD(lParam);     // Macro to get the low-order word
            int height = HIWORD(lParam);    // Macro to get the high-order word

            // Guarda los nuevos valores en las variables globales
            w_width = width;
            w_height = height;

            LoadWindow(hwnd);
            break;
        }
        // Pinta la ventana o una porción de la ventana. Se ejecuta más o menos en los mismos casos que WM_SIZE
        case WM_PAINT:
            Paint(hwnd);
            break;
        // Especifica un tamaño mínimo para la ventana
        case WM_GETMINMAXINFO: {
            // Tamaño mínimo de la pantalla
            LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
            lpMMI->ptMinTrackSize.x = 800;
            lpMMI->ptMinTrackSize.y = 400;
            break;
        }
        default:
            // Para no tener que especificar cada caso, DefWindowProc procesa los mensajes con su valor por defecto
            return DefWindowProc(hwnd, uMsg, wParam, lParam);

    }
    return 0;
}

// Pinta la ventana entera
void Paint(HWND hwnd) {
    PAINTSTRUCT ps;
    // Empieza a pintar
    HDC hdc = BeginPaint(hwnd, &ps);
    // rcPaint contains the entire area to update (all the screen)
    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 2));
    // Termina de pintar
    EndPaint(hwnd, &ps);
}

// La barra que se ve arriba
void AddMenu(HWND hwnd) {
    HMENU hMenuUsers = CreateMenu();
    HMENU hMenu = CreateMenu();
    // Crea el menú hMenu (Administración) con submenú hMenuUsers
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR) hMenuUsers, L"Administración");

    // Añade las entradas "Añadir Usuario" y "Operar como usuario" al menú "Administración" mediante el menú hMenuUsers
    AppendMenuW(hMenuUsers, MF_STRING, MENU_NEW_USER, L"Añadir Usuario");
    AppendMenuW(hMenuUsers, MF_STRING, MENU_CHANGE_USER, L"Operar como usuario");

    // Agrega el menú a la pantalla principal
    SetMenu(hwnd, hMenu);
}

void LoadWindow(HWND hwnd) {
    // Guarda el texto que haya escrito el usuario en la caja de texto
    wchar_t messageboxText[POST_LENGTH] = L"";
    // Si es la primera vez que se carga la ventana no existe hMessageBox aún
    if (hMessageBox != NULL){
        GetWindowTextW(hMessageBox, messageboxText ,POST_LENGTH);
    }

    // Destruye las ventanas del stack (toda la pantalla) para recargarlas en su nueva posición.
    HWND currWindow = popHandle(AppStack);
    while (currWindow != NULL) {
        DestroyWindow(currWindow);
        currWindow = popHandle(AppStack);
    }

    // Calcula las unidades de tamaño para dividir la pantalla proporcionalmente.
    int width_unit = ((w_width) / 12);

    // Separadores
    HWND separator1 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER , 0, 0, 2 * width_unit, w_height,
                                      hwnd, NULL, NULL, NULL);
    HWND separator2 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 2 * width_unit, 0,
                                      8 * width_unit, w_height, hwnd, NULL, NULL, NULL);
    HWND separator3 = CreateWindowExW(0, L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 10 * width_unit, 0,
                                      2 * width_unit, w_height, hwnd, NULL, NULL, NULL);

    // Parte central - chat
    HWND timeline = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL, 2*width_unit, 0, 8 * width_unit,
                                (7*w_height)/8, hwnd, NULL, NULL, NULL);
    if (active_user != NULL){

        wchar_t existing_text[TEXT_LENGTH];

        // Reinicia el "puntero" de la cola para agregar todos los mensajes desde el principio.
        resetQueue(&active_user->posts);

        // Agrega todos los mensajes en orden FIFO a la ventana del "chat"
        Post* currpost = dequeue(&active_user->posts);
        while(currpost != NULL){
            // Carga de la ventana el texto existente
            GetWindowTextW(timeline, existing_text, TEXT_LENGTH);
            // Agrega el mensaje
            wcscat(existing_text, currpost->message);
            // Agrega una separación
            wcscat(existing_text, L"\r\n------------------------\r\n");
            // Introduce el nuevo texto en la ventana
            SetWindowTextW(timeline, existing_text);
            // Carga el siguiente Post
            currpost = dequeue(&active_user->posts);
        }
    }

    // Crea la caja de texto para escribir posts. La asigna a la variable global
    hMessageBox = CreateWindowExW(0, L"edit", messageboxText,
                            WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_LEFT, 2 * width_unit, (7*w_height)/8, 6 * width_unit, (w_height/8)-2, hwnd,
                            NULL, NULL, NULL);

    // Crea el botón para enviar el post
    HWND messagebox_button = CreateWindowExW(0, L"Button", L"Send",
                                      WS_VISIBLE | WS_CHILD , (2 * width_unit) + (6 * width_unit) + width_unit/3, (7*w_height)/8 + w_height/32, width_unit, (w_height/16)-2, hwnd,
                                      (HMENU) POST_MESSAGE, NULL, NULL);


    // Botón parte derecha (Ver detalles)
    HWND ver_detalles = CreateWindowExW(0, L"Button", L"Ver detalles", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24,
                                     (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) USER_DETAILS, NULL, NULL);

    // Botón parte derecha (Agregar amigos)
    HWND add_friends = CreateWindowExW(0, L"Button", L"Agregar amigos", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24+30+12,
                                           (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) OPEN_SEND_FR, NULL, NULL);

    // Botón parte derecha (Solicitudes recibidas)
    HWND friend_requests = CreateWindowExW(0, L"Button", L"Solicitudes recibidas", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24+2*30+2*12,
                                           (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) OPEN_RECEIVED_FR, NULL, NULL);

    // Botón parte derecha (Solicitudes enviadas)
    HWND friend_requests_sent = CreateWindowExW(0, L"Button", L"Solicitudes enviadas", WS_VISIBLE | WS_CHILD, (10 * width_unit) + ((2 * width_unit) / 6), 24+3*30+3*12,
                                           (2 * (2 * width_unit)) / 3, 30, hwnd, (HMENU) OPEN_SENT_FR, NULL, NULL);

    // Título parte izquierda (Usuarios)
    HWND users_title = CreateWindowExW(0, L"static", L"Usuarios", WS_VISIBLE | WS_CHILD | SS_CENTER, 2, 8, (2 * width_unit) - 16, 16,
                                       hwnd, NULL, NULL, NULL);

    // Agregados los handles al stack de la aplicación.
    addHandle(AppStack, separator1);
    addHandle(AppStack, separator2);
    addHandle(AppStack, separator3);
    addHandle(AppStack, timeline);
    addHandle(AppStack, hMessageBox);
    addHandle(AppStack, messagebox_button);
    addHandle(AppStack, users_title);
    addHandle(AppStack, ver_detalles);
    addHandle(AppStack, add_friends);
    addHandle(AppStack, friend_requests);
    addHandle(AppStack, friend_requests_sent);

    // Crea la lista de usuarios
    wchar_t name[MAX_LENGTH+5];
    wchar_t character = '#';
    int id;
    int i = 0;

    // Itera todos los usuarios
    UserNode *curruser = users.first;
    while (curruser != NULL) {
        id = curruser->User->id;
        // Si el usuario activo no está definido
        if (active_user != NULL){
            // Si el usuario iterado es el mismo que el activo no lo muestra en la lista
            if (active_user->id == id){
                curruser = curruser->next;
                continue;
            }
            // Si el usuario iterado está en la lista de amigos del usuario activo no lo muestra en la lista
            if (search_User(active_user->friends, id) == TRUE){
                curruser = curruser->next;
                continue;
            }
        }

        // Formatea el texto para la lista que consiste en el nombre de usuario y la id
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", curruser->User->username, character, id);

        // Crea el texto en la lista y lo añade al Stack.
        HWND user = CreateWindowExW(0, L"static", name, WS_VISIBLE | WS_CHILD | SS_CENTER,
                            2, 36 + i * 16 + i * 4, (2 * width_unit) - 16, 16, hwnd,
                            NULL, NULL, NULL);
        addHandle(AppStack, user);
        curruser = curruser->next;
        i++;
    }

    // Coge el número de usuarios "cargados" en la lista
    int final_elements = i + 1;

    // Título parte izquierda (amigos)
    HWND friends_title = CreateWindowExW(0, L"static", L"Amigos", WS_VISIBLE | WS_CHILD | SS_CENTER, 2, 36 + 8 + final_elements * 16 + final_elements * 4,
                                         (2 * width_unit) - 16, 16, hwnd, NULL, NULL, NULL);

    addHandle(AppStack, friends_title);

    // Si no hay un usuario activo o si el usuario activo no tiene amigos no se pueden cargar amigos.
    if (active_user == NULL) return;
    if (active_user->friends.size == 0){
        return;
    }
    i = 0;
    // Itera por los amigos de la lista de amigos
    user* currfriend;
    while (i < active_user->friends.size) {
        currfriend = active_user->friends.users[i];
        id = currfriend->id;

        // Prepara el texto para la lista
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", currfriend->username, character, id);

        // Crea el texto en la lista y lo agrega al Stack.
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
        // Boton pulsado
        case WM_COMMAND:
            // ID de la acción. En este caso lp contiene el handle del botón
            switch (wp) {
                // Botón de crear usuario del formulario
                case BUTTON_CREATE_USER:
                    // Si crea al usuario destruye el formulario y recarga la ventana.
                    if (createUser(hwnd) == TRUE) {
                        DestroyWindow(hwnd);
                        LoadWindow(mainWindow);
                    }
                    break;
                // Selecciona al usuario en la ventana de Operar Como
                case SELECT_USER: {
                    // Coge el texto del botón y saca el usuario de la lista
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    // Si no se ha podido elegir un usuario
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo seleccionar al usuario", L"Información", MB_OK);
                    } else {
                        // El usuario activo pasa a ser el usuario seleccionado
                        active_user = selected_user;
                        wchar_t text[MAX_LENGTH + 22] = L"Usuario Seleccionado: ";
                        // Avisa al usuario de sus acciones mediante un popup
                        wcscat(text, selected_user->username);
                        MessageBox(hwnd, text, L"Información", MB_OK);
                    }
                    // Destruye la ventana de seleccion y recarga la página
                    DestroyWindow(hwnd);
                    LoadWindow(mainWindow);
                }
                break;
                // Botón en la ventana de enviar solicitudes de amistad
                case FR_BUTTON: {
                    // Coge el texto del botón y saca el usuario de la lista
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    // Si no se ha podido elegir un usuario
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo enviar la solicitud de amistad", L"Error", MB_ICONEXCLAMATION);
                    } else {
                        // Avisa al usuario de si quiere enviar una solicitud de amistad al usuario escogido.
                        wchar_t text[MAX_LENGTH + 32] = L"¿Enviar solicitud de amistad a ";
                        wcscat(text, selected_user->username);
                        wcscat(text, L"?");
                        // Si acepta se envia la solicitud de amistad
                        if (MessageBox(hwnd, text, L"¿Estás seguro?", MB_OKCANCEL) == IDOK){
                            sendFriendRequest(active_user, selected_user);
                        }
                    }
                    // Destruye la ventana con la lista de los usuarios
                    DestroyWindow(hwnd);
                }
                break;
                // Botón en la ventana de solicitudes de amistad recibidas
                case RFR_BUTTON:{
                    // Coge el texto del botón y saca el usuario de la lista
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    // Si no se ha podido elegir un usuario
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo aceptar la solicitud de amistad", L"Error", MB_ICONEXCLAMATION);
                    } else {
                        // Avisa al usuario de si quiere aceptar la solicitud.
                        wchar_t text[MAX_LENGTH + 24] = L"¿Aceptar solicitud de ";
                        wcscat(text, selected_user->username);
                        wcscat(text, L"?");
                        // Si Ok acepta la solicitud, si Cancel la rechaza.
                        if (MessageBox(hwnd, text, L"¿Aceptar o rechazar solicitud?", MB_OKCANCEL) == IDOK){
                            acceptFriendRequest(active_user, selected_user);
                            LoadWindow(mainWindow);
                        } else {
                            rejectFriendRequest(active_user, selected_user);
                        }
                    }
                    // Destruye la ventana con la lista de botones
                    DestroyWindow(hwnd);
                }
                break;
                // Botón en la ventana de cancelar solicitudes de amistad
                case SFR_BUTTON:{
                    // Coge el texto del botón y saca el usuario de la lista
                    wchar_t usernameid[MAX_LENGTH+5];
                    GetWindowTextW((HWND) lp, usernameid, MAX_LENGTH);
                    user* selected_user = getUser(users, usernameid);
                    // Si no se ha podido elegir un usuario
                    if (selected_user == NULL){
                        MessageBox(hwnd, L"No se pudo rechazar la solicitud de amistad", L"Error", MB_ICONEXCLAMATION);
                    } else {
                        // Avisa al usuario de si quiere cancelar la solicitud enviada.
                        wchar_t text[MAX_LENGTH + 25] = L"¿Cancelar solicitud de ";
                        wcscat(text, selected_user->username);
                        wcscat(text, L"?");
                        // Si acepta se cancela la solicitud de amistad enviada
                        if (MessageBox(hwnd, text, L"¿Estás seguro?", MB_OKCANCEL) == IDOK){
                            rejectFriendRequest(selected_user, active_user);
                        }
                    }
                    // Destruye la ventana con la lista de botones
                    DestroyWindow(hwnd);
                }
                break;
            }
            break;
        // Ventana cerrada
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            // Para no tener que especificar cada caso, DefWindowProc procesa los mensajes con su valor por defecto
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

// Registers the dialog/form window class
void registerFormClass(HINSTANCE hInstance) {
    // Crea una clase de ventana nueva para los forms y las listas de los botones
    const wchar_t CLASS_NAME[] = L"DialogWindow";
    WNDCLASSW wc = {};

    // Define las caracteristicas de la ventana
    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    // Utiliza el procedure definido anteriormente
    wc.lpfnWndProc = DialogProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassW(&wc);
}

// Displays the form to add a new user
void displayForm(HWND hwnd) {
    // Parámetros de la ventana iniciales.
    int window_width = 460;
    int window_height = 400;

    // Crea la ventana del formulario, del tipo definido previamente.
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Nuevo Usuario", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                   CW_USEDEFAULT, window_width, window_height, hwnd, NULL, NULL, NULL);

    // Define los parámetros de tamaño de los elementos
    int paddingx = 24;
    int paddingl = 24;

    int marginsides = 12;
    int marginline = 16;

    int Lheight = 16;
    int Lwidth = 200;

    int Theight = 24;
    int Twidth = 200;

    // Nombre de usuario
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

    // Ubicación
    CreateWindowExW(0, L"static", L"Ubicación", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight + Theight + marginline, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    hFormLocation = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_AUTOHSCROLL,
                                    paddingx, paddingl + (Lheight * 2) + Theight + marginline, Twidth, Theight, windowH,
                                    NULL, NULL, NULL);

    // Fecha de nacimiento
    CreateWindowExW(0, L"static", L"Cumpleaños (DD/MM/YYYY)", WS_VISIBLE | WS_CHILD | SS_CENTER | ES_AUTOHSCROLL,
                    paddingx + Lwidth + marginsides, paddingl + Lheight + Theight + marginline, Lwidth, Lheight,
                    windowH, NULL, NULL, NULL);
    hFormBirthday = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                                    paddingx + Lwidth + marginsides, paddingl + Lheight * 2 + Theight + marginline,
                                    Twidth, Theight, windowH, NULL, NULL, NULL);

    // Gustos
    CreateWindowExW(0, L"static", L"Gustos (Gusto1,Gusto2,...,Gusto5)", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight * 2 + Theight * 2 + marginline * 2, Lwidth * 2 + marginsides, Lheight, windowH,
                    NULL, NULL, NULL);
    hFormHobbies = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | SS_CENTER,
                                   paddingx, paddingl + Lheight * 3 + Theight * 2 + marginline * 2,
                                   Lwidth * 2 + marginsides, Theight * 2, windowH, NULL, NULL, NULL);

    // Botón para crear el usuario
    CreateWindowExW(0, L"button", L"Crear", WS_VISIBLE | WS_CHILD | SS_CENTER, window_width / 2 - 100,
                    paddingl + Lheight * 3 + Theight * 4 + marginline * 4, 200, 24, windowH, (HMENU) BUTTON_CREATE_USER,
                    NULL, NULL);
}

// Muestra los detalles del usuario activo
void userDetails(HWND hwnd){
    // Parámetros de la ventana iniciales.
    int window_width = 460;
    int window_height = 540;

    // Crea la ventana del formulario, del tipo definido previamente.
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Detalles del Usuario", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                   CW_USEDEFAULT, window_width, window_height, hwnd, NULL, NULL, NULL);

    // Define los parámetros de tamaño de los elementos
    int paddingx = 24;
    int paddingl = 24;

    int marginsides = 12;
    int marginline = 16;

    int Lheight = 16;
    int Lwidth = 200;

    int Theight = 40;
    int Twidth = 200;

    // Nombre de usuario
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

    // Ubicación
    CreateWindowExW(0, L"static", L"Ubicación", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight + Theight + marginline, Lwidth, Lheight, windowH, NULL, NULL, NULL);
    CreateWindowExW(0, L"static", active_user->location, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                    paddingx, paddingl + (Lheight * 2) + Theight + marginline, Twidth, Theight, windowH,
                                    NULL, NULL, NULL);

    // Fecha de nacimiento
    CreateWindowExW(0, L"static", L"Cumpleaños", WS_VISIBLE | WS_CHILD | SS_CENTER,
                    paddingx + Lwidth + marginsides, paddingl + Lheight + Theight + marginline, Lwidth, Lheight,
                    windowH, NULL, NULL, NULL);
    CreateWindowExW(0, L"static", active_user->birthday, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                    paddingx + Lwidth + marginsides, paddingl + Lheight * 2 + Theight + marginline,
                                    Twidth, Theight, windowH, NULL, NULL, NULL);

    // Gustos
    CreateWindowExW(0, L"static", L"Gustos", WS_VISIBLE | WS_CHILD | SS_CENTER, paddingx,
                    paddingl + Lheight * 2 + Theight * 2 + marginline * 2, Lwidth * 2 + marginsides, Lheight, windowH,
                    NULL, NULL, NULL);

    // Mete los usuarios en una wide string separados por saltos de línea
    wchar_t hobbies[MAX_HOBBIES*MAX_LENGTH+4];
    hobbies[0] = 0;
    for (int i = 0; i < 5; i++){
        wcscat(hobbies, active_user->hobbies[i]);
        if (i == 4){
            break;
        }
        wcscat(hobbies, L"\n");
    }

    // Crea el texto de la lista de gustos y le asigna la wide string
    CreateWindowExW(0, L"static", hobbies, WS_VISIBLE | WS_CHILD | SS_CENTER,
                                   paddingx, paddingl + Lheight * 3 + Theight * 2 + marginline * 2,
                                   Lwidth * 2 + marginsides, Theight * 2, windowH, NULL, NULL, NULL);


    // Todas las palabras ordenadas por uso (de mayor a menor).
    CreateWindowExW(0, L"static", L"Ranking de palabras globales", WS_VISIBLE | WS_CHILD | SS_CENTER,
                    paddingx, paddingl + Lheight * 4 + Theight * 4 + marginline * 3, Lwidth,
                    Lheight, windowH, NULL, NULL, NULL);

    HWND hRanking = CreateWindowExW(0, L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL,
                    paddingx, paddingl * 2 + Lheight * 5 + Theight * 4 + marginline * 3,
                    Lwidth * 2 + marginsides, Theight*3, windowH, NULL, NULL, NULL);

    // Crea un diccionario de palabras temporal y lo inicializa. Le asigna a todos los elementos count = 0
    WDict tempDict;
    initDict(&tempDict, WordDictionary.size);
    for (int i = 0; i < tempDict.size; i++){
        tempDict.elements[i].count = WordDictionary.elements[i].count;
        if (tempDict.elements[i].count != 0){
            wcscpy(tempDict.elements[i].key, WordDictionary.elements[i].key);
        }
    }

    // Ordena el diccionario usando un mergeSort.
    mergeSortDict(&tempDict, 0, WordDictionary.size-1);

    // Itera en el diccionario temporal ordenado.
    wchar_t texto[DICT_SIZE*3];
    wchar_t countnum[5];
    for (int i = 0; i < tempDict.size; i++){
        // Si llega a un elemento con count 0, como está ordenado ya ha terminado con los elementos del diccionario
        if (tempDict.elements[i].count == 0){
            break;
        }
        // Escribe la línea
        wcscat(texto, tempDict.elements[i].key);
        wcscat(texto, L" \t- ");
        // Cambia el número de veces que se ha repetido la palabra de int a wchar_t
        swprintf_s(countnum, 5, L"%d", tempDict.elements[i].count);
        wcscat(texto, countnum);
        wcscat(texto, L"\r\n");
    }
    // Asigna el texto a la caja de texto
    SetWindowTextW(hRanking, texto),

    // Vacía la variable para no duplicar la lista cada vez que se llama a la función
    wcscpy(texto, L"");

    // Libera la memoria del diccionario temporal
    free(tempDict.elements);

}

// Checks the form and creates the user
int createUser(HWND hwnd) {
    wchar_t username[MAX_LENGTH];
    wchar_t email[MAX_LENGTH];
    wchar_t birthday[MAX_LENGTH];
    wchar_t location[MAX_LENGTH];
    wchar_t hobbies[MAX_LENGTH * 5];

    // Coge el texto de los diferentes campos del formulario
    GetWindowTextW(hFormUsername, username, MAX_LENGTH);
    GetWindowTextW(hFormEmail, email, MAX_LENGTH);
    GetWindowTextW(hFormBirthday, birthday, MAX_LENGTH);
    GetWindowTextW(hFormLocation, location, MAX_LENGTH);
    GetWindowTextW(hFormHobbies, hobbies, MAX_LENGTH * 5 + 4);

    // Comprueba los diferentes campos, si hay un problema con uno devuelve FALSE

    // Si el email no contiene @ es inválido
    for (int i = 0; i < MAX_LENGTH; i++) {
        if (email[i] == '@') {
            break;
        }
        if (email[i] == 0) {
            MessageBox(hwnd, L"Formato de mail incorrecto", L"Información", MB_OK);
            return FALSE;
        }
    }

    int year;
    int month;
    int day;

    // Comprueba que sea una fecha válida
    if (swscanf(birthday, L"%d/%d/%d", &day, &month, &year) < 3) {
        MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Información", MB_OK);
        return FALSE;
    }

    struct tm time = getCurrentDate();

    // Comprueba que el año esté entre 1900 y el año actual - 1
    if (year > time.tm_year + 1900 || year < 1900) {
        MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Información", MB_OK);
        return FALSE;
    }

    // Comprueba que los meses sean válidos
    if (month > 12 || month < 1) {
        MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Información", MB_OK);
        return FALSE;
    }

    // Comprueba que los días estén bien dependiendo del mes.
    if (day > 28) {
        if (month == 2) {
            if ((year % 4 && (year % 100 != 0 || year % 400 == 0)) || day > 29) {
                MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Información", MB_OK);
                return FALSE;
            }
        }
    }
    if (day == 31) {
        if (month == 2 || month == 4 || month == 6 || month == 9 || month == 11) {
            MessageBox(hwnd, L"Fecha de nacimiento incorrecta", L"Información", MB_OK);
            return FALSE;
        }
    }


    wchar_t hobby1[MAX_LENGTH];
    wchar_t hobby2[MAX_LENGTH];
    wchar_t hobby3[MAX_LENGTH];
    wchar_t hobby4[MAX_LENGTH];
    wchar_t hobby5[MAX_LENGTH];
    // Saca los gustos separados por comas menos el último, que es hasta el final de la wide string.
    int source = swscanf_s(hobbies, L"%[^,],%[^,],%[^,],%[^,],%[^\0]", hobby1, MAX_LENGTH, hobby2, MAX_LENGTH, hobby3, MAX_LENGTH, hobby4, MAX_LENGTH, hobby5, MAX_LENGTH);
    if (source < 5) {
        MessageBox(hwnd, L"Formato de gustos incorrecto/Menos de 5 gustos introducidos", L"Información", MB_OK);
        return FALSE;
    }

    // Si todos los campos son válidos crea el usuario.
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

    // Agrega el usuario a la lista de usuarios global, actualiza el serial y el tamaño de la lista
    addUser(&users, newuser);
    serial += 1;

    MessageBox(hwnd, L"User Created", L"Información", MB_OK);

    return TRUE;
}


// Crea un Post y lo agrega a la cola del usuario
void sendMessage(){
    // Usuario no seleccionado
    if (active_user==NULL){
        return;
    }

    // Coge texto de la caja de texto
    wchar_t messageboxText[POST_LENGTH] = L"";
    GetWindowTextW(hMessageBox, messageboxText ,POST_LENGTH);



    // Mensaje vacío
    if (wcscmp(messageboxText, L"") == 0){
        return;
    }

    // Crea el post y lo agrega a la lista de posts del usuario activo
    Post* newpost = createPost(messageboxText);
    addToQueue(&active_user->posts, newpost);
    // Vacía la caja de texto
    SetWindowTextW(hMessageBox, L"");

    // Divide el texto de la caja por cualquier espacio blanco. Espacios, saltos de línea, tabulaciones, etc.
    wchar_t* token = wcstok(messageboxText, (const wchar_t *) L"\t\r\n\v\f ");
    DictNode* currnode;

    // Itera entre las palabras
    while (token != NULL) {
        // Busca si la clave (palabra) ya existe en el diccionario
        currnode = searchDict(&WordDictionary, token);
        // Si no existe la añade
        if (currnode == NULL){
            addToDict(&WordDictionary, token);
        } else {
            // Si existe le añade 1 al contador
            currnode->count += 1;
        }
        // Coge la siguiente palabra
        token = wcstok(NULL, (const wchar_t *) L"\t\r\n\v\f ");
    }
    // Recarga la interfaz
    LoadWindow(mainWindow);
}


















