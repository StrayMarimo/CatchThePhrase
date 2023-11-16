#!/bin/bash

echo compiling programs...

# CONSTANTS
ROOT_HEADER="-I./include"
ROOT_SRC="src/"
SERVER_FILE="src/server.c"
CLIENT_FILE="src/client.c"

# Add directories as needed
INCLUDE_DIRS="
    ${ROOT_HEADER}/constants/ 
    ${ROOT_HEADER}/utilities/
    ${ROOT_HEADER}/structs/
"
# Add source files as needed
SRC_FILES="
    ${ROOT_SRC}utilities/*.c
    ${ROOT_SRC}game_logic/*.c
"
# compile
gcc $INCLUDE_DIRS -o src/server $SERVER_FILE $SRC_FILES \
    || { echo 'Server compilation failed'; exit 1; }
echo "Server compiled successfully!"

gcc $INCLUDE_DIRS -o src/client $CLIENT_FILE $SRC_FILES \
    || { echo 'Client compilation failed'; exit 1; }
echo "Client compiled successfully!"
