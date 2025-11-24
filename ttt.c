#include <stdio.h>
#include <stdbool.h>
#include <string.h>

static const char g_symbolX = 'x';
static const char g_symbolO = 'o';
static const char g_empty = ' ';

typedef struct 
{ 
    int row;
    int column;
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

bool checkBounds( int row, int column )
{ 
    return (row >= 0 && row <=2) && (column >= 0 && column <=2);
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

Place findBestMove(char t[3][3], bool isMax)
{
    int bestScore = isMax? -100000 : 100000;
    Place winningPlace = { .row = -1, .column = -1 };
    int i = 0;
    for(; i < 3; ++i)
    {
        int j = 0;
        for(; j < 3; ++j)
        { 
            if(t[i][j] == g_empty)
            {
                t[i][j] = isMax? g_symbolX : g_symbolO;
                int newScore = miniMax( t, 0, !isMax );
                t[i][j] = g_empty;

                if(isMax)
                { 
                    if( newScore > bestScore )
                    {
                        bestScore = newScore;
                        winningPlace.row = i;
                        winningPlace.column = j;
                    }
                }
                else
                { 
                    if( newScore < bestScore )
                    {
                        bestScore = newScore;
                        winningPlace.row = i;
                        winningPlace.column = j;
                    }
                }
            }
        }
    }
    return winningPlace;
}

void computerMove( char t[3][3], bool isMax )
{ 
    Place selected = findBestMove(t, isMax);
    if( selected.row == -1 && selected.column == -1 )
    {
        return;
    }

    t[selected.row][selected.column] = isMax? g_symbolX : g_symbolO;
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
        printf("Enter column: ");
        int column = 0;
        scanf("%d", &column);

        if(!checkBounds(row, column))
        {
            printf("\nOut of bounds, I am sorry try again: \n");
            continue;
        }

        if( ttt[row][column] != g_empty )
        {
            printf("\nalready used I am afraid try again: \n");
            continue;
        }

        ttt[row][column] = (isTurnX)? 'x' : 'o';
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
        printf("Enter column: ");
        int column = 0;
        scanf("%d", &column);

        if( ttt[row][column] != g_empty )
        {
            printf("\nalready used I am afraid try again: \n");
            continue;
        }

        if(!checkBounds(row, column))
        {
            printf("\n Out of bounds, I am sorry try again: \n");
        }

        ttt[row][column] = 'o';
        computerMove(ttt, true);
        printBoard(ttt);

        score = evaluate(ttt);
        if( score != 0 )
        {
            break;
        }
    }
    check(score);
}

void botVbot()
{
    bool isMax = true;
    char board[3][3] = {
        { g_empty, g_empty, g_empty },
        { g_empty, g_empty, g_empty },
        { g_empty, g_empty, g_empty }
    };
    while(isTurnLeft(board) && evaluate(board) == 0)
    {
        printf("%c's turn\n", isMax? 'x' : 'o');
        computerMove(board, isMax);
        isMax = !isMax;
        printBoard(board);
    }
    printf("Results\n");
    if( evaluate(board) == 0)
        printf("Its a tie\n");
    else if( evaluate(board) == 10 )
        printf("X won\n");
    else
        printf("O won\n");
    printBoard(board);
}


int main(int argc, char* argv[])
{ 
    if( argc != 2 )
    { 
        printf("Usage: %s [ -multi | -single | -bvb ]\n", argv[0]);
        printf(" -multi for multiplayer \n");
        printf(" -single for singleplayer \n");
        printf(" -bvb for bot vs bot \n");
        return 1;
    }
    else if(strcmp(argv[1], "-single") == 0)
    {
        printf("Singleplayer: \n");
        singlePlayer();
    }
    else if(strcmp(argv[1], "-multi") == 0 )
    {
        printf("Multiplayer: \n");
        multiplayer();
    }
    else if(strcmp(argv[1], "-bvb") == 0 )
    {
        printf("Bot Vs Bot:\n");
        botVbot();
    }
    else 
    { 
        printf("Error: The user is retarded\n");
    }

    return 0;
}
