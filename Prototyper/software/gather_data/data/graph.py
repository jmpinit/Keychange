import numpy as np
import matplotlib.pyplot as plt

timeArray = np.ones((10, 10))

keyData = np.genfromtxt('trial3.csv', delimiter=',')[1:]

print keyData.shape

for row in keyData:
	fromKey, toKey, millis = row
	
	x = toKey
	y = fromKey
	
	if timeArray[y, x] == 0:
		timeArray[y, x] = millis
	else:
		timeArray[y, x] = (timeArray[y, x] + millis)/2.0

normKeyData = []
for i in timeArray:
    a = 0
    tmp_arr = []
    a = sum(i, 0)
    for j in i:
        tmp_arr.append(float(j)/float(a))
    normKeyData.append(tmp_arr)

fig = plt.figure()
plt.clf()
ax = fig.add_subplot(111)
ax.set_aspect(1)
res = ax.imshow(np.array(normKeyData), cmap=plt.cm.jet, 
                interpolation='nearest')

width = len(timeArray)
height = len(timeArray[0])

cb = fig.colorbar(res)
plt.xticks(range(width), range(0, width))
plt.yticks(range(height), range(0, height))
plt.savefig('key_timing.png', format='png')