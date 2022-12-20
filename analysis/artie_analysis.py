"""
"""
import numpy as np
import uproot
from matplotlib import pyplot as plt
import csv

class ArtieAnalysis:

    def __init__(self,
        ideal_file:     str="artieI_ideal_0.root",
        vacuum_file:    str="artieI_vacuum_0.root",
        argon_file:     str="artieI_argon_0.root",
        flight_path:    float=70,    # m
        resolution:     float=0.125  # us
    ):
        self.ideal_file = uproot.open(ideal_file)
        self.ideal_neutron_data = self.ideal_file["NeutronEventData"].arrays(library="np")
        self.ideal_energy = self.ideal_neutron_data["neutron_energy"] * 1000        # keV
        self.ideal_safe_passage = self.ideal_neutron_data["safe_passage"]
        self.ideal_first_scatter_z = self.ideal_neutron_data["first_scatter_z"]
        self.ideal_arrival_time = self.ideal_neutron_data["arrival_time"] / 1000.0  # us
        self.ideal_num_scatters = self.ideal_neutron_data["num_scatter"]

        self.vacuum_file = uproot.open(vacuum_file)
        self.vacuum_neutron_data = self.vacuum_file["NeutronEventData"].arrays(library="np")
        self.vacuum_energy = self.vacuum_neutron_data["neutron_energy"] * 1000        # keV
        self.vacuum_safe_passage = self.vacuum_neutron_data["safe_passage"]
        self.vacuum_arrival_time = self.vacuum_neutron_data["arrival_time"] / 1000.0  # us
        self.vacuum_num_scatters = self.vacuum_neutron_data["num_scatter"]

        self.argon_file = uproot.open(argon_file)
        self.argon_neutron_data = self.argon_file["NeutronEventData"].arrays(library="np")
        self.argon_energy = self.argon_neutron_data["neutron_energy"] * 1000       # keV
        self.argon_safe_passage = self.argon_neutron_data["safe_passage"]
        self.argon_arrival_time = self.argon_neutron_data["arrival_time"] / 1000.0  # us
        self.argon_num_scatters = self.argon_neutron_data["num_scatter"]

        self.configuration = self.argon_file['Configuration'].arrays(library="np")
        self.config = {
            self.configuration['parameter'][ii]: self.configuration['value'][ii]
            for ii in range(len(self.configuration['parameter']))
        }

        self.argon_mass = 6.6338e-26    # mass in kg
        self.lar_density = float(self.config['lar_density']) * 1000
        self.target_length = float(self.config['target_length']) / 100
        self.n = self.lar_density * self.target_length / self.argon_mass

        endf_energy = []
        endf_transmission = []
        endf_cross_section = []
        with open("endf_natar.csv", "r") as file:
            reader = csv.reader(file, delimiter=",")
            for row in reader:
                endf_energy.append(float(row[0]))
                endf_transmission.append(np.exp(-self.n * float(row[1])))
                endf_cross_section.append(float(row[1]))
        self.endf_energy = np.array(endf_energy)
        self.endf_transmission = np.array(endf_transmission)
        self.endf_cross_section = np.array(endf_cross_section)

        self.neutron_mass = 939.5654133*1000.0  # keV 
        self.speed_of_light  = 2.99792458*100.0    # m / mus
        self.flight_path = flight_path
        self.resolution = resolution

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
        number_of_bins: int=200,
        energy_min:     float=40,
        energy_max:     float=70,
        name:           str="Neutrons",
        save:   str='',
        show:   bool=False
    ):
        ideal_hist_detected, ideal_edges_detected = np.histogram(
            self.ideal_arrival_time[(self.ideal_arrival_time > 0)], 
            bins=number_of_bins,
        )
        vacuum_hist_detected, vacuum_edges_detected = np.histogram(
            self.vacuum_arrival_time[(self.vacuum_arrival_time > 0)], 
            bins=number_of_bins,
        )
        argon_hist_detected, argon_edges_detected = np.histogram(
            self.argon_arrival_time[(self.argon_arrival_time > 0)], 
            bins=number_of_bins,
        )
        bin_centers = 0.5*(ideal_edges_detected[1:] + ideal_edges_detected[:-1])

        fig, axs = plt.subplots()
        axs.errorbar(
            bin_centers,
            ideal_hist_detected,
            yerr=ideal_hist_detected**0.5,
            marker='.',
            c='b',
            label='ideal transmitted'
        )
        axs.errorbar(
            bin_centers,
            argon_hist_detected,
            yerr=argon_hist_detected**0.5,
            marker='.',
            c='r',
            label='argon transmitted'
        )
        axs.errorbar(
            bin_centers,
            vacuum_hist_detected,
            yerr=vacuum_hist_detected**0.5,
            marker='.',
            c='k',
            label='vacuum transmitted'
        )
        axs.set_xlabel("Time of Flight [" + r"$\mu$" + "s]")
        axs.set_ylabel("Neutrons")
        axs.set_title(f"{name} Time of Flight Distribution - L={self.target_length}m")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"{save}_neutron_time_of_flight.png")
        if(show):
            plt.show()
    
    def plot_scattering(self,
    ):

        categories = [
            'Scatter LAr','Scatter Out',
            'Elastic', 'Inelastic',
            'Capture', 'Fission'
        ]
        xticks = np.array([1,2,3,4,5,6])
        width = 0.25
        ideal_scatters = np.array([
            sum(self.ideal_neutron_data['num_scatter']),
            sum(self.ideal_neutron_data['num_scatter_out']),
            sum(self.ideal_neutron_data['num_elastic']),
            sum(self.ideal_neutron_data['num_inelastic']),
            sum(self.ideal_neutron_data['num_capture']),
            sum(self.ideal_neutron_data['num_fission'])
        ])
        ideal_total = ideal_scatters[0] + ideal_scatters[1]
        ideal_percent = np.round(100 * ideal_scatters / ideal_total).astype(int)
        argon_scatters = np.array([
            sum(self.argon_neutron_data['num_scatter']),
            sum(self.argon_neutron_data['num_scatter_out']),
            sum(self.argon_neutron_data['num_elastic']),
            sum(self.argon_neutron_data['num_inelastic']),
            sum(self.argon_neutron_data['num_capture']),
            sum(self.argon_neutron_data['num_fission'])
        ])
        argon_total = argon_scatters[0] + argon_scatters[1]
        argon_percent = np.round(100 * argon_scatters / argon_total).astype(int)
        vacuum_scatters = np.array([
            sum(self.vacuum_neutron_data['num_scatter']),
            sum(self.vacuum_neutron_data['num_scatter_out']),
            sum(self.vacuum_neutron_data['num_elastic']),
            sum(self.vacuum_neutron_data['num_inelastic']),
            sum(self.vacuum_neutron_data['num_capture']),
            sum(self.vacuum_neutron_data['num_fission'])
        ])
        vacuum_total = vacuum_scatters[0] + vacuum_scatters[1]
        vacuum_percent = np.round(100 * vacuum_scatters / vacuum_total).astype(int)

        fig, axs = plt.subplots(figsize=(10,6))
        ideal_bars = axs.bar(
            xticks - width, ideal_scatters, 
            width, color='b', label="ideal",
            alpha=0.5
        )
        argon_bars = axs.bar(
            xticks, argon_scatters, 
            width, color='r', label="argon",
            alpha=0.5
        )
        vacuum_bars = axs.bar(
            xticks + width, vacuum_scatters, 
            width, color='k', label="vacuum",
            alpha=0.5
        )
        axs.bar_label(ideal_bars, ideal_percent, padding=3)
        axs.bar_label(argon_bars, argon_percent, padding=3)
        axs.bar_label(vacuum_bars, vacuum_percent, padding=3)
        axs.set_xticks(xticks)
        axs.set_xticklabels(categories, rotation=45, ha='right')
        axs.set_yscale("log")
        axs.set_title(f"Number/Type of Scatters - L={self.target_length}m")
        plt.legend()
        plt.tight_layout()
        plt.show()


    
    def plot_energy(self,
        number_of_bins: int=200,
        energy_min:     float=40,
        energy_max:     float=70,
        name:           str="Neutrons",
        simulated:      bool=False,
        save:   str='',
        show:   bool=False
    ):
        # obtain simulated data time stamps from MC:
        if simulated:
            vacuum_simulated_t, vacuum_simulated_E = self.simulate_data_from_mc("vacuum")
            argon_simulated_t, argon_simulated_E = self.simulate_data_from_mc("argon")

        ideal_hist_all, ideal_edges_all = np.histogram(
            self.ideal_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        ideal_hist_detected, ideal_edges_detected = np.histogram(
            self.ideal_energy[(self.ideal_num_scatters == 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_all, vacuum_edges_all = np.histogram(
            self.vacuum_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_detected, vacuum_edges_detected = np.histogram(
            self.vacuum_energy[(self.vacuum_num_scatters == 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_all, argon_edges_all = np.histogram(
            self.argon_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_detected, argon_edges_detected = np.histogram(
            self.argon_energy[(self.argon_num_scatters == 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        bin_centers = 0.5*(ideal_edges_all[1:] + ideal_edges_all[:-1])

        fig, axs = plt.subplots()
        axs.errorbar(
            bin_centers,
            ideal_hist_all,
            yerr=ideal_hist_all**0.5,
            marker='.',
            c='k',
            label='ideal produced'
        )
        axs.errorbar(
            bin_centers,
            ideal_hist_detected,
            yerr=ideal_hist_detected**0.5,
            marker='.',
            c='r',
            label='ideal transmitted'
        )
        axs.hist(
            self.ideal_energy, 
            bins=number_of_bins, 
            range=[energy_min, energy_max],  
            color='k',
            histtype="step"
        )
        axs.hist(
            self.ideal_energy[(self.ideal_num_scatters == 0)], 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            color='r',
            histtype="step"
        )
        axs.set_xlabel("Kinetic Energy [keV]")
        axs.set_ylabel("Neutrons")
        axs.set_title(f"{name} Kinetic Energy Distribution - L={self.target_length}m")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"{save}_neutron_energy.png")
        if(show):
            plt.show()
    
    def plot_transmission(self,
        number_of_bins: int=200,
        energy_min:     float=40,
        energy_max:     float=70,
        name:           str="Neutrons",
        simulated:      bool=False,
        endf:           bool=True,
        save:   str='',
        show:   bool=False
    ):
        # obtain simulated data time stamps from MC:
        if simulated:
            vacuum_simulated_t, vacuum_simulated_E = self.simulate_data_from_mc("vacuum")
            argon_simulated_t, argon_simulated_E = self.simulate_data_from_mc("argon")

        ideal_hist_all, ideal_edges_all = np.histogram(
            self.ideal_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        ideal_hist_safe_passage, ideal_edges_safe_passage = np.histogram(
            self.ideal_energy[(self.ideal_safe_passage > 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        ideal_hist_detected, ideal_edges_detected = np.histogram(
            self.ideal_energy[(self.ideal_neutron_data['safe_passage'] == 1)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )

        vacuum_hist_all, vacuum_edges_all = np.histogram(
            self.vacuum_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_safe_passage, vacuum_edges_safe_passage = np.histogram(
            self.vacuum_energy[(self.vacuum_safe_passage > 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_detected, vacuum_edges_detected = np.histogram(
            self.vacuum_energy[(self.vacuum_neutron_data['safe_passage'] == 1)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )

        argon_hist_all, argon_edges_all = np.histogram(
            self.argon_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_safe_passage, argon_edges_safe_passage = np.histogram(
            self.argon_energy[(self.argon_safe_passage > 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_detected, argon_edges_detected = np.histogram(
            self.argon_energy[(self.argon_neutron_data['safe_passage'] == 1)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )

        if simulated:
            vacuum_hist_simulated, vacuum_edges_simulated = np.histogram(
                vacuum_simulated_E, 
                bins=number_of_bins,
                range=[energy_min, energy_max]
            )
            argon_hist_simulated, argon_edges_simulated = np.histogram(
                argon_simulated_E, 
                bins=number_of_bins,
                range=[energy_min, energy_max]
            )
        cbins = (ideal_edges_all[1:] + ideal_edges_all[:-1])/2.0

        ideal_transmission = np.zeros(cbins.size, dtype=float)
        ideal_errorbars = np.zeros(cbins.size, dtype=float)
        ideal_mask = (ideal_hist_all > 0)
        ideal_transmission[ideal_mask] = ideal_hist_detected[ideal_mask] / ideal_hist_all[ideal_mask]
        ideal_in = ideal_hist_detected[ideal_mask].astype(float)
        ideal_out = ideal_hist_all[ideal_mask].astype(float) - ideal_in
        ideal_errorbars[ideal_mask] = np.sqrt(
            np.divide(1.0, ideal_in, out=np.zeros_like(ideal_in), casting='unsafe', where=ideal_in!=0) +
            np.divide(1.0, ideal_out, out=np.zeros_like(ideal_out), casting='unsafe', where=ideal_out!=0)
        )

        vacuum_transmission = np.zeros(cbins.size, dtype=float)
        vacuum_errorbars = np.zeros(cbins.size, dtype=float)
        vacuum_mask = (vacuum_hist_all > 0)
        vacuum_transmission[vacuum_mask] = vacuum_hist_detected[vacuum_mask] / vacuum_hist_all[vacuum_mask]
        vacuum_in = vacuum_hist_detected[vacuum_mask].astype(float)
        vacuum_out = vacuum_hist_all[vacuum_mask].astype(float) - vacuum_in
        vacuum_errorbars[vacuum_mask] = np.sqrt(
            np.divide(1.0, vacuum_in, out=np.zeros_like(vacuum_in), casting='unsafe', where=vacuum_in!=0) +
            np.divide(1.0, vacuum_out, out=np.zeros_like(vacuum_out), casting='unsafe', where=vacuum_out!=0)
        )


        argon_transmission = np.zeros(cbins.size, dtype=float)
        argon_errorbars = np.zeros(cbins.size, dtype=float)
        argon_mask = (argon_hist_all > 0)
        argon_transmission[argon_mask] = argon_hist_detected[argon_mask] / argon_hist_all[argon_mask]
        argon_in = argon_hist_detected[argon_mask].astype(float)
        argon_out = argon_hist_all[argon_mask].astype(float) - argon_in
        argon_errorbars[argon_mask] = np.sqrt(
            np.divide(1.0, argon_in, out=np.zeros_like(argon_in), casting='unsafe', where=argon_in!=0) +
            np.divide(1.0, argon_out, out=np.zeros_like(argon_out), casting='unsafe', where=argon_out!=0)
        )


        if simulated:
            simulated_transmission = np.zeros(cbins.size, dtype=float)
            vacuum_simulated_mask = (vacuum_hist_simulated > 0)
            simulated_transmission[vacuum_simulated_mask] = argon_hist_simulated[vacuum_simulated_mask] / vacuum_hist_simulated[vacuum_simulated_mask]

        fig, axs = plt.subplots()
        axs.errorbar(
            cbins, ideal_transmission,
            yerr=ideal_errorbars,
            linestyle="--",
            color="b",
            label="ideal"
        )
        axs.errorbar(
            cbins, vacuum_transmission,
            #yerr=vacuum_errorbars,
            linestyle="--",
            color="k",
            label="vacuum"
        )
        axs.errorbar(
            cbins, argon_transmission,
            yerr=argon_errorbars,
            linestyle="--",
            color="r",
            label="argon"
        )
        if simulated:
            axs.plot(
                cbins, simulated_transmission,
                linestyle=":",
                color="m",
                label="simulated"
            )
        if endf:
            axs.plot(
                self.endf_energy, self.endf_transmission,
                linestyle='-',
                color='g',
                label='endf'
            )
        axs.set_xlim(energy_min, energy_max)
        axs.set_ylim(10e-3,1)
        axs.set_xlabel("Energy bin [keV]")
        axs.set_ylabel("Transmission")
        axs.set_title(f"Transmission vs. Energy [keV] - L={self.target_length}m")
        axs.set_yscale("log")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"{save}_neutron_transmission.png")
        if(show):
            plt.show()
    
    def plot_cross_section(self,
        number_of_bins: int=200,
        energy_min:     float=40,
        energy_max:     float=70,
        name:           str="Ideal Neutrons",
        simulated:      bool=False,
        endf:           bool=True,
        save:   str='',
        show:   bool=False
    ):
        # obtain simulated data time stamps from MC:
        if simulated:
            vacuum_simulated_t, vacuum_simulated_E = self.simulate_data_from_mc("vacuum")
            argon_simulated_t, argon_simulated_E = self.simulate_data_from_mc("argon")

        ideal_hist_all, ideal_edges_all = np.histogram(
            self.ideal_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        ideal_hist_detected, ideal_edges_detected = np.histogram(
            self.ideal_energy[(self.ideal_neutron_data['safe_passage'] == 1)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_all, vacuum_edges_all = np.histogram(
            self.vacuum_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_detected, vacuum_edges_detected = np.histogram(
            self.vacuum_energy[(self.vacuum_neutron_data['safe_passage'] == 1)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_all, argon_edges_all = np.histogram(
            self.argon_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_detected, argon_edges_detected = np.histogram(
            self.argon_energy[(self.argon_neutron_data['safe_passage'] == 1)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        if simulated:
            vacuum_hist_simulated, vacuum_edges_simulated = np.histogram(
                vacuum_simulated_E, 
                bins=number_of_bins,
                range=[energy_min, energy_max]
            )
            argon_hist_simulated, argon_edges_simulated = np.histogram(
                argon_simulated_E, 
                bins=number_of_bins,
                range=[energy_min, energy_max]
            )
        cbins = (ideal_edges_all[1:] + ideal_edges_all[:-1])/2.0

        ideal_transmission = np.zeros(cbins.size, dtype=float)
        ideal_errorbars = np.zeros(cbins.size, dtype=float)
        ideal_mask = (ideal_hist_all > 0)
        ideal_transmission[ideal_mask] = ideal_hist_detected[ideal_mask] / ideal_hist_all[ideal_mask]
        ideal_in = ideal_hist_detected[ideal_mask].astype(float)
        ideal_out = ideal_hist_all[ideal_mask].astype(float) - ideal_in
        ideal_errorbars[ideal_mask] = np.sqrt(
            np.divide(1.0, ideal_in, out=np.zeros_like(ideal_in), casting='unsafe', where=ideal_in!=0) +
            np.divide(1.0, ideal_out, out=np.zeros_like(ideal_out), casting='unsafe', where=ideal_out!=0)
        )

        vacuum_transmission = np.zeros(cbins.size, dtype=float)
        vacuum_errorbars = np.zeros(cbins.size, dtype=float)
        vacuum_mask = (vacuum_hist_all > 0)
        vacuum_transmission[vacuum_mask] = vacuum_hist_detected[vacuum_mask] / vacuum_hist_all[vacuum_mask]
        vacuum_in = vacuum_hist_detected[vacuum_mask].astype(float)
        vacuum_out = vacuum_hist_all[vacuum_mask].astype(float) - vacuum_in
        vacuum_errorbars[vacuum_mask] = np.sqrt(
            np.divide(1.0, vacuum_in, out=np.zeros_like(vacuum_in), casting='unsafe', where=vacuum_in!=0) +
            np.divide(1.0, vacuum_out, out=np.zeros_like(vacuum_out), casting='unsafe', where=vacuum_out!=0)
        )


        argon_transmission = np.zeros(cbins.size, dtype=float)
        argon_errorbars = np.zeros(cbins.size, dtype=float)
        argon_mask = (argon_hist_all > 0)
        argon_transmission[argon_mask] = argon_hist_detected[argon_mask] / argon_hist_all[argon_mask]
        argon_in = argon_hist_detected[argon_mask].astype(float)
        argon_out = argon_hist_all[argon_mask].astype(float) - argon_in
        argon_errorbars[argon_mask] = np.sqrt(
            np.divide(1.0, argon_in, out=np.zeros_like(argon_in), casting='unsafe', where=argon_in!=0) +
            np.divide(1.0, argon_out, out=np.zeros_like(argon_out), casting='unsafe', where=argon_out!=0)
        )


        if simulated:
            simulated_transmission = np.zeros(cbins.size, dtype=float)
            vacuum_simulated_mask = (vacuum_hist_simulated > 0)
            simulated_transmission[vacuum_simulated_mask] = argon_hist_simulated[vacuum_simulated_mask] / vacuum_hist_simulated[vacuum_simulated_mask]

        ideal_cross_section = np.zeros(cbins.size, dtype=float)
        ideal_mask = (ideal_transmission > 0)
        ideal_cross_section[ideal_mask] = -(1.0/self.n) * np.log(ideal_transmission[ideal_mask])

        vacuum_cross_section = np.zeros(cbins.size, dtype=float)
        vacuum_mask = (vacuum_transmission > 0)
        vacuum_cross_section[vacuum_mask] = -(1.0/self.n) * np.log(vacuum_transmission[vacuum_mask])

        argon_cross_section = np.zeros(cbins.size, dtype=float)
        argon_mask = (argon_transmission > 0)
        argon_cross_section[argon_mask] = -(1.0/self.n) * np.log(argon_transmission[argon_mask])

        if simulated:
            simulated_cross_section = np.zeros(cbins.size, dtype=float)
            simulated_mask = (simulated_transmission > 0)
            simulated_cross_section[simulated_mask] = -(1.0/self.n) * np.log(simulated_transmission[simulated_mask])

        fig, axs = plt.subplots()
        axs.errorbar(
            cbins, ideal_cross_section,
            yerr=(1.0/self.n)*ideal_errorbars,
            linestyle="--",
            color="b",
            label="ideal"
        )
        axs.errorbar(
            cbins, vacuum_cross_section,
            #yerr=vacuum_errorbars,
            linestyle="--",
            color="k",
            label="vacuum"
        )
        axs.errorbar(
            cbins, argon_cross_section,
            yerr=(1.0/self.n)*argon_errorbars,
            linestyle="--",
            color="r",
            label="argon"
        )
        if simulated:
            axs.errorbar(
                cbins, simulated_cross_section,
                linestyle=":",
                color="m",
                label="simulated"
            )
        if endf:
            axs.errorbar(
                self.endf_energy, self.endf_cross_section,
                linestyle='-',
                color='g',
                label='endf'
            )
        axs.set_xlim(energy_min, energy_max)
        axs.set_xlabel("Energy bin [keV]")
        axs.set_ylabel("Cross Section [barn]")
        axs.set_title(f"Cross Section [barn] vs. Energy [keV] - L={self.target_length}m")
        axs.set_yscale("log")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"{save}_neutron_cross_section.png")
        if(show):
            plt.show()

    