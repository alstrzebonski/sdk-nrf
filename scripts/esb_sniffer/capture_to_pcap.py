#
# Copyright (c) 2025 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

import argparse
import struct
import sys
from threading import Thread

from pynrfjprog.Parameters import DeviceFamily
from Sniffer import Sniffer


class Capture:
    '''Read packets from the DK and write them into pcap file.'''
    def __init__(self, fd, dev: Sniffer):
        self.fd = fd
        self.dev = dev
        self.is_running = True

    def _init_pcap_file(self):
        self.fd.write(struct.pack('<IHHiIII',0xa1b2c3d4,2,4,0,0,65535,147))

    def stop(self):
        self.is_running = False

    def capture(self):
        self._init_pcap_file()
        self.dev.start()
        while self.is_running:
            pkt = self.dev.read()
            if pkt:
                self.fd.write(pkt)

        self.dev.stop()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Capture ESB packets to pcap file")
    parser.add_argument("output", help="Output pcap filename")
    parser.add_argument("--device-family", choices=[f.name for f in DeviceFamily],
                        default="AUTO",
                        help="Device family to use (default: AUTO). "
                             "Use NRF54L for nRF54LS05 DK if auto-detection fails.")
    args = parser.parse_args()

    try:
        f = open(args.output, 'wb')
    except Exception as err:
        print(f"Failed to open {args.output}: {err}", file=sys.stderr)
        sys.exit(1)

    dev = Sniffer(device_family=DeviceFamily[args.device_family])
    if dev.connect() != 0:
        sys.exit(1)

    session = Capture(f, dev)
    t = Thread(target=session.capture, daemon=True)
    t.start()

    print("Type \"quit\" or \"q\" to stop capture")
    while True:
        command = input()
        if command in ("quit", "q"):
            break
        else:
            print("Unrecognized input")

    session.stop()
    t.join(1)
    f.close()
    dev.disconnect()
