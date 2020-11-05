Adapted from nrf9160 SDK, C:\\ncs\\zephyr\\samples\\sensor\\dht

Prints temp amd humidity to serial port every 2 seconds

Changes Made:

1) Renamed boards\nrf52dk_nrf52832.overlay to boards\circuitdojo_feather_nrf9160ns.overlay

2) Edit: boards\circuitdojo_feather_nrf9160ns.overlay

Changed from (&gpio0 11) to (&gpio0 29), which is D2 on the nrf9160-feather

3) Edit prj.conf and add the following lines to enable the bootloader so you can load with newtmgr

# Enable Zephyr application to be booted by MCUboot
CONFIG_BOOTLOADER_MCUBOOT=y
# default 115200
CONFIG_SERIAL=y

4) Connect DHT22 to D2, 3.3V and GND. Have not tried DHT11


Original Readme
.. _dht:

DHT: Aosong DHT Digital-output Humidity and Temperature Sensor
##############################################################

Description
***********

This sample application periodically (0.5 Hz) measures the ambient
temperature and humidity. The result is written to the console.

Wiring
*******

This sample uses an external breakout for the sensor.  A devicetree
overlay must be provided to identify the sensor variant and the GPIO
used to control the sensor.

Building and Running
********************

After providing a devicetree overlay that specifies the sensor location,
build this sample app using:

.. zephyr-app-commands::
   :zephyr-app: samples/sensor/dht
   :board: nrf52dk_nrf52832
   :goals: build flash

Sample Output
=============

.. code-block:: console

   *** Booting Zephyr OS build zephyr-v2.1.0-329-g38418b26c4cc  ***
   [0:00:00.027]: 20.0 Cel ; 48.7 %RH
   [0:00:02.053]: 19.8 Cel ; 48.7 %RH
   [0:00:04.079]: 20.0 Cel ; 48.7 %RH
   [0:00:06.105]: 19.8 Cel ; 48.7 %RH
   [0:00:08.130]: 20.0 Cel ; 48.8 %RH
   [0:00:10.156]: 20.1 Cel ; 48.8 %RH
   [0:00:12.182]: 19.7 Cel ; 48.7 %RH
   [0:00:14.207]: 20.0 Cel ; 48.8 %RH

<repeats endlessly>
