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

string ansi_black = "\u001b[30m";
string ansi_red = "\u001b[31m";
string ansi_green = "\u001b[32m";
string ansi_yellow = "\u001b[33m";
string ansi_blue = "\u001b[34m";
string ansi_magenta = "\u001b[35m";
string ansi_cyan = "\u001b[36m";
string ansi_white = "\u001b[37m";
string ansi_reset = "\u001b[0m";

// ��������� ������� ��������� �� ������
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

//�������� ���� ������
bool check_player_moves(Table board, int old_i, int old_j, int new_i, int new_j)
{
    if (new_i > 8 || new_i < 0) {
        return false;
    }
    if (new_j > 8 || new_j < 0) {
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
    if (board.at(old_i).at(old_j).at(0)=='K'&&( board[0][0][0] == 'X' || board[0][8][0] == 'X' 
                                             || board[8][0][0] == 'X' || board[8][8][0] == 'X')) {
        return true;
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
    if (board.at(new_i).at(new_j).at(0) == 'X') {
        return true;
    }

    if (board.at(new_i).at(new_j) == "---") {
        return true;
    }
}
// ����� ��������� ����� ������
static vector<vector<int>> find_player_available_moves(Table board)
{
    // ������, � ������� ����� ����������� ��������� ���� � ���� {int old_i, int old_j, int new_i, int new_j}
    vector<vector<int>> available_moves;

    // ���������� ������ ������ �� �����
    for (int m = 0; m < 9; m++) {
        for (int n = 0; n < 9; n++) {
            // ���� �� ������� ������ ��������� ������ ������ 'b'
            if (board.at(m).at(n).at(0) == 'b') {
                for (int c = 1; c <= 8; ++c) {
                    // ��������� ��������� ���� �����
                    if (check_player_moves(board, m, n, m - c, n))
                        available_moves.push_back({ m, n, m - c, n });
                    // ��������� ��������� ���� ����
                    if (check_player_moves(board, m, n, m + c, n))
                        available_moves.push_back({ m, n, m + c, n });
                    // ��������� ��������� ���� �����
                    if (check_player_moves(board, m, n, m, n - c))
                        available_moves.push_back({ m, n, m, n - c });
                    // ��������� ��������� ���� ������
                    if (check_player_moves(board, m, n, m, n + c))
                        available_moves.push_back({ m, n, m, n + c });
                }
            }
            else if (board.at(m).at(n).at(0) == 'K') {
                for (int c = 1; c <= 3; ++c) {
                    // ��������� ��������� ���� �����
                    if (check_player_moves(board, m, n, m - c, n))
                        available_moves.push_back({ m, n, m - c, n });
                    // ��������� ��������� ���� ����
                    if (check_player_moves(board, m, n, m + c, n))
                        available_moves.push_back({ m, n, m + c, n });
                    // ��������� ��������� ���� �����
                    if (check_player_moves(board, m, n, m, n - c))
                        available_moves.push_back({ m, n, m, n - c });
                    // ��������� ��������� ���� ������
                    if (check_player_moves(board, m, n, m, n + c))
                        available_moves.push_back({ m, n, m, n + c });
                }
            }
        }
    }
    return available_moves;
}

// ��������� ����
void make_a_move(Table* board, int old_i, int old_j, int new_i, int new_j)
{
    // �������� ��� �����
    char letter = (*board).at(old_i).at(old_j).at(0);
    // ���������� ����� �� ����� ������
    (*board).at(old_i).at(old_j) = "---";
    (*board).at(new_i).at(new_j) = letter + std::to_string(new_i) + std::to_string(new_j);

    // �������� ��� ������
    if (letter == 'K' || letter == 'b') {
        // �������� ����� � ������
        if (new_j - 1 >= 0 && ((*board)[new_i][new_j - 1][0] == 'c' || (*board)[new_i][new_j - 1][0] == 'X') &&
            new_j + 1 < 9 && ((*board)[new_i][new_j + 1][0] == 'c' || (*board)[new_i][new_j + 1][0] == 'X')) {
            (*board).at(new_i).at(new_j) = "---";
            return;
        }
        // �������� ������ � �����
        if (new_i - 1 >= 0 && ((*board)[new_i - 1][new_j][0] == 'c' || (*board)[new_i - 1][new_j][0] == 'X') &&
            new_i + 1 < 9 && ((*board)[new_i + 1][new_j][0] == 'c' || (*board)[new_i + 1][new_j][0] == 'c')) {
            (*board).at(new_i).at(new_j) = "---";
            return;
        }
        // �������� �����
        if (new_j - 1 >= 0 && (*board)[new_i][new_j - 1][0] == 'c') {
            if (new_j - 2 >= 0 &&  ((*board)[new_i][new_j - 2][0] == 'K' ||
                                    (*board)[new_i][new_j - 2][0] == 'b' ||
                                    (*board)[new_i][new_j - 2][0] == 'X')) {
                (*board)[new_i][new_j - 1]= "---";
            }
        }
        // �������� ������
        if (new_j + 1 < 9 && (*board)[new_i][new_j + 1][0] == 'c') {
            if (new_j + 2 < 9 &&  ((*board)[new_i][new_j + 2][0] == 'K' 
                                || (*board)[new_i][new_j + 2][0] == 'b' 
                                || (*board)[new_i][new_j + 2][0] == 'X')) {
                (*board)[new_i][new_j + 1] = "---";
            }
        }
        // �������� ������
        if (new_i - 1 >= 0 && (*board)[new_i - 1][new_j][0] == 'c') {
            if (new_i - 2 >= 0 && ((*board)[new_i - 2][new_j][0] == 'K' 
                                || (*board)[new_i - 2][new_j][0] == 'b' 
                                || (*board)[new_i - 2][new_j][0] == 'X')) {
                (*board)[new_i-1][new_j] = "---";
            }
        }
        // �������� �����
        if (new_i + 1 < 9 && (*board)[new_i + 1][new_j][0] == 'c') {
            if (new_i + 2 < 9 &&  ((*board)[new_i + 2][new_j][0] == 'K' 
                                || (*board)[new_i + 2][new_j][0] == 'b' 
                                || (*board)[new_i + 2][new_j][0] == 'X')) {
                (*board)[new_i + 1][new_j] = "---";
            }
        }
    }

    // �������� ��� ����������
    if (letter == 'c') {
        // �������� ����� � ������
        if (new_j - 1 >= 0 && ((*board)[new_i][new_j - 1][0] == 'K' 
                            || (*board)[new_i][new_j - 1][0] == 'b'
                            || (*board)[new_i][new_j - 1][0] == 'X') &&
            new_j + 1 < 9 && ((*board)[new_i][new_j + 1][0] == 'K' 
                           || (*board)[new_i][new_j + 1][0] == 'b'
                           || (*board)[new_i][new_j + 1][0] == 'X')) {
            (*board).at(new_i).at(new_j) = "---";
            return;
        }
        // �������� ������ � �����
        if (new_i - 1 >= 0 && ((*board)[new_i - 1][new_j][0] == 'K' 
                            || (*board)[new_i - 1][new_j][0] == 'b' 
                            || (*board)[new_i - 1][new_j][0] == 'X') 
            && new_i + 1 < 9 && ((*board)[new_i + 1][new_j][0] == 'K' 
                              || (*board)[new_i + 1][new_j][0] == 'b'
                              || (*board)[new_i + 1][new_j][0] == 'X')) {
            (*board).at(new_i).at(new_j) = "---";
            return;
        }

        // �������� �����
        if (new_j - 1 >= 0 && ((*board)[new_i][new_j - 1][0] == 'K' || (*board)[new_i][new_j - 1][0] == 'b')) {
            if (new_j - 2 >= 0 && ((*board)[new_i][new_j - 2][0] == 'c' || (*board)[new_i][new_j - 2][0] == 'X')) {
                (*board)[new_i][new_j - 1] = "---";
            }
        }
        // �������� ������
        if (new_j + 1 < 9 && ((*board)[new_i][new_j + 1][0] == 'K' || (*board)[new_i][new_j + 1][0] == 'b')) {
            if (new_j + 2 < 9 && ((*board)[new_i][new_j + 2][0] == 'c' || (*board)[new_i][new_j + 2][0] == 'X')) {
                (*board)[new_i][new_j + 1] = "---";
            }
        }

        // �������� ������
        if (new_i - 1 >= 0 && ((*board)[new_i - 1][new_j][0] == 'K' || (*board)[new_i - 1][new_j][0] == 'b')) {
            if (new_i - 2 >= 0 && ((*board)[new_i - 2][new_j][0] == 'c' || (*board)[new_i - 2][new_j][0] == 'X')) {
                (*board)[new_i - 1][new_j] = "---";
            }
        }

        // �������� �����
        if (new_i + 1 < 9 && ((*board)[new_i + 1][new_j][0] == 'K' || (*board)[new_i + 1][new_j][0] == 'b')) {
            if (new_i + 2 < 9 && ((*board)[new_i + 2][new_j][0] == 'c' || (*board)[new_i + 2][new_j][0] == 'X')) {
                (*board)[new_i + 1][new_j] = "---";
            }
        }
    }

}

// �������� ���� ��
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
// ����� ��������� ����� ��
vector<vector<int>> find_available_moves(Table board) 
{
    // ������, � ������� ����� ����������� ��������� ���� � ���� {int old_i, int old_j, int new_i, int new_j}
    vector<vector<int>> available_moves;

    // ���������� ������ ������ �� �����
    for (int m = 0; m < 9; m++) {
        for (int n = 0; n < 9; n++) {
            // ���� �� ������� ������ ��������� ������ ������ 'c'
            if (board.at(m).at(n).at(0) == 'c') {
                for (int c = 1; c <= 8; ++c) {
                    // ��������� ��������� ���� �����
                    if (check_moves(board, m, n, m - c, n))
                        available_moves.push_back({ m, n, m - c, n });
                    // ��������� ��������� ���� ����
                    if (check_moves(board, m, n, m + c, n))
                        available_moves.push_back({ m, n, m + c, n });
                    // ��������� ��������� ���� �����
                    if (check_moves(board, m, n, m, n - c))
                        available_moves.push_back({ m, n, m, n - c });
                    // ��������� ��������� ���� ������
                    if (check_moves(board, m, n, m, n + c))
                        available_moves.push_back({ m, n, m, n + c });
                }
            }
        }
    }
    return available_moves;
}

// ������� ������� �����
void count_checker(Table board, int& chips_computer, int& chips_player, int& king_exit) {
    // ���������� ���������� ����� � ������ � ������ � ����������
    for (int m = 0; m < 9; m++) {
        for (int n = 0; n < 9; n++) {
            if (board.at(m).at(n).at(0) == 'c') {
                chips_computer++;
            }
            else if (board.at(m).at(n).at(0) == 'b') {
                chips_player++;
            }
            else if (board.at(m).at(n).at(0) == 'X') {
                if (m == 0 && n == 0) {
                    if (!(board[m + 1][n] != "---" && board[m][n + 1] != "---")) { king_exit++; }
                }
                else if (m == 9 - 1 && n == 9 - 1) {
                    if (!(board[m - 1][n] != "---" && board[m][n - 1] != "---")) { king_exit++; }
                }
                else if (m == 9 - 1 && n == 0) {
                    if (!(board[m - 1][n] != "---" && board[m][n + 1] != "---")) { king_exit++; }
                }
                else if (m == 0 && n == 9 - 1) {
                    if (!(board[m + 1][n] != "---" && board[m][n - 1] != "---")) { king_exit++; }
                }

            };
        }
    }
}
// ��������� �������
int evaluation_function(Table board)
{
    int chips_computer = 0, chips_player = 0, king_exit = 0;
    count_checker(board, chips_computer, chips_player, king_exit);
    return (chips_computer - chips_player) - (2 * king_exit);
}

class Node {
 private:
  Table board;      //������� ��������� ����
  int value;        
  vector<int> move; //������ ����
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

  // ���������� ��� ��������� ��������� ���� ��� �������� ���������
  vector<Node> get_children(bool minimizing_player) {
      vector<vector<int>> available_moves;              // ������ ��� �������� ��������� �����
      vector<Node> children_states;                     // ������ ��� �������� �������� ���������
      Table current_state(board);                       // ������� ������ �������� ��������� ����

      string big_letter;                                // ���������� ��� �������� ������� �����, ������������ �����

      // ���������� ��������� ���� � ����������� �� �������� ������
      if (minimizing_player == true) {
          available_moves = find_available_moves(current_state);
          big_letter = 'C'; // ����� ���������� ������������ 'C'
      }
      else {
          available_moves = find_player_available_moves(current_state);
          big_letter = 'K'; // ����� ������ ������������ 'K'
      }

      // �������� �� ���� ��������� ����� � ������� �������� ��������� ��� ������� ����
      for (int i = 0; i < available_moves.size(); i++) {
          // ��������� ��������� ��� ������� ���� �� ������� ����� available_moves
          int old_i = available_moves.at(i).at(0);
          int old_j = available_moves.at(i).at(1);
          int new_i = available_moves.at(i).at(2);
          int new_j = available_moves.at(i).at(3);

          Table state(current_state);                                    // ������� ����� �������� ���������
          make_a_move(&state, old_i, old_j, new_i, new_j);               // ��������� ���
          vector<int> new_move = { old_i, old_j, new_i, new_j };         // ������� ������ � ������������ ����
          Node child(state, new_move, this);                   // ������� ��������� � ����� ������� ����� � �����
          children_states.push_back(child);                              // ��������� �������� ��������� � ������
      }

      return children_states; // ���������� ������ �������� ���������
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
    Table matrix = { {}, {}, {}, {}, {}, {}, {}, {}, {} }; // �������� ������ ������� 9x9 ��� �������� ����
    bool player_turn = true;               // ����������, �����������, ��� ������ ��� (true - ��� ������, false - ��� ����������)
    int computer_pieces = 16;              // ���������� ����� � ���������� � ������ ����
    int player_pieces = 8;                 // ���������� ����� � ������ � ������ ����
    int king = 1;                          // ����������, �����������, ���� �� ������ �� ������� ���� (1 - ����, 0 - ���)
    vector<Table> available_moves;         // ������, ���������� ��������� ���� ��� �������� ������
    int depth = 1;                         // ������� ������ ��� ��������� �������� ������� ���������� (������� ���������)


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
    // ����������� ����� ��
    void position_computer() {
        matrix.at(4).at(1) = ('c' + std::to_string(4) + std::to_string(1));
        matrix.at(4).at(0) = ('c' + std::to_string(4) + std::to_string(0));
        matrix.at(5).at(0) = ('c' + std::to_string(5) + std::to_string(0));
        matrix.at(3).at(0) = ('c' + std::to_string(3) + std::to_string(0));
        matrix.at(0).at(4) = ('c' + std::to_string(0) + std::to_string(4));
        matrix.at(0).at(5) = ('c' + std::to_string(0) + std::to_string(5));
        matrix.at(0).at(3) = ('c' + std::to_string(0) + std::to_string(3));
        matrix.at(1).at(4) = ('c' + std::to_string(1) + std::to_string(4));
        matrix.at(8).at(5) = ('c' + std::to_string(8) + std::to_string(5));
        matrix.at(8).at(3) = ('c' + std::to_string(8) + std::to_string(3));
        matrix.at(8).at(4) = ('c' + std::to_string(8) + std::to_string(4));
        matrix.at(7).at(4) = ('c' + std::to_string(7) + std::to_string(4));
        matrix.at(4).at(8) = ('c' + std::to_string(4) + std::to_string(8));
        matrix.at(4).at(7) = ('c' + std::to_string(4) + std::to_string(7));
        matrix.at(3).at(8) = ('c' + std::to_string(3) + std::to_string(8));
        matrix.at(5).at(8) = ('c' + std::to_string(5) + std::to_string(8));
    }
    // ����������� ����� ������
    void position_player() {
        matrix.at(4).at(5) = ('b' + std::to_string(4) + std::to_string(5));
        matrix.at(4).at(6) = ('b' + std::to_string(4) + std::to_string(6));
        matrix.at(4).at(3) = ('b' + std::to_string(4) + std::to_string(3));
        matrix.at(4).at(2) = ('b' + std::to_string(4) + std::to_string(2));
        matrix.at(3).at(4) = ('b' + std::to_string(3) + std::to_string(4));
        matrix.at(2).at(4) = ('b' + std::to_string(2) + std::to_string(4));
        matrix.at(5).at(4) = ('b' + std::to_string(5) + std::to_string(4));
        matrix.at(6).at(4) = ('b' + std::to_string(6) + std::to_string(4));
    }
    // ���������� ������
    void king1() {
        matrix.at(4).at(4) = ('K' + std::to_string(4) + std::to_string(4));
    }
    // ���������� �������
    void exitplay() {
        matrix.at(0).at(0) = ('X' + std::to_string(0) + std::to_string(0));
        matrix.at(0).at(8) = ('X' + std::to_string(0) + std::to_string(8));
        matrix.at(8).at(0) = ('X' + std::to_string(8) + std::to_string(0));
        matrix.at(8).at(8) = ('X' + std::to_string(8) + std::to_string(8));
    }
    // ��������� ����
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

    // ��� ������
    void get_player_input()
    {
        // ������� ��������� ���� ��� ������ �� ������� ������� ����. ������� ������ ����� (������ 
        vector<vector<int>> available_moves = find_player_available_moves(matrix);

        // ���� ��������� ���� �����������
        if (available_moves.empty()) {
            // ��������� ������� ��������� ��� ��������� ����
            if (computer_pieces > player_pieces) {
                cout << ansi_red
                    << "� ��� �� �������� �����, � ��������� ����� ������ ����� ��� ��.\n "
                    "�� ���������!"
                    << ansi_reset << endl;
                return;
            }
            else {
                cout << ansi_yellow << "� ��� �� �������� �����.\n���� ��������!"
                    << ansi_reset << endl;
                return;
            }
        }

        // ����������� ���� ��� ����� ����� �� ������
        while (true) {
            string coord1, coord2;
            int old_i, old_j, new_i, new_j;
            cout << "�������� ����� ������� ������ �������[i,j]: ";
            cin >> coord1;

            // ��������� �����
            if (coord1 == "s") {
                cout << ansi_cyan << "�� �������." << ansi_reset << endl;
                return;
            }

            cout << "�������� ����� ���� � ���������[i,j]:";
            cin >> coord2;

            // ��������� �����
            if (coord2 == "s") {
                cout << ansi_cyan << "�� �������." << ansi_reset << endl;
                return;
            }

            // ��������� ��������� ���������� �� ������������
            vector<string> old_coord = split_string(coord1, ',');
            vector<string> new_coord = split_string(coord2, ',');

            // �������� ������������ ����� ���������
            if (old_coord.size() != 2 || new_coord.size() != 2) {
                cout << ansi_red << "������������ ����" << ansi_reset << endl;
                continue;
            }

            try {
                old_i = stoi(old_coord.at(0));
                old_j = stoi(old_coord.at(1));
                new_i = stoi(new_coord.at(0));
                new_j = stoi(new_coord.at(1));
            }
            catch (const std::exception& e) {
                cout << ansi_red << "������������ ����" << ansi_reset << endl;
                continue;
            }

            vector<int> move = { old_i, old_j, new_i, new_j };

            // �������� ��� ����� ����� ������� �� ������ ������� move
            if (find(available_moves.begin(), available_moves.end(), move) == available_moves.end()) {
                cout << ansi_red << "������������ ���!" << ansi_reset << endl;
                continue;
            }

            // ���������� ���� ������
            make_a_move(&matrix, old_i, old_j, new_i, new_j);

            break; // ����� �� ����� ����� ��������� ����
        }
    }

    // ������� ����� ��
    void evaluate_states()
    {
        double t1 = time(NULL); // ���������� ������� �����

        Node current_state(matrix); // ������� ������ �������� ��������� ����

        // �������� ������ ����� ��� ����������
        vector<Node> first_computer_moves = current_state.get_children(true);

        // ���������, ���� �� ���� ��� ����������
        if (first_computer_moves.size() == 0) {
            if (player_pieces > computer_pieces) {
                cout << ansi_yellow + "� ���������� �� �������� ��������� �����, � � ��� �������� ������ ����� ��� � ����������.\n�� ��������!" + ansi_reset;
                return;
            }
            else {
                cout << ansi_yellow + "� ���������� �� �������� ��������� �����.\n���� ��������!" + ansi_reset;
                return;
            }
        }

        std::map<double, Node> dict; // ������� ������� ��� ��������� ���������

        // �������� �� ���� ����� ����������
        for (int i = 0; i < first_computer_moves.size(); i++) {
            Node child = first_computer_moves.at(i);
            // ��������� ������� ��������� � �������������� ��������� ���������
            double value = minimax(child.get_board(), depth, -INT_MAX, INT_MAX, false);
            dict.insert(std::pair<double, Node>(value, child)); // ��������� ��������� ��������� � �������
            
        }

        // ���������, ��������� �� ������ �������
        if (dict.empty()) {
            cout << ansi_green + "��������� ������ ���� � ����.\n�� ��������!" +
                ansi_reset;
            return;
        }

        // �������� ����� ��������� �������� ���� � ��� ���������� �� �������
        Table new_board = dict.rbegin()->second.get_board();
        vector<int> move = dict.rbegin()->second.get_move();

        matrix = new_board; // ��������� ������� ����

        double t2 = time(NULL); // ���������� ������� �����
        double diff = difftime(t2, t1); // ��������� ������� ������� ����������

        // ������� ���������� � ���� ���������� � ������� ����������
        cout << "��������� ������ � (" << move.at(0) << "," << move.at(1)
            << ") �� (" << move.at(2) << "," << move.at(3) << ").\n";
        cout << "� ���� ������ " << diff << " ������.\n";
    }


    // ������� ����
    void play() {
        cout << ansi_cyan << "##### ������ ####" << ansi_reset << endl;
        cout << "\n�������:" << endl;
        cout << "1.��� ���� �� ���������� ���������� � ����� i,j." << endl;
        cout << "2.�� ������ ������� � ����� ������, ��� ������ ������� 's'." << endl;
        cout << endl;


        while (true) {
            print_matrix();                                                   // ������� ������� ��������� ����

            if (player_turn == true) {                                        // ���� ��� ������
                cout << ansi_cyan << "\n��� ���." << ansi_reset << endl;
                get_player_input();                                           // �������� ���������� ���� �� ������
            }
            else {                                                            // ���� ��� ����������
                cout << ansi_cyan << "��� ����������." << ansi_reset << endl;
                cout << "�����..." << endl;
                evaluate_states();                                            // ��������� ��������� ��������� ���� � ������ ��� ���������
            }

            // �������� ���������� ����� � ������ � ������ � ����������
            player_pieces = 0;
            computer_pieces = 0;
            king = 0;
            // ���������� ���������� ����� � ������ � ������ � ����������
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

            if (king == 0 || player_pieces == 0) {                            // ���� � ������ �� �������� ������
                print_matrix();
                cout << ansi_red << "�� ���������!" << ansi_reset << endl;
                return;
            }
            else if (computer_pieces == 0) {                                  // ���� � ���������� �� �������� �����
                print_matrix();
                cout << ansi_green << "� ���������� �� �������� �����.\n�� ��������!" << ansi_reset << endl;
                return;
            }
            else if (matrix[0][0][0] == 'K' || matrix[0][8][0] == 'K' || matrix[8][0][0] == 'K' || matrix[8][8][0] == 'K') {
                print_matrix();
                cout << ansi_red << "�� ��������!" << ansi_reset << endl;
                return;
            }
            player_turn = !player_turn; // ����������� ��� ����� ������� � �����������
        }
    }
};