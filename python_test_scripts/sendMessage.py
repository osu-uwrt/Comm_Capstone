import numpy as np
import matplotlib.pyplot as plt

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
    b = np.array([0.0422, 0, -0.0422]) # Example numerator coefficients
    a = np.array([1.0000, -1.8714, 0.9157])  # Example denominator coefficients
    
    # Apply filter
    return apply_iir_filter(x, b, a)

frequencies_ = np.zeros((20))
def messageToFrequencies(message: str):
    for i in range(10):
        if (i < len(message)):
            frequencies_[i * 2] = (ord(message[i]) >> 4) * 250 + 14000
            frequencies_[i * 2 + 1] = (ord(message[i]) & 0x0F) * 250 + 14000

# Define parameters
fs = 500000  # Sampling rate (samples per second)
segment_size = 1000  # Number of samples per frequency segment
total_samples = segment_size * 2 * 10  # Total number of samples
frequencies = np.linspace(15000, 19000, total_samples // segment_size)  # Frequencies in Hz
messageToFrequencies("blah")
print(total_samples)


time = np.arange(total_samples) / fs  # Time array
signal = np.zeros(total_samples)  # Preallocate signal array

# Generate sine wave segments with phase continuity
phase = 0  # Initial phase
for i, freq in enumerate(frequencies_):
    t_segment = time[i * segment_size:(i + 1) * segment_size]
    segment = np.sin(2 * np.pi * freq * t_segment + phase) * 5  # Scale to -5 to 5V
    signal[i * segment_size:(i + 1) * segment_size] = segment
    phase = (2 * np.pi * freq * t_segment[-1] + phase) % (2 * np.pi)  # Carry over phase

# Clip values to stay within -5V to 5V range
clipped_signal = np.clip(signal, 0, 3.3)

filtered_signal = bandpass_filter(clipped_signal)

# msgFile = open("/home/markc/OSU/ECE4905/Comm_Capstone/message.txt", "x")
# msgFile.close()

with open("/home/markc/OSU/ECE4905/Comm_Capstone/message.txt", "w") as msgFile:
    for i in range(len(filtered_signal)):
        msgFile.write(f"{filtered_signal[i]}\n")
    

# print(frequencies_)

# Plot the signal
plt.figure(figsize=(12, 6))
plt.plot(time, clipped_signal)  # Plot only first 2000 samples for clarity
plt.plot(time, filtered_signal)
plt.xlabel("Time (s)")
plt.ylabel("Amplitude (V)")
plt.title("Modulated Sine Wave (Clipped to -5V - 5V)")
plt.grid()
plt.show()
