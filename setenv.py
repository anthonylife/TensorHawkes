#!/usr/bin/env python
#encoding=utf8

import os, json

if __name__ == "__main__":
    settings = {}
    setting_file = "SETTINGS.json"
    wfp = open(setting_file, "w")
    settings["ROOT_PATH"] = os.getcwd() + "/"

    ## Configuration

    # Data setting
    ###########################################################
    settings["SYN_DATA_1_"] = "data/user_item_1.syn.dat"

    settings["REAL_DATA_!_"] = "data/tmall"


    # Write result
    json.dump(settings, wfp, sort_keys=True, indent=4)
