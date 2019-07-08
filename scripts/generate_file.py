#!/usr/bin/env python3
from jinja2 import Environment, FileSystemLoader
import subprocess

command = "whoami"
process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
username, error = process.communicate()


command = ""
process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
output, error = process.communicate()


env = Environment(loader=FileSystemLoader('templates'))
template = env.get_template('template.cc')
output = template.render(value = "YO")
print(output)

with open("file.cc", "w") as fh:
    fh.write(output)

