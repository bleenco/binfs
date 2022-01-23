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
  std::string filepath = filename;
  // if dirpath is empty and filename is from the root, leave it alone
  if (!((filename[0] == '/') && (dirpath == "")))
  {
      dirpath = dirpath == "" ? "./" : dirpath + "/";
      filepath = dirpath + filename;
  }
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

void BinFS::output_hpp_file(const std::string &filename)
{
  std::ofstream out;
  out.open(filename);

  out << "#ifndef _BINFS_OUTPUT_HPP_" << std::endl;
  out << "#define _BINFS_OUTPUT_HPP_" << std::endl;
  out << std::endl;
  out << "#include <string>" << std::endl;
  out << "#include <vector>" << std::endl;
  out << "#include <iostream>" << std::endl;
  out << "#include <fstream>" << std::endl;
  out << "#include <sstream>" << std::endl;
  out << "#include <iomanip>" << std::endl;
  out << std::endl;
  out << "namespace BinFS" << std::endl;
  out << "{" << std::endl;
  out << std::endl;
  out << "class BinFS" << std::endl;
  out << "{" << std::endl;
  out << "private:" << std::endl;
  out << "  std::vector<std::pair<std::string, std::string>> files;" << std::endl;
  out << "  std::string hex_to_string(const std::string &in)" << std::endl;
  out << "  {" << std::endl;
  out << "    std::string output;" << std::endl;
  out << "    if ((in.length() % 2) != 0)" << std::endl;
  out << "    {" << std::endl;
  out << "      throw std::runtime_error(\"string is not valid length!\");" << std::endl;
  out << "    }" << std::endl;
  out << "    size_t cnt = in.length() / 2;" << std::endl;
  out << "    for (size_t i = 0; cnt > i; ++i)" << std::endl;
  out << "    {" << std::endl;
  out << "      uint32_t s = 0;" << std::endl;
  out << "      std::stringstream ss;" << std::endl;
  out << "      ss << std::hex << in.substr(i * 2, 2);" << std::endl;
  out << "      ss >> s;" << std::endl;
  out << "      output.push_back(static_cast<unsigned char>(s));" << std::endl;
  out << "    }" << std::endl;
  out << "    return output;" << std::endl;
  out << "  }" << std::endl;
  out << "public:" << std::endl;
  out << "  BinFS() {};" << std::endl;
  out << "  ~BinFS() {};" << std::endl;
  out << "  std::string get_file(const std::string &filename)" << std::endl;
  out << "  {" << std::endl;
  out << "    auto it = files.begin();" << std::endl;
  out << "    for (const std::pair<std::string, std::string> &file : files)" << std::endl;
  out << "    {" << std::endl;
  out << "      if (file.first == filename)" << std::endl;
  out << "      {" << std::endl;
  out << "        return hex_to_string(file.second);" << std::endl;
  out << "      }" << std::endl;
  out << "      it++;" << std::endl;
  out << "    }" << std::endl;
  out << "    throw std::runtime_error(filename + \" not found!\");" << std::endl;
  out << "  }" << std::endl;
  out << "  void init()" << std::endl;
  out << "  {" << std::endl;
  for (const std::pair<std::string, std::string> &file : files)
  {
    out << "    files.emplace_back(\"" << file.first << "\",\"" << file.second << "\");" << std::endl;
  }
  out << "  }" << std::endl;
  out << "};" << std::endl;
  out << std::endl;
  out << "} // BinFS" << std::endl;
  out << std::endl;
  out << "#endif // _BINFS_OUTPUT_HPP_" << std::endl;
  out << std::endl;
}

} // BinFS
