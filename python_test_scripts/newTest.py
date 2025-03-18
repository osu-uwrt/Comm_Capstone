import matplotlib.pyplot as plt
import numpy as np
import scipy.signal

samples = np.linspace(0, 0.0005, 250)
signal = np.sin(samples * np.pi * 2 * 18000)

# signal[signal > 3.3] = 255
# signal[signal < 0] = 0

plt.scatter(samples, signal)
b_ = np.array([0.0004, -0.0016, 0.0020, -0.0000, -0.0020, 0.0016, -0.0004])
# b_ = np.flip(b_)
a_ = np.array([1.0000, -5.8214, 14.2556, -18.7922, 14.0633, -5.6654, 0.9601])
a_ = np.flip(a_)

def transfer_function(x, b, a):
    # Ensure a[0] is 1 as per convention
    if a[0] != 1:
        b = np.array(b) / a[0]
        a = np.array(a) / a[0]
    
    M = len(b) - 1  # Order of numerator
    N = len(a) - 1   # Order of denominator
    y = np.zeros_like(x, dtype=float)  # Output signal initialized to zero

    # Check for potential instability
    if sum(np.abs(a)) > 2:
        print("Warning: Potential instability detected due to large denominator coefficients.")

    # Compute output signal
    for n in range(len(x)):
        # Compute numerator part
        num = sum(b[k] * x[n - k] if n - k >= 0 else 0 for k in range(M + 1))
        
        # Compute denominator part (excluding a[0] which is assumed to be 1)
        den = sum(a[k] * y[n - k] if n - k >= 0 else 0 for k in range(1, N + 1))
        
        # Update output
        y[n] = num - den

        # Clip output to prevent excessive growth (simple saturation to handle instability)
        if np.abs(y[n]) > 1e6:  # Threshold to detect instability
            print("Warning: Output too large, possible instability detected. Clipping the output.")
            y[n] = np.sign(y[n]) * 1e6  # Clip to prevent runaway growth
    
    return y

def bpfilter(x):

    y = np.zeros((len(x), 1))
    for i in range(len(x)):
        match i:
            case 0:
                y[0] = (b_[0]*x[i])/a_[0]
            case 1:
                y[1] = ((b_[0]*x[i]) + (b_[1]*x[i-1]) - (a_[1]*y[i-1]))/a_[0]
            case 2:
                y[2] = ((b_[0]*x[i]) + (b_[1]*x[i-1]) + (b_[2]*x[i-2]) - (a_[1]*y[i-1]) - (a_[2]*y[i-2]))/a_[0]
            case 3:
                y[3] = ((b_[0]*x[i]) + (b_[1]*x[i-1]) + (b_[2]*x[i-2]) + (b_[3]*x[i-3]) - (a_[1]*y[i-1]) - (a_[2]*y[i-2]) - (a_[3]*y[i-3]))/a_[0]
            case _:
                y[i] = ((b_[0]*x[i]) + (b_[1]*x[i-1]) + (b_[2]*x[i-2]) + (b_[3]*x[i-3]) + (b_[4]*x[i-4])- \
                        (a_[1]*y[i-1]) - (a_[2]*y[i-2]) - (a_[3]*y[i-3]) - (a_[4]*y[i-4]))/a_[0]
                
    return y
def iir_filter(b, a, x):
    """
    Apply an IIR filter to a signal using Direct Form I.
    
    Parameters:
    b : array-like
        Feedforward coefficients.
    a : array-like
        Feedback coefficients. a[0] should be 1.
    x : array-like
        Input signal.
    
    Returns:
    y : ndarray
        Filtered output signal.
    """
    x = np.asarray(x)
    b = np.asarray(b)
    a = np.asarray(a)
    
    if a[0] != 1:
        b = b / a[0]
        a = a / a[0]
    
    y = np.zeros_like(x)
    past_inputs = np.zeros(len(b))
    past_outputs = np.zeros(len(a) - 1)
    
    for i in range(len(x)):
        past_inputs[1:] = past_inputs[:-1]  # Shift inputs
        past_inputs[0] = x[i]
        
        y[i] = np.dot(b, past_inputs) - np.dot(a[1:], past_outputs)
        
        past_outputs[1:] = past_outputs[:-1]  # Shift outputs
        past_outputs[0] = y[i]
    
    return y
# filtered = bpfilter(20 * np.log10(signal))
# filtered = bpfilter(signal)
filtered = iir_filter(b_, a_, signal)

# plt.plot(signal)
plt.scatter(samples, filtered)
plt.show()