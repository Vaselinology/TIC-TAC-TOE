#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
//Constantes
#define PLAYER 'X'
#define PLAYER2 'O'
#define COMPUTER 'O'
#define MAX 50

//Player structure
typedef struct
{
    char username[MAX];
    int wins;
    int totalWins;
} Player;
//variables
char board[3][3];
char username1[MAX], username2[MAX];
COORD coord = {0, 0} ;
//Used functions
void displayMenu();
void playGame(char currentPlayer);
bool playAgainstComputer(char currentPlayer);
bool playAgainstComputer2(char currentPlayer);
void playerMove(char currentPlayer);
void playerMoveX();
void playerMoveO();
void computerMoveX();
void computerMoveO();
void playAgainstFriend();
void helpMenu();
void displayBoard();
void resetBoard();
void saveUsername(const char *username);
bool check(const char *username);
int freespaces();
char checkWinner();
void printWinner(char winner, char currentPlayer);
void printWinnerO(char winner, char currentPlayer);
void viewHighestScore();


//function used to enable cursor movement within the console
void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//function to display the main menu
void displayMenu()
{
    gotoxy(47,5);
    printf("Welcome to a Game of Tic Tac Toe! \n");
    printf("------------------------------------------------------------------------------------------------------------------------\n\n");
    gotoxy(55,8);
    printf("S. Start\n\n");
    gotoxy(55,10);
    printf("H. Help menu\n\n");
    gotoxy(55,12);
    printf("V. View highest score\n\n");
    gotoxy(55,14);
    printf("ESC. Quit\n\n");
}

//function to display the help menu
void helpMenu()
{
    char choice;
    do
    {
        system("cls");
        printf("\n\n\n");
        printf("========================================================================================================================\n");
        printf("                                             Tic Tac Toe Help\n");
        printf("========================================================================================================================\n\n\n");
        printf("     1. The game is played on a 3x3 grid.\n\n");
        printf("     2. Players take turns marking a square with their symbol (X or O).\n\n");
        printf("     3. The player who succeeds in placing three of their marks in a horizontal, vertical, or diagonal row wins the game\n\n");
        printf("     4. If no player achieves this, the game is a draw.\n\n");
        printf("     5. Winning multiple times against a friend allows you to get a new nickname that changes depending on your wins.\n\n\n");
        printf("========================================================================================================================\n");
        printf("Press (B) to go Back.");
        choice = toupper(getch());
    }
    while (choice != 'B');
    system("cls");
    main();
}

//function to display the Tic Tac Toe board
void displayBoard()
{
    printf("\n\n");
    printf("                          1                        |2                        |3\n");
    printf("                                         %c         |           %c             |           %c\n",board[0][0],board[0][1],board[0][2]);
    printf("                                                   |                         | \n");
    printf("                          -----------------------------------------------------------------------------\n");
    printf("                          4                        |5                        |6                          \n");
    printf("                                         %c         |           %c             |           %c           \n", board[1][0], board[1][1], board[1][2]);
    printf("                                                   |                         | \n");
    printf("                          -----------------------------------------------------------------------------\n");
    printf("                          7                        |8                        |9                          \n");
    printf("                                        %c          |           %c             |           %c\n", board[2][0], board[2][1], board[2][2]);
    printf("                                                   |                         | \n");
}

//function to reset the values of the board
void resetBoard()
{
   int i, j;
   for(i = 0; i < 3; i++)
   {
       for(j = 0; j < 3; j++)
       {
          board[i][j] = ' ';
       }
   }
}

// function to save the username to a file
void saveUsername(const char *username)
{
    FILE *file = fopen("usernames.txt", "a");
    // Write the username to the file with initial wins as 0
    fprintf(file, "%s 0\n", username);
    fclose(file);
}

//Checks whether the username exists already or not
bool check(const char *username)
{
    // Open file in read mode
    FILE *file = fopen("usernames.txt", "r");
    char line[MAX + 10];
    // Assuming each username is less than 50 characters long
    while (fgets(line, sizeof(line), file))
    {
        char name[MAX];
        int wins;
        sscanf(line, "%s %d", name, &wins);
        // Compare the username read from the file with the player's input
        if (strcmp(username, name) == 0) {
            fclose(file);
            return true;
            // Username exists
        }
    }
    fclose(file);
    return false;
    // Username doesn't exist
}

//counting the highest number of wins from the usernames file
void viewHighestScore()
{
    FILE *file = fopen("usernames.txt", "r");
    char choice;
    if (file == NULL)
    {
        do
        {
            printf("\n\n\n                                         No usernames found.\n");
            printf("\n\n Press B to go back.");
            choice = toupper(getch());
        }
        while (choice != 'B');
        system("cls");
        main();
    }
    Player highestScorePlayer;
    highestScorePlayer.totalWins = -1;
    Player currentPlayer;
    do
    {
        // Reset file pointer to the beginning
        rewind(file);
        // Reset highest score
        highestScorePlayer.totalWins = -1;
        while (fscanf(file, "%s %d", currentPlayer.username, &currentPlayer.wins) != EOF)
        {
            // Accumulate wins for the same username
            if (strcmp(currentPlayer.username, highestScorePlayer.username) == 0)
            {
                highestScorePlayer.totalWins += currentPlayer.wins;
            }
            else if (currentPlayer.wins > highestScorePlayer.totalWins)
            {
                strcpy(highestScorePlayer.username, currentPlayer.username);
                highestScorePlayer.totalWins = currentPlayer.wins;
            }
        }
        char nickname[MAX];
        if (highestScorePlayer.totalWins >= 10)
        {
            strcpy(nickname, "THE MASTER");
        }
        else if (highestScorePlayer.totalWins >= 5)
        {
            strcpy(nickname, "THE CHAMPION");
        }
        else
        {
            strcpy(nickname, "THE BEGINNER");
        }
        printf("\n\n\n                                              Highest Score:\n\n\n");
        printf("                                            %s %s\n\n                                                 %d wins\n", highestScorePlayer.username,nickname, highestScorePlayer.totalWins);
        printf("\n\n Press B to go back.");
        choice=toupper(getch());
    }
    while (choice!= 'B');
    system("cls");
    main();
}

//counts remaining free spaces
int freespaces()
{
   int freeSpaces = 9;
   int i, j;
   for(i = 0; i < 3; i++)
   {
       for(j = 0; j < 3; j++)
       {
          if(board[i][j] != ' ')
          {
             freeSpaces--;
          }
       }
   }
   return freeSpaces;
}

//adds 1 win to a specific username
void AddWin(const char *username)
{
    FILE *file = fopen("usernames.txt", "r+");
    char line[MAX + 10];
    bool found = false;
    while (fgets(line, sizeof(line), file))
    {
        char name[MAX];
        int wins;
        sscanf(line, "%s %d", name, &wins);
        if (strcmp(username, name) == 0)
        {
            fseek(file, -strlen(line), SEEK_CUR);
            fprintf(file, "%s %d\n", name, wins + 1);
            found = true;
            break;
        }
    }
    if (!found)
    {
        fprintf(file, "%s 1\n", username);
    }

    fclose(file);
}

//puts the player's respective sign in their chosen case
void playerMove(char currentPlayer)
{
    int row, col, Case;
    char input;
    do
    {
        printf("Enter case number: \n");
        input = getch();
        Case = input - '0';
    }
    while (Case < 1 || Case > 9);
    switch (Case)
    {
        case 1:
            row = 0;
            col = 0;
            break;
        case 2:
            row = 0;
            col = 1;
            break;
        case 3:
            row = 0;
            col = 2;
            break;
        case 4:
            row = 1;
            col = 0;
            break;
        case 5:
            row = 1;
            col = 1;
            break;
        case 6:
            row = 1;
            col = 2;
            break;
        case 7:
            row = 2;
            col = 0;
            break;
        case 8:
            row = 2;
            col = 1;
            break;
        case 9:
            row = 2;
            col = 2;
            break;
    }
    board[row][col] = (currentPlayer == PLAYER ? PLAYER : PLAYER2);
}

//same funstion but puts X for the player's turn instead of alternating
void playerMoveX()
{
    int row, col, Case;
    char input;
    do
    {
        printf("Enter case number: \n");
        input = getch();
        Case = input - '0';
    }
    while (Case < 1 || Case > 9);
    switch (Case)
    {
        case 1:
            row = 0;
            col = 0;
            break;
        case 2:
            row = 0;
            col = 1;
            break;
        case 3:
            row = 0;
            col = 2;
            break;
        case 4:
            row = 1;
            col = 0;
            break;
        case 5:
            row = 1;
            col = 1;
            break;
        case 6:
            row = 1;
            col = 2;
            break;
        case 7:
            row = 2;
            col = 0;
            break;
        case 8:
            row = 2;
            col = 1;
            break;
        case 9:
            row = 2;
            col = 2;
            break;
    }
    // Assign 'X' to the player's moves
    board[row][col] = PLAYER;
}

//same funstion but puts X for the player's turn instead of alternating
void playerMoveO()
{
    int row, col, Case;
    char input;
    do
    {
        printf("Enter case number: \n");
        input = getch();
        Case = input - '0';
    }
    while (Case < 1 || Case > 9);
    switch (Case)
    {
        case 1:
            row = 0;
            col = 0;
            break;
        case 2:
            row = 0;
            col = 1;
            break;
        case 3:
            row = 0;
            col = 2;
            break;
        case 4:
            row = 1;
            col = 0;
            break;
        case 5:
            row = 1;
            col = 1;
            break;
        case 6:
            row = 1;
            col = 2;
            break;
        case 7:
            row = 2;
            col = 0;
            break;
        case 8:
            row = 2;
            col = 1;
            break;
        case 9:
            row = 2;
            col = 2;
            break;
    }
    // Assign 'O' to the player's moves
    board[row][col] = COMPUTER;
}

//lets the computer have a random first move the searches for winning or losing moves then acts accordingly
void computerMoveX()
{
    srand(time(0));
    int x, y;
    // For the first move, choose a random position
    if (freespaces() == 9)
    {
        do
        {
            x = rand() % 3;
            y = rand() % 3;
        } while (board[x][y] != ' ');

        board[x][y] = PLAYER;
        return;
    }
    // Check for winning moves
    int i, j;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            if (board[i][j] == ' ')
            {
                // Simulate placing a mark and check for victory
                board[i][j] = PLAYER;
                if (checkWinner() == PLAYER)
                {
                    return;
                    // Win if possible
                }
                board[i][j] = ' ';
                // Undo move
            }
        }
    }
    // Block opponent's winning moves
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            if (board[i][j] == ' ')
            {
                // Simulate placing a mark for the opponent and check for their victory
                board[i][j] = COMPUTER;
                if (checkWinner() == COMPUTER)
                {
                    board[i][j] = PLAYER; // Block opponent's winning move
                    return;
                }
                board[i][j] = ' '; // Undo move
            }
        }
    }
    // Try to take any remaining available side position
    const int sides[4][2] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
    int k;
    for (k = 0; k < 4; ++k)
    {
        x = sides[k][0];
        y = sides[k][1];
        if (board[x][y] == ' ')
        {
            board[x][y] = PLAYER;
            return;
        }
    }
}

//Same thing but with O instead of X
void computerMoveO()
{
    srand(time(0));
    int x, y;
    // For the first move, choose a random position
    if (freespaces() == 9)
    {
        do
        {
            x = rand() % 3;
            y = rand() % 3;
        } while (board[x][y] != ' ');

        board[x][y] = COMPUTER;
        return;
    }
    // Check for winning moves
    int i, j;
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            if (board[i][j] == ' ')
            {
                // Simulate placing a mark and check for victory
                board[i][j] = COMPUTER;
                if (checkWinner() == COMPUTER)
                {
                    return;
                    // Win if possible
                }
                board[i][j] = ' ';
                // Undo move
            }
        }
    }
    // Block opponent's winning moves
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            if (board[i][j] == ' ')
            {
                // Simulate placing a mark for the opponent and check for their victory
                board[i][j] = PLAYER;
                if (checkWinner() == PLAYER)
                {
                    board[i][j] = COMPUTER;
                    // Block opponent's winning move
                    return;
                }
                board[i][j] = ' ';
                // Undo move
            }
        }
    }
    // Try to take any remaining available side position
    const int sides[4][2] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
    int k;
    for (k = 0; k < 4; ++k)
    {
        x = sides[k][0];
        y = sides[k][1];
        if (board[x][y] == ' ')
        {
            board[x][y] = COMPUTER;
            return;
        }
    }
}

//checks for a winner
char checkWinner()
{
   //check rows
   int i;

   for(i=0;i<3;i++)
   {
       if(board[i][0]==board[i][1] && board[i][0]==board[i][2])
       {
           return board[i][0];
       }
   }
   //check columns
   for(i=0;i<3;i++)
   {
       if(board[0][i]==board[1][i] && board[0][i]==board[2][i])
       {
           return board[0][i];
       }
   }
   //check diagonals
   if(board[0][0]==board[1][1] && board[0][0]==board[2][2])
   {
       return board[0][0];
   }
   if(board[0][2]==board[1][1] && board[0][2]==board[2][0])
   {
       return board[0][2];
   }
   return ' ';
}

//prints the winner on the screen
void printWinner(char winner, char currentPlayer)
{
    if(winner == PLAYER)
    {
        printf("\n\n\n                                          YOU WIN!\n\n\n");
    }
    else if(winner == COMPUTER)
    {
        printf("\n\n\n                                          YOU LOSE!\n\n\n");
    }
    else{
        printf("\n\n\n                                          IT'S A TIE!\n\n\n");
    }
}

//printts winner's name when the player chooses to play as O
void printWinnerO(char winner, char currentPlayer)
{
    if(winner == COMPUTER)
    {
        printf("\n\n\n                                          YOU WIN!\n\n\n");
    }
    else if(winner == PLAYER)
    {
        printf("\n\n\n                                          YOU LOSE!\n\n\n");
    }
    else{
        printf("\n\n\n                                          IT'S A TIE!\n\n\n");
    }
}

//main function for playing the game; takes care of asking the player which gameplay mode they want and whether they want to play as X or O
void playGame(char currentPlayer)
{
    char choice;
    do
    {
        printf("\n\n\n\n\n\n\n\n             C. PLAY AGAINST A COMPUTER                                    F. PLAY AGAINST A FRIEND\n");
        choice = toupper(getch());
        if (choice == 'C')
        {
            system("cls");
            gotoxy(55,10);
            printf("X      or      O");
            currentPlayer = toupper(getch());
            if (currentPlayer==PLAYER)
            {
                playAgainstComputer(currentPlayer);
            }
            else
            {
                playAgainstComputer2(currentPlayer);
            }
        }
        else if (choice == 'F')
        {
            playAgainstFriend();
        }
    } while (choice == 'C' || choice == 'F');
}

//alternates between player's and computer's moves until there's a winner
bool playAgainstComputer(char currentPlayer)
{
    char winner = ' ';
    char response = ' ';

    do
    {
        resetBoard();

        while(winner == ' ' && freespaces() != 0)
        {
            system("cls");
            displayBoard();

            printf("\n                                    It's your turn\n");
            playerMoveX();

            winner = checkWinner();

            if(winner != ' ' || freespaces() == 0)
            {
                break;
            }

            computerMoveO();

            winner = checkWinner();

            if(winner != ' ' || freespaces() == 0)
            {
                break;
            }
        }
        system("cls");
        printWinner(winner,currentPlayer);
        printf("\n\n                                  Would you like to play again?\n\n\n                     Y: Yes                                          N: No\n ");

        do
        {
            response= toupper(getch());
        }
        while (response != 'Y' && response != 'N');

        if(response=='N')
        {
            system("cls");
            main();
        }
        else
        {
            playAgainstComputer(currentPlayer);
        }
    }
    while(response=='Y');

    return true;
}

//same function for the case when a player wants to play as O
bool playAgainstComputer2(char currentPlayer)
{
    char winner = ' ';
    char response = ' ';

    do
    {
        resetBoard();

        while(winner == ' ' && freespaces() != 0)
        {
            system("cls");
            displayBoard();

            printf("\n                                    It's your turn\n");
            playerMoveO();

            winner = checkWinner();

            if(winner != ' ' || freespaces() == 0)
            {
                break;
            }

            computerMoveX();

            winner = checkWinner();

            if(winner != ' ' || freespaces() == 0)
            {
                break;
            }
        }
        system("cls");
        printWinnerO(winner,currentPlayer);
        printf("\n\n                                  Would you like to play again?\n\n\n                     Y: Yes                                          N: No\n ");

        do
        {
            response= toupper(getch());
        }
        while (response != 'Y' && response != 'N');

        if(response=='N')
        {
            system("cls");
            main();
        }
        else
        {
            playAgainstComputer2(currentPlayer);
        }
    }
    while(response=='Y');

    return true;
}

//asks for usernames and alternates between both players until there's a winner
void playAgainstFriend()
{
    system("cls");
    printf("Player 1 ( X ):\n");
    printf("Enter your username: ");
    scanf("%s", username1);
    printf("Player 2 ( O ):\n");
    printf("Enter your username: ");
    scanf("%s", username2);
    resetBoard();
    // Player 1 starts with 'X'
    char currentPlayer = PLAYER;
    int moveCount = 0;
    bool gameOver = false;
    char winner;

    // Main game loop
    while (!gameOver)
    {
        // Display the board
        system("cls");
        displayBoard();

        // Get player's move
        printf("%s's turn (%c):\n", (currentPlayer == PLAYER ? username1 : username2), currentPlayer);
        playerMove(currentPlayer);

        moveCount++;

        // Check for a win or draw
        winner = checkWinner();
        if (winner != ' ')
        {
            system("cls");
            displayBoard();
            printf("\n\n                              %s wins!\n\n", (winner == PLAYER ? username1 : username2));
            gameOver = true;

            FILE *file = fopen("usernames.txt", "r+");
            char line[MAX + 10];
            bool found = false;
            while (fgets(line, sizeof(line), file))
            {
                char name[MAX];
                int wins;
                sscanf(line, "%s %d", name, &wins);

                if (strcmp((winner == PLAYER ? username1 : username2), name) == 0)
                {
                    fseek(file, -strlen(line), SEEK_CUR);
                    fprintf(file, "%s %d\n", name, wins + 1);
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                fprintf(file, "%s 1\n", (winner == PLAYER ? username1 : username2));
            }

            fclose(file);
            break;
        }
        else if (moveCount == 9)
        {
            system("cls");
            displayBoard();
            printf("\n\n\n                                       It's a draw!\n");
            gameOver = true;
            break;
            // Exit the loop since the game is over
        }

        // Switch players
        currentPlayer = (currentPlayer == PLAYER ? PLAYER2 : PLAYER);
    }

    // Ask for a rematch
    printf("\n\n\n                                  Would you like to play again?\n\n\n");
    printf("                                Y: Yes                             N: No\n");
    char response;
    do
    {
        response = toupper(getch());
    }
    while (response != 'Y' && response != 'N');

    if (response == 'Y')
    {
        system("cls");
        // Restart the game
        playAgainstFriend();
    }
    else
    {
        system("cls");
        main();
    }
}

int main()
{
    char choice;
    choice='\0';
    displayMenu();
    char currentPlayer;
    do
    {
        choice = toupper(getch());
        switch(choice)
        {
            case 'S':
                currentPlayer = toupper(getch());
                system("cls");
                playGame(currentPlayer);
                break;
            case 'H':
                system("cls");
                helpMenu();
                break;
            case 'V':
                system("cls");
                viewHighestScore();
                break;
            case 27:
                exit(0);
                break;
        }
    }
    while(choice == 'S' || choice == 'H' || choice == 'V'|| choice == 27 );
    return 0;
}
