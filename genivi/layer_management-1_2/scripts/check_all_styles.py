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

"""
Runs all enabled style checking

"""

import sys, re, string
from common_modules.common import *

from check_license import check_license_in_file
from check_header_guards import check_header_guards
from check_consistent_code_style import check_consistent_code_style
from check_indentation import check_indentation
from check_curly_braces_alone_on_line import check_curly_braces_alone_on_line
from check_long_lines import check_long_lines
from check_single_definition_on_line import check_single_definition_on_line
from check_single_statement_on_line import check_single_statement_on_line
from check_tabbing_and_spacing import check_tabbing_and_spacing
from check_empty_lines import check_empty_lines
from check_class_name import check_class_name

#######################################################################################################################
#CONFIGURATION
#######################################################################################################################
CHECK_LICENSE                       = "ON"
CHECK_HEADER_GUARDS                 = "ON"
CHECK_TABBING_AND_SPACING           = "ON"
CHECK_INDENTATION                   = "ON"
CHECK_CONSISTENT_CODE_STYLE         = "ON"
CHECK_CURLY_BRACES_ALONE_ON_LINE    = "ON"
CHECK_LONG_LINES                    = "ON"
CHECK_SINGLE_STATEMENT_ON_LINE      = "ON"
CHECK_SINGLE_DEFINITION_ON_LINE     = "ON"
CHECK_EMPTY_LINES                   = "ON"
CHECK_CLASS_NAME                    = "ON"
#######################################################################################################################


if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\tTakes a list of files and directories as input and runs all enabled style-checking on all files.

\tTo enable/disable style-checking you can toggle the "ON"/"OFF" value of the configuration variables inside the file.
"""
        exit(0)

    for t in targets:
        if t[-2:] == ".h" or t[-4:] == ".cpp" or t[-2:] == ".c":
            file_contents, clean_file_contents, file_lines, clean_file_lines = read_file(t)

            if CHECK_LICENSE == "ON":
                check_license_in_file(t, file_contents)
            if CHECK_HEADER_GUARDS == "ON" and t[-2:] == ".h":
                check_header_guards(t, file_contents)
            if CHECK_TABBING_AND_SPACING == "ON":
                check_tabbing_and_spacing(t, file_lines)
            if CHECK_INDENTATION == "ON":
                check_indentation(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
            if CHECK_CONSISTENT_CODE_STYLE == "ON":
                check_consistent_code_style(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
            if CHECK_CURLY_BRACES_ALONE_ON_LINE == "ON":
                check_curly_braces_alone_on_line(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
            if CHECK_LONG_LINES == "ON":
                check_long_lines(t, file_lines)
            if CHECK_SINGLE_STATEMENT_ON_LINE == "ON":
                check_single_statement_on_line(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
            if CHECK_SINGLE_DEFINITION_ON_LINE == "ON":
                check_single_definition_on_line(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
            if CHECK_EMPTY_LINES == "ON":
                check_empty_lines(t, file_contents, file_lines)
            if CHECK_CLASS_NAME == "ON" and t[-2:] == ".h":
                check_class_name(t, clean_file_contents)
