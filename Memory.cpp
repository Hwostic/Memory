#include <iostream> 
#include <ctime>    
#include <windows.h>  //для использования функцийоперационной системы Windows
#include <mmsystem.h> // для работы с мультимедиа
#include <chrono> // для работы со временем

#pragma comment (lib, "Winmm.lib") //для работы с PlaySound

using namespace std;


// Создание доски и карточек

void createBoard(char** board, char* cards, int size, int size2) 
{
 
    // Заполнение карточек
    for (int i = 0; i < size * size2 / 2; i++) 
    {
        // Каждой паре карточек присваиваем одну букву (A, B, C и т.д.)
        cards[i * 2] = 'A' + i;       // Первая карточка пары
        cards[i * 2 + 1] = 'A' + i;   // Вторая карточка пары
    }

    // Перемешивание карточек

    for (int i = 0; i < size * size2; i++) 
    {
        // Генерируем случайный индекс для перемешивания
        int j = rand() % (size * size2);
        // Меняем местами карточки по индексам i и j
        swap(cards[i], cards[j]);
    }

    // Заполнение доски символами закрытых карточек
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size2; j++) 
        {
            board[i][j] = '*'; // рубашка карточки
        }
    }
}

void printBoard(char** board, int size, int size2) 
{
 
    cout << "\nТекущая доска:" << endl;
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size2; j++) 
        {
            cout << board[i][j] << " "; 
        }
        cout << endl; 
    }
}

// Функция для проверки, совпадают ли открытые карточки
bool isVin(char** board, int x1, int y1, int x2, int y2) 
{
    return board[x1][y1] == board[x2][y2]; // Возвращает true, если карточки совпадают
}


//Функция, находящая совпадающие карточки

void LuckCounts(char** board, char* cards, int size, int size2, int& count)
{
    int luckyCount = 0; // Счетчик найденных пар

    while (luckyCount < size * size2 / 2)
    { // Цикл продолжается, пока не найдены все пары

        printBoard(board, size, size2); // Выводим текущее состояние доски

        int x1, y1, x2, y2; // Координаты открываемых карточек

       
        cout << "\nВведите координаты первой карточки:\nX: ";
        cin >> x1;
        cout << "Y: ";
        cin >> y1;
    
        cout << "Введите координаты второй карточки:\nX: ";
        cin >> x2;
        cout << "Y: ";
        cin >> y2;

        x1 = x1 - 1, y1 = y1 - 1, x2 = x2 - 1, y2 = y2 - 1;

        if (x1 < 0 || x1 >= size || y1 < 0 || y1 >= size ||
            x2 < 0 || x2 >= size || y2 < 0 || y2 >= size) 
        {
            cout << "Некорректные координаты. Попробуйте снова." << endl;
            continue; 
        }
        if (x1 == x2 && y1 == y2 && x1 == y1 && x2 == y2)
        {
            cout << "Это одна и та жа карточка, введите заново" << endl;
            continue;
        }



        // Открытие карточек 
        board[x1][y1] = cards[x1 * size + y1]; 
        count++;
        board[x2][y2] = cards[x2 * size + y2]; 
        count++;


        printBoard(board, size, size2); // Выводим доску с открытыми карточками

        // Проверка на совпадение открытых карточек
        if (isVin(board, x1, y1, x2, y2)) 
        {
            PlaySoundA("luck.wav", NULL, SND_FILENAME | SND_ASYNC);
            cout << "Совпадение найдено!" << endl; 
            luckyCount++; 

        }
        else 
        {
            PlaySoundA("fail.wav", NULL, SND_FILENAME | SND_ASYNC);

            cout << "Нет совпадения. Закрываем карточки.\n" << endl; 
            board[x1][y1] = '*'; 
            board[x2][y2] = '*'; 
        }
    }

}


int main()

{
    setlocale(LC_ALL, "RU"); 
    srand(time(0)); 
    auto start = chrono::high_resolution_clock::now(); //функция фиксирует точное время

    int count = 0; // количество открываемых карточек

    cout << "Приветствуем в игре Memory!\nПравилы игры просты: открывать по 2 карточки, пока не найдутся совпадения. Приступим! :)\n";

    while (true)
    {
        int size; 
        int size2; 

        cout << "Введите четный размер игрового поля (например, 4 и 8 для 4x8): \nПервое значение: ";
        cin >> size;
        cout << "Второе значение: ";
        cin >> size2; 

        // Проверка на четность размера
        if (((size * size2) % 2 != 0 || size < 2 || size >= 20) && (size2 % 2 != 0 || size2 < 2 || size2 >= 20)) {
            cout << "Размер должен быть четным, не меньше 2 и не больше 20." << endl;
            continue; 
        }

       
        char** board = new char* [size];
        for (int i = 0; i < size; i++) {
            board[i] = new char[size]; 
        }
        char* cards = new char[size * size2]; // Массив для хранения карточек

        createBoard(board, cards, size, size2); //Создаем доску с карточками

        LuckCounts(board, cards, size, size2, count); //основное тело программы, по завершению заиграет победная музыка


        PlaySoundA("vin.wav", NULL, SND_FILENAME | SND_ASYNC);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> finalTime = end - start; //находим, сколько времени прошло
        int minutes = static_cast<int>(finalTime.count()) / 60; 
        int seconds = static_cast<int>(finalTime.count()) % 60;

        cout << "Поздравляем! Вы нашли все пары! Время игры: " << minutes << " минут " << seconds << " секунд"
            << "\nКоличество переворотов карт: " << count << endl;

        for (int i = 0; i < size; i++) 
        {
            delete[] board[i]; 
        }
        delete[] board; 
        delete[] cards; 


        // Запрос на повторную игру
        char playAgain;
        cout << "\nХотите сыграть еще раз? (y/n): " << endl;
        cin >> playAgain;
        if (playAgain != 'y' && playAgain != 'Y') {
            break;
        }

    }

    return 0;
}