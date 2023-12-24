#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

COORD coord;
int i, j, currIndex = 0;
char board[3][3] = {
	{' ', ' ', ' '},
	{' ', ' ', ' '},
	{' ', ' ', ' '}
};
char turn = 'X';
char aiTurn, humanTurn;
char firstPlayer;
int cursorX = 0, cursorY = 0;
float xScore = 0.0;
float oScore = 0.0;

typedef struct
{
	char name[50];
	float score;
}
PLAYERSTAT;

PLAYERSTAT players[100];

void gotoXY(int x, int y)
{
	coord.X = x;
	coord.Y = y;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, coord);
}

void blinkCursor(int mode)
{
	int x = cursorX * 6;
	int y = 3 * cursorY + 4;
	
	if (mode == 1)
	{
		gotoXY(x, y);
		printf(">");
		gotoXY(x + 4, y);
		printf("<");
	}
	else
	{
		gotoXY(x, y);
		printf(" ");
		gotoXY(x + 4, y);
		printf(" ");
	}
}

void cleanBoard()
{
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			board[i][j] = ' ';
		}
	}
}

void draw()
{	
	board[cursorY][cursorX] = turn;
	int x = 6 * cursorX + 2;
	int y = 3 * cursorY + 4;
	gotoXY(x, y);
	printf("%c", turn);
}

void inputName()
{
	gotoXY(0, 15);
	printf("Enter your name: ");
	scanf("%[^\n]", players[currIndex].name); getchar();
	if (turn == 'X')
	{
		players[currIndex].score = 20 / xScore;
	}
	else
	{
		players[currIndex].score = 20 / oScore;
	}
	currIndex++;
}

int legal(int x, int y)
{
	if (board[y][x] != ' ') return 0;
	return 1;
}

int isDraw()
{
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (board[i][j] == ' ')
			{
				return 0;
			}
		}
	}
	return 1;
}

int TerminalState()	//x menang 1	o menang -1
{	
	for (i = 0; i < 3; i++)
	{
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
		{
			if (board[i][0] == 'X')
			{
				return 1;
			}
			else if (board[i][0] == 'O')
			{
				return -1;
			}
		}
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
		{
			if (board[0][i] == 'X')
			{
				return 1;
			}
			else if (board[0][i] == 'O')
			{
				return -1;
			}
		}
	}
	
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
	{
		if (board[0][0] == 'X')
		{
			return 1;
		}
		else if (board[0][0] == 'O')
		{
			return -1;
		}
	}
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
	{
		if (board[0][2] == 'X')
		{
			return 1;
		}
		else if (board[0][2] == 'O')
		{
			return -1;
		}
	}
	return 0;
}

int minimax(int depth, int isMaximizing)
{
	int isOver = TerminalState();
	if (isOver) return isOver;
	if (isDraw()) return 0;
	
	if (isMaximizing)
	{
		int bestScore = -10;
		int i, j;
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				if (board[i][j] == ' ')
				{
					if (firstPlayer == '1') board[i][j] = humanTurn;
					else board[i][j] = aiTurn;
					int score = minimax(depth + 1, 0);
					board[i][j] = ' ';
					if (score > bestScore)
					{
						bestScore = score;
					}
				}
			}
		}
		return bestScore;
	}
	else
	{
		int bestScore = 10;
		int i, j;
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				if (board[i][j] == ' ')
				{
					if (firstPlayer == '1') board[i][j] = aiTurn;
					else board[i][j] = humanTurn;
					int score = minimax(depth + 1, 1);
					board[i][j] = ' ';
					if (score < bestScore)
					{
						bestScore = score;
					}
				}
			}
		}
		return bestScore;
	}
}

void bestMove(int isMax)
{
	if (isMax)
	{
		int bestScore = -10;
		int i, j;
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				if (board[i][j] == ' ')
				{
					board[i][j] = aiTurn;
					int score = minimax(0, 0);
					board[i][j] = ' ';
					if (score > bestScore)
					{
						bestScore = score;
						cursorY = i;
						cursorX = j;
					}
				}
			}
		}
	}
	else
	{
		int bestScore = 10;
		int i, j;
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				if (board[i][j] == ' ')
				{
					board[i][j] = aiTurn;
					int score = minimax(0, 1);
					board[i][j] = ' ';
					if (score < bestScore)
					{
						bestScore = score;
						cursorY = i;
						cursorX = j;
					}
				}
			}
		}
	}
	draw();
//	gotoXY(0, 16);
//	printf("%c\n", turn);
}

void Game(char mode)
{
	system("cls");
	puts("Press W, A, S, or D to move cursor");		//tutorial
	puts("Press space to select\n");
	printf("     |     |\n");
	printf("     |     |\n");
	printf("_____|_____|_____\n");
	printf("     |     |\n");
	printf("     |     |\n");
	printf("_____|_____|_____\n");
	printf("     |     |\n");
	printf("     |     |\n");
	printf("     |     |\n\n");
	printf("It's %c's turn", turn);
	
	int gameOver = 0, row, col;
	clock_t xTimer = clock();
	clock_t oTimer = clock();
	xScore = 0.0;
	oScore = 0.0;
	char input;
	
	blinkCursor(1);
	
	while (!gameOver && mode == '2')	//main loop (multiplayer)
	{
		if (_kbhit())
		{
			blinkCursor(0);
			input = tolower(getch());
			
			if (input == 'w' && cursorY > 0)
			{
				cursorY--;
			}
			else if (input == 's' && cursorY < 2)
			{
				cursorY++;
			}
			else if (input == 'a' && cursorX > 0)
			{
				cursorX--;
			}
			else if (input == 'd' && cursorX < 2)
			{
				cursorX++;
			}
			else if (input == ' ' && legal(cursorX, cursorY))
			{
				//place turn & check state
				draw();
				if (TerminalState())
				{
					gameOver = 1;
					gotoXY(0, 13);
					printf("The winner is %c !!!\n\n", turn);
					getch();
					cleanBoard();
					inputName();
				}
				else if (isDraw())
				{
					gameOver = 1;
					gotoXY(0, 13);
					printf("Its a draw !!!");
					getch();
					cleanBoard();
				}
				else
				{
					if (turn == 'X')
					{
						xScore += (float) (clock() - xTimer) / CLOCKS_PER_SEC;
						turn = 'O';
					}
					else
					{
						oScore += (float) (clock() - oTimer) / CLOCKS_PER_SEC;
						turn = 'X';
					}
					xTimer = clock();
					oTimer = clock();
					gotoXY(5, 13);
					printf("%c", turn);
				}
			}
			
			blinkCursor(1);

//			board[cursorY][cursorX] = input;
//			draw();
			
		}
//		gotoXY(0, 15);
//		for (i = 0; i < 3; i++)
//		{
//			for (j = 0; j < 3; j++)
//			{
//				printf("%c ", board[i][j]);
//			}
//			printf("\n");
//		}
		
//		double cursorBlink = (double) (clock() - prevBlinkTime) / CLOCKS_PER_SEC;
//		if (cursorBlink >= 0.5)	//cursor blink
//		{
//			blinkCursor(cursorState);
//			cursorState = !cursorState;
//			prevBlinkTime = clock();
//		}
	}
	
	while (!gameOver && mode == '1')	//main loop (singleplayer)
	{
		if (turn == aiTurn)
		{
			blinkCursor(0);
			
			if (aiTurn == 'X') bestMove(1);
			else bestMove(0);
			
			if (isDraw())
			{
				gameOver = 1;
				gotoXY(0, 13);
				printf("Its a draw !!!");
				getch();
				cleanBoard();
			}
			else if (TerminalState())
			{
				gameOver = 1;
				gotoXY(0, 13);
				printf("The winner is %c !!!\n\n", turn);
				getch();
				cleanBoard();
//				inputName();
			}
			
			turn = humanTurn;
			
			blinkCursor(1);
		}
		else
		{
			int isPlayerTurn = 1;
			while (isPlayerTurn)
			{
				if (_kbhit())
				{
					blinkCursor(0);
					input = tolower(getch());
					
					if (input == 'w' && cursorY > 0)
					{
						cursorY--;
					}
					else if (input == 's' && cursorY < 2)
					{
						cursorY++;
					}
					else if (input == 'a' && cursorX > 0)
					{
						cursorX--;
					}
					else if (input == 'd' && cursorX < 2)
					{
						cursorX++;
					}
					else if (input == ' ' && legal(cursorX, cursorY))
					{
						draw();
						if (TerminalState())
						{
							gameOver = 1;
							gotoXY(0, 13);
							printf("The winner is %c !!!\n\n", turn);
							getch();
							cleanBoard();
	//						inputName();
						}
						else if (isDraw())
						{
							gameOver = 1;
							gotoXY(0, 13);
							printf("Its a draw !!!");
							getch();
							cleanBoard();
						}
//						else
//						{
//							if (turn == 'X')
//							{
//								xScore += (float) (clock() - xTimer) / CLOCKS_PER_SEC;
//								turn = 'O';
//							}
//							else
//							{
//								oScore += (float) (clock() - oTimer) / CLOCKS_PER_SEC;
//								turn = 'X';
//							}
//							xTimer = clock();
//							oTimer = clock();
//							gotoXY(5, 13);
//							printf("%c", turn);
//						}
						isPlayerTurn = 0;
					}
					blinkCursor(1);
				}
			}
			turn = aiTurn;
		}
		gotoXY(5, 13);
		printf("%c", turn);
	}
	
	turn = 'X';
}

void sort()
{
	for (i = 0; i < currIndex - 1; i++)
	{
		for (j = 0; j < currIndex - i - 1; j++)
		{
			if (players[j].score < players[j + 1].score)
			{
				PLAYERSTAT temp = players[j];
				players[j] = players[j + 1];
				players[j + 1] = temp;
			}
		}
	}
}

void displayScore()
{
	system("cls");
	printf("Scoreboard:\n\n");
	for (i = 0; i < currIndex; i++)
	{
		printf("%d | %.2f %s\n", i + 1, players[i].score, players[i].name);
	}
	getch();
}

int search(char nama[])
{
	for (i = 0; i < currIndex; i++)
	{
		if (strcmp(players[i].name, nama) == 0)
		{
			return i;
		}
	}
	return -1;
}

void displaySearchScore()
{
	system("cls");
	char nama[50];
	printf("Search Score: ");
	scanf("%[^\n]", nama); getchar();
	printf("\n");
	int index = search(nama);
	if (index == -1)
	{
		printf("Name doesn't exist\n");
	}
	else
	{
		printf("%d | %.2f %s\n", i + 1, players[i].score, players[i].name);
	}
	getch();
}

void saveToFile()
{
	FILE *fp = fopen("scoreboard.txt", "w");
	for (i = 0; i < currIndex; i++)
	{
		fprintf(fp, "%f %s\n", players[i].score, players[i].name);
	}
	fclose(fp);
}

int main()
{
	//hide cursor
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &info);
	
	//input score from file
	FILE *fp = fopen("scoreboard.txt", "r");
	if (fp != NULL)
	{
		while (fscanf(fp, "%f %[^\n]", &players[currIndex].score, players[currIndex].name) != EOF)
		{
			currIndex++;
		}
	}
	fclose(fp);
	
	//menu
	do
	{
		puts(" _______ _____ _____   _______       _____   _______ ____  ______");
		puts("|__   __|_   _/ ____| |__   __|/\\   / ____| |__   __/ __ \\|  ____|");
		puts("   | |    | || |         | |  /  \\ | |         | | | |  | | |__   ");
		puts("   | |    | || |         | | / /\\ \\| |         | | | |  | |  __|  ");
		puts("   | |   _| || |____     | |/ ____ \\ |____     | | | |__| | |____ ");
		puts("   |_|  |_____\\_____|    |_/_/    \\_\\_____|    |_|  \\____/|______|\n");
		
		char mode;
		static char msg[20] = "";
		puts("Choose Gamemode");
		puts("1. 1P");
		puts("2. 2P");
		puts("3. Scoreboard");
		puts("4. Search Score");
		puts("5. Exit");
		printf("%s", msg);
		mode = getch();
		switch (mode)
		{
			case '1':
				strcpy(msg, "");
				do
				{
					system("cls");
					puts("Select turn (X plays first)");
					puts("1. Play as X");
					puts("2. Play as O");
					firstPlayer = getch();
					if (firstPlayer == '1')
					{
						aiTurn = 'O';
						humanTurn = 'X';
						break;
					}
					else if (firstPlayer == '2')
					{
						aiTurn = 'X';
						humanTurn = 'O';
						break;
					}
				}
				while (1);
				Game(mode);
				//SP
				break;
			case '2':
				strcpy(msg, "");
				//MP
				Game(mode);
				break;
			case '3':
				sort();
				displayScore();
				break;
			case '4':
				sort();
				displaySearchScore();
				break;
			case '5':
				//exit;
				saveToFile();
				exit(0);
			default:
				strcpy(msg, "\nInvalid Gamemode!\n");
		}
		system("cls");
	} while (1);
	
	return 0;
}
