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

  // BinFS::BinFS *binfs = new BinFS::BinFS();
  // binfs->init();
  // std::string out = binfs->get_file("CMakeLists.txt");
  // std::ofstream output("out", std::fstream::binary);
  // output << out;


  // std::string filename(argv[1], strlen(argv[1]));
  // BinFS::BinFS *binfs = new BinFS::BinFS();

  // binfs->add_file(filename);

  // binfs->output_hpp_file("include/output.hpp");

  // std::string out = binfs->get_file(filename);

  // std::ofstream output("out", std::fstream::binary);
  // output << out;

  return 0;
}
