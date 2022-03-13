import math
import numpy as np
import matplotlib.pyplot as plt

N = 256

def hanning_function(N):
    
    hanning_envelope = np.zeros(N)
    
    for i in range (0,N):
        hanning_envelope[i] = np.sin(math.pi * i / N)**2
        
    return hanning_envelope

def triangle_function(N):
    
    triangle_envelope = np.zeros(N)
    M = math.ceil(N/2)
    
    for i in range (0,M):
        triangle_envelope[i] = i / M
        triangle_envelope[N - (i + 1)] = i / M
        
    return triangle_envelope

def square_function(N):
    
    square_envelope = np.zeros(N)
    M1 = math.ceil(N / 10)
    M2 = math.ceil(N / 2)
    
    for i in range (0,M1):
        square_envelope[i] = i / M1
        square_envelope[N - (i + 1)] = i / M1
    
    for i in range (M1,M2):
        square_envelope[i] = 1
        square_envelope[N - (i + 1)] = 1
    
    return square_envelope

hanning_env = hanning_function(N)
triangle_env = triangle_function(N)
square_env = square_function(N)

plt.plot(square_env)
plt.plot(triangle_env)
plt.plot(hanning_env)