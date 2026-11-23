
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYER 4
#define BOARD_SIZE 20
#define INITIAL_ENERGY 100
#define MAX_ENERGY 100
#define GRADUATION_CREDIT 12

#define START 0
#define CITY 1
#define EVENT 2
#define ISLAND 3
#define RESTAURANT 4

typedef struct {
    char name[30];
    int energy;
    int position;
    int credit;
    int graduated;
} Player;

typedef struct {
    char name[30];
    int type;
    int credit;
    int energy;
} BoardNode;

BoardNode board[BOARD_SIZE];
Player players[MAX_PLAYER];

char typeName[5][20] = {
    "START",
    "CITY",
    "EVENT",
    "ISLAND",
    "RESTAURANT"
};

char gradeName[5][5] = {
    "A+",
    "A0",
    "A-",
    "B+",
    "B0"
};

char *getName(int type)
{
    if (type < 0 || type >= 5)
        return "UNKNOWN";

    return typeName[type];
}

char *getGradeName(int grade)
{
    if (grade < 0 || grade >= 5)
        return "UNKNOWN";

    return gradeName[grade];
}

void createBoard(void)
{
    int i;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        strcpy(board[i].name, "Lecture");
        board[i].type = CITY;
        board[i].credit = 3;
        board[i].energy = -20;
    }

    strcpy(board[0].name, "Start");
    board[0].type = START;
    board[0].credit = 0;
    board[0].energy = 20;

    for (i = 5; i < BOARD_SIZE; i += 5)
    {
        strcpy(board[i].name, "Restaurant");
        board[i].type = RESTAURANT;
        board[i].credit = 0;
        board[i].energy = 30;
    }

    strcpy(board[7].name, "Event");
    board[7].type = EVENT;
    board[7].credit = 0;
    board[7].energy = -10;

    strcpy(board[12].name, "Island");
    board[12].type = ISLAND;
    board[12].credit = 0;
    board[12].energy = -15;
}

void createPlayer(Player *p)
{
    printf("Player name: ");
    scanf("%29s", p->name);

    p->energy = INITIAL_ENERGY;
    p->position = 0;
    p->credit = 0;
    p->graduated = 0;
}

void printPlayer(Player *p)
{
    printf("Name: %s\n", p->name);
    printf("Energy: %d\n", p->energy);
    printf("Position: %d\n", p->position);
    printf("Credit: %d\n", p->credit);
    printf("Graduated: %s\n",
           p->graduated ? "YES" : "NO");
}

void stepForward(Player *p, int steps)
{
    int oldPosition = p->position;

    p->position = (p->position + steps) % BOARD_SIZE;

    printf("%s moved %d steps!\n", p->name, steps);

    if (oldPosition + steps >= BOARD_SIZE)
    {
        p->energy += 20;
        printf("Passed START! Energy +20\n");
    }

    if (p->energy > MAX_ENERGY)
        p->energy = MAX_ENERGY;
}

void movePlayer(Player *p, int steps)
{
    stepForward(p, steps);
}

void actionNode(Player *p)
{
    BoardNode *node = &board[p->position];

    printf("Arrived at %s (%s)\n",
           node->name, getName(node->type));

    p->credit += node->credit;
    p->energy += node->energy;

    if (node->credit > 0)
        printf("Credit +%d\n", node->credit);

    if (node->energy > 0)
        printf("Energy +%d\n", node->energy);
    else if (node->energy < 0)
        printf("Energy %d\n", node->energy);

    if (p->energy > MAX_ENERGY)
        p->energy = MAX_ENERGY;

    if (p->energy < 0)
        p->energy = 0;

    if (p->credit >= GRADUATION_CREDIT)
    {
        p->graduated = 1;
        printf("%s graduated!\n", p->name);
    }
}

int main(void)
{
    int playerCount;
    int i;
    int dice;
    int round = 1;
    int graduatedCount;

    srand((unsigned int)time(NULL));

    createBoard();

    printf("=== Node Types ===\n");

    for (i = 0; i < 5; i++)
        printf("Type %d : %s\n", i, getName(i));

    printf("\n=== Grade Types ===\n");

    for (i = 0; i < 5; i++)
        printf("Grade %d : %s\n", i, getGradeName(i));

    while (1)
    {
        printf("\nNumber of players (2~4): ");

        if (scanf("%d", &playerCount) != 1)
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if (playerCount >= 2 && playerCount <= 4)
            break;

        printf("Please enter 2~4 players.\n");
    }

    for (i = 0; i < playerCount; i++)
    {
        printf("\nPlayer %d\n", i + 1);
        createPlayer(&players[i]);
    }

    printf("\n=== Player Information ===\n");

    for (i = 0; i < playerCount; i++)
    {
        printPlayer(&players[i]);
        printf("\n");
    }

    while (1)
    {
        graduatedCount = 0;

        printf("\n=== Round %d ===\n", round);

        for (i = 0; i < playerCount; i++)
        {
            if (players[i].graduated)
            {
                graduatedCount++;
                continue;
            }

            dice = rand() % 6 + 1;

            printf("\n%s rolled %d\n",
                   players[i].name, dice);

            movePlayer(&players[i], dice);
            actionNode(&players[i]);
            printPlayer(&players[i]);

            if (players[i].graduated)
                graduatedCount++;
        }

        if (graduatedCount == playerCount)
        {
            printf("\nAll players graduated!\n");
            break;
        }

        round++;
    }

    printf("\n=== Final Player Information ===\n");

    for (i = 0; i < playerCount; i++)
    {
        printPlayer(&players[i]);
        printf("\n");
    }

    return 0;
}
