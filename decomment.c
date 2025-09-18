#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
enum Statetype {START, BACKSLASH, STRING_QUOTE_ONE, CHAR_QUOTE_ONE, STAR_ONE, 
STRING_QUOTE_AFTER_STAR_ONE, CHAR_QUOTE_AFTER_STAR_ONE, STAR_TWO, ESCAPED_STRING_ONE,
ESCAPED_CHAR_ONE, ESCAPED_STRING_TWO, ESCAPED_CHAR_TWO};

static int lineNumber = 1;
enum Statetype handleStartState(char c){
    enum Statetype state;
    if(c == '/'){
        state = BACKSLASH;
    }
    else if (c == '"'){
        state = STRING_QUOTE_ONE;
        putchar(c);
    }
    else if (c == '\''){
        state = CHAR_QUOTE_ONE;
        putchar(c);
    }
    
    else{
        if (c == '\n'){
            ++lineNumber;
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
    else if (c == '/'){
        state = BACKSLASH;
        putchar(c);
    }
    else if (c == '"'){
        state = STRING_QUOTE_ONE;
        putchar('/');
        putchar(c);
    }
    else if (c == '\''){
        state = CHAR_QUOTE_ONE;
        putchar('/');
        putchar(c);
    }
    else{
        if (c == '\n'){
            ++lineNumber;
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
    else if(c == '\\'){
        state = ESCAPED_STRING_ONE;
    }
    else{
        if (c == '\n'){
            ++lineNumber;
        }
        state = STRING_QUOTE_ONE;
    }
    putchar(c);
    return state;
}

enum Statetype handleEscapedStringOne(char c) {
    enum Statetype state;
    putchar(c);
    state = STRING_QUOTE_ONE;
    return state;            
}

enum Statetype handleCharQuoteOneState(char c){
    enum Statetype state;
    if (c == '\''){
        state = START;
    }
    else if(c == '\\'){
        state = ESCAPED_CHAR_ONE;
    }
    else{
        if (c == '\n'){
            ++lineNumber;
        }
        state = CHAR_QUOTE_ONE;
    }
    putchar(c);
    return state;
}


enum Statetype handleEscapedCharOne(char c) {
    enum Statetype state;
    putchar(c);
    state = CHAR_QUOTE_ONE;
    return state;            
}


enum Statetype handleStarOneState(char c){
    enum Statetype state;
    if(c == '*'){
        state = STAR_TWO;
    }
    else if (c == '"'){
        state = STRING_QUOTE_AFTER_STAR_ONE;
    }
    else if (c == '\''){
        state = CHAR_QUOTE_AFTER_STAR_ONE;
    }
    else{
        if (c == '\n'){
            ++lineNumber;
            putchar('\n');
        }
        state = STAR_ONE;
    }
    return state;
}

enum Statetype handleStringQuoteAfterStarOneState(char c){
    enum Statetype state;
    if (c == '"'){
        state = STAR_ONE;
    }
    else if(c == '*'){
        state = STAR_TWO;
    }
    else if(c == '\\'){
        state = ESCAPED_STRING_TWO;
    }
    else{
        state = STRING_QUOTE_AFTER_STAR_ONE;
    }
    return state;
}

enum Statetype handleEscapedStringTwo(char c) {
    enum Statetype state;
    putchar(c);
    state = STRING_QUOTE_AFTER_STAR_ONE;
    return state;            
}

enum Statetype handleCharQuoteAfterStarOneState(char c){
    enum Statetype state;
    if (c == '\''){
        state = STAR_ONE;
    }
    else if(c == '\\'){
        state = ESCAPED_CHAR_ONE;
    }
    else if(c == '*'){
        state = STAR_TWO;
    }
    else{
        state = CHAR_QUOTE_AFTER_STAR_ONE;
    }
    return state;
}

enum Statetype handleEscapedCharTwo(char c) {
    enum Statetype state;
    putchar(c);
    state = CHAR_QUOTE_AFTER_STAR_ONE;
    return state;            
}


enum Statetype handleStarTwoState(char c){
    enum Statetype state;
    if(c == '/'){
        state = START;
    }
    else if (c == '"'){
        state = STRING_QUOTE_AFTER_STAR_ONE;
    }
    else if (c == '\''){
        state = CHAR_QUOTE_AFTER_STAR_ONE;
    }
    else if (c == '*'){
        state = STAR_TWO;
    }
    else{
        state = STAR_ONE;
    }
    if(c == '\n'){
        putchar('\n');
        ++lineNumber;
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
            case STRING_QUOTE_ONE:
                state = handleStringQuoteOneState(c);
                break;
            case CHAR_QUOTE_ONE:
                state = handleCharQuoteOneState(c);
                break;
            case ESCAPED_STRING_ONE:
                state = handleEscapedStringOne(c);
                break;
            case ESCAPED_CHAR_ONE:
                state = handleEscapedCharOne(c);
                break;
            case STAR_ONE:
                state = handleStarOneState(c);
                break;
            case STRING_QUOTE_AFTER_STAR_ONE:
                state = handleStringQuoteAfterStarOneState(c);
                break;
            case CHAR_QUOTE_AFTER_STAR_ONE:
                state = handleCharQuoteAfterStarOneState(c);
                break;
            case ESCAPED_STRING_TWO:
                state = handleEscapedStringTwo(c);
                break;
            case ESCAPED_CHAR_TWO:
                state = handleEscapedCharTwo(c);
                break;
            case STAR_TWO:
                state = handleStarTwoState(c);
                break;

        }
    }

    if(state == START || state == BACKSLASH 
    || state == STRING_QUOTE_ONE || state == CHAR_QUOTE_ONE 
    || state == ESCAPED_CHAR_ONE || state == ESCAPED_STRING_ONE){
        if(state == BACKSLASH){
            putchar('/');
        }
        return EXIT_SUCCESS;
    }
    else{
        fprintf(stderr, "Error: line %i: unterminated comment\n", lineNumber);
        return EXIT_FAILURE;
    }
}
