/*File name: functions of trie adt that is prefix trees
Author: Anushka Tadphale
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"trieadt.h"
#include"gui.h"

#define PASSWORD "dsa21"

extern HWND hsug;
static int wordcount;
//long filesize;

void showCount(trie t){
    printf("Total words: %d\n", wordcount);
    return;
}

char toLower(char c){
    if(c>='a' && c<='z')
        return c;
    return c+32;
}

tnode* newnode(){
    tnode *nn= (tnode*)malloc(sizeof(tnode));
    if(nn== NULL){
        printf("Out of space\n");
        return NULL;
    }
    //printf("alloced\n");
    if(nn){
        nn->eow= false;
        for(int i=0; i<26; i++)
            nn->alph[i]=NULL;
        //nn->filename= NULL;
        nn->meaning= NULL;
    }
    //printf("done node\n");
    return nn;
}

void trieinit(trie *t){
    *t=NULL;
    return;
}

#define INDEX(c) ((int)c - (int)'a')
//insert the given word with its description into the trie
void insert_word(trie *t, char *word, char *mean){
    int len= strlen(word);
    int level, index;
    tnode *p= *t;
    for(level=0; level<len; level++){
        if(word[level]>='A' && word[level]<='Z'){
            //if character is in uppercase change it to lower case
            char ch= toLower(word[level]);
            index= INDEX(ch);
        }
        else
            index= INDEX(word[level]);

        if(index>25 || index<0)
            continue;

        //now insert the character into the trie
        //in trie insertion of character means the creation of node at that index
        if(!(p->alph[index])){
            p->alph[index]= newnode(); //node creation for each alphabet
        }
        //go to next destination
        p=p->alph[index];
    }
    p->eow= true;
    //printf("inserting word done\n");
    //p->filename= word; //this makes easy to search for word in text file
    //p->meaning= mean;
    int len2= strlen(mean);
    p->meaning= (char*)malloc(sizeof(char)* len2);
    strcpy(p->meaning, mean);
    //for every word inserted, wordcount incremented
    wordcount++;
    return;
}

char* output(char *word, char *meaning){
    char *out= (char*)malloc(sizeof(char)* (strlen(word)+strlen(meaning)+3));
    strcpy(out, word);
    strcat(out, ": ");
    strcat(out, meaning);
    strncat(out, "\n", 1);
    return out;
}

int print_meaning(trie t, char *word){
    int level, index;
    int len= strlen(word);
    tnode *p= t;
    for(level=0; level<len; level++){
        index= toLower(word[level])- 'a';
        if(p->alph[index]!=NULL){
            p=p->alph[index];
            //printf("%c\n", word[level]);
        }
        else if(p->alph[index]==NULL){
            //printf("level= %d\n", level);
            return level;
        }
    }
    if(level== len && p->eow ==false)
        return level;
    if(p->eow==true){
        //printf("%s: %s\n", word, p->meaning);
        char *text= output(word, p->meaning);
        showtext(text);
        //printf("%s", text);
        return -1; //error free searching flag
    }
    /*else
        printAutosuggest(t, word);*/
}

//loads the dictionary file which contains all the words. Returns true if loaded succesfully, else false
tnode* load_dict(char *diction){
    FILE* dict= fopen(diction, "rb");
    //open file in read binary mode for optimization
    if(dict== NULL){
        //file opening error
        printf("Could not open the file\n");
        return NULL;
    }

    //now take the size of the file
    //fseek(dict, 0, SEEK_END);
    //filesize= ftell(dict);

    //initialise  a trie for the dictionary
    tnode *root= newnode();
    if(root==NULL){
        printf("Out of space, cannot load dictionary\n");
        return NULL;
    }
    //int index;
    char word[20];
    char meaning[100];

    while(fscanf(dict, "%s ", word)!= EOF){
        //word copied into wrod string.
        //fscanf will not input characters after space is encountered into the word string
        // inthe file words and meanings are separated by a space
        //now use fgets to input meaning, since it may contain some spaces
        fgets(meaning, 100, dict);
        //int wordlen= strlen(word);
        insert_word(&root, word, meaning);
    }
    fclose(dict);

    return root;
}

//we will unload the dictionary at the end of the program, this will clear the space
void unload(tnode *dict){
    tnode *p= dict;

    //recursively free the allocated memory for each child
    for(int i=0; i<26; i++){
        if(p->alph[i]!=NULL)
            unload(p->alph[i]);
    }
    free(p);
    p= NULL;
}

//search function which will return description/meaning corresponding to word
char* searchword(tnode *root, char *key){
    tnode *p= root;
    char ch;
    int len= strlen(key);
    for(int i=0; i<len; i++){
        ch= toLower(key[i]);
        int index= INDEX(ch);
        //for every alphabet it goes to next level, check whether that letter is present or not
        if(p->alph[index]==NULL)
            return NULL;
        p= p->alph[index];
    }
    //if the letter(level) we have reached is a word i.e end of word is reached
    //(flag true)
    if(p->eow== true)
        return p->meaning;
    else
        return NULL;
}

int checkpass(char *pass){
    //char pass[6];
    //printf("Enter password to add new word to dictionary: ");
    //scanf("%s", pass);
    if(strcmp(pass, PASSWORD)!=0){
        printf("Incorrect password\n");
        return 0;
    }
    return 1;
}

//this function allows user to add new words to the dictionary
void addNew(trie *root, char *word, char *meaning, char *pass){
    //char word[20];
    //char des[100];

    //let the user add new word only if he knows the password
    if(checkpass(pass)==0)
        return;

    /*printf("Enter the word you want to add: ");
    fgets(word, 20, stdin);
    printf("Enter the meaning or description of the word: ");
    fgets(des, 100, stdin);*/

    if(word[0]=='\0' || word[0]=='\n' || meaning[0]=='\0' || meaning[0]=='\n')
        return;  //if user has not entered any word and just enter key is pressed
    strtok(word, "\n");
    strtok(meaning, "\n");

    //insert the new word with its meaning into trie
    insert_word(root, word, meaning);

    //also append this word to the file
    FILE *dict;
    dict= fopen("D:/test.txt", "a");
    fprintf_s(dict, "%s %s\n", word, meaning);
    fclose(dict);

    return;
}

//function returns 1 if the node passed has at least one child
int hasChild(tnode *n){
    for(int i=0; i<26; i++)
        if(n->alph[i]!=NULL)
            return 1; //child present, so return
    return 0;
}

//recursive function to delete a given key from trie
int deleteWord(trie *t, char *key){
    if(*t==NULL)
        return 0;
    //if end of string is not reached
    if(*key){
        //delete recursively for the next character
        if(*t!= NULL && (*t)->alph[*key - 'a']!= NULL && deleteWord(&((*t)->alph[*key - 'a']), key+1) && (*t)->eow==false){
            if(!hasChild(*t)){
                //a leaf node
                free(*t);
                *t= NULL;
                return 1; //deletion successful
            }
            else
                return 0;
        }
    }

    //if end of string is reached
    if(*key== '\0' && (*t)->eow== true){
        if(!hasChild(*t)){
            //a leaf node
            free(*t);
            *t= NULL;
            return 1; //deletion successful
        }

        //not a leaf node
        else{
            //we wont delete this node, as this node may be a part of another word
            //mark it as leaf node
            (*t)->eow= true;
            return 0; //not deletion
        }
    }

    return 0;
}

//if a particular word searched is not found, then words similar to it are printed
//this is similar to auto complete feature, principle used is prefix searching
/*void searchSimilar(tnode *root, char *key){
    int level, in;
    int len= strlen(key);
    int flag=0;
    tnode *p= root;
    tnode *q= root;
    //char c;
    int cnt=0;
    for(level= 0; level< len-2; level++){
        in= INDEX(key[level]);
        if(p->alph[in])
            p=p->alph[in];
        else
            flag=1;
    }
    //in= INDEX(key[level]);
    if(flag!= 1){
        //i.e nodes upto this alphabet exist, prefix exists
        printf("\nSuggestions...\n");
        for(int i=0; i<26; i++){
            if(p->alph[i]){
                //for charcters common, i.e. prefix print it
                for(int l=0; l<len-2; l++)
                    printf("%c", key[l]);

                printf("%c", i+'a');
                q= p->alph[i];
                while(q && cnt==0){
                    for(int j=0; j<26; j++){
                        if(q->alph[j]){
                            printf("%c", j+'a');
                            q=q->alph[j];
                        }
                        else
                            cnt=1;
                    }
                }
                printf("\n");
            }
            cnt=0;
        }
    }
    if(flag==1)
        printf("Similar words not found\n");
}*/

void trimLastchar(char *str){
    int len= strlen(str);
    str[len-1]= '\0';
    return;
}

//recursive function to search for suggestions
void suggestions(trie root, char* pref, HWND hwnd, int y){
    //found a word in trie with given prefix
    if(root->eow){
        char *text= output(pref, root->meaning);
        //showtext(text);
        addCon(hwnd, y,text);
        y++;
        //printf("%s", text);
    }

    //this is last or leaf node
    if(!hasChild(root))
        return;

    for(int i=0; i<26; i++){
        if(root->alph[i]){
            //append this character to current prefix
            char cur= i + 'a';
            strncat(pref, &cur, 1);
            suggestions(root->alph[i], pref, hwnd, y);
            trimLastchar(pref);
        }
    }
}

//print the auto suggestions
int printAutosuggest(trie root, char *query, HWND hwnd){
    tnode *p= root;
    int level;
    int len= strlen(query);
    //check if prefix is present
    //go till the last character of the word present in the trie
    for(level=0; level< len; level++){
        char c= toLower(query[level]);
        int in= INDEX(c);

        //if no string found with this prefix
        if(p->alph[in]== NULL)
            return 0;
        p= p->alph[in];
    }

    //if prefix is present as a word itself
    bool isWord= (p->eow== true);
    //if we have reached last node
    bool isLast= (hasChild(p)==0);
    int i=0;
    if(isWord && isLast){
        //i.e prefix is present as complete word
        //and there is no subtree present below it
        char *text= output(query, p->meaning);
        addCon(hwnd, i, text);
        //printf("%s", text);
        free(text);
        i+=1;
        return -1;
    }

    //there is subtree below
    //i,e there are matching characters below the last character of query
    if(!isLast){
        suggestions(p, query, hwnd, i);
        return 1;
    }
}

void results(trie t, char *key, HWND hwnd){
    int res= print_meaning(t, key);
    //printf("%d", res);
    if(res!= -1){
        showtext(" ");
        char disc[]= "Ooops! Couldn't find a matching search. Suggestions";
        char *sub= (char*)calloc(sizeof(char), (res+10));
        for(int i=0; i<res; i++){
            strncat(sub, &key[i], 1);
        }
        int as= printAutosuggest(t, sub, hwnd);
        if(as==0)
            printf("No similar words found\n");
    }
    historyadd(key);
    return;
}

//this function prints a random word from the dictionary with its meaning
//word of the day feature
void wordToday(trie root){
    int r= (rand() %(26));
    tnode *p= root->alph[r];
    while(!p){
        r= (rand() %(26));
        p= root->alph[r];
    }
    char ch, word[20];
    while(p || !(p->eow)){
        for(int i=0; i<26; i++){
            if(p->alph[i]){
                ch= i + 'a';
                strncat(word, &ch, 1);
                p=p->alph[i];
            }
        }

    }
    printf("WORD OF THE DAY IS\n%d", r);
    //printf("%s: %s\n", word, p->meaning);
}

void showhistory(char *name){
    FILE *fp= fopen(name, "r");
    if(fp==NULL){
        printf("Error in opening file\n");
        return;
    }
    printf("Word-search history: ");
    char res[20];
    while(!feof(fp)){
        fscanf(fp, "%s", res);
        printf("%s\n", res);
    }
    fclose(fp);
}

void historyadd(char *key){
    FILE *fp= fopen("D:/history.txt", "a");
    if(fp==NULL){
        printf("Error in opening file\n");
        return;
    }
    else{
        fprintf(fp, "%s\n", key);
    }
    fclose(fp);
    return;
}
