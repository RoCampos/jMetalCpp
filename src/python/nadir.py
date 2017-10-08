
# -*- coding: utf-8 -*-

import sys

def nadir (pareto):
	# Z = max(pareto, key=lambda item:item[0])[0]
	Z = 1
	C = max(pareto, key=lambda item:item[1])[1]
	H = max(pareto, key=lambda item:item[2])[2]

	C = int (C * 1.5)
	H = int (H * 1.5)

	return (Z, C, H)

def main(instance):
	pareto = []
	with open(instance) as f:
		for line in f:
			t = tuple(map(int, line.split()))
			pareto.append (t)

	# print (pareto)
	print ("%s %s %s" % nadir (pareto))


if __name__ == '__main__':
	instance = sys.argv[1]
	main(instance)