#!/usr/bin/env python3

import os
import sys
import subprocess
import re

if len(sys.argv) < 2:
    print("Usage: ./pnrytexttofile.py <text_string>")
    sys.exit(1)

text = ' '.join(sys.argv[1:])

        


# Translation function using translate-shell
# sudo apt install translate-shell 
def translate_text(text):
    try:
        result = subprocess.run(['trans', ':en', text], capture_output=True, text=True)
        output = result.stdout.strip()
        clean_output = re.sub(r'\x1b\[[0-9;]*m', '', output)  # Remove escape sequences
        return clean_output
    except FileNotFoundError:
        print("Error: translate-shell is not installed or not found.")
        sys.exit(1)



# Translate the input text
translated_text = translate_text(text)
print("Translated Text:", translated_text)

file_path = os.path.expanduser("~/pnrytexttofile.txt")

try:
    with open(file_path, "a") as file:
        file.write(text + '\n')
        file.write("Translated Text: " + translated_text + '\n')
except IOError:
    print("Failed to open", file_path)
    sys.exit(1)

os.system("gedit " + file_path)


# Generated with ChatGPT

# sudo apt install translate-shell 
# sudo apt install tesseract-ocr-script-jpan

# Remember to chmod + x translate_script.py
# Add full path to this file in the Run executable
