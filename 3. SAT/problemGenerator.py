import sys
from random import randint
if(len(sys.argv)<4):
    print("ERROR: Too few Arguments")
    sys.exit(1)
N = int(sys.argv[1])
K1 = int(sys.argv[2])
K2 = int(sys.argv[3])
E = randint(N-1,N*(N-1)/2)
G = {}
G[1] = []
num_edges = 0
for i in range(2,N+1):
    v = randint(1,i-1)
    G[i] = [v]
    num_edges += 1
while num_edges < E:
    v1 = randint(1,N)
    v2 = randint(1,N)
    if v2 in G[v1] or v1 in G[v2] or v2==v1:
        continue
    else:
        G[v1].append(v2)
        num_edges += 1
fp = open('test.graph','w')
fp.write('%d %d %d %d\n' %(N,E, K1, K2))
for i in range(1,N+1):
    for v in G[i]:
        fp.write('%d %d\n' %(i,v))
fp.close()