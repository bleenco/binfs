#include "binfs.h"

namespace BinFS
{

BinFS::BinFS(std::string dirpath_) : dirpath(dirpath_){};

BinFS::~BinFS(){};

bool BinFS::file_exists(const std::string &filename)
{
  std::ifstream file(filename.c_str());
  return file.good();
}

std::string BinFS::read_file(const std::string &filename)
{
  dirpath = dirpath == "" ? "./" : dirpath + "/";
  std::string filepath(dirpath + filename);
  if (!file_exists(filepath))
  {
    throw std::runtime_error(filepath + " does not exists!");
  }

  std::ifstream file(filepath, std::ios::in | std::ios::binary | std::ios::ate);
  std::ifstream::pos_type size;
  char *buffer = nullptr;

  if (file.is_open())
  {
    size = file.tellg();
    buffer = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();
  }

  std::string str(buffer, size);
  return str;
}

std::string BinFS::string_to_hex(const std::string &in)
{
  std::stringstream ss;

  ss << std::hex << std::setfill('0');
  for (size_t i = 0; in.length() > i; ++i)
  {
    ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
  }

  return ss.str();
}

std::string BinFS::hex_to_string(const std::string &in)
{
  std::string output;

  if ((in.length() % 2) != 0)
  {
    throw std::runtime_error("string is not valid length!");
  }

  size_t cnt = in.length() / 2;

  for (size_t i = 0; cnt > i; ++i)
  {
    uint32_t s = 0;
    std::stringstream ss;
    ss << std::hex << in.substr(i * 2, 2);
    ss >> s;

    output.push_back(static_cast<unsigned char>(s));
  }

  return output;
}

void BinFS::add_file(const std::string &filename)
{
  std::string data = read_file(filename);
  std::string hexdata = string_to_hex(data);

  files.emplace_back(filename, hexdata);
}

void BinFS::remove_file(const std::string &filename)
{
  auto it = files.begin();
  for (const std::pair<std::string, std::string> &file : files)
  {
    if (file.first == filename)
    {
      files.erase(it);
      return;
    }
    it++;
  }
}

std::string BinFS::get_file(const std::string &filename)
{
  auto it = files.begin();
  for (const std::pair<std::string, std::string> &file : files)
  {
    if (file.first == filename)
    {
      return hex_to_string(file.second);
    }
    it++;
  }

  throw std::runtime_error(filename + " not found!");
}

} // BinFS
