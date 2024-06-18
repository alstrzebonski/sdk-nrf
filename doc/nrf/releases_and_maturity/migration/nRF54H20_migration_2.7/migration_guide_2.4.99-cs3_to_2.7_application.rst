.. _migration_cs3_to_to_2_7_app:

Migrate your application for the nRF54H20 DK to |NCS| v2.7 (for v2.4.99-cs3 users)
##################################################################################

.. contents::
   :local:
   :depth: 2

This document describes the changes you should be aware of when migrating your application from |NCS| for the nRF54 customer sampling release v2.4.99-cs3 to |NCS| v2.6.99-cs2.


Overview
********

|NCS| v2.6.99-cs2 introduced a series of changes that might affect your existing applications.
The following is a summary of the most important ones:

Updated |NCS| toolchain
  The |NCS| toolchain has been updated.
  See :ref:`transition_cs3_to_2_7_env` for more info on how to upgrade your current |NCS| installation based on version 2.4.99-cs3.

SDFW and SCFW
  The Secure Domain Firmware (SDFW) and System Controller Firmware (SCFW) are no longer built from the source during the application build process, but they are installed as binaries from the provided firmware bundle and programmed when flashing the application to the device.
  For additional details, see :ref:`transition_cs3_to_2_7_env_bringup`.

nRF Util has now replaced nRF Command Line Tools.
  nRF Util is now the main command line backend utility.

Updated boards
  SOC1-based boards have been removed and FP1-based board have been added.
  The board names for the Application, Radio, and PPR core have been updated.

DTS changes
  The layout of DTS files and the names of DTS nodes related to the updated board names have been also updated, also affecting overlay files from applications and samples.
  If your application required a specific custom board, you must update the custom board files to match the changes done to the nRF54H20 SoC DTS files.

Lifecycle State changes
  To correctly operate the nRF54H20 DK, it must be set in LCS ``RoT``.

  Also, it is no longer possible to perform an unauthenticated backward LCS transitions.
  This means that once set to ``RoT``, it is no longer possible to revert to LCS state ``EMPTY``.

..
   ### Add DTS changes ###
   add sysbuild and hwmodelv2 links

.. note::
   Not all the features supported in |NCS| v2.4.99-cs3 have been ported to v2.6.99-cs2 (for example, the nRF54H20 support for the nRF Desktop and nRF Machine Learning applications).
   Additional features will be provided in future releases.

Required changes
****************

The following changes are mandatory to make your application work in the same way as in previous releases.

This section describes the changes related to samples and applications.

Samples and applications
========================



General
-------

.. toggle::

   * The nRF54H20 DK samples and applications are now using the following FP1-based board names:

     * Application core: ``nrf54h20dk_nrf54h20_cpuapp`` (Previously ``nrf54h20pdk_nrf54h20_cpuapp@soc1``)
     * Radio core: ``nrf54h20dk_nrf54h20_cpurad`` (Previously ``nrf54h20pdk_nrf54h20_cpurad@soc1``)
     * PPR core: ``nrf54h20dk_nrf54h20_cpuppr`` (Previously ``nrf54h20pdk_nrf54h20_cpuppr@soc1``)

     The previously used SOC1-based board files have been removed.

CoreMark
--------

.. toggle::

   * Several changes have been made to migrate the :ref:`coremark_sample` sample from the nRF54 customer sampling release v2.4.99-cs3 to |NCS| v2.7.0:

     * The sample has been moved from the :file:`<ncs_path>/zephyr/samples/benchmarks/coremark` folder in the Zephyr repository to the :file:`<ncs_path>/nrf/samples/benchmarks/coremark` folder in the main |NCS| repository.
     * The build system has been aligned to the hardware model v2.
       The ``SB_CONFIG_IMAGE_2_BOARD`` and ``SB_CONFIG_IMAGE_3_BOARD`` Kconfig options have been replaced with the ``SB_CONFIG_APP_CPUNET_RUN`` and ``SB_CONFIG_APP_CPUPPR_RUN`` Kconfig options.
       The new Kconfig options more clearly indicate on which cores the sample will be run.
       See the :file:`sysbuild.cmake` file to find out how these options are used to determine the board target.
     * Because the |NCS| v2.7.0 does not support ARM Coresight System Trace Macrocell (STM) logging for the nRF54 device, STM logging has been removed from the sample.
       The sample now uses usual UART logging, which allows for sending logs from only one core for each UART instance.
       The nRF54 device has only two UART instances, so the sample can now be run on two cores at most.
       The sample is always run on the application core, and depending on configuration, it can be run on either the radio core or the PPR core.
       See the ``SB_CONFIG_APP_CPUNET_RUN`` and ``SB_CONFIG_APP_CPUPPR_RUN`` Kconfig options for more details.
     * The DTS overlays have been aligned to the |NCS| v2.7.0.
     * The :file:`system_nrf.h` library has been included explicitly in the :file:`main.c` file to print the CPU frequency.
     * The ``SB_CONFIG_PARTITION_MANAGER`` Kconfig option has been disabled in the :file:`sysbuild.conf` file to avoid conflicts with the Partition Manager.
     * The :kconfig:option:`CONFIG_APP_MODE_FLASH_AND_RUN` Kconfig option has been made promptless and enabled for the PPR core.
       Currently, the PPR core does not have access to buttons and thus, the :kconfig:option:`CONFIG_APP_MODE_FLASH_AND_RUN` Kconfig option must be enabled for this core to run the benchmark.
     * To make the sample run on the PPR core, pass the ``-DSB_CONFIG_APP_CPUNET_RUN=n -DSB_CONFIG_APP_CPUPPR_RUN=y -Dcoremark_SNIPPET=nordic-ppr-xip`` build-time arguments to the build system.
       The ``coremark_SNIPPET`` argument is set to make the application core start the PPR core.
       Alternatively, you can build the sample from the :file:`sample.yaml` file using the following command:

       .. code-block:: console

          west build -p -b nrf54h20dk/nrf54h20/cpuapp -T sample.benchmark.coremark_ppr_xip .

Security
========

.. toggle::

   * For samples using ``CONFIG_NRF_SECURITY``:

     * RSA keys are no longer enabled by default.
       This reduces the code size by 30 kB if not using RSA keys.
       This also breaks the configuration if using the RSA keys without explicitly enabling an RSA key size.
       Enable the required key size to fix the configuration, for example by setting the Kconfig option :kconfig:option:`CONFIG_PSA_WANT_RSA_KEY_SIZE_2048` if 2048-bit RSA keys are required.

     * The PSA config is now validated by the :file:`ncs/nrf/ext/oberon/psa/core/library/check_crypto_config.h` file.
       Users with invalid configurations must update their PSA configuration according to the error messages that the :file:`check_crypto_config.h` file provides.

   * For the :ref:`crypto_persistent_key` sample:

     * The Kconfig option ``CONFIG_PSA_NATIVE_ITS`` is replaced by the Kconfig option :kconfig:option:`CONFIG_TRUSTED_STORAGE`, which enables the new :ref:`trusted_storage_readme` library.
       The :ref:`trusted_storage_readme` library provides the PSA Internal Trusted Storage (ITS) API for build targets without TF-M.
       It is not backward compatible with the previous PSA ITS implementation.
       Migrating from the PSA ITS implementation, enabled by the ``CONFIG_PSA_NATIVE_ITS`` option, to the new :ref:`trusted_storage_readme` library requires manual data migration.

   * For :ref:`lib_wifi_credentials` library and Wi-Fi samples:

     * ``CONFIG_WIFI_CREDENTIALS_BACKEND_PSA_UID_OFFSET`` has been removed because it was specific to the previous solution that used PSA Protected Storage instead of PSA Internal Trusted Storage (ITS).
       Use :kconfig:option:`CONFIG_WIFI_CREDENTIALS_BACKEND_PSA_OFFSET` to specify the key offset for PSA ITS.
       Be aware that Wi-Fi credentials stored in Protected Storage will not appear in ITS when switching.
       To avoid re-provisioning Wi-Fi credentials, manually read out the old credentials from Protected Storage in the previously used UID and store to ITS.

zcbor
=====

.. toggle::

   * If you have zcbor-generated code that relies on the zcbor libraries through Zephyr, you must regenerate the files using zcbor 0.8.1.
     Note that the names of generated types and members has been overhauled, so the code using the generated code must likely be changed.

     For example:

      * Leading single underscores and all double underscores are largely gone.
      * Names sometimes gain suffixes like ``_m`` or ``_l`` for disambiguation.
      * All enum (choice) names have now gained a ``_c`` suffix, so the enum name no longer matches the corresponding member name exactly (because this previously broke the C++ namespace rules).

    * The functions :c:func:`zcbor_new_state`, :c:func:`zcbor_new_decode_state` and the macro :c:macro:`ZCBOR_STATE_D` have gained new parameters related to the decoding of unordered maps.
      If you are not using this functionality, you can set the functions and the macro to ``NULL`` or ``0``.
    * The functions :c:func:`zcbor_bstr_put_term` and :c:func:`zcbor_tstr_put_term` have gained a new parameter ``maxlen``, referring to the maximum length of the parameter ``str``.
      This parameter is passed directly to :c:func:`strnlen` under the hood.
    * The function :c:func:`zcbor_tag_encode` has been renamed to :c:func:`zcbor_tag_put`.
    * Printing has been changed significantly, for example, :c:func:`zcbor_print` is now called :c:func:`zcbor_log`, and :c:func:`zcbor_trace` with no parameters is gone, and in its place are :c:func:`zcbor_trace_file` and :c:func:`zcbor_trace`, both of which take a ``state`` parameter.

Recommended changes
*******************

The following changes are recommended for your application to work optimally after the migration.

General
=======

.. toggle::

   * Applications that use :file:`prj_<board>.conf` Kconfig configurations should be transitioned to using :file:`boards/<board>.conf` Kconfig fragments.
