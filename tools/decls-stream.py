types = [
	["int8_t", "Int8"],
	["uint8_t", "UInt8"],
	["int16_t", "Int16"],
	["uint16_t", "UInt16"],
	["int32_t", "Int32"],
	["uint32_t", "UInt32"],
	["int64_t", "Int64"],
	["uint64_t", "UInt64"],
	["float", "Float"],
	["double", "Double"],
]

for t in types:
	print(t[0] + " DgStreamRead" + t[1] + "(DgStream *stream);")

print()

for t in types:
	print("void DgStreamWrite" + t[1] + "(DgStream *stream, " + t[0] + " *data);")
