import numpy as np
from matplotlib import pyplot as plt
import uproot
import os

if __name__ == "__main__":

    if not os.path.isdir("plots/"):
        os.maksdirs("plots/")

    f = uproot.open("../data/Profile_188m.root")
    w, x, y = f['histfluka'].to_hist().to_numpy()

    fig, axs = plt.subplots(figsize=(10,10))
    mesh = axs.pcolormesh(x, y, w.T)
    axs.set_ylabel("y [cm]")
    axs.set_xlabel("x [cm]")
    axs.set_title("nTOF Beam Profile at 188m [10 - 100 keV]")
    fig.colorbar(mesh, ax=axs)
    plt.tight_layout()
    plt.savefig("plots/ntof_beam_profile.png")

    total_w = np.sum(w)
    ws = []
    r_vals = np.linspace(0.0,4.0,1000)
    for r_val in r_vals:
        x_mask = (np.abs(x) <= r_val)
        y_mask = (np.abs(y) <= r_val)
        x2, y2 = [], []
        for ii in range(len(x_mask)-1):
            if x_mask[ii] and x_mask[ii+1]:
                x2.append(True)
            else:
                x2.append(False)
        for ii in range(len(y_mask)-1):
            if y_mask[ii] and y_mask[ii+1]:
                y2.append(True)
            else:
                y2.append(False)
        r_mask, vals = np.meshgrid(np.asarray(x2),np.asarray(y2))
        ws.append(np.sum(w[r_mask])/total_w)
    fig, axs = plt.subplots(figsize=(10,10))
    axs.plot(r_vals, ws)
    axs.set_xlabel("Collimator radius [cm]")
    axs.set_ylabel("Signal efficiency")
    axs.set_title("nTOF Signal efficiency vs. Collimator radius [cm] at 188m")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("plots/ntof_efficiency_vs_collimator_radius.png")


    

    f = uproot.open("../data/RF.root")
    w, x, y = f['histfluka'].to_hist().to_numpy()
    fig, axs = plt.subplots(figsize=(10,10))
    mesh = axs.pcolormesh(x, y, w.T)
    axs.set_ylabel(r"$\lambda$")
    axs.set_xlabel("Energy [eV]")
    axs.set_title("nTOF EAR1 Resolution Function")
    fig.colorbar(mesh, ax=axs)
    plt.tight_layout()
    plt.savefig("plots/ntof_resolution_function.png")
    
    # plot lambda for energy ~ 20-70 keV
    first_bin = 0
    second_bin = 0
    bins=100
    for ii in range(len(x)-1):
        if x[ii] <= 20000.0 and x[ii+1] > 20000.0:
            first_bin = ii
        if x[ii] <= 70000.0 and x[ii+1] > 70000.0:
            second_bin = ii
    fig, axs = plt.subplots(figsize=(10,10))
    w_min = np.min(np.concatenate([w[first_bin],w[second_bin]]))
    w_max = np.max(np.concatenate([w[first_bin],w[second_bin]]))
    axs.hist(
        w[first_bin],
        bins=bins,
        range=[w_min,w_max],
        histtype='step',
        stacked=True,
        label=r"$\lambda(E=20$"+"keV)"
    )
    axs.hist(
        w[second_bin],
        bins=bins,
        range=[w_min,w_max],
        histtype='step',
        stacked=True,
        label=r"$\lambda(E=70$"+"keV)"
    )
    axs.set_xlabel(r"$\lambda$")
    axs.set_title(f"nTOF "+r"$\lambda$"+" values")
    plt.legend()
    plt.tight_layout()
    plt.savefig("plots/ntof_lambdas_20_70_keV.png")


    f = uproot.open("../data/Counts_TOF_3p3_natAr.root")
    hist, edges = f['LiG_TOF_Argon_1e17'].to_numpy()
    fig, axs = plt.subplots(figsize=(10,10))
    axs.stairs(hist, edges, fill=True)
    axs.set_xlim([50000,70000])
    axs.set_xlabel("TOF [ns]")
    axs.set_ylabel("Counts/bin/day")
    axs.set_title("nTOF Counts/bin/day vs. TOF [ns]")
    plt.tight_layout()
    plt.savefig("plots/ntof_counts_tof.png")

