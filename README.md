## binfs

This package converts any file into manageable C++ source code. It is useful for embedding binary or text data into a C++ program.

### Installation

To compile and install the library use the following:

```sh
git clone https://github.com/bleenco/binfs.git
cd binfs/
make
make install
```

### Usage

Conversion is done on one or more sets of files. They are all embedded in a new C++ source file.

The simplest invocation generates a `binfs.hpp` file in the current working directory. It include all files recursively from the `data/` directory.

```sh
$ binfs data/
```

To specify the name of the output file being generated, we use the following:

```sh
$ binfs -o myfile.hpp data/
```

Multiple input directories or files can be specified if necessary.

```sh
$ binfs data/ data2/ /path/to/image.png /path/to/my_video.mp4
```

### Accessing the asset

To access asset data, we use the `binfs->get_file(filepath);` function which is included in the generated output.

```c++
#include <string>
#include <fstream>
#include "bindata.hpp"

int main()
{
  BinFS::BinFS *binfs = new BinFS::BinFS();
  binfs->init(); // load the files

  std::string txt_file = binfs->get_file("data/CMakeLists.txt");
  std::string video_file = binfs->get_file("/path/to/my_video.mp4");

  // write accessed video data into output file
  std::ofstream output("out.mp4", std::fstream::binary);
  output << video_file;

  return 0;
}
```

### Optional compression

Not yet implemented.

### Credits

This package was heavily inspired by [go-bindata](https://github.com/jteeuwen/go-bindata) library with same purpose but for `Go` language.

### Licence

```
The MIT License (MIT)

Copyright (c) 2018 Bleenco GmbH https://bleenco.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
