import matplotlib.pyplot as plt
import numpy as np
import csv

numSamples = 5001

def apply_iir_filter(x, b, a):
    """
    Apply an IIR filter to the input signal x using the given coefficients b and a.
    Implements the standard difference equation:
    y[n] = (b[0]*x[n] + b[1]*x[n-1] + ... - a[1]*y[n-1] - a[2]*y[n-2] - ...)
    """
    y = np.zeros_like(x)
    for n in range(len(x)):
        y[n] = b[0] * x[n]
        for i in range(1, len(b)):
            if n - i >= 0:
                y[n] += b[i] * x[n - i]
        for j in range(1, len(a)):
            if n - j >= 0:
                y[n] -= a[j] * y[n - j]
    return y

def bandpass_filter(x):
    """
    Design a Chebyshev Type II bandpass filter and apply it to signal x.
    fs: Sampling rate
    f1: Lower cutoff frequency (Hz)
    f2: Upper cutoff frequency (Hz)
    order: Filter order
    """
    
    # Got from matlab
    # b = np.array([0.0422, 0, -0.0422]) # Example numerator coefficients
    # a = np.array([1.0000, -1.8714, 0.9157])  # Example denominator coefficients

    b = np.array([0.0018, 0, -0.0036, 0, 0.0018])
    a = np.array([1.0000, -3.7861, 5.4626, -3.5575, 0.8830])

    # b = np.array([0.0781, 0, -0.2342, 0, 0.2342, 0, -0.0781]) * 1e-9
    # a = np.array([1.0000, -5.6894, 13.6183, -17.5503, 12.8422, -5.0594, 0.8386])

    # b = np.array([0.0029, -0.0339, 0.1814, -0.5914,  1.3098, -2.0759,  2.4142, -2.0759,  1.3098, -0.5914,  0.1814, -0.0339,  0.0029])
    # a = np.array([1.0000, -11.5045,  60.9286, -196.4111, 429.2138, -669.8312, 765.4610, -645.3925, 398.4656, -175.6879,  52.5117,  -9.5535,   0.8001])
    
    # Apply filter
    return apply_iir_filter(x, b, a)

filteredSignal = np.zeros((numSamples, 1))
noisySignal = np.zeros_like(filteredSignal)
with open("/home/markc/Documents/TestCSVs/20000kHz.csv") as filterSignal:
# with open("/home/markc/close.csv") as filterSignal:
    reader = csv.reader(filterSignal)
    for row in reader:
        if reader.line_num > numSamples:
            break
        # print(row)
        noisySignal[reader.line_num] = row[1]
        filteredSignal[reader.line_num] = row[2]

# filtered = bandpass_filter(filteredSignal)
# plt.plot(filteredSignal)#[1000:2000])
# plt.plot(noisySignal)
plt.scatter(range(numSamples), filteredSignal)
plt.scatter(range(numSamples), noisySignal)
# plt.scatter(range(89664), filteredSignal)
# plt.scatter(range(1000), filteredSignal[1000:2000])
# plt.plot(filtered)
plt.show()