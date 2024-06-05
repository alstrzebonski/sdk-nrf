.. _transition_cs3_to_2_7_env:

Transition your development environment to |NCS| v2.7 (for v2.4.99-cs3 users)
#############################################################################

.. contents::
   :local:
   :depth: 2

This document describes how to transition your development environment for the nRF54H20 DK from |NCS| v2.4.99-cs3 to |NCS| v2.7.

The main development environment changes introduced by 2.7 for the nRF54H20 DK are the following:

* The |NCS| toolchain has been updated.
* nRF Util has now replaced nRF Command Line Tools.
  The bootstrap script will now install and update nRF Util.
* SDFW and SCFW are now provided as precompiled binaries.
  The Secure Domain Firmware (SDFW) and System Controller Firmware (SCFW) are no longer built from the source during the application build process, but they must be provisioned as binaries from the provided firmware bundle before the DK can be used.
  See the details in the `nRF54H20 DK bring-up`_ section below.
* The nRF54H20 SoC lifecycle state must now be set to Root of Trust (RoT).
  See the details in the `Transitioning the nRF54H20 SoC to RoT`_ section.

Minimum requirements
********************

Make sure you have all the required hardware, software, and that your computer has one of the supported operating systems.

Hardware
========

* nRF54H20 DK version PCA10175 v0.7.x or v0.8.0 (ES3), ES4.
  These are the only versions of the nRF54H20 DK compatible with |NCS| 2.7.
  Check the version number on your DK's sticker to verify its compatibility with |NCS| version 2.7.
* USB-C cable.

Software
========

On your computer, one of the following operating systems:

.. include:: ../../../../nrf/installation/recommended_versions.rst
    :start-after: os_table_start
    :end-before: os_table_end

See :ref:`supported_OS` for more information about the tier definitions.

|supported OS|

You also need the following:

* `Git`_ or `Git for Windows`_ (on Linux and Mac, or Windows, respectively)
* `curl`_
* On Windows, SEGGER USB Driver for J-Link
* SEGGER `J-Link version 7.94e`_

.. note::
   The SEGGER USB Driver for J-Link was included, on Windows, in the nRF Command Line Tools bundle required by the |NCS| v2.4.99-cs3.
   When updating SEGGER J-Link to version 7.94e on Windows, select :guilabel:`Update existing installation` under :guilabel:`Choose destination` in the :guilabel:`choose optional components` window.

   See the following screenshot:

      .. figure:: images/jlink794e_install.png
         :alt: Optional components in the J-Link installation

Preliminary step
****************

Before transitioning to the new toolchain, rename your existing ``ncs-lcs`` and ``.west`` folders (for example as ``ncs-lcs_old`` and ``.west_old``) to backup their files.

Transitioning to the |NCS| v2.7
*******************************

You can transition to the |NCS| v2.7 by using Toolchain Manager.

Toolchain Manager is a tool available from `nRF Connect for Desktop`_, a cross-platform tool that provides different applications that simplify installing the |NCS|.
Both the tool and the application are available for Windows, Linux, and macOS.

To install the toolchain and the SDK using the Toolchain Manager app, complete the following steps:

1. Install Toolchain Manager:

   a. `Download nRF Connect for Desktop`_ for your operating system.
   #. Install and run the tool on your machine.
   #. In the **APPS** section, click :guilabel:`Install` next to Toolchain Manager.

   The app is installed on your machine, and the :guilabel:`Install` button changes to :guilabel:`Open`.

#. Install the |NCS| source code:

   a. Open Toolchain Manager in nRF Connect for Desktop.

      .. figure:: ../../../../nrf/installation/images/gs-assistant_tm.png
         :alt: The Toolchain Manager window

         The Toolchain Manager window

   #. Click :guilabel:`SETTINGS` in the navigation bar to specify where you want to install the |NCS|.
   #. In :guilabel:`SDK ENVIRONMENTS`, click the :guilabel:`Install` button next to the |NCS| version 2.7.

      The |NCS| version 2.7 is installed on your machine.
      The :guilabel:`Install` button changes to :guilabel:`Open VS Code`.

#. Set up the preferred building method:

   .. tabs::

      .. tab:: nRF Connect for Visual Studio Code

         To build on the |nRFVSC|, complete the following steps:

         a. In Toolchain Manager, click the :guilabel:`Open VS Code` button.

            A notification appears with a list of missing extensions that you need to install, including those from the `nRF Connect for Visual Studio Code`_ extension pack.
         #. Click **Install missing extensions**.
         #. Once the extensions are installed, click **Open VS Code** button again.

         You can then follow the instructions in :ref:`creating_vsc`.

      .. tab:: Command line

         To build on the command line, complete the following steps:

         1. With admin permissions enabled, download and install the `nRF Command Line Tools`_.
         #. Restart the Toolchain Manager application.
         #. Click the dropdown menu for the installed nRF Connect SDK version.

            .. figure:: ../../../../nrf/installation/images/gs-assistant_tm_dropdown.png
               :alt: The Toolchain Manager dropdown menu for the installed nRF Connect SDK version, cropped

               The Toolchain Manager dropdown menu options

         #. Select :guilabel:`Open command prompt`.

         You can then follow the instructions in :ref:`creating_cmd`.

#. If you have any existing custom applications created for 2.4.99-cs3 that you would like to migrate, move its files from the previous ``ncs-lcs_old`` folder to the newly created |NCS| installation folder.

Updating the Terminal application
*********************************

To update `Serial Terminal from nRF Connect for Desktop`, follow these steps:

1. On your computer, open `nRF Connect for Desktop`_
   If there is an update available, a pop up will notify you of its availability.
#. If available, install the update from the pop up screen.
#. Update `Serial Terminal from nRF Connect for Desktop`.

If you are using the nRF Terminal application part of the `nRF Connect for Visual Studio Code`_ extension, open Visual Studio Code instead and ensure you are running the newest version of both the editor and the extension.

Installing nRF Util and its commands
************************************

Using the nRF54H20 DK with the |NCS| 2.7 requires the following:

* nRF Util version 7.11.1 or above
* nRF Util ``device`` version 2.4.0

1. Download the nrfutil executable file from the `nRF Util development tool`_ product page.
#. Add nRF Util to the system path to run it from anywhere on the system.
   Use one of the following options:

   * Add nRF Util's directory to the system path.
   * Move the file to a directory in the system path.

#. On macOS and Linux, give ``nrfutil`` execute permissions by typing ``chmod +x nrfutil`` in a terminal or using a file browser.
   This is typically a checkbox found under file properties.
#. Verify the version of the nRF Util installation on your machine by running the following command::

      nrfutil --version

#. If your version is below 7.11.1, run the following command to update nRF Util::

      nrfutil self-upgrade

#. Install the nRF Util ``device`` command to version 2.4.0 as follows::

      nrfutil install device=2.4.0 --force

.. _transition_cs3_to_2_7_env_bringup:

nRF54H20 DK bring-up
********************

The following sections describe the steps required for the nRF54H20 bring-up.

.. rst-class:: numbered-step

Programming the BICR
====================

The Board Information Configuration Registers (BICR) are non-volatile memory (NVM) registers that contain information on how the nRF54H20 SoC must interact with other board elements, including the information about the power and clock delivery to the SoC.
To prepare the nRF54H20 DK for first use, you must manually program the values of the BICR using a precompiled BICR binary file (:file:`bicr_ext_loadcap.hex`).

1. Download the `BICR binary file`_ .
#. List all the connected development kits to see their serial number (matching the one on the DK's sticker)::

      nrfutil device list

#. Move the BICR HEX file to a folder of your choice, then program the BICR by running nRF Util from that folder using the following command::

      nrfutil device program --options chip_erase_mode=ERASE_NONE --firmware bicr_ext_loadcap.hex --core Secure --serial-number <serial_number>

.. rst-class:: numbered-step

Programming the SDFW and SCFW
=============================

After programming the BICR, the nRF54H20 SoC requires the provisioning of a bundle ( :file:`nrf54h20_soc_binaries_v0.5.0.zip`) containing the precompiled firmware for the Secure Domain and System Controller.
To program the Secure Domain Firmware (SDFW, also known as ``urot``) and the System Controller Firmware (SCFW) from the firmware bundle to the nRF54H20 DK, do the following:

1. Download the `nRF54H20 firmware bundle v0.5.0`_.
#. Move the :file:`ZIP` bundle to a folder of your choice, then run nRF Util to program the binaries using the following command::

      nrfutil device x-provision-nrf54h --firmware <path-to_bundle_zip_file> --serial-number <serial_number>

.. rst-class:: numbered-step

Updating the FICR
=================

.. caution::
   This step is required only if your nRF54H20 DK is version PCA10175 v0.7.x or v0.8.0 (ES3).
   Jump to the next step if your DK is version ES4.

After programming the SDFW and SCFW from the firmware bundle, you must update the Factory Information Configuration Registers (FICR) to correctly configure some trims of the nRF54H20 SoC.
To update the FICR, you must run a J-Link script:

1. Get the Jlink script that updates the FICR::

      curl -LO https://files.nordicsemi.com/artifactory/swtools/external/scripts/nrf54h20es_trim_adjust.jlink

#. Run the script:

   * Linux and Mac OS::

        JLinkExe -CommanderScript nrf54h20es_trim_adjust.jlink

   * Windows::

        jlink.exe -CommanderScript nrf54h20es_trim_adjust.jlink

.. _transition_cs3_to_2_7_env_lcsrot:

.. rst-class:: numbered-step

Transitioning the nRF54H20 SoC to RoT
=====================================

The current nRF54H20 DK is delivered with its lifecycle state (LCS) set to ``EMPTY``.
To correctly operate, its lifecycle state must be transitioned to Root of Trust (``RoT``).

.. note::
   The forward transition to LCS ``RoT`` is permanent.
   After the transition, it is not possible to transition backward to LCS ``EMPTY``.

To transition the LCS to ``RoT``, do the following:

1. Verify the current lifecycle state of the nRF54H20::

      nrfutil device x-adac-discovery --serial-number <serial_number>

   The output will look similar to the following::

      *serial_number*
      adac_auth_version     1.0
      vendor_id             Nordic VLSI ASA
      soc_class             0x00005420
      soc_id                [e6, 6f, 21, b6, dc, be, 11, ee, e5, 03, 6f, fe, 4d, 7b, 2e, 07]
      hw_permissions_fixed  [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00]
      hw_permissions_mask   [01, 00, 00, 00, 87, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00]
      psa_lifecycle         LIFECYCLE_EMPTY (0x1000)
      sda_id                0x01
      secrom_revision       0xad3b3cd0
      sysrom_revision       0xebc8f190
      token_formats         [TokenAdac]
      cert_formats          [CertAdac]
      cryptosystems         [Ed25519Sha512]
      Additional TLVs:
      TargetIdentity: [ff, ff, ff, ff, ff, ff, ff, ff]

#. If the lifecycle state (``psa_lifecycle``) shown is ``RoT`` (``LIFECYCLE_ROT (0x2000)``), no LCS transition is required.
   If the lifecycle state (``psa_lifecycle``) shown is not ``RoT`` (``LIFECYCLE_EMPTY (0x1000)`` means the LCS is set to ``EMPTY``), set it to Root of Trust using the following command::

      nrfutil device x-adac-lcs-change --life-cycle rot --serial-number <serial_number>

#. Verify again the current lifecycle state of the nRF54H20::

      nrfutil device x-adac-discovery --serial-number <serial_number>

   The output will look similar to the following::

      *serial_number*
      adac_auth_version     1.0
      vendor_id             Nordic VLSI ASA
      soc_class             0x00005420
      soc_id                [e6, 6f, 21, b6, dc, be, 11, ee, e5, 03, 6f, fe, 4d, 7b, 2e, 07]
      hw_permissions_fixed  [00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00]
      hw_permissions_mask   [01, 00, 00, 00, 87, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00]
      psa_lifecycle         LIFECYCLE_ROT (0x2000)
      sda_id                0x01
      secrom_revision       0xad3b3cd0
      sysrom_revision       0xebc8f190
      token_formats         [TokenAdac]
      cert_formats          [CertAdac]
      cryptosystems         [Ed25519Sha512]
      Additional TLVs:
      TargetIdentity: [ff, ff, ff, ff, ff, ff, ff, ff]

   The lifecycle state (``psa_lifecycle``) is now correctly set to *Root of Trust* (``LIFECYCLE_ROT (0x2000)``)

#. After the LCS transition, reset the device::

      nrfutil device reset --reset-kind RESET_PIN --serial-number <serial_number>

Next steps
**********

..
   ##TODO update all the following links

Your environment is now set to use the |NCS| v2.7 with the nRF54H20 DK:

* If you want to modify your existing custom applications previously developed for |NCS| v2.4.99-cs3 to be compatible with v2.7, consult ``new_migration_guide``.
* If you want to build and program a sample application on your nRF54H20 DK, consult the building and programming section in the ``nRF54H20 DK getting started guide for the nRF Connect SDK v2.7``.
