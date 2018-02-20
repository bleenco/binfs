#ifndef _BINFS_H_
#define _BINFS_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <assert.h>

#include "zlib.h"

namespace BinFS
{

class BinFS
{
private:
  std::string dirpath;
  std::vector<std::pair<std::string, std::string>> files;

  bool file_exists(const std::string &filename);
  std::vector<uint8_t> read_file(const std::string &filename);
  std::string base64_decode(std::string const &encoded_string);
  std::string base64_encode(unsigned char const *bytes_to_encode, unsigned int in_len);
  void compress(void *in_data, size_t in_data_size, std::vector<uint8_t> &out_data);
  std::string decompress(const std::string &in);

public:
  BinFS(std::string dirpath_ = "");
  ~BinFS();

  void add_file(const std::string &filename);
  void remove_file(const std::string &filename);
  std::string get_file(const std::string &filename);
  void output_hpp_file(const std::string &filename);
};

} // BinFS

#endif // _BINFS_H_
