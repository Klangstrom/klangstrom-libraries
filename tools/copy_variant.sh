#!/bin/zsh

#
# this script is intended to symlink all files from *this* `variant` folder to the STM32duino variant folder. this 
# script only needs to be run when files are added or deleted. this script is for development purposes only.
#
# e.g `copy_variant.sh $HOME/Documents/dennisppaul/projects/klangstrom/git/klangstrom-hardware/KLST_PANDA/variant $HOME/Library/Arduino15/packages/STMicroelectronics/hardware/stm32/2.7.1/variants/STM32H7xx/H723Z\(E-G\)T_H730ZBT_H733ZGT`
#

# Check if correct number of arguments is passed
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 source_directory destination_directory"
    exit 1
fi

# Assign command line arguments to variables
source_dir=$1
destination_dir=$2

# Check if source directory exists
if [ ! -d "$source_dir" ]; then
    echo "Error: Source directory '$source_dir' does not exist."
    exit 1
fi

# Check if destination directory exists
if [ ! -d "$destination_dir" ]; then
    echo "Error: Destination directory '$destination_dir' does not exist."
    exit 1
fi

# Remove old symbolic links in the destination directory
find "$destination_dir" -type l -exec rm {} \;

# Create new symbolic links from the source directory to the destination directory
for file in "$source_dir"/*; do
    ln -s "$file" "$destination_dir"/$(basename "$file")
done
