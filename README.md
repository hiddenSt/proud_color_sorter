# Proud color sorter

This is an implementation of the test task.

## Prerequisites

Before you begin, ensure you have met following requirements:

* You have installed `cmake` of version `3.15` or later.
* You have installed `C++` compiler, which supports `C++17` standard at least.
* You have installed build system like `ninja`, `GNU Make` or `visual studio`.

Optional:
* You have installed [`Conan Package Manager`](https://docs.conan.io/en/latest/) for easy dependency installation.

## Build

To build the project, follow these steps:

1. Create a build directory
```shell
mkdir build
```
2. Install dependencies using `conan`
```shell
conan install .. --build=missing --build=outdated -r=conancenter -pr={YOUR_CONAN_PROFILE}
```
<i>Note: you should install packages for profile, that will match the following</i>

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

This project using [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). 

## Installing

You can optionally install the app binary to the desired location, i. e. to a directory which in the system $PATH variable to use the app widely.
```shell
cmake --install <path_to_your_build_dir> --component proud_color_sorter --prefix <path_to_desired_dir>
```

## Run

To run program:
```shell
./pcs [OPTIONS]
```

Usage: `./pcs [OPTIONS]`
```
Options:
  -h,--help                   Print this help message and exit
  --max_size UINT [100]       Max length of generated color sequence.
  --colors_order CHAR x 3 REQUIRED
                              Elements order. Possible values: 'r', 'g', 'b'

```

## Contact