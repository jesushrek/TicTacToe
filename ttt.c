#include <stdio.h>
#include <stdbool.h>

static const char g_symbolX = 'x';
static const char g_symbolO = 'o';
static const char g_empty = ' ';

typedef struct 
{ 
    int row;
    int colomn;
}Place;


void check(int score)
{ 
    if( score == 0)
        printf("\nIts a tie");
    else if( score == -10)
        printf("\no won the game");
    else
        printf("\nx won the game");
}

/*
 * 00 01 02
 * 10 11 12
 * 20 21 22 
 */

static int evaluate( char t[3][3] )
{
    int i = 0;
    for(; i < 3; ++i)
    {
        if( t[i][0] != g_empty && t[i][0] == t[i][1] && t[i][1] == t[i][2] )
            return (t[i][0] == g_symbolX)? +10 : -10;
        if( t[0][i] != g_empty && t[0][i] == t[1][i] && t[1][i] == t[2][i] )
            return (t[0][i] == g_symbolX)? +10 : -10;
    }
    if( t[0][0] != g_empty && t[1][1] == t[0][0] && t[1][1] == t[2][2] )
        return (t[0][0] == g_symbolX)? +10 : -10;
    if( t[0][2] != g_empty && t[0][2] == t[1][1] && t[1][1] == t[2][0] )
        return (t[0][2] == g_symbolX)? +10 : -10;

    return 0;
}

static void populate( char t[3][3] )
{ 
    int i = 0;
    for(; i < 3; ++i)
    {
        int j = 0;
        for(; j < 3; ++j)
        {
            t[i][j] = g_empty;
        }
    }
}

static const bool isTurnLeft( char t[3][3] )
{
    int i = 0;
    for(; i < 3; ++i)
    {
        int j = 0;
        for(; j < 3; ++j)
            if(t[i][j] == g_empty)
                return true;
    }
    return false;
}

static void printBoard( char t[3][3] )
{
    int i = 0;
    for(; i < 3; ++i)
    {
        int j = 0;
        for(; j < 3; ++j)
        {
            printf("[%c]", t[i][j]);
        }
        printf("\n");
    }
}

static const int miniMax( char t[3][3], int depth, bool isMax )
{
    int score = evaluate(t);
    if(score != 0)
        return score;

    if(!isTurnLeft(t))
        return 0;

    if(isMax)
    {
        int value = -1000;
        int i = 0;
        for(; i < 3; ++i)
        {
            int j = 0;
            for(; j < 3; ++j)
            {
                if( t[i][j] == g_empty )
                {
                    t[i][j] = g_symbolX;
                    int newValue = miniMax(t, depth - 1, false);
                    value = (value > newValue)? value : newValue;
                    t[i][j] = g_empty;
                }
            }
        }
        return value;
    }

    else 
    { 
        int value = +1000;
        int i = 0;
        for(; i < 3; ++i)
        {
            int j = 0;
            for(; j < 3; ++j)
            {
                if( t[i][j] == g_empty )
                {
                    t[i][j] = g_symbolO;
                    int newValue = miniMax(t, depth - 1, true);
                    value = (value > newValue)? newValue : value;
                    t[i][j] = g_empty;
                }
            }
        }
        return value;
    }
}

Place findBestMove(char t[3][3])
{
    int bestScore = -1000;
    Place winningPlace = { .row = -1, .colomn = -1 };
    int i = 0;
    for(; i < 3; ++i)
    {
        int j = 0;
        for(; j < 3; ++j)
        { 
            if(t[i][j] == g_empty)
            {
                t[i][j] = g_symbolX;
                int newScore = miniMax( t, 0, false );
                t[i][j] = g_empty;

                if( newScore > bestScore )
                {
                    bestScore = newScore;
                    winningPlace.row = i;
                    winningPlace.colomn = j;
                }
            }
        }
    }
    return winningPlace;
}

void computerMove( char t[3][3] )
{ 
    Place selected = findBestMove(t);
    if( selected.row == -1 && selected.colomn == -1 )
    {
        return;
    }

    t[selected.row][selected.colomn] = g_symbolX;
}

static void multiplayer()
{
    char ttt[3][3];
    populate(ttt);
    int score = 0;
    bool isTurnX = true;

    while(isTurnLeft(ttt))
    {
        printf("its %c's turn\n", isTurnX? 'x' : 'o');

        printf("Enter row: ");
        int row = 0;
        scanf("%d", &row);
        printf("Enter colomn: ");
        int colomn = 0;
        scanf("%d", &colomn);

        if( ttt[row][colomn] != g_empty )
        {
            printf("\nalready used I am afraid try again: \n");
            continue;
        }

        ttt[row][colomn] = (isTurnX)? 'x' : 'o';
        printBoard(ttt);

        score = evaluate(ttt);
        if( score != 0 )
        {
            break;
        }

        isTurnX = !isTurnX;

    }
    check(score);
}

int singlePlayer()
{
    char ttt[3][3];
    populate(ttt);
    int score = 0;

    while(isTurnLeft(ttt))
    {
        printf("Enter row: ");
        int row = 0;
        scanf("%d", &row);
        printf("Enter colomn: ");
        int colomn = 0;
        scanf("%d", &colomn);

        if( ttt[row][colomn] != g_empty )
        {
            printf("\nalready used I am afraid try again: \n");
            continue;
        }

        ttt[row][colomn] = 'o';
        computerMove(ttt);
        printBoard(ttt);

        score = evaluate(ttt);
        if( score != 0 )
        {
            break;
        }
    }
    check(score);
}

int main()
{ 
    printf(" 1 to play multiplayer and 2 to play single: ");
    int choice = 0;
    scanf("%d", &choice);
    switch(choice)
    { 
        case 1: 
            multiplayer();
            break;
        case 2: 
            singlePlayer();
            break;
    }
}
