#include "tafl.hpp"

int main()
{
	setlocale(LC_ALL, "Russian");
  int depth = 1;
  Checkers checkers(depth);
  checkers.play();
}
