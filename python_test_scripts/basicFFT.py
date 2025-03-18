import csv
import numpy as np
import matplotlib.pyplot as plt

def filter_close_indecies(indecies_, threshold):
    """
    Filters out timestamps that are too close to their neighbors.

    :param timestamps: List of timestamps (as floats or datetime objects)
    :param threshold: Minimum allowed time difference
    :return: Filtered list of timestamps
    """
    # timestamps_ = sorted(timestamps_)  # Ensure they are sorted
    filtered = [indecies_[0]]  # Keep the first timestamp

    for i in range(1, len(indecies_)):
        if indecies_[i] - filtered[-1] >= threshold:
            filtered.append(indecies_[i])  # Only add if difference is large enough

    return filtered

samples = []
with open("/home/markc/test.csv", "r") as file:

    reader = csv.reader(file)

    for row in reader:
        # print(row)
        if (len(row) > 0):
            samples.append(float(row[1]))
# samples = samples[:100]
indices = range(len(samples))

sampleRate = 250000

fftOutput = np.fft.fft(samples[:100000])
frequencies = np.fft.fftfreq(len(samples), 1 / sampleRate)
dominant_frequency_index = np.argmax(np.abs(fftOutput[1:len(fftOutput)//2])) + 1
dominant_frequency = frequencies[dominant_frequency_index]

print("Dominant frequency:", dominant_frequency, "Hz")

sampleDerivatives = [0]
for i in range(len(samples) - 1):
    sampleDerivatives.append((samples[i+1] - samples[i]))

sampleDerivatives = np.array(sampleDerivatives)

upper = np.where(sampleDerivatives < 0.025)
lower = np.where(sampleDerivatives > -0.025)
intersect = np.intersect1d(upper, lower)
positivePeaks = np.where(np.array(samples) > 0.03)
intersect = np.intersect1d(intersect, positivePeaks)
intersect = filter_close_indecies(intersect, 4)

frequencies = []

for i in range(len(intersect) - 1):
    frequencies.append(1. / (float(intersect[i + 1] - intersect[i]) * (1. / 250000.)))

meanFreq = np.mean(frequencies)
print(meanFreq)
# frequencies = frequencies[abs(frequencies - meanFreq) > 1000]
filteredFreq = np.where(abs(np.array(frequencies) - meanFreq) > 1000)
print(filteredFreq[0].size)

indices = range(len(sampleDerivatives))

# plt.plot(indices, sampleDerivatives)
# plt.scatter(intersect, np.zeros(len(intersect)))
# plt.show()