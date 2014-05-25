#!/usr/bin/env python3
import sys
#TODO: real option
width = 1000
if len(sys.argv) == 2:
    width=10000
input()
#M, N, nnz = all(
M, N, nnz= (int(i) for i in input().split())
height = int(M/(N/width))
scale = N/width
freqImage = [[0 for i in range(width+1)] for j in range(height+1)]
for i in range(nnz):
    y, x, v = (int(j)-1 for j in input().split())
    freqImage[int(y/scale)][int(x/scale)] = freqImage[int(y//scale)][int(x//scale)] + 1

maxEl=0
freqs=[]
for i in range(height):
    for j in range(width):
        if(freqImage[i][j] != 0):
            freqs.append(freqImage[i][j])
        if(freqImage[i][j] > maxEl):
            maxEl=freqImage[i][j]

freqs.sort()
maxEl=min(freqs[len(freqs)*4//5],maxEl)
sys.stderr.write("hello world")

print("P2")
print(str(width) + " "+str(height))
print(maxEl)
for i in range(height):
    for j in range(width):
        print(str(max(maxEl-freqImage[i][j],0)) + " ", end="")
    print()
#M*N
