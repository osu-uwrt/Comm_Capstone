import matplotlib.pyplot as plt
from itertools import islice
import numpy as np

def filter_close_timestamps(timestamps_, threshold):
    """
    Filters out timestamps that are too close to their neighbors.

    :param timestamps: List of timestamps (as floats or datetime objects)
    :param threshold: Minimum allowed time difference
    :return: Filtered list of timestamps
    """
    # timestamps_ = sorted(timestamps_)  # Ensure they are sorted
    filtered = [timestamps_[0]]  # Keep the first timestamp

    for i in range(1, len(timestamps_)):
        if timestamps_[i] - filtered[-1] >= threshold:
            filtered.append(timestamps_[i])  # Only add if difference is large enough

    return filtered

file = open("./samples.txt")

timestamps = []
samples = []

for line in islice(file, 0, 100000):
    samples.append(int(line[:line.find(',')]))
    timestamps.append(float(line[line.find(',') + 1:]))

sampleDerivatives = [0]
for i in range(len(samples) - 1):
    sampleDerivatives.append((samples[i+1] - samples[i]) / (timestamps[i+1] - timestamps[i]))

sampleDerivatives = np.array(sampleDerivatives)

# sampleDerivatives = 4096 * (sampleDerivatives - min(sampleDerivatives)) / (max(sampleDerivatives) - min(sampleDerivatives))

upper = np.where(sampleDerivatives < 5e7)
lower = np.where(sampleDerivatives > -5e7)
intersect = np.intersect1d(upper, lower)
positivePeaks = np.where(np.array(samples) > 2000)
intersect = np.intersect1d(intersect, positivePeaks)
# print(zeros)

peakTimestamps = np.array(timestamps)[intersect]
peakTimestamps = filter_close_timestamps(peakTimestamps, 5e-5)

frequencies = []

for i in range(len(peakTimestamps) - 1):
    frequencies.append(1 / (peakTimestamps[i + 1] - peakTimestamps[i]))

meanFreq = np.mean(frequencies)
# frequencies = frequencies[abs(frequencies - meanFreq) > 1000]
filteredFreq = np.where(abs(np.array(frequencies) - meanFreq) > 1)
print(filteredFreq[0].size)

# print(np.mean(np.array(frequencies)[filteredFreq]))
# print(np.mean(frequencies))

fftOutput = np.fft.fft(samples)
sampleRate = (timestamps[-1] - timestamps[0]) / len(samples)

frequencies = np.fft.fftfreq(len(samples), sampleRate)
dominant_frequency_index = np.argmax(np.abs(fftOutput[1:len(fftOutput) // 2])) + 1
dominant_frequency = frequencies[dominant_frequency_index]

print("Dominant frequency:", dominant_frequency, "Hz")

# peakTimestamps = np.array(timestamps)[intersect]

plt.plot(timestamps, samples, '-b', label='samples')
# plt.plot(timestamps, sampleDerivatives, '-ro', label='derivatives')
plt.scatter(peakTimestamps, 2048 * np.ones(np.array(peakTimestamps).shape), label='peaks')
plt.legend(loc="upper left")
plt.show()