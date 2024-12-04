#!/bin/bash

# Function to prompt user for creating a directory
prompt_create_directory() {
    read -p "$1 directory does not exist. Do you want to create it (Y/N)? " response
    case $response in
        [Yy]* )
            echo "Creating $1 directory."
            mkdir -p "$1"
            ;;
        [Nn]* )
            echo "Exiting script."
            exit 0
            ;;
        * )
            echo "Invalid response. Exiting script."
            exit 1
            ;;
    esac
}

# Function to prompt user for creating a file
prompt_create_file() {
    read -p "$2 file does not exist in $1 directory. Do you want to create it (Y/N)? " response
    case $response in
        [Yy]* )
            echo "Creating $2 file in $1 directory."
            touch "$1/$2"
            ;;
        [Nn]* )
            echo "Here is the list of files in $1 directory:"
            ls -l "$1"
            exit 0
            ;;
        * )
            echo "Invalid response. Exiting script."
            exit 1
            ;;
    esac
}

# Main script execution
if [ $# -ne 2 ]; then
    echo "Usage: $0 <Directory> <File>"
    exit 1
fi

directory=$1
file=$2

if [ ! -d "$directory" ]; then
    prompt_create_directory "$directory"
fi

echo "$directory directory exists."

if [ ! -f "$directory/$file" ]; then
    prompt_create_file "$directory" "$file"
fi

echo "Here is the list of files in $directory directory:"
ls -l "$directory"
