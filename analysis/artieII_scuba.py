from artie_analysis import *

if __name__ == "__main__":

    artieII_cfg = {
        "inputs":   {
            "argon":    "../outputs/artieII_ntof_ideal_scuba_medium_0.root",
            "vacuum":   "../outputs/artieII_ntof_vacuum_scuba_medium_0.root"
        }
    }

    artie = ArtieAnalysis(
        artieII_cfg
    )

    number_of_bins=100
    energy_min=200
    energy_max=2000
    save='artieII_ntof_scuba_medium'
    show=False

    artie.plot_all(
        artieII_cfg["inputs"].keys(),
        number_of_bins=number_of_bins,
        energy_min=energy_min,
        energy_max=energy_max,
        save=save,
        show=show,
        name="nTOF"
    )