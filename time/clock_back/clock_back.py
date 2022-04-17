#!/usr/bin/env python3

from matplotlib import pyplot as plt
import csv

def main():
    print("sdfsdf")
    data = []
    with open("/tmp/samples.log") as csv_file:
        samples = csv.reader(csv_file)
        for row in samples:
            data.append(int(row[0]))

        data_diff = [j-i for i, j in zip(data[:-1], data[1:])]
        for d in data_diff:
            if d < 0:
                print("NEGGGGGG")
        plt.plot(data, marker="x")
        plt.show()


if __name__ == "__main__":
    main()
