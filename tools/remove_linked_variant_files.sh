#!/bin/zsh

#
# this script remove all symlinks in `variant` folder. script only needs to be run when files are added or deleted. 
# this script is for development purposes only.
#

# Check if correct number of arguments is passed
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 destination_directory"
    exit 1
fi

# Assign command line arguments to variables
destination_dir=$1

# Check if destination directory exists
if [ ! -d "$destination_dir" ]; then
    echo "Error: Destination directory '$destination_dir' does not exist."
    exit 1
fi

# Remove old symbolic links in the destination directory
find "$destination_dir" -type l -exec rm {} \;
