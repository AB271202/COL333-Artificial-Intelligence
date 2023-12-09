import random

t=4
z=250
l=300

print(t,z,l,sep="\n")

for i in range(z):
    for j in range(z):
        print(random.randint(0,100), end=" " if j!=z-1 else "\n")


import numpy as np

n = l  # Change this value to the desired size of the matrix

# Generate a symmetric matrix with integer entries and diagonal elements set to 0
symmetric_matrix = np.random.randint(1, 100, size=(n, n))  # Generates random integers between 1 and 9
symmetric_matrix = (symmetric_matrix + symmetric_matrix.T) // 2  # Make it symmetric (use integer division)
np.fill_diagonal(symmetric_matrix, 0)  # Set diagonal elements to 0

# Print the symmetric matrix
for row in symmetric_matrix:
    for element in row:
        print(element, end=" ")
    print()  # Move to the next row


