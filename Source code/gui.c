/*File name:
Author: Anushka Tadphale
*/

//Problem:
#include"gui.h"
#define GENR_BUTTON 4
#define ADD_BUT 5
#define ADD_MENU 3
#define HISTORY_MENU 2
#define DEL_MENU 10

extern trie dict;
char word[15], meaning[30], pass[6];
HWND hName, hOut, hsug, hBut;
HMENU hmenu;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*void showtext(){
    char name[30], age[5], out[50];
    GetWindowText(hName, name, 30);
    GetWindowText(hAge, age, 5);
    strcpy(out, name);
    strcat(out, " is ");
    strcat(out, age);
    strcat(out, " years old.");
    SetWindowText(hOut, out);
}*/

void addCon(HWND hwnd, int y, char *text){
    int j= y*50;
    hsug= CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 300+j, 300, 50, hwnd, NULL, NULL, NULL);
    SetWindowText(hsug, text);
}

void showtext(char *text){
    SetWindowText(hOut, text);
}
//                instance of app                   string      cmd line
/*int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hprev, LPSTR args, int ncmd){
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
    MSG msg = {0};
    while(GetMessage(&msg, NULL, NULL, NULL)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}*/

void AddMenu(HWND hwnd){
    hmenu= CreateMenu();

    AppendMenu(hmenu, MF_STRING, ADD_MENU, "Add new");
    AppendMenu(hmenu, MF_STRING, HISTORY_MENU, "View History");
    AppendMenu(hmenu, MF_STRING, DEL_MENU, "DELETE WORD");
    SetMenu(hwnd, hmenu);
}

void AddControls(HWND hwnd){
    //static controls
    CreateWindowW(L"static", L"Enter word: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 50, 98, 38, hwnd, NULL, NULL, NULL);
    hName= CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 50, 98, 38, hwnd, NULL, NULL, NULL); //edit controls
    //CreateWindowW(L"Button", L"Add new", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | BS_OWNERDRAW, 20, 20, 90, 30, hwnd, (HMENU)ADD_BUT, );
    //CreateWindowW(L"static", L"Age: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 90, 98, 38, hwnd, NULL, NULL, NULL);
    //hAge= CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 90, 98, 38, hwnd, NULL, NULL, NULL);

    CreateWindowW(L"Button", L"Search", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | BS_OWNERDRAW, 150, 140, 98, 38, hwnd, (HMENU)GENR_BUTTON, NULL, NULL);
    //SetBkColor(hBut, RGB(0, 0, 0));
    hOut= CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE| SS_CENTER | ES_AUTOVSCROLL, 100, 200, 300, 50, hwnd, (HMENU)GENR_BUTTON, (HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE), NULL);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
    LPDRAWITEMSTRUCT lpds= (DRAWITEMSTRUCT*)lp;
    char name[15];
    switch(msg)
    {
    case WM_DRAWITEM:{

        SIZE size;
        char text[100];
        GetTextExtentPoint32(lpds->hDC, "Search", strlen("Search"), &size);
        SetTextColor(lpds->hDC, RGB(0, 0, 0));
        SetBkColor(lpds->hDC, RGB(255, 255, 100));
        ExtTextOut(lpds->hDC, (lpds->rcItem.left+20), (lpds->rcItem.top+10), ETO_OPAQUE | ETO_CLIPPED, &(lpds->rcItem), "Search", strlen("Search"), NULL);
        DrawEdge(lpds->hDC, &lpds->rcItem, (lpds->itemState & ODS_SELECTED? EDGE_SUNKEN: EDGE_RAISED), BF_RECT);
        return true;}
        break;

    case WM_CREATE:
        displayDialog(hwnd);
        AddControls(hwnd);
        AddMenu(hwnd);
        break;
    case WM_DESTROY:
        unload(dict);
        PostQuitMessage(0); //makes getmessage value false thus breaks loop and function end
        break;

    case WM_COMMAND:
        switch(wp)
        {
        case GENR_BUTTON:
            //showtext();

            GetWindowText(hName, name, 10);
            //addCon(hwnd, 0, name);
            results(dict, name, hwnd);
            break;

        case ADD_BUT:
            displayDialog(hwnd);
            break;
        case DEL_MENU:
            deleteWord(dict, name);
            break;
        case HISTORY_MENU:
            showhistory("D:/history.txt");
            break;
        }
    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }
}

LRESULT CALLBACK DialogProcedur(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg)
    {
    case WM_CREATE:
        displayDialog(hwnd);
    case WM_COMMAND:
        switch(wp)
        {
        case ADD_BUT:
            addNew(dict, word, meaning, pass);
            break;
        }
    case WM_CLOSE:
        DestroyWindow(hwnd);
    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }
}

void regDialog(HINSTANCE hInt){
    WNDCLASSW dia= {0};
    HBRUSH hbrush= CreateSolidBrush(RGB(255, 255, 255));
    dia.hbrBackground = hbrush;
    dia.hCursor= LoadCursor(NULL, IDC_ARROW);
    dia.hInstance= hInt;
    dia.lpszClassName= L"myDialogClass";
    dia.lpfnWndProc= DialogProcedur;
    RegisterClassW(&dia);

    /*CreateWindowW(L"myWindowclass", L"My window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
    MSG msg = {0};
    while(GetMessage(&msg, NULL, NULL, NULL)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }*/
}

void displayDialog(HWND hwnd){
    HWND hdig= CreateWindowW(L"myDialogClass", L"Add new", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 400, 400, 300, 300, hwnd, NULL, NULL, NULL);
    CreateWindowW(L"static", L"Enter word: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 50, 98, 30, hdig, NULL, NULL, NULL);
    HWND hword= CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 50, 98, 30, hdig, NULL, NULL, NULL);
    GetWindowText(hword, word, 15);
    CreateWindowW(L"static", L"Enter Meaning: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 90, 98, 30, hdig, NULL, NULL, NULL);
    HWND hmean= CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 90, 98, 30, hdig, NULL, NULL, NULL);
    GetWindowText(hmean, meaning, 30);
    CreateWindowW(L"static", L"Enter Password: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 100, 98, 30, hdig, NULL, NULL, NULL);
    HWND hpass= CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 100, 98, 30, hdig, NULL, NULL, NULL);
    GetWindowText(hpass, pass, 6);
    CreateWindowW(L"Button", L"Add", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 150, 160, 98, 30, hdig, (HMENU)ADD_BUT, NULL, NULL);
    ShowWindow(hwnd, SW_SHOW);
}
