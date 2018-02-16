#include <string>
#include <fstream>
#include "binfs.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s input-file\n", argv[0]);
    exit(1);
  }

  std::string filename(argv[1], strlen(argv[1]));
  BinFS *binfs = new BinFS();

  binfs->add_file(filename);

  std::string out = binfs->get_file(filename);

  std::ofstream output("out", std::fstream::binary);
  output << out;

  return 0;
}
