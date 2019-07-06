#!/usr/bin/env python3

import sys
import os
import glob
import argparse
import yaml
import collections
import subprocess
import datetime

import jinja2 as jinja
import parser.ICD as ICD

def print_results(output, err):
    if any(output):
        print(output.decode("utf-8"))
    if any(err):
        print(err.decode("utf-8"))
        sys.exit(1)

def run_cmd(cmd, cwd='./', verbose=False):
    if verbose:
        print(cmd)
    p = subprocess.Popen(cmd, cwd=cwd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, err = p.communicate()
    print_results(output, err)

# Write the file to disk ... create directories as needed
def write(fullpath, text):
    os.makedirs(os.path.dirname(fullpath), exist_ok=True)
    f = open(fullpath, 'w')
    f.write(text)
    f.close()

# The main callback
def main():
    parser = argparse.ArgumentParser(description='Generate message from template files')
    parser.add_argument('-v', '--verbose', help='Verbose output', action='store_true', default=False)
    parser.add_argument(      '--dryrun', help='Dryrun the commands', action='store_true', default=False)

    parser.add_argument(      '--cpp', help='Generate the CPP files', action='store_true', default=False)

    parser.add_argument('-t', '--template-dir', help='Set the Jinja language specific template directory', default='./templates')
    parser.add_argument(      '--cpp-out-dir', help='Set the generated output directory for the CPP files', default='../../../cpp/messages')

    args = parser.parse_args()

    now  = datetime.datetime.now().strftime('%Y-%m-%d %H:%M')

    # If nothing was specified, then generate all the files.
    if not args.cpp
        args.cpp = True

    # Check that we have a 'protoc' file, and compile if necessary
    if not os.path.isfile(args.protoc):
        print('Failed to find \'%s\' ... you need to run \'make protoc\'' % (args.protoc))
        sys.exit(1)

    # Setup the Jinja environment from the directory that this script is executed
    env = jinja.Environment(loader=jinja.FileSystemLoader(os.path.dirname(os.path.abspath(__file__))), trim_blocks=True)

    # Get all of the protocol directories and the ICD messages contained within
    icd_directories = sorted(glob.iglob(os.path.join(args.icd_dir, '*'), recursive=True))
    icd_directories = filter(lambda path: os.path.isdir(path), icd_directories)
    for icd_directory in icd_directories:
        # Load all of the YAML ICD files from the protocol directory
        icds[icd_directory] = ICD.ICD(sorted(glob.iglob(os.path.join(icd_directory, '*'), recursive=True)), protocol=os.path.basename(icd_directory))

    # Now that all of the data models objects have been created, let's resolve the 'types' that are data model objects
    # An example definition would be: MessageC -> MessageA, MessageB, FieldA, FieldB
    #  So we need to resolved the MessageA and MessageB objects within the MessageC definition

    # Iterate over all of the ICD collections
    for directory, icd in icds.items():
        # Extract the protocol name from the directory listing
        protocol  = os.path.basename(directory)

        # Inform the operator of the current protocol being processed
        info = 'Processing %s ICDs (%s):' % (os.path.basename(directory), directory)
        if icd.empty():
            print('%s ... FAILED (no ICDs found: %s)' % (info, directory))
            continue
        print(info)

        #########
        # Generate the C++ output files
        cpp_src_path = os.path.join(args.template_dir, 'cpp')
        cpp_out_path = os.path.join(args.cpp_out_dir, protocol)
        if args.cpp:
            print('  Generating CPP files');
            for msg in icd.get_messages():
                # The C++ wrapper classes for the messages in the ICD
                write(os.path.join(cpp_out_path, msg.get_name() + 'Interface.h'), env.get_template(cpp_src_path + '/MessageInterface.h').render(path=cpp_src_path, protocol=icd.get_protocol(), msg=msg, now=now))
                write(os.path.join(cpp_out_path, msg.get_name() + '.h'), env.get_template(cpp_src_path + '/Message.h').render(path=cpp_src_path, protocol=icd.get_protocol(), msg=msg, now=now))
                write(os.path.join(cpp_out_path, msg.get_name() + '.cpp'), env.get_template(cpp_src_path + '/Message.cpp').render(path=cpp_src_path, protocol=icd.get_protocol(), msg=msg, now=now))
                write(os.path.join(cpp_out_path, msg.get_name() + 'Randomizer.hpp'), env.get_template(cpp_src_path + '/MessageRandomizer.hpp').render(path=cpp_src_path, protocol=icd.get_protocol(), msg=msg, now=now))
            # Generate the helper header that will include all the messages from a particular protocol
            write(os.path.join(cpp_out_path, 'AllMessages.h'), env.get_template(cpp_src_path + '/AllMessages.h').render(path=cpp_src_path, icd=icd, now=now))
            write(os.path.join(cpp_out_path, 'AllMessageRandomizers.h'), env.get_template(cpp_src_path + '/AllMessageRandomizers.h').render(path=cpp_src_path, icd=icd, now=now))
            write(os.path.join('../../../cpp/tests/unit_test/messages/' + icd.get_protocol() + 'Test.hpp'), env.get_template(cpp_src_path + '/UnitTest.hpp').render(path=cpp_src_path, protocol=icd.get_protocol(), icd=icd, now=now))
            write(os.path.join('../../../cpp/tests/unit_test/messages/' + icd.get_protocol() + 'Test.cpp'), env.get_template(cpp_src_path + '/UnitTest.cpp').render(path=cpp_src_path, protocol=icd.get_protocol(), icd=icd, now=now))
            write(os.path.join('../../../cpp/tests/functional_test/messages/' + icd.get_protocol() + 'Test.cpp'), env.get_template(cpp_src_path + '/PerformanceTest.cpp').render(path=cpp_src_path, protocol=icd.get_protocol(), icd=icd, now=now))

        #########
        # Generate the GO output files
        go_src_path = os.path.join(args.template_dir, 'go')
        go_out_path = os.path.join(args.go_out_dir, protocol)
        if args.go:
            print('  Generating GO files');
            for msg in icd.get_messages():
                # The GO wrapper classes for the messages in the ICD
                write(os.path.join(go_out_path, msg.get_name() + 'Interface.go'), env.get_template(go_src_path + '/MessageInterface.go').render(path=go_src_path, protocol=icd.get_protocol(), msg=msg, now=now, icd=icd))
                write(os.path.join(go_out_path, msg.get_name() + '.go'), env.get_template(go_src_path + '/Message.go').render(path=go_src_path, protocol=icd.get_protocol(), msg=msg, now=now, icd=icd))

        #########
        # Generate the PYTHON output files
        #print('  Generating PYTHON files');
        #python_out_path = os.path.join(args.python_out_dir, protocol, 'python')
        #########
        # Generate the Python output files
        py_src_path = os.path.join(args.template_dir, 'python')
        py_out_path = os.path.join(args.py_out_dir, protocol)
        if args.py:
            print('  Generating Python files');
            for msg in icd.get_messages():
                # The C++ wrapper classes for the messages in the ICD
                write(os.path.join(py_out_path, msg.get_name() + '.cpp'), env.get_template(py_src_path + '/Message.cpp').render(path=py_src_path, protocol=icd.get_protocol(), msg=msg, now=now))


        #########
        # Generate the JAVA output files
        #print('  Generating JAVA files');
        #java_out_path = os.path.join(args.java_out_dir, protocol, 'java')

        #########
        # Generate the proto output files
        print('  Generating PROTO files');
        proto_src_path = os.path.join(args.template_dir, 'proto')
        proto_out_path = os.path.join(args.proto_dir, protocol)
        for msg in icd.get_messages():
            write(os.path.join(proto_out_path, msg.get_name() + '.proto'), env.get_template(proto_src_path + '/Message.proto').render(path=proto_src_path, protocol=icd.get_protocol(), msg=msg))

        #########
        # Compile the protobuf language bindings (requires protoc to compiler the generated proto files)
        commands = []
        for out_path in [ cpp_out_path, go_out_path ]:
            os.makedirs(os.path.join(out_path, 'proto'), exist_ok=True)

        if args.cpp:
            commands.append(['  Compiling PROTO files (CPP)', '%s --cpp_out=%s/proto -I=%s %s/*.proto' % (os.path.basename(args.protoc), os.path.abspath(os.path.expanduser(cpp_out_path)), os.path.abspath(os.path.expanduser(args.proto_dir)), os.path.abspath(os.path.expanduser(proto_out_path))), os.path.dirname(args.protoc)])

        if args.go:
            commands.append(['  Compiling PROTO files (GO)', '%s --go_out=%s/proto -I=%s %s/*.proto' % (os.path.basename(args.protoc), os.path.abspath(os.path.expanduser(go_out_path)), os.path.abspath(os.path.expanduser(args.proto_dir)), os.path.abspath(os.path.expanduser(proto_out_path))), os.path.dirname(args.protoc)])

        # Iterate over the commands that need to be executed
        for command in commands:
            print(command[0])
            run_cmd(command[1], cwd=command[2], verbose=args.verbose)

    #########
    # Generate the top-level C++ output files
    print('Processing top-level ICD files:')

    cpp_src_path = os.path.join(args.template_dir, 'cpp')
    cpp_out_path = os.path.join(args.cpp_out_dir)
    if args.cpp:
        print('  Generating CPP files')
        write(os.path.join(cpp_out_path, 'MessageUtils.hpp'), env.get_template(cpp_src_path + '/MessageUtils.hpp')
                .render(path=cpp_src_path, icds=icds, now=now))
    
    py_src_path = os.path.join(args.template_dir, 'python')                                                   
    py_out_path = os.path.join(args.py_out_dir) 
    if args.py:
        print('  Generating Python files')
        write(os.path.join(py_out_path, 'message_map.py'), env.get_template(py_src_path + '/message_map.py')
                .render(path=py_src_path, icds=icds, now=now))

    go_src_path = os.path.join(args.template_dir, "go")
    go_out_path = os.path.join(args.go_out_dir)
    if args.go:
        print('  Generating GO files')
        write(os.path.join(go_out_path, 'meta/MessageInit.go'), env.get_template(go_src_path + '/MessageInit.go')
                .render(path=go_src_path, icds=icds, now=now))

if __name__ == '__main__':
  main()


