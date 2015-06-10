[:var_set('', """
#/ Compile command
aoikdyndocdsl -s README.src.md -n aoikdyndocdsl.ext.all::nto -g README.md
""")]\
[:var_set('source_file_name', 'AoikWinWhich.cpp')]\
[:var_set('source_file_url', '/src/AoikWinWhich/AoikWinWhich.cpp')]\
[:var_set('solution_file_url', 'src/AoikWinWhich.sln')]\
[:var_set('program_file_url', 'src/Debug/aoikwinwhich.exe')]\
[:HDLR('heading', 'heading')]\
# AoikWinWhich
[AoikWinWhich](https://github.com/AoiKuiyuyou/AoikWinWhich) written in C++.

Tested working with:
- Visual Studio 2013
- Windows 8.1
- Windows earlier versions should work but not tested

## Table of Contents
[:toc(beg='next', indent=-1)]

## Setup
Clone this git repository to local:
```
git clone https://github.com/AoiKuiyuyou/AoiWinWhich-Cpp
```

In the local repository directory, the source file is
[[:var('source_file_name')]]([:var('source_file_url')]).

Use **Visual Studio 2013** to open solution file **[:var('solution_file_url')]**
and build the program file. If the build configuration is **Debug**, the
generated program file is **[:var('program_file_url')]**.

## Usage
See [usage](https://github.com/AoiKuiyuyou/AoikWinWhich#how-to-use) in the
general project [AoikWinWhich](https://github.com/AoiKuiyuyou/AoikWinWhich).
