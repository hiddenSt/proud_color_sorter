# Proud color sorter

This is an implementation of the test task.

## Prerequisites

Before you begin, ensure you have met following requirements:

* You have installed `cmake` of version `3.15` or later.
* You have installed a `C++` compiler, which supports the `C++17` standard at least.
* You have installed a build system like `ninja`, `GNU Make` or `visual studio`.

Optional:
* You have installed [`Conan Package Manager`](https://docs.conan.io/en/latest/) for the easy dependency installation.

### Third-party dependencies:

* [fmt](https://fmt.dev/latest/index.html) of version 9.1.0 or newer.
* [CLI11](https://github.com/CLIUtils/CLI11) of version 2.3.2 or newer.

To build the app your system must satisfy this dependencies. This repository contains [conanfile.txt](conanfile.txt)
which provides a convenient way to install dependencies. If you want to build this with conan provided dependencies, we strongly recommend using this [conan config](https://github.com/hiddenSt/conan-config.git).

## Build

To build the project, follow these steps:

1. Create a build directory
```shell
mkdir build
```
2. Install dependencies using `conan`
```shell
conan install <path_to_sources> -if <path_to_your_build_dir> -of <path_to_your_build_dir> --build=missing --build=outdated -r=conancenter -pr=<conan_profile_for_target_platform>
```
This step is optional

3. Run cmake
```shell
cmake -S <path_to_sources> -B <path_to_your_build_dir>
```

4. Run build system
```shell
cmake --build <path_to_your_build_directory> -j4
```

After this command, app and tests binaries will be placed at `<path_to_your_build_dir>/bin`<br>

## Running tests

To run tests on Linux:
1. Go to your build directory

2. Run tests using `ctest`
```shell
ctest
```

or you can run tests directly by executing the tests binary:
```shell
./bin/proud_color_sorter_tests
```

## Contributing

This project is using [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). 

## Installing

You can optionally install the app binary to the desired location, i. e. to a directory which in the system $PATH environment variable.
```shell
cmake --install <path_to_your_build_dir> --component proud_color_sorter --prefix <path_to_desired_dir>
```

## Usage

`./pcs [OPTIONS]`

```shell
OPTIONS:
  -h,--help                   Print this help message and exit
  --max_size UINT [100]       Max length of generated color sequence.
  --colors_order CHAR x 3 REQUIRED
                              Elements order. Possible values: 'r', 'g', 'b'

```

The app will be working until you interrupt it by `SIGINT` signal (`CTRL+C`), after that the program will stop producer and consumer threads, drain pending color sequences from the queue and print the following message to `STDOUT`:
```shell
Threads are stropped.
```

Example:<br>
To run a program, which generates color sequences no longer than 10 elements and sorts them in the following order `red blue green`, you call the app as follows:
```shell
./pcs --max_size 10 --colors_order r b g
```

## Contact

If you want to contact me you can reach me at hiddenstmail@gmail.com

## License

This project uses the following license: [MIT license](LICENSE)