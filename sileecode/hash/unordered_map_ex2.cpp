#include <iostream>
#include <vector>
#include <unordered_map>

struct Pos
{
  int row;
  int col;

  Pos() { }
  Pos(int row, int col)
  {
    this->row = row;
    this->col = col;
  }

  bool operator==(const Pos& otherPos) const
  {
    if (this->row == otherPos.row && this->col == otherPos.col) return true;
    else return false;
  }

  struct HashFunction
  {
    size_t operator()(const Pos& pos) const
    {
      size_t rowHash = std::hash<int>()(pos.row);
      size_t colHash = std::hash<int>()(pos.col) << 1;
      return rowHash ^ colHash;
    }
  };
};

int main(void)
{
  std::unordered_map<Pos, int, Pos::HashFunction> umap;

  // at row 1, col 2, set value to 5
  umap[Pos(1, 2)] = 5;

  // at row 3, col 4, set value to 10
  umap[Pos(3, 4)] = 10;

  // print the umap
  std::cout << "\n";
  for (auto& element : umap)
  {
    std::cout << "( " << element.first.row << ", " << element.first.col << " ) = " << element.second << "\n";
  }
  std::cout << "\n";

  return 0;
}
