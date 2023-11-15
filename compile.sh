#!/bin/bash

echo compiling programs...

# Add directories as needed
INCLUDE_DIRS="-I./include/constants/"

SERVER_FILE="src/server.c"
CLIENT_FILE="src/client.c"

# Compile the program
gcc $INCLUDE_DIRS -o src/server $SERVER_FILE
echo "Server compiled successfully"

gcc $INCLUDE_DIRS -o src/client $CLIENT_FILE
echo "Client compiled successfully"
