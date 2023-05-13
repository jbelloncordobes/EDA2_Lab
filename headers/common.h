//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_COMMON_H
#define EDA2_LAB_COMMON_H

// Definitions
#define MENU_NEW_USER 1
#define BUTTON_CREATE_USER 2
#define OPEN_CHAT 3
#define CHANGE_USER 4
#define SELECT_USER 5



// Forward declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void displayForm(HWND hwnd);
void OnSize(HWND hwnd, UINT flag, int width, int height);
void Paint(HWND hwnd);
void AddMenu(HWND hwnd);
void LoadWindow(HWND hwnd);
void registerFormClass(HINSTANCE hInstance);
int createUser(HWND hwnd);





#endif //EDA2_LAB_COMMON_H
