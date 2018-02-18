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

std::string BinFS::compress(const std::string &in)
{
  std::string output;
  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;

  output.resize(in.size() * 1.01 + 12);
  int status = Z_OK;
  status = deflateInit(&z, Z_BEST_COMPRESSION);

  if (status != Z_OK)
  {
    exit(1);
  }

  z.total_out = 0;
  z.next_in = (Bytef *)in.data();
  z.avail_in = in.size();

  do
  {
    z.next_out = (Bytef *)output.data() + z.total_out;
    z.avail_out = output.size() - z.total_out;
    status = deflate(&z, Z_FINISH);
  } while (status == Z_OK);

  output.resize(z.total_out);
  deflateEnd(&z);

  printf("-------------------------\n");
  printf("Uncompressed size: %lu\n", in.size());
  printf("Compressed size: %lu\n", z.total_out);

  return output;
}

std::string BinFS::decompress(const std::string &in)
{
  z_stream z;
  memset(&z, 0, sizeof(z));

  if (inflateInit(&z) != Z_OK)
  {
    throw(std::runtime_error("inflateInit failed while decompressing."));
  }

  z.next_in = (Bytef *)in.data();
  z.avail_in = in.size();

  int ret;
  char outbuffer[32768];
  std::string outstring;

  do
  {
    z.next_out = reinterpret_cast<Bytef *>(outbuffer);
    z.avail_out = sizeof(outbuffer);

    ret = inflate(&z, 0);

    if (outstring.size() < z.total_out)
    {
      outstring.append(outbuffer, z.total_out - outstring.size());
    }

  } while (ret == Z_OK);

  inflateEnd(&z);

  if (ret != Z_STREAM_END)
  {
    std::ostringstream oss;
    oss << "Exception during zlib decompression: (" << ret << ") " << z.msg;
    throw(std::runtime_error(oss.str()));
  }

  return outstring;
}

void BinFS::add_file(const std::string &filename)
{
  std::string data = read_file(filename);
  std::string hexdata = compress(string_to_hex(data));

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
      return decompress(hex_to_string(file.second));
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
