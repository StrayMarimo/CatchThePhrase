#!/bin/bash
# Check if Raylib is already installed
if [ ! -d "raylib" ]; then
    echo "Raylib not found, cloning..."
    git clone https://github.com/raysan5/raylib.git
fi

cd raylib/src
make PLATFORM=PLATFORM_DESKTOP || { echo 'Raylib build failed'; exit 1; }
cd ../..

echo compiling programs...

# CONSTANTS
ROOT_HEADER="-I./include"
ROOT_SRC="src/"
SERVER_FILE="src/server.c"
CLIENT_FILE="src/client.c"
LIBS="-lraylib"

# Add directories as needed
INCLUDE_DIRS="
    ${ROOT_HEADER}/constants/ 
    ${ROOT_HEADER}/utilities/
    ${ROOT_HEADER}/structs/
    ${ROOT_HEADER}/ui/
    ${ROOT_HEADER}/raylib/src
"
# Add source files as needed
SRC_FILES="
    ${ROOT_SRC}utilities/*.c
    ${ROOT_SRC}game_logic/*.c
    ${ROOT_SRC}ui/*.c
"

UNAME=$(uname)

# Linux-specific settings
if [ "$UNAME" == "Linux" ]; then
    LIBS="$LIBS -ldl -lpthread -lm -lX11 -lXrandr -lXi -lGL -lGLU"
fi

# compile
gcc $INCLUDE_DIRS $LIBS -o src/server $SERVER_FILE $SRC_FILES  \
    || { echo 'Server compilation failed'; exit 1; }
echo "Server compiled successfully!"

gcc $INCLUDE_DIRS $LIBS -o src/client $CLIENT_FILE $SRC_FILES \
    || { echo 'Client compilation failed'; exit 1; }
echo "Client compiled successfully!"
