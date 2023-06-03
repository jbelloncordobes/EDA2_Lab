//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_COMMON_H
#define EDA2_LAB_COMMON_H

// Definitions
// Acciones de la pantalla principal
#define MENU_NEW_USER 1
#define USER_DETAILS 2
#define MENU_CHANGE_USER 4
#define POST_MESSAGE 3
#define OPEN_SEND_FR 5
#define OPEN_RECEIVED_FR 6
#define OPEN_SENT_FR 7

// Acciones de los modals
#define BUTTON_CREATE_USER 2
#define SELECT_USER 5
#define SEND_FR 7
#define CANCEL_FR 8
#define ACCEPT_FR 9
#define DENY_FR 10
#define CANCEL 11
#define FR_BUTTON 12
#define RFR_BUTTON 13
#define SFR_BUTTON 14

#define TRUE 1
#define FALSE 0
#define TEXT_LENGTH 10000

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
void sendMessage(HWND hwnd);
void userDetails(HWND hwnd);




#endif //EDA2_LAB_COMMON_H
