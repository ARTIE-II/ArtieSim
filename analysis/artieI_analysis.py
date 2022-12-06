"""
"""
import numpy as np
import uproot
from matplotlib import pyplot as plt

class ArtieIAnalysis:

    def __init__(self,
        input_file:     str="artieI_0.root",
        flight_path:    float=70,    # m
        resolution:     float=0.125  # us
    ):
        self.input_file = uproot.open(input_file)
        self.neutron_data = self.input_file["NeutronEventData"].arrays(library="np")
        self.energy = self.neutron_data["neutron_energy"] * 10e5        # keV
        self.arrival_time = self.neutron_data["arrival_time"] / 1000.0  # us

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
        filename
    ):
        t = np.array(self.neutron_data["arrival_time"]/1000.0)
        t = t[(t > 0)]
        t = self.smear_time(t)
        e = self.energy_from_tof(t)
        return t, e

    def plot_time_of_flight(self,
        number_of_bins: int=200,
        energy_min:     float=40,
        energy_max:     float=70,
        name:           str="Ideal Neutrons",
        save:   str='',
        show:   bool=False
    ):
        detected_time = self.arrival_time[(self.arrival_time>0)]
        energy_detected = self.energy[(self.arrival_time>0)]
        energy_tof = self.energy_from_tof(detected_time)

        tfin  = np.linspace(15,30,150)
        efin = self.energy_from_tof(tfin)

        fig, axs = plt.subplots()
        axs.plot(
            energy_detected, detected_time,
            linestyle="-.", color="b",
            label="Geant4"
        )
        axs.plot(
            efin, tfin,
            linestyle='--', color='r',
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
        
        fig, axs = plt.subplots()
        delta = energy_tof - energy_detected
        print("mean time difference:  ", np.mean(delta))
        print("variance:              ", np.var(delta))
        hist_delta, edges_delta = np.histogram(
            delta, 
            bins=number_of_bins,
            range=[-10.0,10.0]
        )
        cbins = (edges_delta[:-1] + edges_delta[1:])/2.0
        axs.plot(
            cbins, hist_delta,
            linestyle='-',
            color="b",
            label=r"$\mu$" + f" = {np.mean(delta):.2e}\n" + r"$\sigma$" + f" = {np.std(delta):.2e}"
        )
        axs.set_xlabel("Kinetic Energy (measured - actual) [keV]")
        plt.legend()
        plt.tight_layout()
        if(save != ''):
            plt.savefig(f"{save}_neutron_time_of_flight_delta.png")
        if(show):
            plt.show()

    
    def plot_energy(self,
        number_of_bins: int=200,
        energy_min:     float=40,
        energy_max:     float=70,
        name:           str="Ideal Neutrons",
        save:   str='',
        show:   bool=False
    ):
        fig, axs = plt.subplots()
        axs.hist(
            self.energy, 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="produced", 
            histtype="step", stacked=True, density=True
        )
        axs.hist(
            self.energy[(self.arrival_time > 0)], 
            bins=number_of_bins, 
            range=[energy_min, energy_max], 
            label="detected", 
            histtype="step", stacked=True, density=True
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
        name:           str="Ideal Neutrons",
        save:   str='',
        show:   bool=False
    ):
        hist_all, edges_all = np.histogram(
            self.energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        hist_detected, edges_detected = np.histogram(
            self.energy[(self.arrival_time > 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        cbins = (edges_all[1:] + edges_all[:-1])/2.0
        transmission = np.zeros(cbins.size)
        mask = (hist_all > 0)
        transmission[mask] = hist_detected[mask] / hist_all[mask]

        fig, axs = plt.subplots()
        axs.plot(
            cbins, transmission,
            linestyle="--",
            color="k",
            label="true"
        )
        if(save != ''):
            plt.savefig(f"{save}_neutron_transmission.png")
        if(show):
            plt.show()
    
    def plot_cross_section(self,
        number_of_bins: int=200,
        energy_min:     float=40,
        energy_max:     float=70,
        name:           str="Ideal Neutrons",
        save:   str='',
        show:   bool=False
    ):
        hist_all, edges_all = np.histogram(
            self.energy, 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        hist_detected, edges_detected = np.histogram(
            self.energy[(self.arrival_time > 0)], 
            bins=number_of_bins,
            range=[energy_min, energy_max]
        )
        cbins = (edges_all[1:] + edges_all[:-1])/2.0
        transmission = np.zeros(cbins.size)
        mask = (hist_all > 0)
        transmission[mask] = hist_detected[mask] / hist_all[mask]

        cross_section = np.zeros(cbins.size)
        mask = (transmission > 0)
        cross_section[mask] = -(1.0 / 4.2) * np.log(transmission[mask])

        fig, axs = plt.subplots()
        axs.plot(
            cbins, cross_section,
            linestyle="--",
            color="k",
            label="true"
        )
        if(save != ''):
            plt.savefig(f"{save}_neutron_cross_section.png")
        if(show):
            plt.show()

    