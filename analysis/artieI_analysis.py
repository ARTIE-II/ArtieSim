from artie_analysis import *

if __name__ == "__main__":

    artieI_cfg = {
        "inputs":   {
            "ideal":    "../outputs/artieI_ideal_short_0.root",
            "argon":    "../outputs/artieI_argon_short_0.root",
            "vacuum":   "../outputs/artieI_vacuum_short_0.root"
        }
    }

    artie = ArtieAnalysis(
        artieI_cfg
    )

    number_of_bins=200
    energy_min=70
    energy_max=200
    save='artieI_short_high'
    show=False

    artie.plot_all(
        artieI_cfg["inputs"].keys(),
        number_of_bins=number_of_bins,
        energy_min=energy_min,
        energy_max=energy_max,
        save=save,
        show=show
    )