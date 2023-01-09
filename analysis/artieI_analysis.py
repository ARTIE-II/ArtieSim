from artie_analysis import *

if __name__ == "__main__":

    artieI_cfg = {
        "inputs":   {
            "ideal":    "../outputs/artieI_ideal_long_0.root",
            "argon":    "../outputs/artieI_argon_long_0.root",
            "vacuum":   "../outputs/artieI_vacuum_long_0.root"
        }
    }

    artie = ArtieAnalysis(
        artieI_cfg
    )

    number_of_bins=200
    energy_min=20
    energy_max=70
    save='artieI_long_low'
    show=False

    artie.plot_all(
        artieI_cfg["inputs"].keys(),
        number_of_bins=number_of_bins,
        energy_min=energy_min,
        energy_max=energy_max,
        save=save,
        show=show
    )