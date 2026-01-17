#include <iostream>
#include <fstream>
#include "windows.h"

int** create_board(int rows, int cols);
int** init_game_board(int&, int&);
int** count_living_neighbors(int**, int, int);
int** new_game_board(int**, int**, int, int);
void copy_game_board(int**, int**, int, int);
int summ_array(int**, int, int);
bool compare(int**, int**, int, int);
void print_game_status(int**, int, int, int, int);
void delete_two_dim_array(int**, int);

int** create_board(int rows, int cols) {
    int** array = new int* [rows];
    for (int r = 0; r < rows; r++) {
        array[r] = new int[cols]();
    }
    return array;
}

int main()
{
    setlocale(LC_ALL, "ru");

    int rows{}, columns{};
    int step{ 1 };
    bool diff{ 1 };
    int** gameBoard = init_game_board(rows, columns);
    if (!gameBoard)
    {
        return EXIT_FAILURE;
    }
    int countLiving = summ_array(gameBoard, rows, columns);
    print_game_status(gameBoard, rows, columns, step, countLiving);

    while (diff && countLiving) {
        Sleep(1000);
        step++;
        int** countLivingNeighbors = count_living_neighbors(gameBoard, rows, columns);
        int** newGameBoard = new_game_board(gameBoard, countLivingNeighbors, rows, columns);
        delete_two_dim_array(countLivingNeighbors, rows);
        diff = compare(gameBoard, newGameBoard, rows, columns);
        countLiving = summ_array(newGameBoard, rows, columns);
        std::system("CLS");
        print_game_status(newGameBoard, rows, columns, step, countLiving);
        copy_game_board(newGameBoard, gameBoard, rows, columns);
        delete_two_dim_array(newGameBoard, rows);
    }
    delete_two_dim_array(gameBoard, rows);
    std::cout << (diff == 0 ? "Игровое поле стабильно. Конец игры." : "Живые клетки отсутствуют. Конец игры.") << std::endl;

    return EXIT_SUCCESS;
}

int** init_game_board(int& rows, int& columns) {
    std::ifstream file("init.txt");
    if (file.is_open()) {
        file >> rows >> columns;

        int** array = create_board(rows, columns);

        int r{}, c{};
        while (file >> r >> c) {
            if (r >= 0 && r < rows && c >= 0 && c < columns)
                array[r][c] = 1;
        }
        file.close();
        return array;
    }
    else {
        std::cout << "Ошибка открытия файла" << std::endl;
        rows = columns = 0;
        return nullptr;
    }
}


int** count_living_neighbors(int** array, int rows, int columns) {
    int** newArray = create_board(rows, columns);

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            if (((r - 1) >= 0) && ((c - 1) >= 0)) newArray[r][c] += array[r - 1][c - 1];
            if ((r - 1) >= 0) newArray[r][c] += array[r - 1][c];
            if (((r - 1) >= 0) && ((c + 1) < columns)) newArray[r][c] += array[r - 1][c + 1];
            if ((c + 1) < columns) newArray[r][c] += array[r][c + 1];
            if (((r + 1) < rows) && ((c + 1) < columns)) newArray[r][c] += array[r + 1][c + 1];
            if ((r + 1) < rows) newArray[r][c] += array[r + 1][c];
            if (((r + 1) < rows) && ((c - 1) >= 0)) newArray[r][c] += array[r + 1][c - 1];
            if ((c - 1) >= 0) newArray[r][c] += array[r][c - 1];
        }
    }
    return newArray;
}

int** new_game_board(int** gameBoard, int** countGameBoard, int rows, int columns) {
    int** array = create_board(rows, columns);

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            array[r][c] = gameBoard[r][c];
            if ((gameBoard[r][c] == 0) && (countGameBoard[r][c] == 3)) {
                array[r][c] = 1;
            }
            else if ((countGameBoard[r][c] < 2) || (countGameBoard[r][c] > 3)) {
                array[r][c] = 0;
            }
        }
    }
    return array;
}

void copy_game_board(int** newBoard, int** board, int rows, int columns) {
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            board[r][c] = newBoard[r][c];
        }
    }
}

int summ_array(int** array, int rows, int columns) {
    int summ{};
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            summ += array[r][c];
        }
    }
    return summ;
}

bool compare(int** board1, int** board2, int rows, int columns) {
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            if (board1[r][c] != board2[r][c]) {
                return true;
            }
        }
    }
    return false;
}

void print_game_status(int** array, int rows, int columns, int step, int countLiving)
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            std::cout << (array[r][c] == 1 ? '*' : '-') << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Поколение: " << step << ". Живых клеток: " << countLiving << std::endl;
}

void delete_two_dim_array(int** array, int rows)
{
    for (int r = 0; r < rows; r++)
    {
        delete[] array[r];
    }
    delete[] array;
}
