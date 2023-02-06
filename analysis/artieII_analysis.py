from artie_analysis import *

if __name__ == "__main__":

    artieII_cfg = {
        "inputs":   {
            "argon":    "../outputs/artieII_ideal_long_0.root",
            "vacuum":   "../outputs/artieII_vacuum_long_0.root"
        }
    }

    artie = ArtieAnalysis(
        artieII_cfg
    )

    number_of_bins=100
    energy_min=20
    energy_max=70
    save='artieII_long_low'
    show=False

    artie.plot_all(
        artieII_cfg["inputs"].keys(),
        number_of_bins=number_of_bins,
        energy_min=energy_min,
        energy_max=energy_max,
        save=save,
        show=show
    )