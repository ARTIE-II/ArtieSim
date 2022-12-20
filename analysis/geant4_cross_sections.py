import numpy as np
from matplotlib import pyplot as plt
import csv

geant4_elastic_file = "ArIsotopes_elastic.csv"
geant4_elastic_energy = []
geant4_elastic_xsec = []
with open(geant4_elastic_file, "r") as file:
    reader = csv.reader(file, delimiter=",")
    for row in reader:
        geant4_elastic_energy.append(float(row[0]) / 1000.0)
        geant4_elastic_xsec.append(float(row[1]))
geant4_elastic_energy = np.array(geant4_elastic_energy)
geant4_elastic_xsec = np.array(geant4_elastic_xsec)

endf_energy = []
endf_transmission = []
endf_cross_section = []
with open("endf_natar.csv", "r") as file:
    reader = csv.reader(file, delimiter=",")
    for row in reader:
        endf_energy.append(float(row[0]))
        endf_transmission.append(np.exp(-4.2 * float(row[1])))
        endf_cross_section.append(float(row[1]))
endf_energy = np.array(endf_energy)
endf_transmission = np.array(endf_transmission)
endf_cross_section = np.array(endf_cross_section)

def compare_geant4_elastic(
    energy_min: float=40,
    energy_max: float=70,
):
    fig, axs = plt.subplots()
    axs.plot(
        geant4_elastic_energy,
        geant4_elastic_xsec,
        label="geant4"
    )
    axs.plot(
        endf_energy, endf_cross_section,
        linestyle='-',
        color='g',
        label='endf'
    )
    axs.set_xlim(energy_min,energy_max)
    axs.set_xlabel("Energy [keV]")
    axs.set_ylabel("Cross Section [barn]")
    axs.set_title("LAr Cross Section vs. Energy [barn/keV]")
    axs.set_yscale("log")
    plt.legend()
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    compare_geant4_elastic()