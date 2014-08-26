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
from common_modules.config import G_LICENSE_TEMPLATES


def clean_comment_chars(s):
    """
    Removes comment characters from a string

    """
    s = string.replace(s, "/", "")
    s = string.replace(s, "*", "")
    s = string.replace(s, "#", "")
    return s

def make_license_re(license_text):
    """
    Makes a regular expression for every line in the license, this would match the license
    text tolerating extra spaces

    """
    license_lines = license_text.split("\n")
    license_lines_re = {}
    for i in range(len(license_lines)):
        license_line = license_lines[i]
        re_text = clean_comment_chars(license_line)
        #remove white space paddings
        re_text = re_text.strip(" \n\t\r\f")
        #replace special characters
        re_text = string.replace(re_text, "(", "\(")
        re_text = string.replace(re_text, ")", "\)")
        re_text = string.replace(re_text, " ", "(\s+)")
        re_text = string.replace(re_text, "\n", "(\s*)")
        re_text = string.replace(re_text, "\t", "(\s+)")
        re_text = string.replace(re_text, ".", "\.")

        #this replaces the text [YYYY] with a regex that mathces years in one of the following forms:
        #2013 or 2000-2013 or 2000 or 2000, 2001, 2002, 2013
        re_text = string.replace(re_text, "[YYYY]", r"(\d{4})(((-(\d{4}))|(((\s*),(\s*)\d{4})+))?)")

        if len(re_text) > 0:
            re_text = "(\s*)" + re_text + "(\s*)"
            current_text = ""
            #remove unneeded space matches
            while current_text != re_text:
                current_text = re_text
                re_text = string.replace(re_text, "(\s*)(\s*)", "(\s*)")
                re_text = string.replace(re_text, "(\s+)(\s+)", "(\s+)")
                re_text = string.replace(re_text, "(\s*)(\s+)", "(\s+)")

            license_lines_re[i] = re_text

    return license_lines_re

def check_specific_license_in_file(filename, clean_file_contents, license_text):
    """
    Checks if the file contains a valid license according to the license template provided

    """
    license_lines = license_text.split("\n")

    license_re = make_license_re(license_text)

    #search for the first line of the license in the target file
    line_re = re.compile(license_re.values()[0])
    found_match = line_re.search(clean_file_contents)

    if found_match:
        clean_file_contents = clean_file_contents[found_match.start():]

    #check that license exists without any added or removed words
    for (line_num, line_re_text) in license_re.items():
        line_re = re.compile(line_re_text)
        found_match = line_re.match(clean_file_contents)

        if found_match:
            clean_file_contents = clean_file_contents[found_match.end():]
        else:
            #log_warning(filename, 1, "license does not match at", license_lines[line_num])
            return (line_num, license_lines[line_num])

    return None # success

def check_license_in_file(filename, file_contents):
    """
    Checks if the file contains a valid license.
    It tries to find a match inside the file with any of the licenses configured

    """
    clean_file_contents = clean_comment_chars(file_contents)

    #license that had the best match with the file
    best_match = (-1, None)

    #try to match with every license
    for license in G_LICENSE_TEMPLATES:
        call_result = check_specific_license_in_file(filename, clean_file_contents, license)

        #if match is found just return
        if call_result == None:
            return None

        #if no match found check if this license was a good candidate for the match
        else:
            best_match = call_result if call_result[0] > best_match[0] else best_match

    #(this else clause is executed if the for loop exists naturally)
    #if loop ended without return, this means no license matched
    else:
        #if no license matched at all
        if best_match[1] == None:
            log_warning(filename, 1, "no license found")

        #get the license with the best match
        else:
            log_warning(filename, 1, "license does not match at", best_match[1])

if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories.

\tGives warnings if the file does not contain a valid license text. It does not check if Copyright statements are included.
"""
        exit(0)

    for t in targets:
        file_contents, _, _, _ = read_file(t)
        check_license_in_file(t, file_contents)
