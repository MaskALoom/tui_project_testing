#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

void SetTyping(struct termios* old, struct termios* new, bool disable);
void Update(void);
void ClearTerm(void);
bool CheckKeyInput(int key);

int main(int argc, char* argv[]){
    struct termios old;
    struct termios new;

    //ClearTerm();
    SetTyping(&old, &new, false);
    Update();
    SetTyping(&old, &new, true);

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

void Update(void){
    bool running = true;
    char key;
    while(running){
        read(STDIN_FILENO, &key, 1);
        if(CheckKeyInput(key)) running = false;
    }
}
bool CheckKeyInput(int key){
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
    }
    return false;
}
