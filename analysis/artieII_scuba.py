from artie_analysis import *

if __name__ == "__main__":

    artieII_cfg = {
        "inputs":   {
            "argon":    "../outputs/artieII_ntof_ideal_scuba_high_0.root",
            "vacuum":   "../outputs/artieII_ntof_vacuum_scuba_high_0.root"
        }
    }

    artie = ArtieAnalysis(
        artieII_cfg
    )

    number_of_bins=100
    energy_vals = [20, 70, 200, 400, 600, 800, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 15000, 20000]
    save='artieII_ntof_scuba_high_'
    show=False
    for ii in range(len(energy_vals) - 1):
        energy_min = energy_vals[ii]
        energy_max = energy_vals[ii+1]
        artie.plot_all(
            artieII_cfg["inputs"].keys(),
            number_of_bins=number_of_bins,
            energy_min=energy_min,
            energy_max=energy_max,
            save=save+f"{energy_min}_{energy_max}",
            show=show,
            name="nTOF"
        )