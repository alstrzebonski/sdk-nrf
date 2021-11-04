#
# Copyright (c) 2018 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause

from multiprocessing import Process, Event
import argparse
import logging
import os
from rtt2socket import Rtt2Socket
from model_creator import ModelCreator
from plot_nordic import PlotNordic

if os.name == 'posix':
    RTT2SOCKET_OUT_ADDR_DICT = {
        'descriptions': '\0' + 'rtt2socket_desc',
        'events': '\0' + 'rtt2socket_ev'
    }

    MODEL_CREATOR_IN_ADDR_DICT = {
        'descriptions': '\0' + 'model_creator_desc',
        'events': '\0' + 'model_creator_ev'
    }

    MODEL_CREATOR_OUT_ADDR_DICT = {
        'descriptions': '\0' + 'model_creator_proc_desc',
        'events': '\0' + 'model_creator_proc_ev'
    }

    PLOT_IN_ADDR_DICT = {
        'descriptions': '\0' + 'plot_desc',
        'events': '\0' + 'plot_ev'
    }
else:
    IP_ADDRESS = 'localhost'
    PORT_START = 8000

    RTT2SOCKET_OUT_ADDR_DICT = {
        'descriptions': (IP_ADDRESS, PORT_START),
        'events': (IP_ADDRESS, PORT_START + 1)
    }

    MODEL_CREATOR_IN_ADDR_DICT = {
        'descriptions': (IP_ADDRESS, PORT_START + 2),
        'events': (IP_ADDRESS, PORT_START + 3)
    }

    MODEL_CREATOR_OUT_ADDR_DICT = {
        'descriptions': (IP_ADDRESS, PORT_START + 4),
        'events': (IP_ADDRESS, PORT_START + 5)
    }

    PLOT_IN_ADDR_DICT = {
        'descriptions': (IP_ADDRESS, PORT_START + 6),
        'events': (IP_ADDRESS, PORT_START + 7)
    }


def rtt2socket(event_plot, event_model_creator, log_lvl_number):
    try:
        rtt2s = Rtt2Socket(RTT2SOCKET_OUT_ADDR_DICT, MODEL_CREATOR_IN_ADDR_DICT, log_lvl=log_lvl_number)
        event_plot.wait()
        event_model_creator.wait()
        rtt2s.read_and_transmit_data()
    except KeyboardInterrupt:
        rtt2s.close()

def model_creator(event, dataset_name, log_lvl_number):
    try:
        mc = ModelCreator(MODEL_CREATOR_IN_ADDR_DICT,
                          own_send_socket_dict=MODEL_CREATOR_OUT_ADDR_DICT,
                          remote_socket_dict=PLOT_IN_ADDR_DICT,
                          event_filename=dataset_name + ".csv",
                          event_types_filename=dataset_name + ".json",
                          log_lvl=log_lvl_number)
        event.set()
        mc.start()
    except KeyboardInterrupt:
        mc.close()

def dynamic_plot(event, log_lvl_number):
    try:
        pn = PlotNordic(own_recv_socket_dict=PLOT_IN_ADDR_DICT, log_lvl=log_lvl_number)
        event.set()
        pn.plot_events_real_time()
    except KeyboardInterrupt:
        pn.close_event(None)

def main():
    parser = argparse.ArgumentParser(
        description='Collecting data from Nordic profiler for given time, plotting and saving to files.')
    parser.add_argument('dataset_name', help='Name of dataset')
    parser.add_argument('--log', help='Log level')
    args = parser.parse_args()

    if args.log is not None:
        log_lvl_number = int(getattr(logging, args.log.upper(), None))
    else:
        log_lvl_number = logging.INFO

    # events are made to ensure that PlotNordic and ModelCreator classes are initialized before Rtt2Socket starts sending data
    event_plot = Event()
    event_model_creator = Event()

    try:
        processes = []
        processes.append(Process(target=dynamic_plot,
                                 args=(event_plot, log_lvl_number),
                                 daemon=True))
        processes.append(Process(target=model_creator,
                                 args=(event_model_creator, args.dataset_name, log_lvl_number),
                                 daemon=True))
        processes.append(Process(target=rtt2socket,
                                 args=(event_plot, event_model_creator, log_lvl_number),
                                 daemon=True))
        for p in processes:
            p.start()
        for p in processes:
            p.join()
    except KeyboardInterrupt:
        for p in processes:
            p.join()

if __name__ == "__main__":
    main()
