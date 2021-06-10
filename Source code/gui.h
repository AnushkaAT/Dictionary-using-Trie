#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include<windows.h>
#include<stdbool.h>
#include"trieadt.h"

void showtext(char *text);
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
void addCon(HWND hwnd, int y, char *text);
void regDialog(HINSTANCE hInt);
void displayDialog(HWND hwnd);

#endif // GUI_H_INCLUDED
