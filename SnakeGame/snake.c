#include "snake.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define HEIGHT 30
#define WIDTH 60
#define SPEED 75

#define BODY 'o'
#define WALL 178
#define FOOD '@'

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

static int lives = 3;
static int points = 0;

struct coordinate
{
    int x;
    int y;
    int direction;
    char c;
};
typedef struct coordinate Coord;
//The snake
Coord snake[100], food;
static int lastIndex;

static char* start;

void Menu()
{
    printf("Welcome to snake.\n");
    printf("Control your snake using the arrow keys!\n");
    printf("Don't hit the wall and eat as much as you can!\n");
    printf("Press ENTER to start!");
    getchar();

    time_t t;
    time(&t);
    start = ctime(&t);
    system("cls");
}
void StartGame()
{
    DisplayLives();
    DisplayPoints();
    PrintMap();
    CreateSnake();
    SpawnFood();
    Run();
}
void DisplayPoints()
{
    GotoXY(WIDTH+5,1);
    printf("Points: %i", points);
}
void DisplayLives()
{
    GotoXY(WIDTH+5,0);
    printf("Remaining lives: %i", lives);
}
void PrintMap()
{
    for (int x = 0; x < WIDTH; x++) {
        GotoXY(x,0);
        printf("%c",WALL);
        GotoXY(x,HEIGHT-1);
        printf("%c",WALL);
    }
    for (int y = 0; y < HEIGHT; y++) {
        GotoXY(0,y);
        printf("%c",WALL);
        GotoXY(WIDTH-1, y);
        printf("%c",WALL);
    }
}
void GotoXY(int x, int y)
{
    HANDLE csb;
    COORD c;
    fflush(stdout);
    c.X = x;
    c.Y = y;
    csb = GetStdHandle(STD_OUTPUT_HANDLE);  //"pointer" to Console Screen Buffer
    SetConsoleCursorPosition(csb,c);
}
char CharAt(int x, int y)
{
    HANDLE csb;
    csb = GetStdHandle(STD_OUTPUT_HANDLE);

    char read;
    
    LPTSTR l; // <= typedef-ing cancer
    
    COORD where;
    where.X = x;
    where.Y = y;

    DWORD actuallyRead;
    
    ReadConsoleOutputCharacter(csb, &read, 1, where, &actuallyRead);

    return read;
}
void CreateSnake()
{   //create snake in the middle of the map facing right
    int startL = 4;
    int x = WIDTH/2;
    int y = HEIGHT/2;
    snake[0].x = x;
    snake[0].y = y;
    snake[0].direction = RIGHT;
    snake[0].c = '>';
    for (int i = 1; i < startL; ++i) {
        snake[i].x = x-i;
        snake[i].y = y;
        snake[i].c = BODY;
    }
    for (int j = 0; j < startL; ++j) {
        GotoXY(snake[j].x, snake[j].y);
        printf("%c",snake[j].c);
    }
    lastIndex = startL-1;
}
void SpawnFood()
{
    int x = 1 + rand() % (WIDTH-2);
    int y = 1 + rand() % (HEIGHT-2);
    char c = CharAt(x,y);
    while(c == BODY || c == '^' || c == 'v' || c == '<' || c == '>')
    {
        x = 1 + rand() % WIDTH-2;
        y = 1 + rand() % HEIGHT-2;
    }
    food.x = x;
    food.y = y;
    GotoXY(x,y);
    printf("%c",FOOD);
}
void Run()
{
    char a;
    char key;
    do
    {
        if(lives == 0) break;
        Move();
        if(snake[0].direction == LEFT || snake[0].direction == RIGHT)
            _sleep(SPEED);
        else
            _sleep(SPEED*1.5);
    }
    while(!kbhit());

    a=getch();
    if(a==27)
    {
        exit(0);
    }
    key=getch();

    if(key == LEFT && snake[0].direction != RIGHT)
    {
        snake[0].direction = LEFT;
        snake[0].c = '<';
    }
    else if(key == RIGHT && snake[0].direction != LEFT)
    {
        snake[0].direction = RIGHT;
        snake[0].c = '>';
    }
    else if(key == UP && snake[0].direction != DOWN)
    {
        snake[0].direction = UP;
        snake[0].c = '^';
    }
    else if(key == DOWN && snake[0].direction != UP)
    {
        snake[0].direction = DOWN;
        snake[0].c = 'V';
    }
    Run();
}
void Move()
{
    Coord next;
    next.x = snake[0].x;
    next.y = snake[0].y;
    switch(snake[0].direction)
    {
        case LEFT:
            next.x -= 1;
            break;
        case RIGHT:
            next.x += 1;
            break;
        case UP:
            next.y -= 1;
            break;
        case DOWN:
            next.y += 1;
            break;
    }
    char nextChar = CharAt(next.x, next.y);
    if(nextChar == (char)WALL || nextChar == (char)BODY)    //perish
    {
        RespawnOrEndGame();
    }
    else if(nextChar == '@')    //consume and move
    {
        points++;
        DisplayPoints();
        lastIndex++;
        for (int j = lastIndex; j > 0; j--)  //replace snake[j] coords with snake[j-1] coords
        {
            snake[j].x = snake[j-1].x;
            snake[j].y = snake[j-1].y;
        }
        //snake[1].c is still the head character on the console
        GotoXY(snake[1].x, snake[1].y);
        printf("%c",BODY);
        //Move head char to next
        snake[0].x = next.x;
        snake[0].y = next.y;
        //Write head character on console
        GotoXY(snake[0].x, snake[0].y);
        printf("%c", snake[0].c);
        //Respawn food
        SpawnFood();
    }
    else     //just move
    {   //ooo>
        //Reset character at tail position
        GotoXY(snake[lastIndex].x, snake[lastIndex].y);
        printf(" ");
        for (int j = lastIndex; j > 0; j--)  //replace snake[j] coords with snake[j-1] coords
        {
            snake[j].x = snake[j-1].x;
            snake[j].y = snake[j-1].y;
        }
        //snake[1].c is still the head character on the console
        GotoXY(snake[1].x, snake[1].y);
        printf("%c", BODY);
        //Move head char to next
        snake[0].x = next.x;
        snake[0].y = next.y;
        //Write head character on console
        GotoXY(snake[0].x, snake[0].y);
        printf("%c", snake[0].c);

    }
}
void RespawnOrEndGame()
{
    lives--;
    DisplayLives();
    int x = WIDTH/2;
    int y = HEIGHT/2;
    if(lives > 0)
    {
        //Delete snake
        for (int i = 0; i <= lastIndex; ++i) {
            GotoXY(snake[i].x, snake[i].y);
            printf(" ");
        }
        //Delete food
        GotoXY(food.x,food.y);
        printf(" ");
        //Respawn text
        GotoXY(x, y);
        printf("Respawning in 3");
        _sleep(1000);
        GotoXY(x+14,y);
        printf("2");
        _sleep(1000);
        GotoXY(x+14,y);
        printf("1");
        _sleep(1000);

        GotoXY(x,y);
        for (int j = 0; j < 15; ++j) {
            printf(" ");
        }
        CreateSnake();
        SpawnFood();
    }
    else
    {
        system("cls");
        GotoXY(x,y);
        printf("GAME OVER");
        CreateStatisticsFile();
    }
}
void CreateStatisticsFile()
{
    FILE *file;
    char *name = "statistics.txt";
    file = fopen(name,"w");
    fprintf(file, "Game started at: %s",start);
    fprintf(file, "Points earned: %i\n", points);
    time_t t;
    time(&t);
    fprintf(file, "Game ended at: %s",ctime(&t));
    fclose(file);
}
