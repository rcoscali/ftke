export BASE_DIR=`pwd`
export SRC_ROOT=$BASE_DIR
export SHOW_CMDS=true

cd ./build_config/opencore_dynamic; . setup.sh; make
