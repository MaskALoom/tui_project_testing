#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

typedef enum{
    OPTION_BEGIN = 0,
    OPTION_SETTINGS,
    OPTION_EXIT,
}Option;

typedef struct{
    const char text[255];
    Option option;
}OptionText;

bool CheckInput(char c, Option* activeOption){
    bool isReverse = false;
    if(c == 'e'){
        printf("WOOWWWOWO\n");
    }
    else if(c == 'j' || c == 'k'){
        if(c == 'k') isReverse = true;
        switch(*activeOption){
            case OPTION_BEGIN:
                if(!isReverse) *activeOption = OPTION_SETTINGS;
                break;
            case OPTION_SETTINGS:
                if(isReverse) *activeOption = OPTION_BEGIN;
                else *activeOption = OPTION_EXIT;
                break;
            case OPTION_EXIT:
                if(isReverse) *activeOption = OPTION_SETTINGS;
                break;
        }
        //printf("\033[B");
    }
    else if(c == '\n'){
        switch(*activeOption){
            case OPTION_EXIT:
                return true;
        }
    }
    else if(c == 'q'){
        //printf("Exiting...\n");
        return true;
    }
    return false;
}

void drawText(Option* currentOption, OptionText options[]){
    printf("\033[2J");
    int arraySize = sizeof(*options) / sizeof(options[0]);
    printf("arraySize: %d\n", arraySize);
    for(int i = 0; i < 3; ++i){
        char printString[255];
        strcpy(printString, options[i].text);
        if(options[i].option == *currentOption){
            strcat(printString, " <");
        }
        printf("%s\n", printString);
    }
    printf("\033[H");
}

int main(int argc, char* argv[]){
    bool running = true;
    struct termios old;
    struct termios new;

    Option activeOption = OPTION_BEGIN;

    OptionText options[3] = {
        {"Begin", OPTION_BEGIN},
        {"Settings", OPTION_SETTINGS},
        {"Exit", OPTION_EXIT}
    };

    tcgetattr(STDIN_FILENO, &old);
    new = old;

    new.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new);

    char c;

    drawText(&activeOption, options);
    drawText(&activeOption, options);
    while(running){
        read(STDIN_FILENO, &c, 1);
        if(CheckInput(c, &activeOption)){
            printf("\033[2J");
            break;
        }
        drawText(&activeOption, options);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return 0;
}
