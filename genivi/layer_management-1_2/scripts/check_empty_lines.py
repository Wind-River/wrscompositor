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


def check_empty_lines(filename, file_contents, file_lines):
    """
    Check that there are no non-necessary empty lines

    """
    #keep track of the current indentation
    #need only to know if level of indent is zero or not
    line_indent_zero = True

    def is_line_empty(l):
        return len(l.strip(" \t\n\r\f")) == 0

    #check all lines except last line
    for i in range(len(file_lines) - 1):
        line = file_lines[i]
        former_line = file_lines[i - 1] if i > 0 else ""
        next_line = file_lines[i + 1] if i < len(file_lines) - 1 else ""

        def is_next_indent_zero():
            """
            Checks the indentation of the next non-empty line
            return True if it is at zero-level indentation

            """
            for j in range(i + 1, len(file_lines)):
                if not is_line_empty(file_lines[j]):
                    return re.match(r'(\s+)(\S+)', file_lines[j]) == None
            return True

        next_indent_zero = is_next_indent_zero()

        #if empty line
        if is_line_empty(line):
            #check if line is not needed
            #if the line before it was empty (two consecutive white space lines)
            if is_line_empty(former_line) and not line_indent_zero and not next_indent_zero:
                log_warning(filename, i + 1, "unneeded empty line (two or more consecutive empty lines)")
            #if the line before it was a block beginning
            elif re.search(r'\{(\s*)$', former_line):
                log_warning(filename, i + 1, "unneeded empty line (block begins with empty line)")
            #if the line after it is a block end
            elif re.match(r'(\s*)\}', next_line):
                log_warning(filename, i + 1, "unneeded empty line (block ends with empty line)")
        else:
            #if non-empty line: check if line is at zero indentation
            #if the line contains one or more white-space characters followed by
            #one or more non white-space characters
            line_indent_zero = re.match(r'(\s+)(\S+)', line) == None

    #if file does not end with an empty line: give warning
    if re.search(r'\n$', file_contents) == None:
        log_warning(filename, len(file_lines), "file does not end with new line")


if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories

\tGives warnings if there are unneeded empty lines
\tIt also gives a warning if there is NO empty line at end of file (because this leads to warnings on some compilers)
"""
        exit(0)

    for t in targets:
        if t[-2:] == ".h" or t[-4:] == ".cpp" or t[-2] == ".c":
            file_contents, _, file_lines, _ = read_file(t)
            check_empty_lines(t, file_contents, file_lines)
