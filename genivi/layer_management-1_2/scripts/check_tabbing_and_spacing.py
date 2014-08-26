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

def check_tabs_no_spaces(filename, file_lines):
    """
    Checks if any tab character "\t" exists in the file

    """
    for i in range(len(file_lines)):
        if file_lines[i].count("\t") > 0:
            log_warning(filename, i + 1, "tab character (\\t) found, use 4 spaces instead", file_lines[i].strip(" "))

def check_correct_space_count(filename, file_lines):
    """
    Checks that the number of spaces at the beginning of every line is divislbe by four

    """
    in_multiline_comment = False
    for i in range(len(file_lines)):
        if in_multiline_comment:
            #if end of muli-line comment
            if file_lines[i].count("*/") > 0:
                #just make sure there is no other new multi-line comment starting on the same line (after the current mult-line comment is closed)
                in_multiline_comment = file_lines[i].count("/*") > 0 and file_lines[i].index("/*") > file_lines[i].index("*/") 
        else:
            in_multiline_comment = file_lines[i].count("/*") > 0

            #regex searches for the first character that is not a space character
            found_match = re.search("(?! )", file_lines[i])
            if found_match:
                space_count = found_match.start()
                if space_count % 4 != 0:
                    log_warning(filename, i + 1, "number of spaces at beginning of line must be divisible by 4")


def check_no_spacing_line_end(filename, file_lines):
    """
    Checks that lines do not end with unnecessary white space characters

    """
    for i in range(len(file_lines)):
        if re.search(" $", file_lines[i]):
            log_warning(filename, i + 1, "unneeded space(s) at end of line")

def check_tabbing_and_spacing(filename, file_lines):
    """
    Calls other functions that check general issues about tabbing and spacing

    """
    check_tabs_no_spaces(filename, file_lines)
    check_correct_space_count(filename, file_lines)
    check_no_spacing_line_end(filename, file_lines)


if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories.

\tGives warnings if a line contains unneeded spaces at end of line, contains tab characters (\\t)
\tor if spaces at line beginning are not divisible by 4.
"""
        exit(0)

    for t in targets:
        if t[-2:] == ".h" or t[-4:] == ".cpp" or t[-2] == ".c":
            _, _, file_lines, _ = read_file(t)
            check_tabbing_and_spacing(t, file_lines)
