#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* defines constants representing each state in the DFA */
enum Statetype {START, BACKSLASH, STRING_QUOTE_ONE, CHAR_QUOTE_ONE, STAR_ONE, 
STRING_QUOTE_AFTER_STAR_ONE, CHAR_QUOTE_AFTER_STAR_ONE, STAR_TWO, ESCAPED_STRING_ONE,
ESCAPED_CHAR_ONE, ESCAPED_STRING_TWO, ESCAPED_CHAR_TWO};

/* counts the number of lines outside of an unterminated comment */
static int lineNumber = 1; 

/* counts the number of lines inside of a terminated comment */
static int lineNumberInComment = 0; 

/*
handles the start state function. c is the current character of the DFA.
writes c to stdout when appropriate, and moves to other states when necessary.
*/
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

/*
handles the state function for after a backslash is read. c is the current character
of the DFA. writes c to stdout when appropriate, and moves to other states when necessary.
*/
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

/*
implements the string literal state for before we are a state of within the comment
connected to start and backslash states.
*/
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

/*
handles escaped characters within strings when we are not in an in 
comment state.
*/
enum Statetype handleEscapedStringOne(char c) {
    enum Statetype state;
    putchar(c);
    state = STRING_QUOTE_ONE;
    return state;            
}

/*
implements the char literal state for before we are a state of 
within the comment connected to start and backslash states.
*/
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

/*
handles escaped characters within character literals when we are 
not in an in comment state.
*/
enum Statetype handleEscapedCharOne(char c) {
    enum Statetype state;
    putchar(c);
    state = CHAR_QUOTE_ONE;
    return state;            
}

/*
implements state after a star is read in (right after a backslash),
in otherwords, implements the state of within the comment. lineNumberInComment
begins counting within here.
*/
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
            putchar('\n');
            ++lineNumberInComment;
        }
        state = STAR_ONE;
    }
    return state;
}

/*
implements the string literal state for when we are the state within the comment.
*/
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

/*
handles escaped characters within string literals when we are 
in an in comment state. Handles the next character after the
backslash by treating it ordinarily and writing it to stdout.
*/
enum Statetype handleEscapedStringTwo(char c) {
    enum Statetype state;
    putchar(c);
    state = STRING_QUOTE_AFTER_STAR_ONE;
    return state;            
}

/*
implements the char literal state for when we are the state within the comment.
*/
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

/*
handles escaped characters within char literals when we are 
in an in comment state. Handles the next character after the
backslash by treating it ordinarily and writing it to stdout.
*/
enum Statetype handleEscapedCharTwo(char c) {
    enum Statetype state;
    putchar(c);
    state = CHAR_QUOTE_AFTER_STAR_ONE;
    return state;            
}

/*
implements the state of when we reach a second star in the code.
if a backslash is detected right after, it's sent to the start state,
otherwise we are sent to the state deemed by the rest of the DFA
*/
enum Statetype handleStarTwoState(char c){
    enum Statetype state;
    if(c == '/'){
        lineNumber = lineNumber + lineNumberInComment;
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
        ++lineNumberInComment;
    }
    return state;
}


/*
reads characters one by one from stdin until the end of the file 
is reached. starts at Start state then switches based on the DFA.
Failure error is outputted at the presence of unterminated comments.
*/

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

    /* first if statement includes all accepted states*/
    if(state == START || state == BACKSLASH 
    || state == STRING_QUOTE_ONE || state == CHAR_QUOTE_ONE 
    || state == ESCAPED_CHAR_ONE || state == ESCAPED_STRING_ONE){
        if(state == BACKSLASH){
            putchar('/'); /* handles the edge case of ending on a backslash in an accepted state */
        }
        return EXIT_SUCCESS;
    }
    else{
        fprintf(stderr, "Error: line %i: unterminated comment\n", lineNumber);
        return EXIT_FAILURE;
    }
}
