#!/usr/bin/python3
import sys, os
import configparser

cf = configparser.ConfigParser()
cf.read("test.ini")
print("[Section1000]")
for key in cf.options("Section1000"):
    print("%s = %s" % (key, cf.get("Section1000", key)))
