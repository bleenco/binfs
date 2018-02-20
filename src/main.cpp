#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS 1
#endif

#include <string>
#include <vector>
#include <cstring>
#include <sys/stat.h>
#if !defined(WINDOWS)
#include <dirent.h>
#else
#include <3rdparty/win32/dirent.h>
#endif
#include "binfs.h"

std::vector<std::string> get_files(const std::string &path, std::vector<std::string> &paths)
{
  struct stat s;
  if (stat(path.c_str(), &s) == 0)
  {
    if (s.st_mode & S_IFREG)
    {
      paths.push_back(path);
    }
    else if (s.st_mode & S_IFDIR)
    {
      DIR *dir;
      struct dirent *ent;
      if ((dir = opendir(path.c_str())) != NULL)
      {
        while ((ent = readdir(dir)) != NULL)
        {
          std::string found(ent->d_name, strlen(ent->d_name));
          if (found != "." && found != "..")
          {
            found = path + "/" + found;
            get_files(found, paths);
          }
        }
        closedir(dir);
      }
    }
  }

  return paths;
}

std::string parse_output_file(int argc, char *argv[])
{
  std::string output_file("bindata.hpp");
  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i], strlen(argv[i]));
    if (arg == "-outfile" && (argc > i + 1))
    {
      output_file = argv[i + 1];
      continue;
    }
  }

  return output_file;
}

std::vector<std::string> parse_folders(int argc, char *argv[])
{
  std::vector<std::string> files;
  for (int i = 1; i < argc; i++)
  {
    std::string curr_arg(argv[i], strlen(argv[i]));
    std::string prev_arg(argv[i - 1], strlen(argv[i - 1]));
    if (curr_arg != "-outfile" && prev_arg != "-outfile")
    {
      files.push_back(argv[i]);
    }
  }

  return files;
}

void usage(const char *progname)
{
  printf("Usage examples: \n  %s data/\n  %s -outfile include/binfs.hpp data/ /full/path/to/file.mp4\n\n", progname, progname);
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    usage(argv[0]);
  }

  BinFS::BinFS *binfs = new BinFS::BinFS();

  std::vector<std::string> folders = parse_folders(argc, argv);
  std::string outfile = parse_output_file(argc, argv);
  std::vector<std::string> files;

  for (const std::string &path : folders)
  {
    files = get_files(path, files);
  }

  for (const std::string &path : files)
  {
    printf("Adding file %s ...\n", path.c_str());
    binfs->add_file(path);
  }

  binfs->output_hpp_file(outfile);

  return 0;
}
