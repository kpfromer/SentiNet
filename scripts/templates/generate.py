from yaml import YAML
import os
import sys

directory="~/.sentinet"
vars_file="vars.yml"

yaml="""\
global:
    name: NO_NAME
"""

def init(dir_):
    if not os.path.isdir(dir_):
        os.makedirs(dir_)
    name = input("Enter your name: ")
    yml = YAML()
    val = yml.load(yaml)
    val["global"]["name"] = name
    yaml.dump(val, sys.stdout)




init("~/.sentinet")

        
