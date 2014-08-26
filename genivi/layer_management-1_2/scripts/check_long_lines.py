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
from common_modules.config import G_MAX_LINE_LENGTH_THRESHOLD


def check_long_lines(filename, file_lines):
    """
    Checks if a line is too long by comparing it to a (configurable) threshold. If a line contains
    string literals it is not checked

    """
    #regex for finding strings
    string_re = re.compile(r'"((?!((?<!((?<!\\)\\))")).)*"', re.DOTALL)

    for i in range(len(file_lines)):
        line = file_lines[i]

        #if line is very long and DOES NOT contain strings
        if len(line) > G_MAX_LINE_LENGTH_THRESHOLD and re.search(string_re, line) == None:
            log_warning(filename, i + 1, "line too long, {0} characters [maximum is {1}]".format(len(line), G_MAX_LINE_LENGTH_THRESHOLD))


if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories.

\tGives warnings if there are too long lines for readability. A line can have a maximum of {0} characters.
""".format(G_MAX_LINE_LENGTH_THRESHOLD)
        exit(0)

    for t in targets:
        _, _, file_lines, _ = read_file(t)
        check_long_lines(t, file_lines)
