import sys

build_number = 0

# Load build number info
try:
	with open(sys.argv[1] + "/buildnum", "r") as f:
		build_number = int(f.read())
except FileNotFoundError:
	print("Buildnum file not found, creating a new one")

build_number += 1

# Write new build number
with open(sys.argv[1] + "/buildnum", "w") as f:
	f.write(str(build_number))

# Write version.h
with open(sys.argv[1] + "/version.k", "r") as f:
	content = f.read()
	
	content = "// This file was generated by tools/generate_version_headers.py, please edit version.k instead.\n" + content.replace("$$BUILD_NUMBER$$", str(build_number))
	
	with open(sys.argv[1] + "/version.h", "w") as g:
		g.write(content)
