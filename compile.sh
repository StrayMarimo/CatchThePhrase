#!/bin/bash

# Determine the operating system
UNAME=$(uname)

if [ "$UNAME" != "Darwin" ]; then
    sudo apt install libxcursor-dev -y
    sudo apt install libxrandr-dev -y
    sudo apt install libxinerama-dev -y
    sudo apt install libxi-dev -y
fi

# Check if Raylib is already installed
if [ ! -d "raylib" ]; then
    echo "Raylib not found, cloning..."
    git clone https://github.com/raysan5/raylib.git
fi

cd raylib/src

# Make the static version of Raylib
make PLATFORM=PLATFORM_DESKTOP || { echo 'Raylib build failed'; exit 1; }

# Move back to the project root
cd ../..

echo compiling programs...

# CONSTANTS
ROOT_HEADER="-I./include"
ROOT_SRC="src/"
SERVER_FILE="src/server.c"
CLIENT_FILE="src/client.c"
LIBS="-lraylib"
LINKS=""

# Add directories as needed
INCLUDE_DIRS="
    ${ROOT_HEADER}/constants/ 
    ${ROOT_HEADER}/utilities/
    ${ROOT_HEADER}/structs/
    ${ROOT_HEADER}/ui/
"

# Add source files as needed
SRC_FILES="
    ${ROOT_SRC}utilities/*.c
    ${ROOT_SRC}game_logic/*.c
    ${ROOT_SRC}ui/*.c
"

echo "Detected OS: $UNAME"

# Linux-specific settings
if [ "$UNAME" != "Darwin" ]; then
    LIBS="-I./raylib/src $LIBS"
    LINKS="-L./raylib/src -lraylib -lm"
fi

# Compile
gcc $INCLUDE_DIRS $LIBS -o src/server $SERVER_FILE $SRC_FILES $LINKS  \
    || { echo 'Server compilation failed'; exit 1; }
echo "Server compiled successfully!"

gcc $INCLUDE_DIRS $LIBS -o src/client $CLIENT_FILE $SRC_FILES $LINKS \
    || { echo 'Client compilation failed'; exit 1; }
echo "Client compiled successfully!"
