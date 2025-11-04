#!/bin/bash
# Check if one argument was provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 input_file.txt"
    exit 1
fi

# Get the input filename
INPUT_FILE="$1"

# Check if the input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: File '$INPUT_FILE' not found."
    exit 1
fi

# Extract just the filename (no path)
FILENAME=$(basename "$INPUT_FILE")

# Remove extension to get base name
BASENAME="${FILENAME%.*}"

# Output file will be in the current directory with .gdr extension
OUTPUT_FILE="img_gdr/${BASENAME}.gdr"

# Run gdr2txt.exe with wine
echo "Converting $INPUT_FILE to $OUTPUT_FILE..."
wine txt2gdr.exe "$INPUT_FILE" "$OUTPUT_FILE" 256 256
if [ $? -ne 0 ]; then
    echo "Error: gdr2txt.exe failed."
    exit 1
fi

# Run xv.exe on the output
echo "Running xv.exe on $OUTPUT_FILE..."
wine xv.exe "$OUTPUT_FILE"
if [ $? -ne 0 ]; then
    echo "Error: xv.exe failed."
    exit 1
fi

echo "Done."
