/*File name:
Author: Anushka Tadphale
*/

//Problem:

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include"trieadt.h"
#include"gui.h"


trie dict;
/*int main(){

    dict= newnode();
    char filedict[]= "D:/test.txt";
    dict= load_dict(filedict);
    printf("Done loading\n");
    results(dict, "apple");
    //addNew(&dict, filedict);
    //wordToday(dict);
    //showCount(dict);
    unload(dict);
	return 0;
}*/

int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hprev, LPSTR args, int ncmd){
    dict= newnode();
    char filedict[]= "D:/test.txt";
    dict= load_dict(filedict);
    //MessageBox(NULL, "Hello", "First GUI", MB_OK);
    WNDCLASSW wc= {0};
    HBRUSH hbrush= CreateSolidBrush(RGB(255, 255, 255));
    wc.hbrBackground = hbrush;
    wc.hCursor= LoadCursor(NULL, IDC_ARROW);
    wc.hInstance= hInt;
    wc.lpszClassName= L"myWindowclass";
    wc.lpfnWndProc= WindowProcedure;
    if(!RegisterClassW(&wc))
        return -1;
    CreateWindowW(L"myWindowclass", L"My window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    regDialog(hInt);

    MSG msg = {0};
    while(GetMessage(&msg, NULL, NULL, NULL)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
