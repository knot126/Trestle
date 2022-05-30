#!/usr/bin/python

import sys
import os
import shutil

def listFilesInDir(d):
	"""
	List files in a folder
	"""
	
	lst = []
	outline = []
	
	for root, dirs, files in os.walk(d):
		root = str(root)
		
		for f in files:
			lst.append(root + "/" + str(f))
		
		for d in dirs:
			outline.append("src/" + str(d))
	
	return (lst, outline)

def main():
	files, outline = listFilesInDir("src")
	
	shutil.rmtree("temp", ignore_errors = True)
	os.mkdir("temp", mode = 0o755)
	os.mkdir("temp/src", mode = 0o755)
	
	for dir in outline:
		os.mkdir("temp/" + dir, mode = 0o755)
	
	for f in files:
		os.system(f"clang -c -o temp/{f}.output -Wall -Wextra {f}")

if (__name__ == "__main__"):
	main()
