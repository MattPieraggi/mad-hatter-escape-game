## Local build configuration
## Parameters configured here will override default and ENV values.
## Uncomment and change examples:

# Including board Makefile.
include ./Makefile-board.mk

## Add your source directories here separated by space
MODULES = app app/common app/project
# EXTRA_INCDIR = include

## ESP_HOME sets the path where ESP tools and SDK are located.
## Windows:
# ESP_HOME = c:/Espressif

## MacOS / Linux:
ESP_HOME = /Volumes/case-sensitive/opt/esp-open-sdk

## SMING_HOME sets the path where Sming framework is located.
## Windows:
# SMING_HOME = c:/tools/sming/Sming 

## MacOS / Linux
SMING_HOME = /Volumes/case-sensitive/opt/Sming/Sming

## COM port parameter is reqruied to flash firmware correctly.
## Windows: 
# COM_PORT = COM3

## MacOS / Linux:
#COM_PORT = /dev/ttyUSB0
COM_PORT = /dev/tty.usbserial-14630

## Com port speed
#COM_SPEED_ESPTOOL  = 460800
COM_SPEED_ESPTOOL  = 115200
COM_SPEED_SERIAL = $(COM_SPEED)

## SPIFFS options
# DISABLE_SPIFFS = 1
# SPIFF_FILES = files

ENABLE_ESPCONN = 1

# Use minicom as terminal
TERMINAL = minicom -D $(COM_PORT) -b $(COM_SPEED_SERIAL) /Volumes/case-sensitive/opt/minicom.config
