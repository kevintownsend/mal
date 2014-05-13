#!/usr/bin/env python3
input()
#M, N, nnz = all(
M, N, nnz= (int(i) for i in input().split())
width = 1000
height = int(M/(N/1000.0))
scale = N/1000.0
freqImage = [[0 for i in range(width+1)] for j in range(height+1)]
for i in range(nnz):
    y, x, v = (int(j) for j in input().split())
    freqImage[int(y/scale)][int(x/scale)] = freqImage[int(y//scale)][int(x//scale)] + 1

maxEl=0
for i in range(height):
    for j in range(width):
        if(freqImage[i][j] > maxEl):
            maxEl=freqImage[i][j]

print("P2")
print(str(width) + " "+str(height))
print(maxEl)
for i in range(height):
    for j in range(width):
        print(str(maxEl-freqImage[i][j]) + " ", end="")
    print()
#M*N
