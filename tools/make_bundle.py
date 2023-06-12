#!/usr/bin/env python
"""
Makes a trestle format bundle file
"""

import sys
import os
import os.path
import pathlib
import struct

def absolute_path(path):
	return os.path.abspath(path)

def list_folder(folder):
	"""
	List files in a folder
	"""
	
	folder = absolute_path(folder)
	
	lst = []
	outline = []
	
	for root, dirs, files in os.walk(folder):
		root = str(root)
		
		for f in files:
			f = str(f)
			
			base_file_name = absolute_path(root + "/" + f)
			
			# We don't incldue the full path in the list
			lst.append(base_file_name[len(folder) + 1:])
		
		for d in dirs:
			outline.append(root + "/" + str(d))
	
	return (lst, outline)

def load_bytes(path):
	return pathlib.Path(path).read_bytes()

class FileEntry:
	"""
	Represents a file entry and helps turning one into bytes
	"""
	
	def __init__(self, file_name, file_length, entry_start):
		self.file_name = file_name
		self.file_length = file_length
		self.entry_start = entry_start
	
	def toBytes(self):
		return self.file_name.encode('utf-8') + b'\x00' + struct.pack("I", self.file_length) + struct.pack("I", self.entry_start) + b"\x00"

def make_bundle(folder, output):
	file_list, dirs = list_folder(folder)
	entries = []
	
	f = open(output, "wb")
	
	print(file_list)
	
	# Write the data for each file
	for file_name in file_list:
		data = load_bytes(folder + "/" + file_name)
		entry_start = f.tell()
		f.write(data)
		entries.append(FileEntry(file_name, len(data), entry_start))
	
	# Record where entries start
	entry_list_start = f.tell()
	
	# Write the file entries
	for entry in entries:
		f.write(entry.toBytes())
	
	# Main header
	f.write(struct.pack("I", len(entries)))
	f.write(struct.pack("I", entry_list_start))
	f.write(b"\x00\x00\x00\x00")
	f.write(b"Trpk")
	
	# Finalise the file
	f.close()

def main():
	if (len(sys.argv) < 3):
		print("Usage: make_bundle.py [in: dir] [out: bundle]")
	else:
		make_bundle(sys.argv[1], sys.argv[2])

if (__name__ == "__main__"):
	main()
