#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

//#define OPTIONS_MENU (MenuOptionText){"testing_thing", START}

typedef enum{
    START,
    OPTIONS,
    EXIT
}MenuOption;

typedef struct{
    char text[255];
    MenuOption option;
}MenuOptionText;

typedef struct{
    MenuOptionText menu[3];
    MenuOption activeOption;
    int optionIndex;
    bool killGame;
}Game;

void SetTyping(struct termios* old, struct termios* new, bool disable);
void Update(Game* game);
void ClearTerm(void);
void TermSetup(Game* game);
void DrawTermGame(Game* game);
bool CheckKeyInput(int key, Game* game);

int main(int argc, char* argv[]){
    struct termios old;
    struct termios new;

    Game game;
    TermSetup(&game);

    SetTyping(&old, &new, false);
    Update(&game);
    SetTyping(&old, &new, true);
    ClearTerm();

    return 0;
}

void ClearTerm(void){
    printf("\033[2J");
    printf("\033[H");
}

void SetTyping(struct termios* old, struct termios* new, bool disable){
    if(disable){
        tcsetattr(STDIN_FILENO, TCSANOW, old);
        return;
    }
    tcgetattr(STDIN_FILENO, old);
    *new = *old;
    new->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, new);
}

void Update(Game* game){
    bool running = true;
    char key;
    DrawTermGame(game);
    while(running){
        if(game->killGame) break; 
        read(STDIN_FILENO, &key, 1);
        if(CheckKeyInput(key, game)) running = false;
        DrawTermGame(game);
    }
}
void MenuNavigation(Game* game, int key){
    int termSize = sizeof(game->menu) / sizeof(game->menu[0]);
    bool isReverse = false;
    if(key == 'k'){
        --game->optionIndex;
        if(game->optionIndex < 0) ++game->optionIndex;
    }
    if(key == 'j'){
        ++game->optionIndex;
        if(game->optionIndex >= termSize) --game->optionIndex;
    }
    game->activeOption = game->menu[game->optionIndex].option;

    if(key == '\n'){
        switch(game->activeOption){
            case START:
                break;
            case OPTIONS:
                break;
            case EXIT:
                game->killGame = true;
        }
    }
}
bool CheckKeyInput(int key, Game* game){
    bool isReverse = false;
    MenuNavigation(game, key);
    switch(key){
        case 'q':
            printf("Exiting...\n");
            return true;
        case 'e':
            printf("MEOW MEOW MEOW\n");
            break;
        case 'i':
            printf("Lets just pretend you opened your inventory or something!\n");
            break;
        //Term Controls for the options menu
    }
    return false;
}
void TermSetup(Game* game){
    game->menu[0] = (MenuOptionText){"START", START};
    game->menu[1] = (MenuOptionText){"OPTIONS", OPTIONS};
    game->menu[2] = (MenuOptionText){"EXIT", EXIT};

    game->activeOption = START;
    game->optionIndex = 0;
    game->killGame = false;
}
void DrawTermGame(Game* game){
    ClearTerm();
    int termSize = sizeof(game->menu) / sizeof(game->menu[0]);
    for(int i = 0; i < termSize; ++i){
        char drawString[255];
        strcpy(drawString, game->menu[i].text);
        if(game->menu[i].option == game->activeOption)
            strcat(drawString, " <");
        printf("%s\n", drawString);
    }
}












