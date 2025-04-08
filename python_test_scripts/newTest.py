import csv
import os

def longest_chain(numbers, target):
    """
    Finds the length of the longest consecutive chain of a specific number in a list.

    Args:
        numbers: A list of numbers.
        target: The specific number to find the chain of.

    Returns:
        The length of the longest chain of the target number.
    """

    max_chain = 0
    current_chain = 0

    for number in numbers:
        if number == target:
            current_chain += 1
            max_chain = max(max_chain, current_chain)
        else:
            current_chain = 0

    return max_chain

samples = []
with open(f"{os.path.dirname(os.path.realpath(__file__))}/sampleCSVs/15625kHz.csv") as file:
    reader = csv.reader(file)
    for row in reader:
        samples.append(int(row[1]))
        
print(longest_chain(samples, 0))