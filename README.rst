OpenHaystack Zephyr firmware
############################

This project implements firmware with an `OpenHaystack <https://github.com/seemoo-lab/openhaystack>`_ application based on the real-time operating system `Zephyr <https://www.zephyrproject.org/>`_.

OpenHaystack is a framework for tracking personal Bluetooth devices via Apple's massive `Find My <https://developer.apple.com/find-my/>`_ network. Thanks to this firmware based on Zephyr, you can create your own tracking tags with one of the many Bluetooth Low Energy devices that Zephyr supports.

After flashing the firmware to your device, it sends out Bluetooth Low Energy advertisements that will be visible in Apple's Find My network using the OpenHaystack application in macOS.

Disclaimer
**********

The firmware is just a proof-of-concept and currently only implements advertising a single static key. This means that devices running this firmware are trackable by other devices in proximity.

There is also no power management yet. So if you're running this firmware on a battery-powered device, it won't be as energy-efficient as possible. If you want to improve this, all patches are welcome.

Requirements
************

* A Bluetooth Low Energy device, supported by Zephyr
* A `Zephyr development environment <https://docs.zephyrproject.org/latest/getting_started/index.html>`_
* OpenHaystack's macOS application to view the location of your device

Initialization
**************

The first step is to initialize a workspace folder (for instance ``zephyr-workspace``) where the application and all Zephyr modules will be cloned. You can do that by running:

.. code-block:: shell

  # Initialize Zephyr workspace folder for the application (main branch)
  west init -m https://github.com/koenvervloesem/openhaystack-zephyr --mr main zephyr-workspace
  # Update Zephyr modules
  cd zephyr-workspace
  west update
  cd openhaystack-zephyr

Build
*****

To build the firmware, run:

.. code-block:: shell

   west build -p auto -b $BOARD -s app

Replace ``$BOARD`` by your target board.

A sample debug configuration to read logs from the USB UART is also provided. You can apply it by running:

.. code-block:: shell

  west build -p auto -b $BOARD -s app -- -DOVERLAY_CONFIG=debug-usb-uart.conf

This only works with boards that support this, such as Nordic Semiconductor's nRF52840 Dongle.

Once you have built the application, the firmware image is available in ``build/zephyr``.

Use your own key
****************

You need to specify a public key in the firmware image. You can either:

* directly do so in the source (the char array ``public_key`` in `main.c <https://github.com/koenvervloesem/openhaystack-zephyr/blob/main/app/src/main.c>`_) and then build the firmware
* patch the default public key ``OFFLINEFINDINGPUBLICKEYHERE!`` in the bin file to your own key and save the resulting firmware image (see the script `openhaypatch.sh <https://github.com/koenvervloesem/openhaystack-zephyr/blob/main/openhaypatch.sh>`_ for a way to do this)

Flash
*****

How to flash the image to a device depends on the device and its bootloader. For many devices you can run:

.. code-block:: shell

   west flash

Refer to your `board's documentation <https://docs.zephyrproject.org/latest/boards/index.html>`_ for alternative flash instructions if your board doesn't support the ``flash`` target.

For the nRF52840 Dongle with the built-in bootloader, run:

.. code-block:: shell

  nrfutil pkg generate --hw-version 52 --sd-req=0x00 \
          --application build/zephyr/zephyr.hex \
          --application-version 1 openhaystack.zip

This packages the application in the file ``openhaystack.zip``. Now press the reset button and flash the package onto the board with:

.. code-block:: shell

  nrfutil dfu usb-serial -pkg openhaystack.zip -p /dev/ttyACM0

Have a look at ``ls /dev/tty*`` for the right device on Linux and macOS. On Windows it should be something like ``COMx``.

For devices with the `Adafruit nRF52 bootloader <https://github.com/adafruit/Adafruit_nRF52_Bootloader>`_ such as the April USB Dongle 52840 or makerdiary nRF52840 MDK USB Dongle, first generate a UF2 file from the hex file with ``uf2conv.py``:

.. code-block:: shell

  python3 ../zephyr/scripts/uf2conv.py -f 0xADA52840 -c build/zephyr/zephyr.hex

And then drag and drop the file ``flash.uf2`` to the storage device mounted by your operating system.

Supported devices
*****************

This procedure has been tested with:

* Nordic Semiconductor's `nRF52840 Dongle <https://docs.zephyrproject.org/latest/boards/arm/nrf52840dongle_nrf52840/doc/index.html>`_ (board name ``nrf52840dongle_nrf52840``), as well as its derivatives `April USB Dongle 52840 <https://wiki.aprbrother.com/en/BleUsbDongle.html#april-usb-dongle-52840>`_ and `makerdiary nRF52840 MDK USB Dongle <https://wiki.makerdiary.com/nrf52840-mdk-usb-dongle/>`_, which are both using the Adafruit nRF52 bootloader
* the nRF52833-based `BBC micro:bit v2 <https://docs.zephyrproject.org/latest/boards/arm/bbc_microbit_v2/doc/index.html>`_ (board name ``bbc_microbit_v2``)
* Ruuvi's nRF52832-based `RuuviTag <https://docs.zephyrproject.org/latest/boards/arm/ruuvi_ruuvitag/doc/index.html>`_ (board name ``ruuvi_ruuvitag``) using the `RuuviTag Development Kit <https://ruuvi.com/products/ruuvitag-development-kit/>`_

Other Bluetooth Low Energy devices supported by Zephyr should work as well. Please let me know if you manage to run this firmware on another board, so I can add it to the list of devices it has been tested with.

Using OpenHaystack as a module
******************************

The base code is written as a Zephyr module, in the directory `modules/openhaystack <https://github.com/koenvervloesem/openhaystack-zephyr/tree/main/modules/openhaystack>`_. You can reuse this in your own Zephyr applications. The code of the example application using this module can be found in the directory `app <https://github.com/koenvervloesem/openhaystack-zephyr/tree/main/app>`_.

Acknowledgments
***************

This project is inspired by and has used code from:

* the original `OpenHaystack firmware for ESP32 <https://github.com/seemoo-lab/openhaystack/tree/main/Firmware/ESP32>`_
* the original `OpenHaystack firmware for nRF51822 <https://github.com/seemoo-lab/openhaystack/tree/main/Firmware/Microbit_v1>`_
* Antonio Calatrava's alternative `OpenHaystack firmware using Nordic Semiconductor's Softdevice <https://github.com/acalatrava/openhaystack-firmware>`_
* the `Zephyr Example Application <https://github.com/zephyrproject-rtos/example-application>`_ for the project structure and GitHub Actions workflow

License
*******

This project is provided by `Koen Vervloesem <http://koen.vervloesem.eu>`_ as open source software with the MIT license. See the `LICENSE file <LICENSE>`_ for more information.
