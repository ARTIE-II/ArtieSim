from artie_analysis import *

short_ideal_file = "artieI_ideal_short_0.root"
short_argon_file = "artieI_argon_short_0.root"
short_vacuum_file = "artieI_vacuum_short_0.root"

long_ideal_file = "artieI_ideal_long_0.root"
long_argon_file = "artieI_argon_long_0.root"
long_vacuum_file = "artieI_vacuum_long_0.root"

energy_min1 = 20
energy_min2 = 70

energy_max1 = 70
energy_max2 = 200

number_of_bins = 200

artie_long = ArtieAnalysis(
    ideal_file=long_ideal_file,
    vacuum_file=long_vacuum_file,
    argon_file=long_argon_file
)
artie_short = ArtieAnalysis(
    ideal_file=short_ideal_file,
    vacuum_file=short_vacuum_file,
    argon_file=short_argon_file
)

artie_long.plot_time_of_flight(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_long_low'
)
artie_long.plot_energy(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_long_low'
)
artie_long.plot_transmission(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_long_low'
)
artie_long.plot_cross_section(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_long_low'
)
artie_long.plot_time_of_flight(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_long_high'
)
artie_long.plot_energy(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_long_high'
)
artie_long.plot_transmission(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_long_high'
)
artie_long.plot_cross_section(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_long_high'
)

artie_short.plot_time_of_flight(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_short_low'
)
artie_short.plot_energy(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_short_low'
)
artie_short.plot_transmission(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_short_low'
)
artie_short.plot_cross_section(
    number_of_bins=number_of_bins,
    energy_min=energy_min1,
    energy_max=energy_max1,
    save='artieI_short_low'
)
artie_short.plot_time_of_flight(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_short_high'
)
artie_short.plot_energy(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_short_high'
)
artie_short.plot_transmission(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_short_high'
)
artie_short.plot_cross_section(
    number_of_bins=number_of_bins,
    energy_min=energy_min2,
    energy_max=energy_max2,
    save='artieI_short_high'
)
