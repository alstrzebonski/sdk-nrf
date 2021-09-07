.. _ncs_release_notes_changelog:

|NCS| v1.6.0 Release Notes
##########################

.. contents::
   :local:
   :depth: 2

|NCS| delivers reference software and supporting libraries for developing low-power wireless applications with Nordic Semiconductor products.
It includes the TF-M, MCUboot and the Zephyr RTOS open source projects, which are continuously integrated and re-distributed with the SDK.

The |NCS| is where you begin building low-power wireless applications with Nordic Semiconductor nRF52, nRF53, and nRF91 Series devices.
nRF53 Series devices are now supported for production.
Wireless protocol stacks and libraries may indicate support for development or support for production for different series of devices based on verification and certification status.
See the release notes and documentation for those protocols and libraries for further information.

Highlights
**********

* Software support for :ref:`zephyr:thingy53_nrf5340`:

  * :ref:`nrf_machine_learning_app` application with Edge Impulse ML development platform, supported for development.
  * :ref:`Matter weather station <matter_weather_station_app>` application
  * Bluetooth® samples:

    * :ref:`central_uart`
    * :ref:`peripheral_lbs`
    * :ref:`peripheral_uart`

  * Bluetooth mesh samples:

    * :ref:`bluetooth_mesh_light`
    * :ref:`bluetooth_mesh_light_switch`
    * :ref:`bluetooth_mesh_light_lc`
    * :ref:`bluetooth_mesh_sensor_server`
    * :ref:`bluetooth_mesh_sensor_client`

  * Thread samples:

    * :ref:`coap_client_sample`
    * :ref:`coap_server_sample`

* Multi-image DFU for nRF53 Series with simultaneous application and network core firmware update.
* :ref:`ug_radio_fem` for the nRF21540 FEM range extender for product development.

  * :ref:`direct_test_mode` and :ref:`radio_test` sample support.
  * Application support for protocols based on Bluetooth® LE and 802.15.4 (Thread and Zigbee), using :ref:`nrfxlib:mpsl`.
  * Support for :ref:`nRF21540 development kit (DK) <nrf21540dk_nrf52840>`, :ref:`nRF21540 evaluation kit (EK) <ug_radio_fem_nrf21540_ek>` and custom board configurations.

* Added Wi-Fi Coex feature supported for development.
* Added support for NFC and *pair before use* type of accessories to the Apple Find My add-on.

.. note::
    Programming nRF52832 revision 3 and nRF52840 revision 3 devices requires nrfjprog version 10.13 or newer.
    nrfjprog is part of the `nRF Command Line Tools`_.

Release tag
***********

The release tag for the |NCS| manifest repository (|ncs_repo|) is **v1.7.0**.
Check the :file:`west.yml` file for the corresponding tags in the project repositories.

To use this release, check out the tag in the manifest repository and run ``west update``.
See :ref:`cloning_the_repositories` for more information.

Supported modem firmware
************************

See `Modem firmware compatibility matrix`_ for an overview of which modem firmware versions have been tested with this version of the |NCS|.

Use the latest version of the nRF Programmer app of `nRF Connect for Desktop`_ to update the modem firmware.
See `Updating the nRF9160 DK cellular modem`_ for instructions.

Known issues
************

See `known issues for nRF Connect SDK v1.7.0`_ for the list of issues valid for this release.

Changelog
*********

The following sections provide detailed lists of changes by component.

nRF9160
=======

* Added:

  * :ref:`at_monitor_readme` library that lets you reschedule and dispatch AT notifications from the Modem library AT interface to AT monitors.
  * :ref:`at_monitor_sample` sample that shows the usage of the :ref:`at_monitor_readme` library.
  * :ref:`lib_nrf_cloud_rest` library that enables devices to use nRF Cloud's REST-based device API.

* Updated:

  * :ref:`lib_nrf_cloud` library:

    * Added function :c:func:`nrf_cloud_uninit`, which can be used to uninitialize the nRF Cloud library.
      If :ref:`cloud_api_readme` is used, call :c:func:`cloud_uninit`.
    * Added function :c:func:`nrf_cloud_shadow_device_status_update`, which sets the device status in the device's shadow.
    * Added function :c:func:`nrf_cloud_modem_info_json_encode`, which encodes modem information into a cJSON object formatted for use with nRF Cloud.
    * Added function :c:func:`nrf_cloud_service_info_json_encode`, which encodes service information into a cJSON object formatted for use with nRF Cloud.
    * Added function :c:func:`nrf_cloud_client_id_get`, which returns the client ID used to identify the device with nRF Cloud.
    * Added function :c:func:`nrf_cloud_tenant_id_get`, which returns the tenant ID used to identify a customer account with nRF Cloud.
    * Added function :c:func:`nrf_cloud_register_gateway_state_handler` to implement a custom callback on shadow update events.
    * Added Kconfig option :kconfig:`CONFIG_NRF_CLOUD_GATEWAY`, which enables functionality to behave as an nRF Cloud gateway.
    * Added the option to use the P-GPS API independent of nRF Cloud MQTT transport.
    * Implemented functionality for the :c:enumerator:`NRF_CLOUD_EVT_SENSOR_DATA_ACK` event.
      The event is now generated when a valid tag value (NCT_MSG_ID_USER_TAG_BEGIN through NCT_MSG_ID_USER_TAG_END) is provided with the sensor data when calling either :c:func:`nrf_cloud_sensor_data_send` or :c:func:`nrf_cloud_shadow_update`.
    * Updated :c:func:`nrf_cloud_shadow_update` to expect that ``param->data.ptr`` points to a JSON string.
      Previously, a cJSON object was expected.
    * Removed the function ``nrf_cloud_sensor_attach()``, the associated structure ``nrf_cloud_sa_param``, and the event ``NRF_CLOUD_EVT_SENSOR_ATTACHED``.
      These items provided no useful functionality.

  * :ref:`serial_lte_modem` application:

    * Added the :ref:`slm_data_mode` documentation page to explain the data mode mechanism and how it works.
    * Added the :ref:`SLM_AT_FOTA` document page to describe the FOTA service.
    * Added IPv6 support to all SLM services.
    * Added the GNSS service to replace the existing GPS test functionality.
    * Added the optional support of location services from nRF Cloud, such as A-GPS, P-GPS, and cellular positioning.
    * Removed datatype in all sending AT commands.
      If no sending data is specified, switch data mode to receive and send any arbitrary data.

  * :ref:`asset_tracker_v2` application:

    * Changed the custom module responsible for controlling the LEDs to the :ref:`LEDs module <caf_leds>` from :ref:`lib_caf`.
    * Added support for A-GPS when configuring the application for AWS IoT.
    * Added support for P-GPS when configuring the application for AWS IoT.
    * Added a new debug module that implements support for `Memfault`_.
    * Added support for the :ref:`liblwm2m_carrier_readme` library.

  * :ref:`liblwm2m_carrier_readme` library:

    * Added deferred event reason :c:macro:`LWM2M_CARRIER_DEFERRED_SERVICE_UNAVAILABLE`, which indicates that the LwM2M server is unavailable due to maintenance.
    * Removed error code :c:macro:`LWM2M_CARRIER_ERROR_SERVICE_UNAVAILABLE`, which was used incorrectly to indicate a deferred event reason.

  * :ref:`lwm2m_carrier` sample - Adjusted the messages printed in :c:func:`lwm2m_carrier_event_handler` to reflect the updated event definitions in the :ref:`liblwm2m_carrier_readme` library.
  * :ref:`lte_lc_readme` library - Added API to enable modem domain events.

  * Board names:

    * The ``nrf9160dk_nrf9160ns`` and the ``nrf5340dk_nrf5340_cpuappns`` boards have been renamed respectively to ``nrf9160dk_nrf9160_ns`` and ``nrf5340dk_nrf5340_cpuapp_ns``, in a change inherited from upstream Zephyr.
    * The ``thingy91_nrf9160ns`` board has been renamed to ``thingy91_nrf9160_ns`` for consistency with the changes inherited from upstream Zephyr.

* Deprecated:

  * :ref:`asset_tracker` has been deprecated in favor of :ref:`asset_tracker_v2`.
  * :ref:`at_notif_readme` library has been deprecated in favor of the :ref:`at_monitor_readme` library.
  * :ref:`at_cmd_readme` library has been deprecated in favor of Modem library's native AT interface.
  * :ref:`gps_api` driver has been deprecated in favor of the :ref:`nrfxlib:gnss_interface`.

nRF5
====

The following changes are relevant for the nRF52 and nRF53 Series.

Front-end module (FEM)
----------------------

* Added support for the nRF21540 GPIO interface to the nRF5340 network core.
* Added support for RF front-end Modules (FEM) for nRF5340 in the :ref:`mpsl` library.
  The front-end module feature for nRF5340 in MPSL currently supports nRF21540, but does not support the SKY66112-11 device.
* Added a device tree shield definition for the nRF21540 Evaluation Kit.

nRF Desktop
-----------

* Added:

  * Added a functionality to clear the button state reported over Bluetooth® LE if the USB was connected while the button was pressed.
    This fixes an issue related to reporting wrong button state over Bluetooth® LE.
  * Added support for HID keyboard LED output report.
    The report is handled by the nRF Desktop peripherals and forwarded by the nRF Desktop dongles.
  * Added support for nRF5340 DK working as an nRF Desktop dongle.
  * Added a functionality for forwarding HID boot reports in :ref:`nrf_desktop_hid_forward`.
  * Added GPIO LEDs to the ``nrf52820dongle_nrf52820`` board.

* Updated:

  * Changed settings backend from FCB to NVS.
  * Switched to using :ref:`caf_power_manager`.
  * Fixed an issue with generating motion in :ref:`nrf_desktop_motion` (``motion_buttons`` and ``motion_simulated``) while the HID boot protocol was in use.
  * Fixed an issue with :ref:`nrf_desktop_usb_state` and :ref:`nrf_desktop_hids` modules forwarding the HID input reports related to an old protocol after protocol mode change.

Bluetooth® LE
-------------

* Added:

  * Production support for :ref:`nRF21540 GPIO <ug_radio_fem_nrf21540_gpio>` for both nRF52 and nRF53 Series.
  * :ref:`rscs_readme` - This module implements the Running Speed and Cadence Service (RSCS) with the corresponding set of characteristics.
  * :ref:`peripheral_rscs` sample - This sample demonstrates how to use the Running Speed and Cadence Service (RSCS).
  * Experimental implementation of the UART async adapter extension inside the :ref:`peripheral_uart` sample.

* Updated:

  * :ref:`ble_samples` - Changed the Bluetooth® sample Central DFU SMP name to :ref:`Central SMP Client <bluetooth_central_dfu_smp>`.
  * :ref:`direction_finding_connectionless_rx` and :ref:`direction_finding_connectionless_tx` samples - Added default configuration for ``nrf52833dk_nrf52820`` and ``nrf5340dk_nrf5340_cpuapp``, and ``nrf5340dk_nrf5340_cpuapp_ns`` boards.
  * :ref:`direct_test_mode` - Added an automatic build of the :ref:`nrf5340_empty_app_core` sample, when building for ``nrf5340dk_nrf5340_cpunet``.
  * Fixed the NCSDK-9820 known issue in the :ref:`peripheral_lbs` sample.
    When **Button 1** was pressed and released while holding one of the other buttons, the notification for release was the same as for press.
  * Fixed an issue in the :ref:`gatt_dm_readme` library where a memory fault could happen if a peer device disconnected during the service discovery process.
  * :ref:`lbs_readme` library - Added write request data validation in the LED characteristic.

Bluetooth mesh
---------------

* Added:

  * The choice of default Bluetooth® LE Controller changed from Zephyr Bluetooth® LE Controller to SoftDevice Controller.
  * Bluetooth® mesh profiles and models are ready for production.

* Updated:

  * Updated the :ref:`bt_mesh_light_hsl_srv_readme` and the :ref:`bt_mesh_light_xyl_srv_readme` models to no longer extend the :ref:`bt_mesh_lightness_srv_readme` model, and instead get a pointer to this model in the initialization macro.
  * Updated samples with support for the :ref:`zephyr:thingy53_nrf5340`.
  * Fixed an issue where beacons were stopped being sent after node reset.
  * Fixed an issue where the IV update procedure could be started immediately after the device has been provisioned.
  * Fixed multiple issues in the :ref:`bt_mesh_sensor_types_readme` module.

Matter
------

* Added:

  * :ref:`Thingy:53 weather station <matter_weather_station_app>` application.
  * :ref:`Template <matter_template_sample>` sample with a guide about :ref:`ug_matter_creating_accessory`.
  * :ref:`ug_matter_tools` page with information about building options for Matter controllers.
  * PA/LNA GPIO interface support for RF front-end modules (FEM) in Matter.
  * :doc:`Matter documentation set <matter:index>` based on the documentation from the Matter submodule.

Thread
------

* Added:

  * Added support for nRF21540 supported for nRF52 and nRF53 families, including Bluetooth® LE in the multiprotocol configuration.
  * Improvements in :ref:`thread_ug_supported_features_v12`:

    * Thread 1.2 supported in all samples.
    * Retransmissions are now supported when transmission security is handled by the radio driver.
    * Support for CSL Accuracy TLV in the MLE Parent Response.
    * Link Metrics data is now properly updated when using ACK-based Probing.

  * Added support for Thread Backbone Border Router on the :ref:`thread_architectures_designs_cp_rcp` architecture.

* Updated:

  * :ref:`ot_cli_sample` sample - Updated with he USB support.
  * :ref:`ot_coprocessor_sample` sample - Updated with the USB support.
  * Removed ``NET_SHELL`` from the Thread samples due to its limited usefulness.

Zigbee
------

* Added:

  * Added development support for ``nrf5340dk_nrf5340_cpuapp`` to the :ref:`zigbee_ncp_sample` sample.
  * :ref:`lib_zigbee_zcl_scenes` library with documentation.
    This library was separated from the Zigbee light bulb sample.
  * Added production support for :ref:`radio front-end module (FEM) <ug_radio_fem>` for nRF52 Series devices and nRF21540 EK.
  * :ref:`zigbee_template_sample` sample.
    This minimal Zigbee router application can be used as the starting point for developing custom Zigbee devices.
  * Added API for vendor-specific NCP commands.
    See the :ref:`Zigbee NCP sample <zigbee_ncp_vendor_specific_commands>` page for more information.
  * Added API for Zigbee command for getting active nodes.

* Updated:

  * :ref:`ug_zigbee_tools_ncp_host` to the production-ready v1.0.0.
  * Fixed the KRKNWK-9743 known issue where the timer could not be stopped in Zigbee routers and coordinators.
  * Fixed the KRKNWK-10490 known issue that would cause a deadlock in the NCP frame fragmentation logic.
  * Fixed the KRKNWK-6071 known issue with inaccurate ZBOSS alarms.
  * Fixed the KRKNWK-5535 known issue where the device would assert if flooded with multiple Network Address requests.
  * Fixed an issue where the NCS would assert in the host application when the host started just after SoC's SysReset.

* Updated:

  * :ref:`zigbee_ug_logging_stack_logs` - Improved printing ZBOSS stack logs.
    Added new backend options to print ZBOSS stack logs with option for using binary format.
  * ZBOSS Zigbee stack to version 3.8.0.1+4.0.0.
    See the :ref:`nrfxlib:zboss_changelog` in the nrfxlib documentation for detailed information.

ESB
---

* Updated:

  * Modified the ESB interrupts configuration to reduce the ISR latency and enable scheduling decision in the interrupt context.

nRF IEEE 802.15.4 radio driver
------------------------------

* Added:

  * :ref:`802154_phy_test` sample, with an experimental Antenna Diversity functionality.
  * Experimental Wi-Fi Coexistence functionality.

Other samples
-------------

* :ref:`radio_test` - Added an automatic build of the :ref:`nrf5340_empty_app_core` sample, when building for ``nrf5340dk_nrf5340_cpunet``.

Common
======

The following changes are relevant for all device families.

sdk-nrfxlib
-----------

* Updated the default :ref:`nrf_rpc` transport backend to use the RPMsg Service library.

See the changelog for each library in the :doc:`nrfxlib documentation <nrfxlib:README>` for additional information.

Modem library
+++++++++++++

* Added:

  * Added a new API for AT commands.
    See :ref:`nrfxlib:nrf_modem_at` for more information.
  * Added a new API for modem delta firmware updates.
    See :ref:`nrfxlib:nrf_modem_delta_dfu` for more information.

* Updated:

  * Updated :ref:`nrf_modem` to version 1.3.0.
    See the :ref:`nrfxlib:nrf_modem_changelog` for detailed information.

* Deprecated:

  * The AT socket API is now deprecated.
  * The DFU socket API is now deprecated.

NFC
---

* Updated the NFCT interrupt configuration to reduce the ISR latency and enable scheduling decision in the interrupt context.
* Updated the :ref:`nfc_uri` library to allow encoding of URI strings longer than 255 characters.

Trusted Firmware-M
------------------

* Added a test case for the secure read service that verifies that only addresses within the accepted range can be read.
* Updated :file:`tfm_platform_system.c` to fix a bug that returned ``TFM_PLATFORM_ERR_SUCCESS`` instead of ``TFM_PLATFORM_ERR_INVALID_PARAM`` when the address passed is outside of the accepted read range.

Common Application Framework (CAF)
----------------------------------

* Added :ref:`caf_net_state`.
* Added :ref:`caf_power_manager`.

Profiler
--------

* Added profiling string data.
* Optimized numeric data encoding.

Edge Impulse
------------

* Added support for Thingy:53 to :ref:`nrf_machine_learning_app`.
* Added configuration for nRF52840 DK that supports data forwarder over NUS to :ref:`nrf_machine_learning_app`.

Pelion
------

* Updated Pelion Device Management Client library version to 4.10.0.
* Switched to using :ref:`caf_power_manager` and :ref:`caf_net_state` in :ref:`pelion_client`.

MCUboot
=======

The MCUboot fork in |NCS| (``sdk-mcuboot``) contains all commits from the upstream MCUboot repository up to and including ``7a51968``, plus some |NCS| specific additions.

The code for integrating MCUboot into |NCS| is located in :file:`ncs/nrf/modules/mcuboot`.

The following list summarizes the most important changes inherited from upstream MCUboot:

* Added AES support for image encryption (based on mbedTLS).
* MCUboot serial: Ported encoding to use the cddl-gen module (which removes dependency on the `TinyCBOR`_ library).
* bootutil_public library: Made ``boot_read_swap_state()`` declaration public.

Mcumgr
======

The mcumgr library contains all commits from the upstream mcumgr repository up to and including snapshot ``e64f5060b``.

The following list summarizes the most important changes inherited from upstream mcumgr:

* Fixed an issue with SMP file management commands that would fail to read or write files, or both.

Zephyr
======

.. NOTE TO MAINTAINERS: All the Zephyr commits in the below git commands must be handled specially after each upmerge and each NCS release.

The Zephyr fork in |NCS| (``sdk-zephyr``) contains all commits from the upstream Zephyr repository up to and including ``14f09a3b00``, plus some |NCS| specific additions.

For a complete list of upstream Zephyr commits incorporated into |NCS| since the most recent release, run the following command from the :file:`ncs/zephyr` repository (after running ``west update``):

.. code-block:: none

   git log --oneline 14f09a3b00 ^v2.6.0-rc1-ncs1

For a complete list of |NCS| specific commits, run:

.. code-block:: none

   git log --oneline manifest-rev ^14f09a3b00

The current |NCS| master branch is based on the Zephyr v2.7 development branch.

Matter (Project CHIP)
=====================

The Matter fork in the |NCS| (``sdk-connectedhomeip``) contains all commits from the upstream Matter repository up to, and including, ``b77bfb047374b7013dbdf688f542b9326842a39e``.

The following list summarizes the most important changes inherited from the upstream Matter:

* Added:

  * Support for Certificate-Authenticated Session Establishment (CASE) for communication among operational Matter nodes.
  * Support for OpenThread's DNS Client to enable Matter node discovery on Thread devices.

* Updated:

  * Fixed the known issue KRKNWK-10387 where Matter service was needlessly advertised over Bluetooth® LE during DFU.
    Now if Matter pairing mode is not opened and the Bluetooth® LE advertising is needed due to DFU requirements, only the SMP service is advertised.

Partition Manager
=================

* Reworked how external flash memory support is enabled.
  The MCUboot secondary partition can now be placed in external flash memory without modifying any |NCS| files.

Documentation
=============

* Added:

  * User guide :ref:`ug_nrf_cloud`.

* Updated:

  * Renamed nRF Connect for Cloud to nRF Cloud.
  * Renamed :ref:`ncs_release_notes_changelog` (this page).
  * :ref:`gs_installing` - added information about the version folder created when extracting the GNU Arm Embedded Toolchain.
