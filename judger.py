import os

file = open('1.in')

cnt = 0

for line in file:
    cnt+=1
    print("line:", cnt)
    for pos, value in enumerate(line):
        print(pos,value)