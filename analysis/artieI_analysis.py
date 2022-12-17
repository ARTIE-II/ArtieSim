"""
"""
import numpy as np
import uproot
from matplotlib import pyplot as plt
import csv

class ArtieIAnalysis:

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

        self.vacuum_file = uproot.open(vacuum_file)
        self.vacuum_neutron_data = self.vacuum_file["NeutronEventData"].arrays(library="np")
        self.vacuum_energy = self.vacuum_neutron_data["neutron_energy"] * 1000        # keV
        self.vacuum_safe_passage = self.vacuum_neutron_data["safe_passage"]
        self.vacuum_arrival_time = self.vacuum_neutron_data["arrival_time"] / 1000.0  # us

        self.argon_file = uproot.open(argon_file)
        self.argon_neutron_data = self.argon_file["NeutronEventData"].arrays(library="np")
        self.argon_energy = self.argon_neutron_data["neutron_energy"] * 1000       # keV
        self.argon_safe_passage = self.argon_neutron_data["safe_passage"]
        self.argon_arrival_time = self.argon_neutron_data["arrival_time"] / 1000.0  # us

        endf_energy = []
        endf_transmission = []
        endf_cross_section = []
        with open("endf_natar.csv", "r") as file:
            reader = csv.reader(file, delimiter=",")
            for row in reader:
                endf_energy.append(float(row[0]))
                endf_transmission.append(np.exp(-4.2 * float(row[1])))
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
        ideal_detected_time = self.ideal_arrival_time[(self.ideal_arrival_time>0)]
        ideal_energy_detected = self.ideal_energy[(self.ideal_arrival_time>0)]
        ideal_energy_tof = self.energy_from_tof(ideal_detected_time)

        vacuum_detected_time = self.vacuum_arrival_time[(self.vacuum_arrival_time>0)]
        vacuum_energy_detected = self.vacuum_energy[(self.vacuum_arrival_time>0)]
        vacuum_energy_tof = self.energy_from_tof(vacuum_detected_time)

        argon_detected_time = self.argon_arrival_time[(self.argon_arrival_time>0)]
        argon_energy_detected = self.argon_energy[(self.argon_arrival_time>0)]
        argon_energy_tof = self.energy_from_tof(argon_detected_time)

        tfin  = np.linspace(15,30,150)
        efin = self.energy_from_tof(tfin)

        fig, axs = plt.subplots()
        axs.scatter(
            ideal_energy_detected, ideal_detected_time,
            linestyle="-.", color="b",
            label="ideal"
        )
        axs.scatter(
            argon_energy_detected, argon_detected_time,
            linestyle="-.", color="r",
            label="argon"
        )
        axs.scatter(
            vacuum_energy_detected, vacuum_detected_time,
            linestyle="-.", color="k",
            label="vacuum"
        )
        axs.scatter(
            efin, tfin,
            linestyle='--', color='g',
            label="Einstein"
        )
        axs.set_xlim(xmin=energy_min, xmax=energy_max)
        axs.set_xlabel("Kinetic Energy [keV]")
        axs.set_ylabel("Time-of-Flight [" + r"$\mu$" + "s]")
        axs.set_title(f"{name} Time-of-Flight [" + r"$\mu$" + "s] vs. Kinetic Energy [keV]")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"{save}_neutron_time_of_flight.png")
        
        # ideal_delta = ideal_energy_tof - ideal_energy_detected
        # vacuum_delta = vacuum_energy_tof - vacuum_energy_detected
        # argon_delta = argon_energy_tof - argon_energy_detected

        # ideal_hist_delta, ideal_edges_delta = np.histogram(
        #     ideal_delta, 
        #     bins=number_of_bins,
        #     range=[-10.0,10.0]
        # )
        # vacuum_hist_delta, vacuum_edges_delta = np.histogram(
        #     vacuum_delta, 
        #     bins=number_of_bins,
        #     range=[-10.0,10.0]
        # )
        # argon_hist_delta, argon_edges_delta = np.histogram(
        #     argon_delta, 
        #     bins=number_of_bins,
        #     range=[-10.0,10.0]
        # )

        # cbins = (ideal_edges_delta[:-1] + ideal_edges_delta[1:])/2.0

        # fig, axs = plt.subplots()
        # axs.plot(
        #     cbins, ideal_hist_delta,
        #     linestyle='-',
        #     color="b",
        #     label=r"ideal $\mu$" + f" = {np.mean(ideal_delta):.2e}\n" + r"$\sigma$" + f" = {np.std(ideal_delta):.2e}"
        # )
        # axs.plot(
        #     cbins, vacuum_hist_delta,
        #     linestyle='-',
        #     color="k",
        #     label=r"$vacuum \mu$" + f" = {np.mean(vacuum_delta):.2e}\n" + r"$\sigma$" + f" = {np.std(vacuum_delta):.2e}"
        # )
        # axs.plot(
        #     cbins, argon_hist_delta,
        #     linestyle='-',
        #     color="r",
        #     label=r"argon $\mu$" + f" = {np.mean(argon_delta):.2e}\n" + r"$\sigma$" + f" = {np.std(argon_delta):.2e}"
        # )
        # axs.set_xlabel("Kinetic Energy (measured - actual) [keV]")
        # axs.set_ylabel("")
        # plt.legend()
        # plt.tight_layout()
        # if(save != ''):
        #     plt.savefig(f"{save}_neutron_time_of_flight_delta.png")
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
        axs.set_title("Number/Type of Scatters")
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

        fig, axs = plt.subplots()
        axs.hist(
            self.ideal_energy, 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="ideal produced", 
            stacked=True,
            histtype="step", density=True
        )
        axs.hist(
            self.ideal_energy[(self.ideal_arrival_time > 0)], 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="ideal detected", 
            stacked=True,
            histtype="step", density=True
        )
        axs.hist(
            self.vacuum_energy, 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="vacuum produced", 
            stacked=True,
            histtype="step", density=True
        )
        axs.hist(
            self.vacuum_energy[(self.vacuum_arrival_time > 0)], 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="vacuum detected", 
            stacked=True,
            histtype="step", density=True
        )
        axs.hist(
            self.argon_energy, 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="argon produced", 
            stacked=True,
            histtype="step", density=True
        )
        axs.hist(
            self.argon_energy[(self.argon_arrival_time > 0)], 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="argon detected", 
            stacked=True,
            histtype="step", density=True
        )
        if simulated:
            axs.hist(
                argon_simulated_E, 
                bins=number_of_bins, 
                range=[energy_min, energy_max], 
                label="argon simulated", 
                stacked=True,
                histtype="step", density=True
            )
            axs.hist(
                vacuum_simulated_E, 
                bins=number_of_bins, 
                range=[energy_min, energy_max], 
                label="vacuum simulated", 
                histtype="step", density=True
            )
        axs.set_xlabel("Kinetic Energy [keV]")
        axs.set_ylabel("Neutrons")
        axs.set_title(f"{name} Kinetic Energy Distribution")
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
            self.ideal_energy[(self.ideal_neutron_data['num_scatter'] == 0)], 
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
            self.vacuum_energy[(self.vacuum_neutron_data['num_scatter'] == 0)], 
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
            self.argon_energy[(self.argon_neutron_data['num_scatter'] == 0)], 
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

        ideal_transmission = np.zeros(cbins.size)
        ideal_transmission_safe = np.zeros(cbins.size)
        ideal_mask = (ideal_hist_all > 0)
        ideal_transmission[ideal_mask] = ideal_hist_detected[ideal_mask] / ideal_hist_all[ideal_mask]
        ideal_mask = (ideal_hist_safe_passage > 0)
        ideal_transmission_safe[ideal_mask] = ideal_hist_safe_passage[ideal_mask] / ideal_hist_all[ideal_mask]

        vacuum_transmission = np.zeros(cbins.size)
        vacuum_transmission_safe = np.zeros(cbins.size)
        vacuum_mask = (vacuum_hist_all > 0)
        vacuum_transmission[vacuum_mask] = vacuum_hist_detected[vacuum_mask] / vacuum_hist_all[vacuum_mask]
        vacuum_mask = (vacuum_hist_safe_passage > 0)
        vacuum_transmission_safe[vacuum_mask] = vacuum_hist_safe_passage[vacuum_mask] / vacuum_hist_all[vacuum_mask]

        argon_transmission = np.zeros(cbins.size)
        argon_transmission_safe = np.zeros(cbins.size)
        argon_mask = (argon_hist_all > 0)
        argon_transmission[argon_mask] = argon_hist_detected[argon_mask] / argon_hist_all[argon_mask]
        argon_mask = (argon_hist_safe_passage > 0)
        argon_transmission_safe[argon_mask] = argon_hist_safe_passage[argon_mask] / argon_hist_all[argon_mask]

        if simulated:
            simulated_transmission = np.zeros(cbins.size)
            vacuum_simulated_mask = (vacuum_hist_simulated > 0)
            simulated_transmission[vacuum_simulated_mask] = argon_hist_simulated[vacuum_simulated_mask] / vacuum_hist_simulated[vacuum_simulated_mask]

        fig, axs = plt.subplots()
        axs.plot(
            cbins, ideal_transmission,
            linestyle="--",
            color="b",
            label="ideal"
        )
        axs.plot(
            cbins, vacuum_transmission,
            linestyle="--",
            color="k",
            label="vacuum"
        )
        axs.plot(
            cbins, argon_transmission,
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
        axs.set_title("Transmission vs. Energy [keV]")
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
            self.ideal_energy[(self.ideal_arrival_time > 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_all, vacuum_edges_all = np.histogram(
            self.vacuum_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        vacuum_hist_detected, vacuum_edges_detected = np.histogram(
            self.vacuum_energy[(self.vacuum_arrival_time > 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_all, argon_edges_all = np.histogram(
            self.argon_energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        argon_hist_detected, argon_edges_detected = np.histogram(
            self.argon_energy[(self.argon_arrival_time > 0)], 
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

        ideal_transmission = np.zeros(cbins.size)
        ideal_mask = (ideal_hist_all > 0)
        ideal_transmission[ideal_mask] = ideal_hist_detected[ideal_mask] / ideal_hist_all[ideal_mask]

        vacuum_transmission = np.zeros(cbins.size)
        vacuum_mask = (vacuum_hist_all > 0)
        vacuum_transmission[vacuum_mask] = vacuum_hist_detected[vacuum_mask] / vacuum_hist_all[vacuum_mask]

        argon_transmission = np.zeros(cbins.size)
        argon_mask = (argon_hist_all > 0)
        argon_transmission[argon_mask] = argon_hist_detected[argon_mask] / argon_hist_all[argon_mask]

        if simulated:
            simulated_transmission = np.zeros(cbins.size)
            vacuum_simulated_mask = (vacuum_hist_simulated > 0)
            simulated_transmission[vacuum_simulated_mask] = argon_hist_simulated[vacuum_simulated_mask] / vacuum_hist_simulated[vacuum_simulated_mask]

        ideal_cross_section = np.zeros(cbins.size)
        ideal_mask = (ideal_transmission > 0)
        ideal_cross_section[ideal_mask] = -(1.0 / 4.2) * np.log(ideal_transmission[ideal_mask])

        vacuum_cross_section = np.zeros(cbins.size)
        vacuum_mask = (vacuum_transmission > 0)
        vacuum_cross_section[vacuum_mask] = -(1.0 / 4.2) * np.log(vacuum_transmission[vacuum_mask])

        argon_cross_section = np.zeros(cbins.size)
        argon_mask = (argon_transmission > 0)
        argon_cross_section[argon_mask] = -(1.0 / 4.2) * np.log(argon_transmission[argon_mask])

        if simulated:
            simulated_cross_section = np.zeros(cbins.size)
            simulated_mask = (simulated_transmission > 0)
            simulated_cross_section[simulated_mask] = -(1.0 / 4.2) * np.log(simulated_transmission[simulated_mask])

        fig, axs = plt.subplots()
        axs.plot(
            cbins, ideal_cross_section,
            linestyle="--",
            color="b",
            label="ideal"
        )
        axs.plot(
            cbins, vacuum_cross_section,
            linestyle="--",
            color="k",
            label="vacuum"
        )
        axs.plot(
            cbins, argon_cross_section,
            linestyle="--",
            color="r",
            label="argon"
        )
        if simulated:
            axs.plot(
                cbins, simulated_cross_section,
                linestyle=":",
                color="m",
                label="simulated"
            )
        if endf:
            axs.plot(
                self.endf_energy, self.endf_cross_section,
                linestyle='-',
                color='g',
                label='endf'
            )
        axs.set_xlim(energy_min, energy_max)
        axs.set_xlabel("Energy bin [keV]")
        axs.set_ylabel("Cross Section [barn]")
        axs.set_title("Cross Section [barn] vs. Energy [keV]")
        axs.set_yscale("log")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"{save}_neutron_cross_section.png")
        if(show):
            plt.show()

    