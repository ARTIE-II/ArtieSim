import numpy as np
from matplotlib import pyplot as plt
import uproot

if __name__ == "__main__":

    f = uproot.open("../data/resolution13a.root")
    w, x, y = f['tally15'].to_hist().to_numpy()

    fig, axs = plt.subplots(figsize=(10,10))
    mesh = axs.pcolormesh(x, y, w.T)
    axs.set_ylabel(r"$\delta T/T_0$")
    axs.set_xlabel("Energy [MeV]")
    plt.tight_layout()
    #plt.show()

    bin = 1000

    fig, axs = plt.subplots()
    axs.stairs(w[bin], y, fill=True)
    axs.set_xlabel(r"$\delta T/T_0$")
    axs.set_title(f"Energy bin ({x[bin]},{x[bin+1]}) [MeV]")
    plt.show()