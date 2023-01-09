"""
"""
import numpy as np
import uproot
from matplotlib import pyplot as plt
import csv
import os


artieI_cfg = {
    "inputs":   {
        "ideal":    "artieI_ideal_0.root",
        "argon":    "artieI_argon_0.root",
        "vacuum":   "artieI_vacuum_0.root"
    }
}

default_simulation_parameters = {
    'lar_density':      1.39,   #
    'target_length':    1.68,   # cm
}

class ArtieAnalysis:

    def __init__(self,
        cfg:    dict=artieI_cfg,
        flight_path:    float=70,    # m
        resolution:     float=0.125  # us
    ):
        self.cfg = cfg
        self.files = {}
        self.data = {}
        self.configuration = {}
        self.simulation_parameters = {}
        for key, value in self.cfg['inputs'].items():
            try:
                self.files[key] = uproot.open(value)
                self.data[key] = self.files[key]['NeutronEventData'].arrays(library="np")
                self.data[key]["neutron_energy"] *= 1000.0    # MeV -> keV
                self.data[key]["arrival_time"] /= 1000.0      # ns -> us

                self.configuration[key] = self.files[key]["Configuration"].arrays(library="np")
                temp_config = {
                    self.configuration[key]['parameter'][ii]: self.configuration[key]['value'][ii]
                    for ii in range(len(self.configuration[key]['parameter']))
                }
                for param in default_simulation_parameters.keys():
                    if(param not in self.simulation_parameters.keys()):
                        self.simulation_parameters[param] = temp_config[param]
                    else:
                        if(self.simulation_parameters[param] != temp_config[param]):
                            print(
                                f"Warning! parameter '{param}' has different values between input files" + 
                                f"({self.simulation_parameters[param]} vs {temp_config[param]})"
                            )
            except:
                raise FileNotFoundError(value)

        self.argon_mass = 663.38    # mass in kg/barn
        self.lar_density = float(self.simulation_parameters['lar_density']) * 1000
        self.target_length = float(self.simulation_parameters['target_length']) / 100
        self.n = self.lar_density * self.target_length / self.argon_mass

        endf_energy = []
        endf_transmission = []
        endf_cross_section = []
        with open("../data/endf_natar.csv", "r") as file:
            reader = csv.reader(file, delimiter=",")
            for row in reader:
                endf_energy.append(float(row[0]))
                endf_transmission.append(np.exp(-self.n * float(row[1])))
                endf_cross_section.append(float(row[1]))
        self.endf_energy = np.array(endf_energy)
        self.endf_transmission = np.array(endf_transmission)
        self.endf_cross_section = np.array(endf_cross_section)

        self.neutron_mass = 939.5654133 * 1000.0    # keV 
        self.speed_of_light  = 2.99792458 * 100.0   # m / mus
        self.flight_path = flight_path
        self.resolution = resolution

        if not os.path.isdir("plots/"):
            os.mkdir("plots/")

    def bin_with_errors(self,
        data:       None,
        dataset:    str="",
        variable:   str="",
        number_of_bins: int=200,
        range:      list=[],
    ):
        if dataset != "" and variable != "":
            data = self.data[dataset][variable]
        if len(range) == 0:
            hist, edges = np.histogram(
                data, 
                bins=number_of_bins,
            )
        else:
            hist, edges = np.histogram(
                data, 
                bins=number_of_bins,
                range=range
            )
        bin_ids = np.digitize(
            data, 
            edges
        )
        # numpy.digitize only considers bin[ii] <= x < bin[ii+1]
        # so we need to take all the entries outside the last bin and
        # shift them to be included into the last bin.
        bin_ids[(bin_ids == len(edges))] = len(edges) - 1
        unique_bin_ids = np.unique(bin_ids) - 1

        bin_means = np.zeros(hist.size, dtype=float)
        bin_stds = np.zeros(hist.size, dtype=float)
        
        for bin_id in unique_bin_ids:
            bin_means[bin_id] = np.mean(data[(bin_ids == bin_id)])
            bin_stds[bin_id] = np.std(data[(bin_ids == bin_id)])

        hist_errors = np.sqrt(hist)
        return bin_means, bin_stds, hist, hist_errors, edges

    def compute_pull_hist(self,
        hist,
    ):
        pull_hist = np.zeros(hist.size, dtype=float)
        means = np.zeros(hist.size, dtype=float)
        means[0] = 0.5 * (hist[0] + hist[1])
        means[-1] = 0.5 * (hist[-1] + hist[-2])
        for ii in range(1,len(hist)-1):
            means[ii] = (1./3.) * (hist[ii-1] + hist[ii] + hist[ii+1])
        stds = np.sqrt(means)
        pull_hist[(stds > 0)] = (hist[(stds > 0)] - means[(stds > 0)]) / stds[(stds > 0)]
        return pull_hist

    def compute_transmission_with_errors(self,
        argon_hist,
        argon_hist_errors,
        vacuum_hist,
        vacuum_hist_errors
    ):
        transmission = np.zeros(vacuum_hist.size, dtype=float)
        transmission_errors = np.zeros(vacuum_hist.size, dtype=float)

        transmission_mask = (vacuum_hist > 0)
        argon_mask = (argon_hist > 0)
        vacuum_mask = (vacuum_hist > 0)
        transmission[transmission_mask] = argon_hist[transmission_mask] / vacuum_hist[transmission_mask]

        # error in transmission is given by the equation
        # delta T / T = sqrt((delta C_in/C_in)^2 + (delta C_out/C_out)^2)
        # where C_in and C_out are the counts for scatter in/scatter out respectively,
        # (i.e. the bin counts for total_hist and passage_hist), and the errors
        # are the statistical errors (passage_hist_errors and non_passage_hist_errors).
        transmission_errors[argon_mask] += np.power(
            argon_hist_errors[argon_mask] / argon_hist[argon_mask], 2
        )
        transmission_errors[vacuum_mask] += np.power(
            vacuum_hist_errors[vacuum_mask] / vacuum_hist[vacuum_mask], 2
        )
        transmission_errors = transmission * np.sqrt(transmission_errors)

        return transmission, transmission_errors

    def compute_cross_section_with_errors(self,
        transmission,
        transmission_errors
    ):
        cross_section = np.zeros(transmission.size, dtype=float)
        cross_section_errors = np.zeros(transmission.size, dtype=float)

        # cross section is defined from transmission as
        # sigma = -(1/n)log(T), where n is rho * L / m, and 
        # rho is the linear density, L is the length of the detector
        # and m is the mass of Argon.

        # the errors on cross section are 
        # delta sigma = (1/n) delta T/ T
        transmission_mask = (transmission > 0)
        cross_section[transmission_mask] = -(1.0/self.n) * np.log(transmission[transmission_mask])
        cross_section_errors[transmission_mask] = (1.0/self.n) * transmission_errors[transmission_mask] / transmission[transmission_mask]

        return cross_section, cross_section_errors

    def energy_from_tof(self,
        tof
    ):
        beta = (self.flight_path / tof) / self.speed_of_light
        gamma = (1.0 / (1 - beta**2))**0.5
        return (gamma - 1.0) * self.neutron_mass   

    def smear_time(self,
        t
    ):
        t = t + np.random.uniform(
            -self.resolution/2.0, 
            self.resolution/2.0, 
            size=t.size
        ) 
        return t

    def simulate_data_from_mc(self,
        simulation: str="ideal"
    ):
        if simulation == "ideal":
            t = np.array(self.ideal_neutron_data["arrival_time"]/1000.0)
        elif simulation == "vacuum":
            t = np.array(self.vacuum_neutron_data["arrival_time"]/1000.0)
        else:
            t = np.array(self.argon_neutron_data["arrival_time"]/1000.0)
        t = t[(t > 0)]
        t = self.smear_time(t)
        e = self.energy_from_tof(t)
        return t, e

    def plot_time_of_flight(self,
        inputs:         list=[""],
        number_of_bins: int=200,
        name:           str="Neutrons",
        save:   str='',
        show:   bool=False,
        make_pull:  bool=True
    ):
        fig, axs = plt.subplots(figsize=(10, 6))
        pull_hist = {}
        for input in inputs:
            bin_means, bin_stds, hist, hist_errors, edges = self.bin_with_errors(
                data=self.data[input]["arrival_time"][(self.data[input]["arrival_time"] > 0)],
                number_of_bins=number_of_bins
            )
            bin_centers = 0.5*(edges[1:] + edges[:-1])
            pull_hist[input] = self.compute_pull_hist(hist)
            axs.errorbar(
                bin_centers, hist,
                #xerr=bin_stds, 
                yerr=hist_errors,
                #linestyle='',
                marker='.', label=input
            )
        axs.set_xlabel("Time of Flight [" + r"$\mu$" + "s]")
        axs.set_ylabel("Neutrons")
        axs.set_title(f"{name} Time of Flight Distribution - L={self.target_length}m")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"plots/{save}_neutron_time_of_flight.png")
        if(show):
            plt.show()
        plt.close()
        if(make_pull):
            pull_fig, pull_axs = plt.subplots()
            for input in inputs:
                pull_axs.hist(
                    pull_hist[input], 
                    bins=20, range=[-3,3],
                    histtype='step', stacked=True,
                    label=f'{input} ({np.mean(pull_hist[input]):.2f},{np.std(pull_hist[input]):.2f})'
                )
            pull_axs.set_xlabel(r"$(X_i - \bar{X}_i)/\sqrt{\bar{X}_i}$")
            pull_axs.set_title(f"{name} Time of Flight Pull Distribution - L={self.target_length}m")
            plt.legend()
            plt.tight_layout()
            if(save != ''):
                plt.savefig(f"plots/{save}_neutron_time_of_flight_pull.png")
            if(show):
                plt.show()
            plt.close()

    def plot_energy(self,
        inputs:         list=[""],
        number_of_bins: int=200,
        energy_min:     float=-1,
        energy_max:     float=-1,
        name:           str="Neutrons",
        save:   str='',
        show:   bool=False,
        make_pull:  bool=True
    ):
        fig, axs = plt.subplots(figsize=(10, 6))
        pull_hist = {}
        if energy_min == -1 or energy_max == -1:
            range = []
        else:
            range = [energy_min, energy_max]
            axs.set_xlim(energy_min, energy_max)
        for input in inputs:
            bin_means, bin_stds, hist, hist_errors, edges = self.bin_with_errors(
                data=self.data[input]["neutron_energy"][(self.data[input]["arrival_time"] > 0)],
                number_of_bins=number_of_bins, range=range
            )
            bin_centers = 0.5*(edges[1:] + edges[:-1])
            pull_hist[input] = self.compute_pull_hist(hist)
            axs.errorbar(
                bin_centers, hist,
                #xerr=bin_stds, 
                yerr=hist_errors,
                #linestyle='',
                marker='.', label=input
            ) 
        axs.set_xlabel("Kinetic Energy [keV]")
        axs.set_ylabel("Neutrons")
        axs.set_title(f"{name} Kinetic Energy Distribution - L={self.target_length}m")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"plots/{save}_neutron_energy.png")
        if(show):
            plt.show()
        plt.close()
        if(make_pull):
            pull_fig, pull_axs = plt.subplots()
            for input in inputs:
                pull_axs.hist(
                    pull_hist[input], 
                    bins=20, range=[-3,3],
                    histtype='step', stacked=True,
                    label=f'{input} ({np.mean(pull_hist[input]):.2f},{np.std(pull_hist[input]):.2f})'
                )
            pull_axs.set_xlabel(r"$(X_i - \bar{X}_i)/\sqrt{\bar{X}_i}$")
            pull_axs.set_title(f"{name} Kinetic Energy Distribution - L={self.target_length}m")
            plt.legend()
            plt.tight_layout()
            if(save != ''):
                plt.savefig(f"plots/{save}_neutron_energy_pull.png")
            if(show):
                plt.show()
            plt.close()
    
    def plot_transmission(self,
        inputs:         list=[""],
        number_of_bins: int=200,
        energy_min:     float=-1,
        energy_max:     float=-1,
        name:           str="Neutrons",
        log_scale:      bool=False,
        save:   str='',
        show:   bool=False
    ):
        
        fig, axs = plt.subplots(figsize=(10, 6))
        if energy_min == -1 or energy_max == -1:
            range = []
        else:
            range = [energy_min, energy_max]
            axs.set_xlim(energy_min, energy_max)
        for input in inputs:
            if input == "vacuum":
                continue
            argon_means, argon_stds, argon_hist, argon_hist_errors, argon_edges = self.bin_with_errors(
                data=self.data[input]["neutron_energy"][(self.data[input]["safe_passage"] == 1)],
                number_of_bins=number_of_bins, range=range
            )
            vacuum_means, vacuum_stds, vacuum_hist, vacuum_hist_errors, vacuum_edges = self.bin_with_errors(
                data=self.data["vacuum"]["neutron_energy"][(self.data[input]["safe_passage"] == 1)],
                number_of_bins=number_of_bins, range=range
            )
            bin_centers = 0.5*(argon_edges[1:] + argon_edges[:-1])

            transmission, transmission_errors = self.compute_transmission_with_errors(
                argon_hist, argon_hist_errors, vacuum_hist, vacuum_hist_errors
            )

            axs.errorbar(
                bin_centers, transmission,
                #xerr=total_stds, 
                yerr=transmission_errors,
                marker='.', label=input
            ) 
        axs.set_xlabel("Energy bin [keV]")
        axs.set_ylabel("Transmission C({input})/C(vacuum)")
        axs.set_title(f"Transmission vs. Energy [keV] - L={self.target_length:.2f}m")
        if log_scale:
            axs.set_yscale("log")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"plots/{save}_neutron_transmission.png")
        if(show):
            plt.show()
        plt.close()

    def plot_cross_section(self,
        inputs:         list=[""],
        number_of_bins: int=200,
        energy_min:     float=-1,
        energy_max:     float=-1,
        endf:           bool=True,
        name:           str="Neutrons",
        save:   str='',
        show:   bool=False
    ):
        fig, axs = plt.subplots(figsize=(10, 6))
        if energy_min == -1 or energy_max == -1:
            range = []
        else:
            range = [energy_min, energy_max]
            axs.set_xlim(energy_min, energy_max)
        for input in inputs:
            if input == "vacuum":
                continue
            argon_means, argon_stds, argon_hist, argon_hist_errors, argon_edges = self.bin_with_errors(
                data=self.data[input]["neutron_energy"][(self.data[input]["safe_passage"] == 1)],
                number_of_bins=number_of_bins, range=range
            )
            vacuum_means, vacuum_stds, vacuum_hist, vacuum_hist_errors, vacuum_edges = self.bin_with_errors(
                data=self.data["vacuum"]["neutron_energy"][(self.data[input]["safe_passage"] == 1)],
                number_of_bins=number_of_bins, range=range
            )
            bin_centers = 0.5*(argon_edges[1:] + argon_edges[:-1])

            transmission, transmission_errors = self.compute_transmission_with_errors(
                argon_hist, argon_hist_errors, vacuum_hist, vacuum_hist_errors
            )

            cross_section, cross_section_errors = self.compute_cross_section_with_errors(
                transmission, transmission_errors
            )

            axs.errorbar(
                bin_centers, cross_section,
                #xerr=total_stds, 
                yerr=cross_section_errors,
                marker='.', label=input
            ) 
        if endf:
            axs.errorbar(
                self.endf_energy, self.endf_cross_section,
                linestyle='-',
                color='k',
                label='endf'
            )
        axs.set_xlabel("Energy bin [keV]")
        axs.set_ylabel("Cross Section [barn]")
        axs.set_title(f"Cross Section [barn] vs. Energy [keV] - L={self.target_length}m")
        axs.set_yscale("log")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"plots/{save}_neutron_cross_section.png")
        if(show):
            plt.show()
        plt.close()

    def plot_all(self,
        inputs:         list=[""],
        number_of_bins: int=200,
        energy_min:     float=-1,
        energy_max:     float=-1,
        name:           str="Neutrons",
        save:   str='',
        show:   bool=False
    ):    
        self.plot_time_of_flight(
            inputs=inputs,
            number_of_bins=number_of_bins,
            name=name,
            save=save,
            show=show
        )
        self.plot_energy(
            inputs=inputs,
            number_of_bins=number_of_bins,
            energy_min=energy_min,
            energy_max=energy_max,
            name=name,
            save=save,
            show=show
        )
        self.plot_transmission(
            inputs=inputs,
            number_of_bins=number_of_bins,
            energy_min=energy_min,
            energy_max=energy_max,
            name=name,
            save=save,
            show=show
        )
        self.plot_cross_section(
            inputs=inputs,
            number_of_bins=number_of_bins,
            energy_min=energy_min,
            energy_max=energy_max,
            name=name,
            save=save,
            show=show
        )