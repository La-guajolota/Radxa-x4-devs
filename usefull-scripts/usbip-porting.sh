#!/bin/bash

set -e #exit on any error
#set -x #Print commands as they run

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if [ "$EUID" -ne 0 ]; then
  echo -e "${RED}Please run as root${NC}"
  exit 1
fi

# --MAIN SCRIPT --
echo -e "${GREEN}Initializing USB porting via IP${NC}"
modprobe usbip_host
modprobe vhci_hcd 
usbipd -D
echo -e "${GREEN}Check for the USBs you're willing to port${NC}"
lsusb 
usbip list -l
