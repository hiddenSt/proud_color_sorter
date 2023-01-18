#! /bin/bash

CLANG_FORMAT=clang-format-14

find src tests -name '*pp' -type f | xargs $CLANG_FORMAT -i
