// Tic Tac Toe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <limits>

using namespace std;

const int BOARD_SIZE = 3;
const int INDEX_BORDER_OFFSET = 1;
const int BOT_SEARCH_DEPTH = 4;

const char DEFAULT_BOARD_CELL = '#';
const char PLAYER_CELL = 'X';
const char BOT_CELL = 'O';

const int WIN = 1;
const int TIE = 0;
const int LOSS = -1;
const int CONTINUE = -2;

struct ResultPosition {
    int X;
    int Y;
    int MinimaxResult;

    ResultPosition(int x, int y, int minimaxResult) {
        X = x;
        Y = y;
        MinimaxResult = minimaxResult;
    }
};

void setupBoard(char board[BOARD_SIZE][BOARD_SIZE]);
char getNextPlayer(char currentPlayer);
void getWritedPosition(int* xPointer, int* yPointer);
ResultPosition getBestMove(char board[BOARD_SIZE][BOARD_SIZE]);
int getGameState(char board[BOARD_SIZE][BOARD_SIZE]);
bool isWin(char board[BOARD_SIZE][BOARD_SIZE], char selectedPlayer);
void updateBoard(char board[BOARD_SIZE][BOARD_SIZE], int xPosition, int yPosition, char player);
int getEmptyCellsCount(char board[BOARD_SIZE][BOARD_SIZE]);
void writeBoard(char boardPointer[BOARD_SIZE][BOARD_SIZE]);

int main()
{
    char board[BOARD_SIZE][BOARD_SIZE];
    setupBoard(board);

    char currentPlayer = PLAYER_CELL;
    while (true) {
        if (currentPlayer == BOT_CELL) {
            ResultPosition value = getBestMove(board);
            updateBoard(board, value.X, value.Y, BOT_CELL);

            if (isWin(board, BOT_CELL) == true || getEmptyCellsCount(board) == 0)
                break;

            currentPlayer = PLAYER_CELL;
            continue;
        }

        system("cls");
        cout << "Player " << currentPlayer << " moves:" << endl;
        writeBoard(board);

        int x, y;
        do {
            getWritedPosition(&x, &y);
        } while (board[x][y] != DEFAULT_BOARD_CELL);

        updateBoard(board, x, y, currentPlayer);

        if (isWin(board, currentPlayer) == true)
            break;

        currentPlayer = getNextPlayer(currentPlayer);
        cout << '\n';
    }

    system("cls");
    writeBoard(board);
    if (isWin(board, BOT_CELL) == true || isWin(board, PLAYER_CELL) == true)
        cout << "Player " << currentPlayer << " Win!" << endl;
    else if (getEmptyCellsCount(board) == 0)
        cout << "Tie!" << endl;

    char c[1];
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(c, 1);
}

void setupBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = DEFAULT_BOARD_CELL;
        }
    }
}
char getNextPlayer(char currentPlayer) {
    if (currentPlayer == PLAYER_CELL)
        return BOT_CELL;

    return PLAYER_CELL;
}
void getWritedPosition(int* xPointer, int* yPointer) {
    cout << "X position: ";
    cin >> *xPointer;
    *xPointer -= INDEX_BORDER_OFFSET;
    cout << "Y position: ";
    cin >> *yPointer;
    *yPointer -= INDEX_BORDER_OFFSET;
}
int calculateMax(int value, int minimax) {
    if (value > minimax)
        return value;

    return minimax;
}
int calculateMin(int value, int minimax) {
    if (value < minimax)
        return value;

    return minimax;
}
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int serchDepth, bool max) {
    int gameState = getGameState(board);
    if (gameState != CONTINUE)
        return gameState;
    else if (serchDepth == 0)
        return TIE;

    if (isWin(board, BOT_CELL) == true)
        return WIN;
    else if (isWin(board, PLAYER_CELL) == true)
        return LOSS;
    else if (getEmptyCellsCount(board) == 0 || serchDepth == 0)
        return TIE;

    if (max == true) {
        int value = LOSS;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] != DEFAULT_BOARD_CELL)
                    continue;
                updateBoard(board, i, j, BOT_CELL);
                int minimaxValue = minimax(board, serchDepth - 1, false);
                updateBoard(board, i, j, DEFAULT_BOARD_CELL);
                value = calculateMax(value, minimaxValue);
            }
        }
        return value;
    }
    else {
        int value = WIN;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] != DEFAULT_BOARD_CELL)
                    continue;
                updateBoard(board, i, j, PLAYER_CELL);
                int minimaxValue = minimax(board, serchDepth - 1, true);
                updateBoard(board, i, j, DEFAULT_BOARD_CELL);
                value = calculateMin(value, minimaxValue);
            }
        }
        return value;
    }
}
ResultPosition getBestMove(char board[BOARD_SIZE][BOARD_SIZE]) {
    ResultPosition value(0, 0, -1);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != DEFAULT_BOARD_CELL)
                continue;
            updateBoard(board, i, j, BOT_CELL);
            int minimaxValue = minimax(board, BOT_SEARCH_DEPTH, false);
            updateBoard(board, i, j, DEFAULT_BOARD_CELL);
            if (minimaxValue > value.MinimaxResult)
                value = ResultPosition(i, j, minimaxValue);
        }
    }

    return value;
}
bool isWin(char board[BOARD_SIZE][BOARD_SIZE], char selectedPlayer) {
    bool isMainDiagonalWin = true;
    bool isSideDiagonalWin = true;
    for (int i = 0; i < BOARD_SIZE; i++) {
        bool isHorizontalWin = true;
        bool isVerticalWin = true;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != selectedPlayer)
                isHorizontalWin = false;
            if (board[j][i] != selectedPlayer)
                isVerticalWin = false;
        }
        if (isHorizontalWin == true || isVerticalWin == true)
            return true;

        if (board[i][i] != selectedPlayer)
            isMainDiagonalWin = false;
        if (board[i][(BOARD_SIZE - 1 - i)] != selectedPlayer)
            isSideDiagonalWin = false;
    }
    if (isMainDiagonalWin == true || isSideDiagonalWin == true)
        return true;

    return false;
}
void updateBoard(char board[BOARD_SIZE][BOARD_SIZE], int xPosition, int yPosition, char player) {
    board[xPosition][yPosition] = player;
}
int getEmptyCellsCount(char board[BOARD_SIZE][BOARD_SIZE]) {
    int emptyCellsCount = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == DEFAULT_BOARD_CELL)
                emptyCellsCount++;
        }
    }
    return emptyCellsCount;
}
int getGameState(char board[BOARD_SIZE][BOARD_SIZE]) {
    if (isWin(board, BOT_CELL) == true)
        return WIN;
    else if (isWin(board, PLAYER_CELL) == true)
        return LOSS;
    else if (getEmptyCellsCount(board) == 0)
        return TIE;

    return CONTINUE;
}
void writeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    cout << ' ';
    for (int i = 0; i < BOARD_SIZE; i++)
        cout << i + INDEX_BORDER_OFFSET;
    cout << '\n';
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << i + INDEX_BORDER_OFFSET;
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j];
        }
        cout << '\n';
    }
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
