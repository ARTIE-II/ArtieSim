import numpy as np
import scipy as sp
from matplotlib import pyplot as plt
from scipy.interpolate import interp1d
import csv

endf_ar36_file = "../data/endf_ar36.csv"
endf_ar38_file = "../data/endf_ar38.csv"
endf_ar40_file = "../data/endf_ar40.csv"

ar36_ratio = .00334
ar38_ratio = .00063
ar40_ratio = .99603

endf_ar36_energy = []
endf_ar38_energy = []
endf_ar40_energy = []

endf_ar36_xsec = []
endf_ar38_xsec = []
endf_ar40_xsec = []

with open(endf_ar36_file, "r") as file:
    reader = csv.reader(file, delimiter=",")
    for row in reader:
        endf_ar36_energy.append(float(row[0]) / 1000)
        endf_ar36_xsec.append(float(row[1]))

with open(endf_ar38_file, "r") as file:
    reader = csv.reader(file, delimiter=",")
    for row in reader:
        endf_ar38_energy.append(float(row[0]) / 1000)
        endf_ar38_xsec.append(float(row[1]))

with open(endf_ar40_file, "r") as file:
    reader = csv.reader(file, delimiter=",")
    for row in reader:
        endf_ar40_energy.append(float(row[0]) / 1000)
        endf_ar40_xsec.append(float(row[1]))

endf_ar36_energy = np.array(endf_ar36_energy)
endf_ar38_energy = np.array(endf_ar38_energy)
endf_ar40_energy = np.array(endf_ar40_energy)

endf_ar36_xsec = np.array(endf_ar36_xsec)
endf_ar38_xsec = np.array(endf_ar38_xsec)
endf_ar40_xsec = np.array(endf_ar40_xsec)

ar36_interp = interp1d(endf_ar36_energy, endf_ar36_xsec)
ar38_interp = interp1d(endf_ar38_energy, endf_ar38_xsec)
ar40_interp = interp1d(endf_ar40_energy, endf_ar40_xsec)

nat_ar_energy = np.copy(endf_ar40_energy)
nat_ar_xsec = (
    ar36_ratio * ar36_interp(nat_ar_energy) + 
    ar38_ratio * ar38_interp(nat_ar_energy) + 
    ar40_ratio * ar40_interp(nat_ar_energy)
)

fig, axs = plt.subplots()
axs.plot(endf_ar36_energy, endf_ar36_xsec, label="Ar36")
axs.plot(endf_ar38_energy, endf_ar38_xsec, label="Ar38")
axs.plot(endf_ar40_energy, endf_ar40_xsec, label="Ar40")
axs.set_xlabel("Energy [keV]")
axs.set_ylabel("Cross Section [barn]")
axs.set_yscale("log")
axs.set_xlim(20,70)
plt.legend()
plt.tight_layout()
plt.savefig("endf_all_xsec.png")

fig, axs = plt.subplots()
axs.plot(endf_ar36_energy, endf_ar36_xsec, label="Ar36")
axs.plot(endf_ar38_energy, endf_ar38_xsec, label="Ar38")
axs.plot(endf_ar40_energy, endf_ar40_xsec, label="Ar40")
axs.plot(nat_ar_energy, nat_ar_xsec, label="Natural Ar")
axs.set_xlabel("Energy [keV]")
axs.set_ylabel("Cross Section [barn]")
axs.set_yscale("log")
axs.set_xlim(20,70)
plt.legend()
plt.tight_layout()
plt.savefig("endf_interp_nat_ar_all_xsec.png")

fig, axs = plt.subplots()
axs.plot(nat_ar_energy, nat_ar_xsec, label="Natural Ar")
axs.set_xlabel("Energy [keV]")
axs.set_ylabel("Cross Section [barn]")
axs.set_yscale("log")
axs.set_xlim(20,70)
plt.legend()
plt.tight_layout()
plt.savefig("endf_interp_nat_ar_xsec.png")

nat_ar = np.vstack((nat_ar_energy,nat_ar_xsec)).T
with open("../data/endf_natar.csv", "w") as file:
    writer = csv.writer(file, delimiter=",")
    writer.writerows(nat_ar)
