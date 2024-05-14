#!/bin/zsh

#
# e.g collect_variant_files.sh $HOME/Documents/dennisppaul/projects/klangstrom/git/klangstrom-hardware/KLST_PANDA/firmware/KLST_PANDA-variant-files.txt $HOME/Documents/dennisppaul/projects/klangstrom/git/klangstrom-hardware/KLST_PANDA/firmware $HOME/Documents/dennisppaul/projects/klangstrom/git/klangstrom-hardware/KLST_PANDA/variant
#

# Check if correct number of arguments is passed
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 file_list.txt source_directory destination_directory"
    exit 1
fi

# Assign command line arguments to variables
file_list=$1
source_dir=$2
destination_dir=$3

# Check if file list exists
if [ ! -f "$file_list" ]; then
    echo "Error: File list '$file_list' does not exist."
    exit 1
fi

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

# Copy files from source to destination as per the list
while IFS= read -r file; do
    # Construct full path for source file
    full_path="$source_dir/$file"
    
    # Check if the file exists before copying
    if [ -f "$full_path" ]; then
        cp "$full_path" "$destination_dir"
    else
        echo "Warning: File '$full_path' not found, skipping..."
    fi
done < "$file_list"

