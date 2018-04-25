/* A simple cross-word puzzle game written in C for CMPE 252 
 * Author: T. EMRE SEN */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct
{
    char *word; //word and corresponding hint
    char *clue;
    int x; //Starting x and y positions
    int y;
    char direction; //H for horizontal, V for vertical 
    int f; //solved or not
} Word_t;

Word_t *loadTextFile(FILE *myFile, int nrWords);
void displayBoard(int rows, int cols, char **myBoard);
int isBoardFilled(int rows, int cols, char **myBoard);
char **createArray(int rows, int cols);
void playGame(char **myBoard, int words, Word_t *wordsptr, int rows, int cols, int countToFinish); 
char **updateBoard(char **myBoard, Word_t *words, int solve);
void printAskForHint(int words, Word_t *wordsptr);

int first = 0;
int gameOverCounter;

int main()
{
    char **board;
    FILE *fptr;
    char filename[20];
    int numberOfRows, numberOfColumns, numberOfWords;
    
    Word_t *myMainArray;

    printf("Enter the name of text file -> ");
    scanf("%s", filename);
    fptr = fopen (filename, "r" );

    int status;
    int counter = 0;

    while(status != EOF && counter < 1)
    {
        status = fscanf(fptr,"%d %d %d", &numberOfRows, &numberOfColumns, &numberOfWords);
        counter++;
    }
    printf("Game is %d rows and %d cols with %d words \n", numberOfRows, numberOfColumns, numberOfWords);
    myMainArray = loadTextFile(fptr, numberOfWords);
    board = createArray(numberOfRows, numberOfColumns);
    playGame(board, numberOfWords, myMainArray, numberOfRows, numberOfColumns, numberOfWords);
    return 0;
}

Word_t *loadTextFile(FILE *myFile, int nrWords)
{
    Word_t *myTempArray = (Word_t*) malloc(nrWords * sizeof(Word_t));

    int i = 0;

    while(!feof(myFile))
    {
        myTempArray[i].word = (char*) malloc(80 * sizeof(char));
        myTempArray[i].clue = (char*) malloc(200 * sizeof(char));

        //char *tempone[20];
        //char *temptwo[50];

        //fscanf(myFile," %c %d %d %s %[^\n]", &myTempArray[i].direction, &myTempArray[i].x, &myTempArray[i].y, tempone, temptwo);
        fscanf(myFile," %c %d %d %s %[^\n]", &myTempArray[i].direction, &myTempArray[i].x, &myTempArray[i].y, myTempArray[i].word, myTempArray[i].clue);
        

        //myTempArray[i].word = (char*) malloc(strlen(tempone) + 1);
        //myTempArray[i].clue = (char*) malloc(strlen(temptwo) + 1);
        
        //strcpy(myTempArray[i].word, tempone);
        //strcpy(myTempArray[i].clue, temptwo);
        myTempArray[i].f = 0;
        i++;
    }

    int size = i;
    printf("Words and clues are read!\n\n");
    
    return myTempArray;
}

char **createArray(int rows, int cols)
{
    char **tempBoard = (char**)malloc(rows * sizeof(char*));

    for(int i = 0; i < rows; i++)
    {
        tempBoard[i] = (char*)malloc(cols * sizeof(char));
    }

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            tempBoard[i][j] = '#';
        }
    }

    return tempBoard;
}

void playGame(char **myBoard, int words, Word_t *wordsptr, int rows, int cols, int countToFinish)
{
    int input;
    int wordchecker;
    char solution[20];

    updateBoard(myBoard, wordsptr, words);
    displayBoard(rows, cols, myBoard);
    printAskForHint(words, wordsptr);

    printf("Enter -1 to exit\n");
    printf("Which word to solve next?: ");
    scanf("%d", &input);

    while(input != -1)
    {
        if(wordsptr[input - 1].f == 0)
        {
            printf("Current hint: %s\n", wordsptr[input - 1].clue);
            printf("Enter your solution: ");
            scanf("%s", solution);
        
            int check = strcmp(wordsptr[input-1].word, solution);   

            if(check == 0)
            {
                printf("Correct!\n");
                gameOverCounter++;
                wordsptr[input-1].f = 1;
                updateBoard(myBoard, wordsptr, words);
                displayBoard(rows, cols, myBoard);

                int gameOverController = isBoardFilled(rows, cols, myBoard);
                if(gameOverController == 1 || gameOverCounter == words)
                {
                    printf("Congratulations! You beat the puzzle!\n");
                    free(myBoard);
                    free(wordsptr);
                    exit(0);
                }

            printAskForHint(words, wordsptr);
            }

            else if(check != 0 )
            {
                printf("Wrong!\n");
            }
            printf("Enter -1 to exit\n");
            printf("Which word to solve next?: ");
            scanf("%d", &input);
        }

        else if(wordsptr[input - 1].f == 1)
        {
            printf("\nYou Already Solved That!!\n");
            printAskForHint(words, wordsptr);
            printf("Enter -1 to exit\n");
            printf("Which word to solve next?: ");
            scanf("%d", &input);
        }
    }
}

void printAskForHint(int words, Word_t *wordsptr)
{
    printf("\nAsk For Hint:\n\n");
    printf("#\tDirection\trow\tcol\n");
    printf("------------------------------------\n");

    for(int i = 0; i < words; i++)
    {
        if(wordsptr[i].direction == 'H' && wordsptr[i].f == 0)
        {
            printf("%d:\tHorizontal\t%d\t%d\n",i + 1, wordsptr[i].x, wordsptr[i].y);  
        }

        else if(wordsptr[i].direction == 'V' && wordsptr[i].f == 0)
        {
            printf("%d:\tVertical\t%d\t%d\n",i + 1, wordsptr[i].x, wordsptr[i].y);  
        }
    }
}

char **updateBoard(char **myBoard, Word_t *words, int solve)
{
    for(int i = 0; i < solve; i++)
    {
        switch (words[i].direction)
        {
            case 'H':
                switch (words[i].f)
                {
                    case 0:
                        for(int j = 0; j < strlen(words[i].word); j++)
                        {
                            if(first == 0)
                            {
                                myBoard[words[i].x - 1][words[i].y - 1 + j] = '_';
                            }
                            
                            if(myBoard[words[i].x - 1][words[i].y - 1 + j] == '_' && first != 0)
                            {
                                myBoard[words[i].x - 1][words[i].y - 1 + j] = '_';
                            }
                        }
                    break;

                    case 1:
                        for(int j = 0; j < strlen(words[i].word); j++)
                        {
                            myBoard[words[i].x - 1][words[i].y - 1 + j] = words[i].word[j];
                        }
                    break;
                }
            break;

            case 'V' :
                switch (words[i].f)
                {
                    case 0:
                        for(int j = 0; j < strlen(words[i].word); j++)
                        {
                            if(first == 0)
                            {
                                myBoard[words[i].x - 1 + j][words[i].y - 1] = '_';
                            }
                            if(myBoard[words[i].x - 1 + j][words[i].y - 1] == '_' && first != 0)
                            {
                                myBoard[words[i].x - 1 + j][words[i].y - 1] = '_';
                            }
                        }
                    break;

                    case 1:
                        for(int j = 0; j < strlen(words[i].word); j++)
                        {
                            myBoard[words[i].x - 1 + j][words[i].y - 1] = words[i].word[j];
                        }
                    break;
            }
            break;
        }
    }
    first = 1;
}

void displayBoard(int rows, int cols, char **myBoard)
{
    printf("Current Puzzle:\n\n");
    printf("\t  ");
        for(int j = 0 ; j < rows; j++)
        {
            printf("%d  ", (j+1));
        }
        printf("\n\t -- -- -- -- --\n");
        for(int j=0; j < rows;j++)
        {
            printf("%d\t|", (j+1));
            for(int i = 0 ; i < cols; i++)
            {
                printf(" %c ", myBoard[j][i]);
            }
            
            printf("\n");
        }

    printf("\t****************\n");
}

int isBoardFilled(int rows, int cols, char **myBoard)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(myBoard[i][j] == '_')
            {
                return 0;
            }
        }
    }

    return 1;   
}

