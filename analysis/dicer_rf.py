import numpy as np
import csv
from matplotlib import pyplot as plt
import os

if __name__ == "__main__":
    if not os.path.isdir("plots/dicer"):
        os.makedirs("plots/dicer")

    input_file = "../data/DICER_MARKIII.csv"
    data = {}
    x = []
    y = []
    with open(input_file, "r") as file:
        reader = csv.reader(file, delimiter=",")
        first_row = next(reader)
        ii = 0
        for row in reader:
            if not row:
                ii = 0
                first_row = next(reader)
            if ii == 0:
                data[first_row[0]] = []
                ii += 1
                continue
            else:
                data[first_row[0]].append([float(row[0]),float(row[1])])
                ii += 1
                

    for key, value in data.items():
        fig, axs = plt.subplots()
        x = np.array(value)[:,0]
        y = np.array(value)[:,1]
        energy = ""
        for c in key:
            if c == " " and energy == "":
                continue
            elif c == " " and energy != "":
                break
            else:
                energy += c
        energy = float(energy)
        axs.plot(x, y, label=f"E = {energy} eV")
        axs.set_xlabel("Time [ns]")
        axs.set_ylabel("Resolution Function")
        axs.set_title("Resolution Function vs. Time for MARK-III")
        plt.legend()
        plt.tight_layout()
        plt.savefig(f"plots/dicer/markiii_rf_{energy}.png")
    
    # parse MARK_IV file
    input_file = "../data/DICER_MARKIV.csv"
    data = {}
    x = []
    y = []
    with open(input_file, "r") as file:
        reader = csv.reader(file, delimiter="\t")
        first_row = next(reader)
        ii = 0
        for row in reader:
            if not row:
                ii = 0
                first_row = next(reader)
            if ii == 0:
                data[first_row[0]] = []
                ii += 1
                continue
            else:
                data[first_row[0]].append([float(row[0]),float(row[1])])
                ii += 1
                

    for key, value in data.items():
        fig, axs = plt.subplots()
        x = np.array(value)[:,0]
        y = np.array(value)[:,1]
        energy = ""
        for c in key:
            if c == " " and energy == "":
                continue
            elif c == " " and energy != "":
                break
            else:
                energy += c
        energy = float(energy)
        axs.plot(x, y, label=f"E = {energy} eV")
        axs.set_xlabel("Time [ns]")
        axs.set_ylabel("Resolution Function")
        axs.set_title("Resolution Function vs. Time for MARK-IV")
        plt.legend()
        plt.tight_layout()
        plt.savefig(f"plots/dicer/markiv_rf_{energy}.png")
