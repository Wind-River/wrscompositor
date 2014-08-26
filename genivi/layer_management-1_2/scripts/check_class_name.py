#!/usr/bin/python

###########################################################################
#
# Copyright 2013 BMW Car IT GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###########################################################################

import sys, re, string
from common_modules.common import *


def check_class_name(filename, clean_file_contents):
    """
    Check that the file contains (maximum) one class that has a name identical to file name

    """
    #a class keyword is followed by classname, (and possibly an inherticance) and a left curly brace
    class_def_re = re.compile(r'(?<!\w)class(\s+)(\w+)(\s*)((:( |\w|,)*)?)(\s*)\{')
    #this is used to catch only the class name, because the (?>= ) is not included in the match
    class_name_re = re.compile(r'(?<=((?<!\w)class))(\s+\w+)')

    #filename without path and extension
    good_name = filename

    #remove path from filename (if needed)
    if good_name.count("/") > 0:
        good_name = good_name[good_name.rfind("/") + 1:]

    #remove .h extension
    good_name = good_name[:good_name.find(".h")]

    #initially: no class found yet
    class_found = False

    for class_match in re.finditer(class_def_re, clean_file_contents):
        line_number = clean_file_contents[:class_match.end()].count("\n")

        #if a class was found before: give warning
        if class_found:
            log_warning(filename, line_number, "several class difinitions in file")
        else:
            #if no class was found before: set flag to True
            class_found = True

            #get class name
            class_name_match = class_name_re.search(clean_file_contents, class_match.start())
            class_name = clean_file_contents[class_name_match.start(): class_name_match.end()] if class_name_match != None else ""
            class_name = class_name.strip(" \n\r\f\t")

            #if class name does not match file name: give warning
            if class_name != good_name:
                log_warning(filename, line_number, "class name has to be identical to filename")

if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories.

\tGives warnings if a file contains more than one class definition or if class name is not identical to file name
"""
        exit(0)

    for t in targets:
        if t[-2:] == ".h":
            _, clean_file_contents, _, _= read_file(t)
            check_class_name(t, clean_file_contents)
