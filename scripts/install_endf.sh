#! /bin/bash

# get the directory where this script is stored
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
ARTIE_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )/../"

# try git lfs
git lfs install
git submodule update --init --recursive

ARTIE_NEUTRONDATA_DIR=$ARTIE_DIR/data/ArtieENDF/
ARTIE_NEUTRONDATA_FILE="ENDF-VIII.0.tar.gz"

# if git lfs is unsuccessful then dl from site
if [ test -d "$ARTIE_NEUTRONDATA_DIR" ]
then
	if [ test -f "$ARTIE_NEUTRONDATA_DIR$ARTIE_NEUTRONDATA_FILE" ]; then
        echo "Neutron data library is downloaded."
	else
        DOWNLOAD_PAGE="https://www-nds.iaea.org/geant4/libraries/"
        DL_FILE="ENDF-VIII.0.tar.gz"
        DL_LINK=$DOWNLOAD_PAGE$DL_FILE
        wget $DL_LINK $ARTIE_NEUTRONDATA_DIR
	fi
else
	echo "Directory $ARTIE_NEUTRONDATA_DIR not found."
fi

# untar
tar xvzf $ARTIE_NEUTRONDATA_DIR$ARTIE_NEUTRONDATA_FILE

# set up environment variables 
export G4NEUTRONHPDATA=$ARTIE_NEUTRONDATA_DIR/ENDF-VIII.0/

# add g4neutronhpdata command to bashrc
echo 'export G4NEUTRONHPDATA=$ARTIE_NEUTRONDATA_DIR/ENDF-VIII.0/' >> ~/.bashrc