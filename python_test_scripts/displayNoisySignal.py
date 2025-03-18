import matplotlib.pyplot as plt
import numpy as np
import csv

filteredSignal = np.zeros((2502, 1))
noisySignal = np.zeros_like(filteredSignal)
with open("/home/markc/OSU/ECE4905/bpfC/build/filtered.csv") as filterSignal:
    reader = csv.reader(filterSignal)
    for row in reader:
        filteredSignal[reader.line_num] = row[1]
        noisySignal[reader.line_num] = row[2]

plt.plot(filteredSignal)
# plt.plot(noisySignal)
plt.show()