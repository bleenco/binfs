#include "binfs.h"

namespace BinFS
{

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool is_base64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

BinFS::BinFS(std::string dirpath_) : dirpath(dirpath_){};

BinFS::~BinFS(){};

bool BinFS::file_exists(const std::string &filename)
{
  std::ifstream file(filename.c_str());
  return file.good();
}

std::vector<uint8_t> BinFS::read_file(const std::string &filename)
{
  std::string filepath(dirpath + filename);
  if (!file_exists(filepath))
  {
    throw std::runtime_error(filepath + " does not exists!");
  }

  std::ifstream file(filepath, std::ios::in | std::ios::binary);
  std::vector<uint8_t> contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  return contents;
}

std::string BinFS::base64_encode(unsigned char const *bytes_to_encode, unsigned int in_len)
{
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--)
  {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3)
    {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      ret += '=';
  }

  return ret;
}

std::string BinFS::base64_decode(std::string const &encoded_string)
{
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
  {
    char_array_4[i++] = encoded_string[in_];
    in_++;
    if (i == 4)
    {
      for (i = 0; i < 4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i)
  {
    for (j = i; j < 4; j++)
      char_array_4[j] = 0;

    for (j = 0; j < 4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++)
      ret += char_array_3[j];
  }

  return ret;
}

void BinFS::compress(void *in_data, size_t in_data_size, std::vector<uint8_t> &out_data)
{
  std::vector<uint8_t> buffer;

  const size_t BUFSIZE = 128 * 1024;
  uint8_t temp_buffer[BUFSIZE];

  z_stream strm;
  strm.zalloc = 0;
  strm.zfree = 0;
  strm.next_in = reinterpret_cast<uint8_t *>(in_data);
  strm.avail_in = in_data_size;
  strm.next_out = temp_buffer;
  strm.avail_out = BUFSIZE;

  deflateInit(&strm, Z_BEST_COMPRESSION);

  while (strm.avail_in != 0)
  {
    int res = deflate(&strm, Z_NO_FLUSH);
    assert(res == Z_OK);
    if (strm.avail_out == 0)
    {
      buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
      strm.next_out = temp_buffer;
      strm.avail_out = BUFSIZE;
    }
  }

  int deflate_res = Z_OK;
  while (deflate_res == Z_OK)
  {
    if (strm.avail_out == 0)
    {
      buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
      strm.next_out = temp_buffer;
      strm.avail_out = BUFSIZE;
    }
    deflate_res = deflate(&strm, Z_FINISH);
  }

  assert(deflate_res == Z_STREAM_END);
  buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
  deflateEnd(&strm);

  out_data.swap(buffer);
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
  std::vector<uint8_t> contents = read_file(filename);
  std::vector<uint8_t> cdata;

  compress(contents.data(), contents.size(), cdata);
  files.emplace_back(filename, base64_encode(cdata.data(), cdata.size()));
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
      return decompress(base64_decode(file.second));
    }
    it++;
  }

  throw std::runtime_error(filename + " not found!");
}

void BinFS::output_hpp_file(const std::string &filename)
{
  std::ofstream out;
  out.open(filename);

  out << ""
  "#ifndef _BINFS_OUTPUT_HPP_\n"
  "#define _BINFS_OUTPUT_HPP_\n\n";

  out << "// This file is auto-generated by BinFS" << std::endl;
  out << "// Included files:" << std::endl;

  for (const std::pair<std::string, std::string> &file : files)
  {
    out << "// " << file.first << std::endl;
  }

  out << "\n"
  "#include <string>\n"
  "#include <vector>\n"
  "#include <iostream>\n"
  "#include <fstream>\n"
  "#include <sstream>\n"
  "#include <iomanip>\n"
  "#include <zlib.h>\n\n"
  "namespace BinFS\n"
  "{\n"
  "\n"
  "class BinFS\n"
  "{\n"
  "private:\n"
  "  std::vector<std::pair<std::string, std::string>> files;\n"
  "\n"
  "  std::string base64_chars =\n"
  "      \"ABCDEFGHIJKLMNOPQRSTUVWXYZ\"\n"
  "      \"abcdefghijklmnopqrstuvwxyz\"\n"
  "      \"0123456789+/\";\n"
  "\n"
  "  bool is_base64(unsigned char c)\n"
  "  {\n"
  "    return (isalnum(c) || (c == '+') || (c == '/'));\n"
  "  }\n"
  "\n"
  "  std::string decompress(const std::string &in)\n"
  "  {\n"
  "    z_stream z;\n"
  "    memset(&z, 0, sizeof(z));\n"
  "\n"
  "    if (inflateInit(&z) != Z_OK)\n"
  "    {\n"
  "      throw(std::runtime_error(\"inflateInit failed while decompressing.\"));\n"
  "    }\n"
  "\n"
  "   z.next_in = (Bytef *)in.data();\n"
  "   z.avail_in = in.size();\n"
  "\n"
  "   int ret;\n"
  "   char outbuffer[32768];\n"
  "   std::string outstring;\n"
  "\n"
  "   do\n"
  "   {\n"
  "     z.next_out = reinterpret_cast<Bytef *>(outbuffer);\n"
  "     z.avail_out = sizeof(outbuffer);\n"
  "\n"
  "      ret = inflate(&z, 0);\n"
  "\n"
  "      if (outstring.size() < z.total_out)\n"
  "      {\n"
  "        outstring.append(outbuffer, z.total_out - outstring.size());\n"
  "      }\n"
  "\n"
  "    } while (ret == Z_OK);\n"
  "\n"
  "    inflateEnd(&z);\n"
  "\n"
  "    if (ret != Z_STREAM_END)\n"
  "    {\n"
  "      std::ostringstream oss;\n"
  "      oss << \"Exception during zlib decompression: (\" << ret << \") \" << z.msg;\n"
  "      throw(std::runtime_error(oss.str()));\n"
  "    }\n"
  "\n"
  "    return outstring;\n"
  "  }\n"
  "  std::string base64_decode(const std::string &encoded_string)\n"
  "  {\n"
  "    int in_len = encoded_string.size();\n"
  "    int i = 0;\n"
  "    int j = 0;\n"
  "    int in_ = 0;\n"
  "    unsigned char char_array_4[4], char_array_3[3];\n"
  "    std::string ret;\n"
  "\n"
  "    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))\n"
  "    {\n"
  "      char_array_4[i++] = encoded_string[in_];\n"
  "      in_++;\n"
  "      if (i == 4)\n"
  "      {\n"
  "        for (i = 0; i < 4; i++)\n"
  "        char_array_4[i] = base64_chars.find(char_array_4[i]);\n"
  "        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);\n"
  "        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);\n"
  "        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];\n"
  "\n"
  "        for (i = 0; (i < 3); i++)\n"
  "          ret += char_array_3[i];\n"
  "        i = 0;\n"
  "      }\n"
  "    }\n"
  "\n"
  "    if (i)\n"
  "    {\n"
  "      for (j = i; j < 4; j++)\n"
  "        char_array_4[j] = 0;\n"
  "\n"
  "      for (j = 0; j < 4; j++)\n"
  "        char_array_4[j] = base64_chars.find(char_array_4[j]);\n"
  "\n"
  "      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);\n"
  "      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);\n"
  "      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];\n"
  "\n"
  "      for (j = 0; (j < i - 1); j++)\n"
  "        ret += char_array_3[j];\n"
  "    }\n"
  "\n"
  "    return ret;\n"
  "  }\n"
  "public:\n"
  "  void init()\n"
  "  {\n";
  for (const std::pair<std::string, std::string> &file : files)
  {
    out << "    files.emplace_back(\"" << file.first << "\",\"" << file.second << "\");" << std::endl;
  }
  out << ""
  "  }\n"
  "  std::string get_file(const char *filename)\n"
  "  {\n"
  "    for (const std::pair<std::string, std::string> &file : files)\n"
  "    {\n"
  "      if (file.first == filename)\n"
  "      {\n"
  "        return decompress(base64_decode(file.second));\n"
  "      }\n"
  "    }\n"
  "    std::string fname(filename);\n"
  "    throw(std::runtime_error(fname + \" not found!\"));\n"
  "  }\n"
  "};\n"
  "\n"
  "} // BinFS\n"
  "\n"
  "#endif\n";
}

} // BinFS
