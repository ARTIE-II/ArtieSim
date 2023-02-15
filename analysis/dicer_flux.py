import numpy as np
from matplotlib import pyplot as plt
import uproot
import os

if __name__ == "__main__":

    if not os.path.isdir("plots/dicer"):
        os.makedirs("plots/dicer")

    f = uproot.open("../data/DICER_flux_6Li.root")
    hist, edges = f['h_flux_6Li'].to_hist().to_numpy()
    bin_centers = np.array([0.5*(edges[ii] + edges[ii+1]) for ii in range(len(edges)-1)])
    fig, axs = plt.subplots(figsize=(10,10))

    bin_centers = bin_centers[(hist > 0)]
    hist = hist[(hist > 0)]
    
    axs.plot(bin_centers, hist * bin_centers)
    axs.set_xscale("log")
    axs.set_yscale("log")
    axs.set_xlabel(f"Energy [eV]")
    axs.set_ylabel(r"$\Phi$" + f" neutrons/sec/cm"+r"${}^2$")
    axs.set_title("MARK-III " + r"$\Phi$" + " vs. energy [eV] at Li6")
    plt.tight_layout()
    plt.savefig("plots/dicer/markiii_flux_li6.png")
