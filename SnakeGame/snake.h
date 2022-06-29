#ifndef SNAKEGAME_SNAKE_H
#define SNAKEGAME_SNAKE_H

static int lives;
struct coordinate;

void Menu();
void StartGame();
void DisplayPoints();
void DisplayLives();
void PrintMap();
void GotoXY(int x, int y);
char CharAt(int x, int y);
void CreateSnake();
void SpawnFood();
void Run();
void Move();
void RespawnOrEndGame();
void CreateStatisticsFile();
#endif //SNAKEGAME_SNAKE_H
