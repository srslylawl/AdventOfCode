import sys
import re
from copy import deepcopy
from math import gcd
from collections import defaultdict, Counter, deque
import heapq
import math
D = open(sys.argv[1]).read().strip()
L = D.split('\n')
G = [[c for c in row] for row in L]
R = len(G)
C = len(G[0])

S = []
for line in L:
  pos, vel = line.split('@')
  x,y,z = pos.split(', ')
  vx,vy,vz = vel.split(', ')
  x,y,z = int(x),int(y),int(z)
  vx,vy,vz = int(vx),int(vy),int(vz)
  S.append((x,y,z,vx,vy,vz))

n = len(S)
ans = 0
for i in range(len(S)):
  for j in range(i+1, len(S)):
    x1 = S[i][0]
    x2 = S[i][0]+S[i][3]
    x3 = S[j][0]
    x4 = S[j][0]+S[j][3]
    y1 = S[i][1]
    y2 = S[i][1]+S[i][4]
    y3 = S[j][1]
    y4 = S[j][1]+S[j][4]

    den = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    if den != 0:
      px = ((x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4)) / den
      py = ((x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)) / den
      validA = (px>x1)==(x2>x1)
      validB = (px>x3)==(x4>x3)
      
      if i == 6 and j == 7:
        print("collision", px, py)
        print("validA", validA)
        print("validB", validB)
        print("withinTest", 200000000000000<=px<=400000000000000 and 200000000000000<=py<=400000000000000)

      if 200000000000000<=px<=400000000000000 and 200000000000000<=py<=400000000000000 and validA and validB:
        ans += 1
        print(i,", ",j)
print(ans)