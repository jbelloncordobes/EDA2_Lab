//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_COMMON_H
#define EDA2_LAB_COMMON_H

// Definitions
#define TRUE 1
#define FALSE 0
#define TEXT_LENGTH 1000

// Acciones de la pantalla principal (Botones y menus)
#define MENU_NEW_USER 1
#define USER_DETAILS 2
#define MENU_CHANGE_USER 4
#define POST_MESSAGE 3
#define OPEN_SEND_FR 5
#define OPEN_RECEIVED_FR 6
#define OPEN_SENT_FR 7

// Acciones de los modals (formulario de crear usuarios y ventanas secundarias de amigos y operar como)
#define BUTTON_CREATE_USER 2
#define SELECT_USER 5
#define FR_BUTTON 12
#define RFR_BUTTON 13
#define SFR_BUTTON 14

#include <Windows.h>

// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void displayForm(HWND hwnd);
void Paint(HWND hwnd);
void AddMenu(HWND hwnd);
void LoadWindow(HWND hwnd);
void registerFormClass(HINSTANCE hInstance);
int createUser(HWND hwnd);
void sendMessage();
void userDetails(HWND hwnd);
#endif //EDA2_LAB_COMMON_H
