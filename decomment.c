#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
enum Statetype {START, BACKSLASH, STRING_QOUTE_ONE, CHAR_QOUTE_ONE, STAR_ONE, 
STRING_QOUTE_AFTER_STAR_ONE, CHAR_QOUTE_AFTER_STAR_ONE, STAR_TWO};

static int lineNumber;
enum Statetype handleStartState(char c){
    enum Statetype state;
    if(c == '/'){
        state = BACKSLASH;
    }
    else if (c == '"'){
        state = STRING_QOUTE_ONE;
        putchar(c);
    }
    else if (c == '\''){
        state = CHAR_QOUTE_ONE;
        putchar(c);
    }
    
    else{
        if (c == '\n'){
            lineNumber +=1;
        }
        state = START;
        putchar(c);
    }
    
    return state;
}

enum Statetype handleBackslashState(char c){
    enum Statetype state;
    if(c == '*'){
        state = STAR_ONE;
        putchar(' ');
    }
    else if (c == '"'){
        state = STRING_QOUTE_ONE;
        putchar('/');
        putchar(c);
    }
    else if (c == '\''){
        state = CHAR_QOUTE_ONE;
        putchar('/');
        putchar(c);
    }
    else{
        if (c == '\n'){
            lineNumber +=1;
        }
        state = START;
        putchar('/');
        putchar(c);
    }
    return state;
}

enum Statetype handleStringQuoteOneState(char c){
    enum Statetype state;
    if (c == '"'){
        state = START;
    }
    else{
        if (c == '\n'){
            lineNumber +=1;
        }
        state = STRING_QOUTE_ONE;
    }
    putchar(c);
    return state;
}

enum Statetype handleCharQuoteOneState(char c){
    enum Statetype state;
    if (c == '\''){
        state = START;
    }
    else{
        if (c == '\n'){
            lineNumber +=1;
        }
        state = CHAR_QOUTE_ONE;
    }
    putchar(c);
    return state;
}

enum Statetype handleStarOneState(char c){
    enum Statetype state;
    if(c == '*'){
        state = STAR_TWO;
    }
    else if (c == '\n'){
        putChar("\n");
    }
    else if (c == '"'){
        state = STRING_QOUTE_AFTER_STAR_ONE;
    }
    else if (c == '\''){
        state = CHAR_QOUTE_AFTER_STAR_ONE;
    }
    else{
        state = STAR_ONE;
    }
    return state;
}

enum Statetype handleStringQuoteAfterStarOneState(char c){
    enum Statetype state;
    if (c == '"'){
        state = STAR_ONE;
    }
    else{
        state = STRING_QOUTE_AFTER_STAR_ONE;
    }
    return state;
}

enum Statetype handleCharQuoteAfterStarOneState(char c){
    enum Statetype state;
    if (c == '\''){
        state = STAR_ONE;
    }
    else{
        state = STRING_QOUTE_AFTER_STAR_ONE;
    }
    return state;
}

enum Statetype STAR_TWO(char c){
    enum Statetype state;
    if(c == '/'){
        state = START;
    }
    else if (c == '"'){
        state = STRING_QOUTE_AFTER_STAR_ONE;
    }
    else if (c == '\''){
        state = CHAR_QOUTE_AFTER_STAR_ONE;
    }
    else{
        state = STAR_ONE;
    }
    return state;
}


int main(void)
{
    int c;
    enum Statetype state = START;
    while((c=getchar()) != EOF){
        switch (state) {
            case START:
                state = handleStartState(c);
                break;
            case BACKSLASH:
                state = handleBackslashState(c);
                break;
            }
    }

    if(state == handleStartState || state == handleBackslashState 
    || state == handleStringQuoteOneState || state == handleCharQuoteOneState){
        return EXIT_SUCCESS;
    }
    else{
        printf("Error: line X: unterminated comment/n");
        return EXIT_FAILURE;
    }
}