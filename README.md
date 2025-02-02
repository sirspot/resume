# Resume-As-Code

Copyright (c) 2025, Adam Rosenberg. All rights reserved.

## Features
- formatting string data with macros
- documenting code for IDEs
- manipulating bits for configuration options
- defining data structures
- converting strings to unix timestamps
- using double pointers
- creating a linked-list
- applying callback functions to an insertion sort
- parsing command line arguments
- allocating and freeing heap memory
- taking an object-oriented approach to c
- defining an enum lookup table
- parsing json data
- generating html

# How to Build

## gcc
    gcc adam_resume.c -o adam_resume.exe

## clang
    clang adam_resume.c -o adam_resume.exe

## cl
    cl.exe /Fe:adam_resume.exe adam_resume.c

# How to Run

## display help
    adam_resume.exe -h

## output html to a file
    adam_resume.exe -f html > adam_resume.html

## output all entries
    adam_resume.exe -a
