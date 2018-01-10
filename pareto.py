import sys

file = sys.argv[1]

def non_dominate (lista, ind):
	#Z, cost and
    for x in lista:
        if x[0] <= ind[0] and x[1] <= ind[1] and x[2] <= ind[2] and ind != x:
            return False
    return True


with open (file) as f:
	solution_set = []
	lista = []
	for line in f:
		l = map(int,line.split ()[:3])
		solution_set.append (l)

	for x in solution_set:
	    if non_dominate (solution_set, x):
	        lista.append (tuple(x))
	        # print '%s' % ' '.join(map(str, x))
	a = set (lista)
	for x in a:
		print '%s' % ' '.join(map(str, x))

	
