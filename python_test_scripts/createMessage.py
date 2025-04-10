import numpy as np
import csv
import os
import random

samplesPerFrequency = 1000

frequencySamples = []
frequencies = [15625, 16129, 16667, 17241, 17857, 18518, 19231, 20000]
# frequencies.reverse()

for frequency in frequencies:
    samples = np.zeros((samplesPerFrequency, 1))
    with open(f"{os.path.dirname(os.path.realpath(__file__))}/sampleCSVs/{frequency}kHz.csv") as file:
        reader = csv.reader(file)
        for row in reader:
            if reader.line_num >= samplesPerFrequency:
                break
            samples[reader.line_num] = row[1]
            
    frequencySamples.append(samples)
    
message = "haha"

packed = 0
bitCount = 0
index = 0

messageFrequencies = np.zeros((int(len(message) * 8 / 3) + 1, 1))
messageSamples = np.zeros((samplesPerFrequency * len(messageFrequencies)))

for i in range(len(message)):
    packed = (packed << 8) | ord(message[i])
    bitCount += 8
    while (bitCount >= 3):
        value = (packed >> (bitCount - 3)) & 0x7
        messageFrequencies[index] = value
        index += 1
        bitCount -= 3
if bitCount > 0:
    value = (packed & ((1 << bitCount) - 1)) << (3 - bitCount)
    messageFrequencies[index] = value & 0x7

for frequency in messageFrequencies:
    print(frequency[0])
    
# with open(f"{os.path.dirname(os.path.realpath(__file__))}/sampleCSVs/{message}.csv", "w") as out:
with open(f"/home/markc/Downloads/testMessage.csv", "w") as out:
    # TODO: Add pre message
    for i in range(2):
        randomDelay = 0
        if  i == 0:
            randomDelay = random.randrange(0, 500)
        print(f"Random delay: {randomDelay}")
        # Write frequency into test message file
        for j in range(samplesPerFrequency - randomDelay):
            # First 0 defines which frequency is the pre message frequency 
            out.write(f"{int(frequencySamples[0][j][0])}\n")
        # Write portion when quiet
        for j in range(samplesPerFrequency):
            out.write(f"0\n")
        
    for frequency in messageFrequencies:
        # Write frequency into test message file
        for i in range(samplesPerFrequency):
            out.write(f"{int(frequencySamples[int(frequency[0])][i][0])}\n")
        # Write portion when quiet (Don't need it due to pre message)
        # for i in range(samplesPerFrequency):
        #     out.write(f"0\n")