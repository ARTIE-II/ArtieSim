#! /bin/bash

# check if certain packages are installed
LIST_OF_APPS="cmake gcc g++ libexpat1-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libxmu-dev libmotif-dev libxerces-c-dev"
sudo apt-get install $LIST_OF_APPS -y

# get the directory where this script is stored
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
ARTIE_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )/../"

INSTALL_DIRECTORY="/home/$USER/bin/geant4/"
DOWNLOAD_PAGE="https://github.com/Geant4/geant4/archive/refs/tags/"
VERSION="11.0.3"
DL_FILE="v"$VERSION".tar.gz"
DL_LINK=$DOWNLOAD_PAGE$DL_FILE

mkdir -p $INSTALL_DIRECTORY
cd $INSTALL_DIRECTORY

wget $DL_LINK
tar xvzf $DL_FILE
rm $DL_FILE

cd $ARTIE_DIR