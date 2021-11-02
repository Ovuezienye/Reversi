/* Created by Ovuezienye Carissa Okoro
   Date: 08.05.2021
   Student Number: 20316933
   Email: ovuezienye.okoro@ucdconnect.ie
   The following is a C program that computes a game of othello/reversi */

#include    <stdio.h>
#include    <stdlib.h>
#include    "othello.h"

extern char* name1;// global variable for name1
extern char* name2;// global variable for name2

void startGame(void) { //main function
    //initialise
    int dataArr[SIZE * SIZE];
    int validArr[SIZE * SIZE];
    int color, game_state;
    //initialize the global variables
    name1 = (char*)malloc(100);
    name2 = (char*)malloc(100);
    game_state = 0;
    color = BLACK;

    printf("\t***Welcome to Othello!***\n");
    printf("Enter name of Player 1 (Black): ");
    if (scanf("%s", name1) != 1) {
        scanf("%*s");
    }

    printf("Enter name of Player 2 (White): ");
    if (scanf("%s", name2) != 1) {
        scanf("%*s");
    }
    //initialize the data
    initData(dataArr);
    showBoard(dataArr);

    while( game_state != 2 ){
        //update validation array
        updateValidArr(dataArr, validArr, color);
        if( checkPuttability(validArr)){
            goTurn(dataArr, validArr, color);
            toggleColor(&color);
            game_state = 0;
        }else{
            toggleColor(&color);
            game_state++;
        }
    }
    if( checkStoneNum(dataArr, BLACK) > checkStoneNum(dataArr, WHITE) ){
        printf("Black WIN\nBlack: %d\nWHITE: %d\n",
               checkStoneNum(dataArr,BLACK), checkStoneNum(dataArr,WHITE));
    }else{
        printf("WHITE WIN\nBlack: %d\nWHITE: %d\n",
               checkStoneNum(dataArr,BLACK), checkStoneNum(dataArr,WHITE));
    }
}

void drawLine() { //draw line "  --- --- --- ---"
    printf("   ");
    for (int x = 0; x < SIZE; x++) {
        putchar('-');
        putchar('-');
        putchar('-');
        putchar(' ');
    }
    printf("\n");
}

/* input: disc position information
output:non
function: draw all board with disc */

void showBoard(int *data)
{
    printf(" Score: %s", name1);
    printf("(Black) %d", checkStoneNum(data, BLACK));
    printf(":%d", checkStoneNum(data, WHITE));
    printf(" %s(White)", name2);
    putchar('\n');
    int x,y;
    for( y=0; y<SIZE; y++){
        drawLine();
        printf("%d | ", y+1);

        for( x=0; x<SIZE; x++){
            putSymbol( *retCell(data, x, y ));
            printf(" ");
        }
        putchar('\n');
    }
    drawLine();
}
/* input: cell information
output: none
function: draw a symbol according input value */

void putSymbol(int num)
{
    switch(num){
        case 0:
            putchar(' ');
            putchar(' ');
            putchar('|');
            break;
        case 1:
            putchar('B');
            putchar(' ');
            putchar('|');
            break;
        case 2:
            putchar('W');
            putchar(' ');
            putchar('|');
            break;
    }
}
/* input: every cell's information
 output: uddated cell's information
 function: initialize the board */

int *initData(int *data)
{
    int x,y,center;
    for( y=0; y<SIZE; y++){
        for( x=0; x<SIZE; x++){
            *retCell(data, x, y) = EMPTY;
        }
    }

    center = SIZE/2;
    *retCell(data, center, center) = BLACK;
    *retCell(data, center-1, center-1) = BLACK;
    *retCell(data, center, center-1) = WHITE;
    *retCell(data, center-1, center) = WHITE;

    return data;
}

/* data: data array
x: x-coordination 1~SIZE
y: y-coordination 1~SIZE */

int *retCell(int *data, int x, int y)
{
    return (data + ( (x)+(y)*SIZE));
}
/* input: none
output: valid x value
function: get the value from keyboard and check it's value. */

int inputX(void){
    int x;
    printf("input X: ");
    if(scanf("%d", &x)!=1)
        scanf("%*s");
    while( x < 0 || SIZE <= x ){
        printf("Please input a number 1~%d\n", SIZE);
        printf("input X: ");
        //scanf("%d", &x);
        if(scanf("%d", &x)!=1)
            scanf("%*s");
    }
    return x;
}
/* input: none
output: valid y value
function: get the value from keyboard and check it's value. */

int inputY(void){
    int y;
    printf("input Y: ");
    //scanf("%d", &y);
    if(scanf("%d", &y)!=1)
        scanf("%*s");
    while( y < 0 || SIZE <= y ){
        printf("Please input a number 1~%d\n", SIZE);
        printf("input Y: ");
        //scanf("%d", &y);
        if(scanf("%d", &y)!=1)
            scanf("%*s");
    }
    return y;
}

void showDirectionMap(int *map)
{
    int x,y;
    printf("  ");
    for( x=0; x<SIZE; x++){
        printf("%d", x);
    }
    printf("\n");

    for( y=0; y<SIZE; y++){
        printf("%d|", y);
        for( x=0; x<SIZE; x++){
            printf( "%d", *retCell(map, x, y ) );
        }
        putchar('\n');
    }
}

void showValidMap(int *map)
{
    int x,y;
    printf("  ");
    for( x=0; x<SIZE; x++){
        printf("%d ", x);
    }
    printf("\n  ");
    for( x=0; x<SIZE; x++){
        printf("_");
    }
    printf("\n");

    for( y=0; y<SIZE; y++){
        printf("%d|", y);
        for( x=0; x<SIZE; x++){
            printf( "%d ", *retCell(map, x, y ) != 0 );
        }
        putchar('\n');
    }
}

void updateValidArr(int *data, int *map, int color){
    int x,y;
    for( y=0; y<SIZE; y++){
        for( x=0; x<SIZE; x++){
            int ct, result, initState;
            int *nowCell;
            nowCell = retCell(data, x, y);
            result = 0;
            if( color == BLACK){
                initState = 0;
            }else if(color == WHITE){
                initState = 2;
            }

            if( *nowCell != EMPTY ){
                result = 0;
            }else{
                //validation on each direction
                for(ct = 1; ct != 0x100; ct <<= 1){
                    result |= checkValid(data, x, y, initState, ct);
                }
            }
            *retCell(map, x, y) = result;
        }
    }
}

int checkValid(int *data, int x, int y, int state, int direction){
    int vx, vy, dx,dy;
    vx = 0;
    vy = 0;

    switch(direction){
        case UC:
            vx = 0;
            vy = -1;
            break;
        case UR:
            vx = 1;
            vy = -1;
            break;
        case MR:
            vx = 1;
            vy = 0;
            break;
        case BR:
            vx = 1;
            vy = 1;
            break;
        case BC:
            vx = 0;
            vy = 1;
            break;
        case BL:
            vx = -1;
            vy = 1;
            break;
        case ML:
            vx = -1;
            vy = 0;
            break;
        case UL:
            vx = -1;
            vy = -1;
            break;
    }
    dx = x + vx;
    dy = y + vy;
    if( 0 <= dx && dx < SIZE && 0 <= dy && dy < SIZE ){

        switch(state){
            case 0:
                if(*retCell(data, dx, dy) == WHITE){
                    return checkValid(data, dx, dy, 1, direction);
                }
                break;
            case 1:
                if(*retCell(data, dx, dy) == WHITE){
                    return checkValid(data, dx, dy, 1, direction);
                }else if(*retCell(data, dx, dy) == BLACK){
                    return direction;
                }
                break;
            case 2:
                if(*retCell(data, dx, dy) == BLACK){
                    return checkValid(data, dx, dy, 3, direction);
                }
                break;
            case 3:
                if(*retCell(data, dx, dy) == BLACK){
                    return checkValid(data, dx, dy, 3, direction);
                }
                else if(*retCell(data, dx, dy) == WHITE){
                    return direction;
                }
                break;
        }
    }
    return 0;
}

void putStone(int *data, int *map, int x, int y, int color){
    int ct, initState;
    int nowCellValid;
    nowCellValid = *retCell(map, x, y);
    if( color == BLACK){
        initState = 0;
    }else if(color == WHITE){
        initState = 2;
    }

    if( nowCellValid ){
        for(ct = 1; ct != 0x100; ct <<= 1){
            if( ct & nowCellValid ){
                turnStone(data, x, y, initState, ct);
            }
        }
    }
}

int turnStone(int *data, int x, int y, int state, int direction){
    int vx, vy, dx,dy;
    vx = 0;
    vy = 0;
    switch(direction){
        case UC:
            vx = 0;
            vy = -1;
            break;
        case UR:
            vx = 1;
            vy = -1;
            break;
        case MR:
            vx = 1;
            vy = 0;
            break;
        case BR:
            vx = 1;
            vy = 1;
            break;
        case BC:
            vx = 0;
            vy = 1;
            break;
        case BL:
            vx = -1;
            vy = 1;
            break;
        case ML:
            vx = -1;
            vy = 0;
            break;
        case UL:
            vx = -1;
            vy = -1;
            break;
    }
    dx = x + vx;
    dy = y + vy;

    if( 0 <= dx && dx < SIZE && 0 <= dy && dy < SIZE ){

        switch(state){
            int nextStoneColor;
            case 0:
                if(*retCell(data, dx, dy) == WHITE){
                    *retCell(data, x, y) = BLACK;
                    *retCell(data, dx, dy) = BLACK;
                    return turnStone(data, dx, dy, 1, direction);
                }
                break;
            case 1:
                if(*retCell(data, dx, dy) == WHITE){
                    *retCell(data, dx, dy) = BLACK;
                    return turnStone(data, dx, dy, 1, direction);
                }else if(*retCell(data, dx, dy) == BLACK){
                    return direction;
                }
                break;
            case 2:
                if(*retCell(data, dx, dy) == BLACK){
                    *retCell(data, x, y) = WHITE;
                    *retCell(data, dx, dy) = WHITE;
                    return turnStone(data, dx, dy, 3, direction);
                }
                break;
            case 3:
                if(*retCell(data, dx, dy) == BLACK){
                    *retCell(data, dx, dy) = WHITE;
                    return turnStone(data, dx, dy, 3, direction);
                }
                else if(*retCell(data, dx, dy) == WHITE){
                    return direction;
                }
                break;
        }
    }
    return 0;
}

void goTurn(int *data, int *map, int color)
{
    int x,y;

    if( color == BLACK){
        printf("Black turn\n");
    }else{
        printf("White turn\n");
    }

    //input coordinate and validation
    x = inputX();
    y = inputY();
    while( *retCell(map, x, y) == 0 ){
        printf("You cannot place a stone on (%d, %d).\n", x, y);
        x = inputX();
        y = inputY();
    }
    putStone(data, map, x, y, color);
    printf("*********************************\n");
    showBoard(data);
}

// return
// 1: there is a place that a stone can be put at.
// 0: there is no place that a stone can be put at.

int checkPuttability(int *map){
    int x, y, result;
    result = 0;
    for( x = 0; x < SIZE; x++){
        for( y = 0; y < SIZE; y++){
            if(*retCell(map, x, y)) result = 1;
        }
    }
    return result;
}

void toggleColor(int *c){ // toggles WHITE and BLACK
    if( *c == BLACK ){
        *c = WHITE;
    }else{
        *c = BLACK;
    }
}

int checkStoneNum(int *data, int color){ // returns the number of stones of specified color
    int x, y, num;
    num = 0;
    for( x = 0; x < SIZE; x++){
        for( y = 0; y < SIZE; y++){
            if(*retCell(data, x, y) == color) num++;
        }
    }
    return num;
}

