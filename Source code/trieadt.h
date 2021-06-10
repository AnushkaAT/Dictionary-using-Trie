#ifndef TRIEADT_H_INCLUDED
#define TRIEADT_H_INCLUDED
#include<stdbool.h>
#include<windows.h>

typedef struct tnode{
    struct tnode* alph[26];
    bool eow;
    //char *filename;
    char *meaning;
}tnode;

typedef tnode* trie;

tnode* newnode();
void trieinit(trie *t);
void showCount(trie t);
void insert_word(trie *t, char *word, char *mean);

int deleteWord(trie *t, char *key);
int print_meaning(trie t, char *word);
tnode* load_dict(char *diction);
void unload(tnode *dict);
char* searchword(tnode *root, char *key);
void addNew(trie *root, char *word, char *meaning, char *pass);
void suggestions(trie root, char* pref, HWND hwnd, int y);
int printAutosuggest(trie root, char *query, HWND hwnd);
void results(trie t, char *key, HWND hwnd);
void wordToday(trie root);

void historyadd(char *key);
void showhistory(char *name);


#endif // TRIEADT_H_INCLUDED
