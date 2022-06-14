# Example C Library and Application with Dependencies Built with CMake

This repo demostrates how to use CMake to build an application and a library with external
dependencies. If unfamiliar with CMake, check out
[this presentation](https://indico.jlab.org/event/420/contributions/7961/attachments/6507/8734/CMakeSandCroundtable.slides.pdf).

All of the following examples assume the name of the application or library that is being built is
substitued where `<app_name>` appears.

## Prepare Repo

External libraires that are not installed on the system should be sourced via git submodules.
Submodules sometimes contain submodules themselves so those need to be initialized prior to building
the current project.

```bash
git submodule update --init --recursive
```

## Configure and Build

CMake and many other build systems are split into a configure phase and a build phase.

The available build types are `Debug` (default), `Release`, and `RelWithDebInfo`.

```bash
# Configure
mkdir -p build/<app_name>
cd build/<app_name>
CMAKE_BUILD_TYPE=Debug cmake ../../<app_name>

# Build
make <app_name>
```

When debugging the build system, it is often useful to run make targets with verbose output enabled,
e.g.,

```bash
make VERBOSE=1 <app_name>
```

## Lint

The top-level lint target is `<app_name>_lint`. It runs all linting tasks.

```bash
make <app_name>_lint
```

Lint targets can also be run individually.

```bash
make <app_name>_clang_format # reformats code
make <app_name>_clang_tidy # runs static analyzer
```

Note that CMake has
[`clang-tidy` integration](https://cmake.org/cmake/help/latest/prop_tgt/LANG_CLANG_TIDY.html)
however using that causes `clang-tidy` to run at the same time as the build commands. `clang-tidy`
is slow and often produces errors that an engineer would want to ignore while rapidly iterating
during development so we choose to ignore the built-in support and build separate infrastructure for
running `clang-tidy`.

## Test

CMake and many other build systems have built-in test harnesses. The test harness for CMake is
`ctest`.

```bash
ctest <app_name>_test
```

## Install

Applications and libraries should be installed in a standard way and then packaged after
installation. The `DESTDIR` variable controls the install location. The default install location for
packages is usually `/usr/local` if not specified otherwise.

```bash
DESTDIR=/usr/local make <app_name>_install
```

## Package

TODO

```bash
```

## CI Targets

In CI builds, we want to check for things like format errors but not apply fixes for them. There are
additional targets for this purpose.

```bash
make <app_name>_lint_check # like `<app_name>_lint`, but does not apply any changes
```

## Additional Notes

[clang-format-options](https://releases.llvm.org/13.0.1/tools/clang/docs/ClangFormatStyleOptions.html)
