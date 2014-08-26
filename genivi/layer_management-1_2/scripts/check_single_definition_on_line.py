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

def check_single_definition_on_line(filename, file_contents, clean_file_contents, file_lines, clean_file_lines):
    """
    Checks if a line contains several variable definitions

    """
    #remove arrays
    #an array regex starts with "{", ends with "}" and contains anything in between
    #an array of arrays is removed recursively inside out
    array_re = re.compile(r'\{[^ {};]*\}')
    clean_file_contents = clean_string_from_regex(clean_file_contents, array_re, '')

    #remove brackets
    array_brackets_re = re.compile(r'\[((?!\[|\]|;).)*\]')
    clean_file_contents = clean_string_from_regex(clean_file_contents, array_brackets_re, '')

    #remove angle brackets
    template_re = re.compile(r'<((?![<>;]).)*>')
    clean_file_contents = clean_string_from_regex(clean_file_contents, template_re, '')

    #variable name can be preceeded by * for pointers or & for references, can be followed by an assignment
    var_decl_re_text = r"""(
                                ((&?)(\*)*)                     # can start by & (for reference defs) and/or any number of * (for pointer defs)
                                (\s*)(\w+)(\s*)                 # followed by identifier
                                ((=((?![,;]).)*)?)              # can be followed by an assignment
                            )"""

    #types can be in namespaces or nested classes
    type_re_text = r"\w+(::\w+)*"

    #it is enough to have
    several_defs_re_text = r"""
                                ^(\s*){0}
                                (\s+){1}
                                ((\s*),(\s*){1})+;
                            """.format(type_re_text, var_decl_re_text)

    several_defs_re = re.compile(several_defs_re_text, re.MULTILINE | re.VERBOSE)

    for match in re.finditer(several_defs_re, clean_file_contents):
        line_number = clean_file_contents.count("\n", 0, match.end())
        log_warning(filename, line_number + 1, "several definitions on same line", file_lines[line_number].strip(" \t\r\n"))

if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories.

\tGives warnings if a line contains several variable definitions (This does not include lines that contain template or array declarations).
"""
        exit(0)

    for t in targets:
        if t[-2:] == ".h" or t[-4:] == ".cpp" or t[-2] == ".c":
            file_contents, clean_file_contents, file_lines, clean_file_lines = read_file(t)
            check_single_definition_on_line(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
