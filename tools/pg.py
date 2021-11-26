
u, v = 6, 6

s = f"function mkp(b, uw, vw, a) \n\tpush_patch(b, {u}, {v}, \n\t"

for i in range(u):
	for j in range(v):
		s += f" {i / (u - 1)} * uw, mgRandFloat() * a, {j / (v - 1)} * vw"
		if ((i + 1) * (j + 1) != u * v):
			s += ", "
	s += "\n\t"

s += ") \nend"

print(s)
