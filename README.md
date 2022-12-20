# ArtieSim
Simulation code for ARTIE-II

## Installation
ArtieSim requires installation of [Geant4](https://geant4.web.cern.ch/), which can be downloaded [here](https://github.com/Geant4/geant4/releases), and this code has been tested against [version 11.0.3](https://github.com/Geant4/geant4/releases/tag/v11.0.3).

There are a few subtle things which need to be configured for the installation, such as pointing the environment variable *G4NEUTRONHPDATA* to the correct path.  This can be done with on of the scripts which will install Geant4 for you automatically.  

### Installing Geant4
First determine some directory to install Geant4 to, call this *G4_DIR=~/physics/geant4/*.  Then, create the environment variable and the folder and download the version of Geant4 you'd like to use,
```bash
export G4_DIR=~/physics/geant4
mkdir -p $G4_DIR
wget https://github.com/Geant4/geant4/archive/refs/tags/v11.0.3.tar.gz $G4_DIR
cd $G4_DIR
tar -xvzf v11.0.3.tar.gz 
```
or you can try,
```bash
cd $G4_DIR
git clone https://github.com/Geant4/geant4 --branch 11.0.3
```
Next, we will set up build and install directories.  Once inside *$G4_DIR* do,
```bash
mkdir geant4-11.0.3-build
mkdir geant4-11.0.3-install
cd geant4-11.0.3-build
```
We can now install using CMake, or `ccmake`, which both can be installed with the other required dependencies.  On a Debian based system do,
```bash
sudo apt-get install cmake gcc g++ libexpat1-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libxmu-dev libmotif-dev libxerces-c-dev cmake-curses-gui libopengl-dev
```
 Then just run ccmake, which will bring up a cmake GUI
```shell
ccmake ../geant4-11.0.3/
```
From here, there are several things that you need to enable, such as **GEANT4_BUILD_MULTITHREADED**, **GEANT4_INTALL_DATA=ON**, **GEANT4_USE_GDML**, **GEANT4_USE_OPENGL_X11**, **GEANT4_USE_QT=ON** and **GEANT4_USE_SYSTEM_EXPAT=ON**.  You also need to specify the installation directory, which in the scripts is set to */home/$USER/physics/geant4/geant4.11.0.3-install/*.  If you are not using ccmake, but instead just running cmake from the build directory, you can do this with the following:
```shell
cmake -DCMAKE_INSTALL_PREFIX=<install_dir> -DGEANT_BUILD_MULTITHREADED=ON -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -DGEANT4_USE_OPENGLX11=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_SYSTEM_EXPAT=ON ../geant4-11.0.3/
```
If cmake is successful, you can then build the project by issuing
```shell
make -j N
make install
```
where *N* is the number of cores you want to use to build.

### Installing Git LFS
In order to download the neutron HP data library we use in the Artie simulation, you'll either need to install Git LFS (Large File System), or manually download the correct [ENDF-VIII.0 library](https://www-nds.iaea.org/geant4/) and copy it to the data folder.  To install Git LFS, navigate to [https://git-lfs.com/](https://git-lfs.com/) and click the download link.  Once downloaded, run the bash script,
```bash
cd ~/Downloads/git-lfs-1.X.X
./install.sh
```

### Installing ArtieSim
To begin, first clone the repository and pull the submodules recursively,

```bash
git clone --recursive https://github.com/ARTIE-II/ArtieSim
```
If you've cloned without including recursive then do,

```bash
git clone https://github.com/ARTIE-II/ArtieSim
cd ArtieSim
git submodule update --init --recursive
```
There are several scripts in the *ArtieSim/scripts/* folder which will install Geant4, Git LFS and download and set up the neutron data library for you automatically.  To install Geant4 simply run the script,
```bash
source <path-to-ArtieSim>/scripts/install_geant4.sh
```
Likewise for Git LFS (this may have a broken link if Git LFS has been updated!),
```bash
source <path-to-ArtieSim>/scripts/install_git_lfs.sh
```
and finally for the neutron library,
```bash
source <path-to-ArtieSim>/scripts/install_endf.sh
```

### Neutron HP Data
For the ARTIE simulation, we use the [ENDF-VIII.0](https://www-nds.iaea.org/geant4/libraries/ENDF-VIII.0/) neutron data library, which can be found at the International Atomic Energy Agency (IAEA) [website](https://www-nds.iaea.org/geant4/).  This dataset will be downloaded automatically if Git LFS is set up (see section above), however you will have to manually download it if it is not.  The dataset can be downloaded [here](https://www-nds.iaea.org/geant4/libraries/ENDF-VIII.0.tar.gz) and should be extracted into the data folder.

In order to use the ENDF-VIII.0 library, we need to set the environment variable *G4NEUTRONHPDATA* to point to the folder which contains the library, i.e. *G4NEUTRONHPDATA=<path-to-ArtieSim>/data/ArtieENDF/ENDF-VIII.0/*.  If one is using the various install scripts, this is done for you automatically, however you will need to set it yourself if installing manually before running the simulation,
```bash
export G4NEUTRONHPDATA=<path-to-ArtieSim>/data/ArtieENDF/ENDF-VIII.0/
```

## Configuration Files
