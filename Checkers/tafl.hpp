#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <list>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::map;
using std::string;
using std::vector;

typedef vector<vector<string>> Table;
typedef std::pair<int, int> pair;

string ansi_black = "\u001b[30m";
string ansi_red = "\u001b[31m";
string ansi_green = "\u001b[32m";
string ansi_yellow = "\u001b[33m";
string ansi_blue = "\u001b[34m";
string ansi_magenta = "\u001b[35m";
string ansi_cyan = "\u001b[36m";
string ansi_white = "\u001b[37m";
string ansi_reset = "\u001b[0m";

// Получение массива координат из строки
vector<string> split_string(const std::string& s, char delim) 
{
  vector<string> tokens;
  string token;
  for (char c : s) {
    if (c == delim) {
      tokens.push_back(token);
      token.clear();
    } else {
      token += c;
    }
  }
  if (!token.empty()) {
    tokens.push_back(token);
  }
  return tokens;
}

//Проверка хода игрока
bool check_player_moves(Table board, int old_i, int old_j, int new_i, int new_j)
{
    if (new_i > 8 || new_i < 0) {
        return false;
    }
    if (new_j > 8 || new_j < 0) {
        return false;
    }
    if (board.at(old_i).at(old_j) == "---") {
        return false;
    }
    if (board.at(new_i).at(new_j) != "---") {
        return false;
    }
    if (board.at(old_i).at(old_j).at(0) == 'c') {
        return false;
    }
    if (old_i != new_i && old_j == new_j) {
        if (old_i < new_i) {
            for (int i = 1; i < new_i - old_i; ++i) {
                if (board.at(old_i + i).at(new_j) != "---")
                    return false;
            }
        }
        else {
            for (int i = 1; i < old_i - new_i; ++i) {
                if (board.at(old_i - i).at(new_j) != "---")
                    return false;
            }
        }
    }
    else if (old_j != new_j && old_i == new_i) {
        if (old_j < new_j) {
            for (int i = 1; i < new_j - old_j; ++i) {
                if (board.at(old_i).at(old_j + i) != "---")
                    return false;
            }
        }
        else {
            for (int i = 1; i < old_j - new_j; ++i) {
                if (board.at(old_i).at(old_j - i) != "---")
                    return false;
            }
        }
    }
    if (board.at(new_i).at(new_j) == "---") {
        return true;
    }
}

// Ход ПК
void make_a_move(Table* board, int old_i, int old_j, int new_i, int new_j, char big_letter) // Ходы
{
    char letter = (*board).at(old_i).at(old_j).at(0);       // Получаем тип фишки

    (*board).at(old_i).at(old_j) = "---"; // Освобождаем старую клетку
    (*board).at(new_i).at(new_j) = letter + std::to_string(new_i) + std::to_string(new_j); // Перемещаем фишку на новую клетку
}


// Проверка хода пк
bool check_moves(Table board, int old_i, int old_j, int new_i, int new_j) {
    if (new_i > 8 || new_i < 0 || new_j > 8 || new_j < 0) {
        return false;
    }
    if (board.at(old_i).at(old_j) == "---") {
        return false;
    }
    if (board.at(new_i).at(new_j) != "---") {
        return false;
    }
    if (board.at(old_i).at(old_j).at(0) == 'b' || board.at(old_i).at(old_j).at(0) == 'K') {
        return false;
    }
    if (old_i == new_i && old_j == new_j) {
        return false;
    }
    if (old_i != new_i && old_j == new_j) {
        if (old_i < new_i) {
            for (int i = 1; i < new_i - old_i;++i) {
                if (board.at(old_i+i).at(new_j) != "---")
                    return false;
            }
        }
        else {
            for (int i = 1; i < old_i - new_i; ++i) {
                if (board.at(old_i - i).at(new_j) != "---")
                    return false;
            }
        }
    }
    else if (old_j != new_j && old_i == new_i) {
        if (old_j < new_j) {
            for (int i = 1; i < new_j - old_j; ++i) {
                if (board.at(old_i).at(old_j + i) != "---")
                    return false;
            }
        }
        else {
            for (int i = 1; i < old_j - new_j; ++i) {
                if (board.at(old_i).at(old_j - i) != "---")
                    return false;
            }
        }
    }
    if (board.at(new_i).at(new_j) == "---") {
        return true;
    }
}

// Поиск доступных ходов ПК
vector<vector<int>> find_available_moves(Table board) 
{
    // Вектор, в который будут добавляться доступные ходы в виде {int old_i, int old_j, int new_i, int new_j}
    vector<vector<int>> available_moves;

    // Перебираем каждую клетку на доске
    for (int m = 0; m < 9; m++) {
        for (int n = 0; n < 9; n++) {
            // Если на текущей клетке находится фигура игрока 'c'
            if (board.at(m).at(n).at(0) == 'c') {
                for (int c = 1; c <= 8; ++c) {
                    // Проверяем возможные ходы вверх
                    if (check_moves(board, m, n, m - c, n))
                        available_moves.push_back({ m, n, m - c, n });
                    // Проверяем возможные ходы вниз
                    if (check_moves(board, m, n, m + c, n))
                        available_moves.push_back({ m, n, m + c, n });
                    // Проверяем возможные ходы влево
                    if (check_moves(board, m, n, m, n - c))
                        available_moves.push_back({ m, n, m, n - c });
                    // Проверяем возможные ходы вправо
                    if (check_moves(board, m, n, m, n + c))
                        available_moves.push_back({ m, n, m, n + c });
                }
            }
        }
    }
    return available_moves;
}

int evaluation_function(Table board) //оф
{
  int result = 0;
  int mine = 0;
  int opp = 0;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (board.at(i).at(j).at(0) == 'c') {
        mine += 1;

        if (board.at(i).at(j).at(0) == 'c') {
          result += 5;
        }
        if (i + 1 > 8 || j - 1 < 0 || i - 1 < 0 || j + 1 > 8) {
          continue;
        }
        if ((board.at(i).at(j - 1).at(0) == 'b' ||
            board.at(i).at(j - 1).at(0) == 'K') &&
            board.at(i).at(j + 1) == "---")
        {
          result -= 3;
        }
        if ((board.at(i + 1).at(j).at(0) == 'b' ||
            board.at(i + 1).at(j).at(0) == 'K') &&
            board.at(i - 1).at(j) == "---")
        {
          result -= 3;
        }
        if (board.at(i - 1).at(j ).at(0) == 'K' &&
            board.at(i + 1).at(j ) == "---") {
            result -= 3;
        }

        if (board.at(i ).at(j + 1).at(0) == 'K' &&
            board.at(i ).at(j - 1) == "---") {
            result -= 3;
        }
        if (i + 2 > 8 || j - 2 < 0) {
          continue;
        }
        if ((board.at(i ).at(j - 1).at(0) == 'K' ||
            board.at(i ).at(j - 1).at(0) == 'b') &&
            board.at(i ).at(j - 2) == "---") 
        {
            result += 6;
        }
        if (i + 2 > 8 || j + 2 > 8) {
          continue;
        }
        if ((board.at(i + 1).at(j ).at(0) == 'K' ||
            board.at(i + 1).at(j ).at(0) == 'b') &&
            board.at(i + 2).at(j ) == "---") {
            result += 6;
        }

      } else if (board.at(i).at(j).at(0) == 'b' ||
          board.at(i).at(j).at(0) == 'K') {
        opp += 1;
      }
    }
  }
  return result + (mine - (opp - 1)) - 8;
}

// Поиск доступных ходов игрока
static vector<vector<int>> find_player_available_moves(Table board)
{
    // Вектор, в который будут добавляться доступные ходы в виде {int old_i, int old_j, int new_i, int new_j}
    vector<vector<int>> available_moves;

    // Перебираем каждую клетку на доске
    for (int m = 0; m < 9; m++) {
        for (int n = 0; n < 9; n++) {
            // Если на текущей клетке находится фигура игрока 'b'
            if (board.at(m).at(n).at(0) == 'b') {
                for (int c = 1; c <= 8; ++c) {
                    // Проверяем возможные ходы вверх
                    if (check_player_moves(board, m, n, m - c, n))
                        available_moves.push_back({ m, n, m - c, n });
                    // Проверяем возможные ходы вниз
                    if (check_player_moves(board, m, n, m + c, n))
                        available_moves.push_back({ m, n, m + c, n });
                    // Проверяем возможные ходы влево
                    if (check_player_moves(board, m, n, m, n - c))
                        available_moves.push_back({ m, n, m, n - c });
                    // Проверяем возможные ходы вправо
                    if (check_player_moves(board, m, n, m, n + c))
                        available_moves.push_back({ m, n, m, n + c });
                }
            }
            else if (board.at(m).at(n).at(0) == 'K') {
                for (int c = 1; c <= 3; ++c) {
                    // Проверяем возможные ходы вверх
                    if (check_player_moves(board, m, n, m - c, n))
                        available_moves.push_back({ m, n, m - c, n });
                    // Проверяем возможные ходы вниз
                    if (check_player_moves(board, m, n, m + c, n))
                        available_moves.push_back({ m, n, m + c, n });
                    // Проверяем возможные ходы влево
                    if (check_player_moves(board, m, n, m, n - c))
                        available_moves.push_back({ m, n, m, n - c });
                    // Проверяем возможные ходы вправо
                    if (check_player_moves(board, m, n, m, n + c))
                        available_moves.push_back({ m, n, m, n + c });
                }
            }
        }
    }
    return available_moves;
}

class Node {
 private:
  Table board;
  int value;
  vector<int> move;
  Node* parent;

 public:
  Node() {
    this->board = Table();
    this->move = vector<int>();
    this->parent = nullptr;
    this->value = 0;
  }
  Node(Table board, std::vector<int> move = {}, Node* parent = nullptr,
       int value = 0) {
    this->board = Table(board);
    this->move = move;
    this->parent = parent;
    this->value = value;
  }

  // Генерирует все возможные состояния игры для текущего состояния
  vector<Node> get_children(bool minimizing_player) {
      vector<vector<int>> available_moves;              // Вектор для хранения доступных ходов
      vector<Node> children_states;                     // Вектор для хранения дочерних состояний
      Table current_state(board);                       // Создаем объект текущего состояния игры

      string big_letter;                                // Переменная для хранения большой буквы, обозначающей фишку

      // Определяем доступные ходы в зависимости от текущего игрока
      if (minimizing_player == true) {
          available_moves = find_available_moves(current_state);
          big_letter = 'C'; // Фишки компьютера обозначаются 'C'
      }
      else {
          available_moves = find_player_available_moves(current_state);
          big_letter = 'K'; // Фишки игрока обозначаются 'K'
      }

      // Проходим по всем доступным ходам и создаем дочернее состояние для каждого хода
      for (int i = 0; i < available_moves.size(); i++) {
          // Получение координат для данного хода из вектора ходов available_moves
          int old_i = available_moves.at(i).at(0);
          int old_j = available_moves.at(i).at(1);
          int new_i = available_moves.at(i).at(2);
          int new_j = available_moves.at(i).at(3);

          Table state(current_state);                                           // Создаем копию текущего состояния
          make_a_move(&state, old_i, old_j, new_i, new_j, big_letter.at(0));    // Выполняем ход
          vector<int> new_move = { old_i, old_j, new_i, new_j };                // Создаем вектор с координатами хода
          Node child(state, new_move, this);                                    // Создаем дочернее состояние с новым игровым полем и ходом
          children_states.push_back(child);                                     // Добавляем дочернее состояние в вектор
      }

      return children_states; // Возвращаем вектор дочерних состояний
  }


  void set_value(int value) { this->value = value; }

  int get_value() { return this->value; }

  vector<int> get_move() { return move; }

  Table get_board() { return this->board; }

  Node* get_parent() { return this->parent; }

  void set_parent(Node* parent) { this->parent = parent; }
};

bool IsEndGame(Table matrix) {
    int computer_pieces = 16;
    int player_pieces = 8;
    int king = 1;
    for (int m = 0; m < 9; m++)
    {
        for (int n = 0; n < 9; n++)
        {
            if (matrix.at(m).at(n).at(0) == 'c') {
                computer_pieces++;
            }
            else if (matrix.at(m).at(n).at(0) == 'b') {
                player_pieces++;
            }
            else if (matrix.at(m).at(n).at(0) == 'K') {
                king++;
            };
        }
    }
    if (king == 0 || player_pieces == 0 || computer_pieces == 0) {
        return true;
    }
    else return false;
}

int minimax(Table board, int depth, int alpha, int beta, bool maximizing_player) {
    if (depth == 0 || IsEndGame(board)) {
        return evaluation_function(board);
    }
    Node current_state(board);
    if (maximizing_player) {
        int max_eval = -INT_MAX;
        for (Node child : current_state.get_children(true)) {
            int ev = minimax(child.get_board(), depth - 1, alpha, beta, false);
            max_eval = std::max(max_eval, ev);
            alpha = std::max(alpha, ev);
            if (beta <= alpha) {
                break;
            }
        }
        current_state.set_value(max_eval);
        return max_eval;
    }
    else {
        int min_eval = INT_MAX;
        for (Node child : current_state.get_children(false)) {
            int ev = minimax(child.get_board(), depth - 1, alpha, beta, true);
            min_eval = std::min(min_eval, ev);
            beta = std::min(beta, ev);
            if (beta <= alpha) {
                break;
            }
        }
        current_state.set_value(min_eval);
        return min_eval;
    }
}

class Checkers {
private:
    Table matrix = { {}, {}, {}, {}, {}, {}, {}, {}, {} }; // Создание пустой матрицы 9x9 для игрового поля
    bool player_turn = true;               // Переменная, указывающая, чей сейчас ход (true - ход игрока, false - ход компьютера)
    int computer_pieces = 16;              // Количество фишек у компьютера в начале игры
    int player_pieces = 8;                 // Количество фишек у игрока в начале игры
    int king = 1;                          // Переменная, указывающая, есть ли король на игровом поле (1 - есть, 0 - нет)
    vector<Table> available_moves;         // Вектор, содержащий доступные ходы для текущего игрока
    bool mandatory_jumping = false;        // Переменная, указывающая, есть ли обязательные прыжки для текущего игрока
    int depth = 1;                         // Глубина поиска для алгоритма принятия решений компьютера (уровень сложности)


public:
    Checkers() {
        for (vector<string>& row : matrix) {
            for (int i = 0; i < 9; i++) {
                row.push_back("---");
            }
        }
        position_computer();
        position_player();
        king1();
        exitplay();
    }

    Checkers(int depth) : Checkers() { this->depth = depth; }

    void position_computer() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                matrix.at(4).at(1) = ('c' + std::to_string(i) + std::to_string(j));    //расстановка фишек врага
                matrix.at(4).at(0) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(5).at(0) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(3).at(0) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(0).at(4) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(0).at(5) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(0).at(3) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(1).at(4) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(8).at(5) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(8).at(3) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(8).at(4) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(7).at(4) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(4).at(8) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(4).at(7) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(3).at(8) = ('c' + std::to_string(i) + std::to_string(j));
                matrix.at(5).at(8) = ('c' + std::to_string(i) + std::to_string(j));
            }
        }
    }
    void position_player() {
        for (int i = 2; i < 7; i++) {
            for (int j = 2; j < 7; j++) {
                matrix.at(4).at(5) = ('b' + std::to_string(i) + std::to_string(j));         //расстановка фишек игрока
                matrix.at(4).at(6) = ('b' + std::to_string(i) + std::to_string(j));
                matrix.at(4).at(3) = ('b' + std::to_string(i) + std::to_string(j));
                matrix.at(4).at(2) = ('b' + std::to_string(i) + std::to_string(j));
                matrix.at(3).at(4) = ('b' + std::to_string(i) + std::to_string(j));
                matrix.at(2).at(4) = ('b' + std::to_string(i) + std::to_string(j));
                matrix.at(5).at(4) = ('b' + std::to_string(i) + std::to_string(j));
                matrix.at(6).at(4) = ('b' + std::to_string(i) + std::to_string(j));
            }
        }
    }

    void king1() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                matrix.at(4).at(4) = ('K' + std::to_string(i) + std::to_string(j));
            }
        }
    }

    void exitplay() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                matrix.at(0).at(0) = ('X' + std::to_string(i) + std::to_string(j));
                matrix.at(0).at(8) = ('X' + std::to_string(i) + std::to_string(j));         //расстановка выходов
                matrix.at(8).at(0) = ('X' + std::to_string(i) + std::to_string(j));
                matrix.at(8).at(8) = ('X' + std::to_string(i) + std::to_string(j));
            }
        }
    }
    // Отрисовка поля
    void print_matrix()
    {
        int i = 0;
        std::cout << std::endl;
        for (auto row : this->matrix) {
            std::cout << i << "  |";
            i += 1;
            for (auto elem : row) {
                if (elem.at(0) == 'c') {
                    std::cout << ansi_red << elem << ansi_reset << " ";
                }
                else if (elem.at(0) == 'b') {
                    std::cout << ansi_green << elem << ansi_reset << " ";
                }
                else if (elem.at(0) == 'K') {
                    std::cout << ansi_yellow << elem << ansi_reset << " ";
                }
                else {
                    std::cout << elem << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        for (int j = 0; j < 9; j++) {
            string temp = std::to_string(j);
            if (j == 0) {
                temp = "     0";
            }
            std::cout << temp << "   ";
        }
        std::cout << std::endl;
    }

    // Доступные ходы игрока
    void get_player_input()
    {
        // Находим доступные ходы для игрока на текущем игровом поле. Получен вектор ходов (мтарый 
        vector<vector<int>> available_moves = find_player_available_moves(matrix);

        // Если доступные ходы отсутствуют
        if (available_moves.empty()) {
            // Проверяем условие поражения или окончания игры
            if (computer_pieces > player_pieces) {
                cout << ansi_red
                    << "У вас не осталось ходов, и компьютер имеет больше фишек чем вы.\n "
                    "Вы проиграли!"
                    << ansi_reset << endl;
                return;
            }
            else {
                cout << ansi_yellow << "У вас не осталось ходов.\nИгра окончена!"
                    << ansi_reset << endl;
                return;
            }
        }

        // Обнуляем количество фишек и короля у игрока и компьютера
        player_pieces = 0;
        computer_pieces = 0;
        king = 0;

        // Бесконечный цикл для ввода ходов от игрока
        while (true) {
            string coord1, coord2;
            int old_i, old_j, new_i, new_j;
            cout << "Выберите фишку которой хотите сходить[i,j]: ";
            cin >> coord1;

            // Обработка ввода сдачи игры
            if (coord1 == "") {
                cout << ansi_cyan << "Игра окончена!" << ansi_reset << endl;
                return;
            }
            else if (coord1 == "s") {
                cout << ansi_cyan << "Вы сдались." << ansi_reset << endl;
                return;
            }

            cout << "Выберите место куда её поставить[i,j]:";
            cin >> coord2;

            // Обработка ввода сдачи игры
            if (coord2 == "") {
                cout << ansi_cyan << "Игра окончена!" << ansi_reset << endl;
                return;
            }
            else if (coord2 == "s") {
                cout << ansi_cyan << "Вы сдались." << ansi_reset << endl;
                return;
            }

            // Разбиваем введенные координаты на составляющие
            vector<string> old_coord = split_string(coord1, ',');
            vector<string> new_coord = split_string(coord2, ',');

            // Проверка корректности ввода координат
            if (old_coord.size() != 2 || new_coord.size() != 2) {
                cout << ansi_red << "Неправильный ввод" << ansi_reset << endl;
                continue;
            }

            try {
                old_i = stoi(old_coord.at(0));
                old_j = stoi(old_coord.at(1));
                new_i = stoi(new_coord.at(0));
                new_j = stoi(new_coord.at(1));
            }
            catch (const std::exception& e) {
                cout << ansi_red << "Неправильный ввод" << ansi_reset << endl;
                continue;
            }

            vector<int> move = { old_i, old_j, new_i, new_j };

            // Проверка корректности хода
            if (find(available_moves.begin(), available_moves.end(), move) == available_moves.end()) {
                cout << ansi_red << "Неправильный ход!" << ansi_reset << endl;
                continue;
            }

            // Выполнение хода игрока
            make_a_move(&matrix, old_i, old_j, new_i, new_j, 'K');

            // Обновление количества фишек и короля у игрока и компьютера
            for (int m = 0; m < 9; m++) {
                for (int n = 0; n < 9; n++) {
                    if (matrix.at(m).at(n).at(0) == 'c') {
                        computer_pieces++;
                    }
                    else if (matrix.at(m).at(n).at(0) == 'b') {
                        player_pieces++;
                    }
                    else if (matrix.at(m).at(n).at(0) == 'K') {
                        king++;
                    };
                }
            }

            break; // Выход из цикла после успешного хода
        }
    }

    // Просчёт ходов ПК
    void evaluate_states()
    {
        double t1 = time(NULL); // Записываем текущее время

        Node current_state(matrix); // Создаем объект текущего состояния игры

        // Получаем список ходов для компьютера
        vector<Node> first_computer_moves = current_state.get_children(true);

        // Проверяем, есть ли ходы для компьютера
        if (first_computer_moves.size() == 0) {
            if (player_pieces > computer_pieces) {
                cout << ansi_yellow + "У компьютера не осталось свободных ходов, и у вас осталось больше фишек чем у компьютера.\nВы выиграли!" + ansi_reset;
                return;
            }
            else {
                cout << ansi_yellow + "У компьютера не осталось свободных ходов.\nИгра окончена!" + ansi_reset;
                return;
            }
        }

        std::map<double, Node> dict; // Создаем словарь для оцененных состояний

        // Проходим по всем ходам компьютера
        for (int i = 0; i < first_computer_moves.size(); i++) {
            Node child = first_computer_moves.at(i);
            // Оцениваем текущее состояние с использованием алгоритма минимакса
            double value = minimax(child.get_board(), depth, -INT_MAX, INT_MAX, false);
            dict.insert(std::pair<double, Node>(value, child)); // Добавляем оцененное состояние в словарь

        }

        // Проверяем, получился ли пустой словарь
        if (dict.empty()) {
            cout << ansi_green + "Компьютер загнал себя в угол.\nВы выиграли!" +
                ansi_reset;
            return;
        }

        // Получаем новое состояние игрового поля и ход компьютера из словаря
        Table new_board = dict.rbegin()->second.get_board();
        vector<int> move = dict.rbegin()->second.get_move();

        matrix = new_board; // Обновляем игровое поле

        double t2 = time(NULL); // Записываем текущее время
        double diff = difftime(t2, t1); // Вычисляем разницу времени выполнения

        // Выводим информацию о ходе компьютера и времени выполнения
        cout << "Компьютер сходил с (" << move.at(0) << "," << move.at(1)
            << ") на (" << move.at(2) << "," << move.at(3) << ").\n";
        cout << "У него заняло " << diff << " секунд.\n";
    }


    // Функция игры
    void play() {
        cout << ansi_cyan << "##### Оберег ####" << ansi_reset << endl;
        cout << "\nПравила:" << endl;
        cout << "1.Для хода вы заполняете координаты в форме i,j." << endl;
        cout << "2.Вы можете сдаться в любой момент, для эттого нажмите 's'." << endl;
        cout << endl;

        //cout << "Выбрана глубина (уровень сложности): " << depth << endl;

        while (true) {
            print_matrix();                                                   // Выводит текущее состояние поля

            if (player_turn == true) {                                        // Если ход игрока
                cout << ansi_cyan << "\nВаш ход." << ansi_reset << endl;
                get_player_input();                                           // Получает координаты хода от игрока
            }
            else {                                                            // Если ход компьютера
                cout << ansi_cyan << "Ход компьютера." << ansi_reset << endl;
                cout << "Думаю..." << endl;
                evaluate_states();                                            // Оценивает возможные состояния игры и делает ход компьютер
            }

            if (king == 0 || player_pieces == 0) {                            // Если у игрока не осталось короля
                print_matrix();
                cout << ansi_red << "Вы проиграли!" << ansi_reset << endl;
                return;
            }
            else if (computer_pieces == 0) {                                  // Если у компьютера не осталось фишек (проверка на победу)
                print_matrix();
                cout << ansi_green << "У компьютера не осталось фишек.\nВы выиграли!" << ansi_reset << endl;
                return;
            }
            player_turn = !player_turn; // Переключает ход между игроком и компьютером
        }
    }
};