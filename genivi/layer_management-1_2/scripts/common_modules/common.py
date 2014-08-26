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
Contains utility functions used by other modules
"""

import sys, re, string, os
import config

def clean_string_from_regex(s, regex, marker):
    """
    Removes all occurances of the passed regex from a string s, and puts instances of marker at the beginnings and ends
    of remved substring

    """
    result = s
    found_match = re.search(regex, result)

    while found_match != None:
        matched_string = result[found_match.start(): found_match.end()]
        #replace by x...x where the ... is the \n chars in the matched string (to keep number of lines constant)
        result = result[:found_match.start()] + marker + "\n" * matched_string.count("\n") + marker + result[found_match.end():]

        found_match = re.search(regex, result)

    return result

def get_clean_file_contents(file_contents):
    """
    Removes strings, C style and C++ comments style comments and macros from file_contents

    """

    clean_file_contents = file_contents

    #remove all strings
    #a string is something that starts with a (") and ends with a ("), can have any character inside it except a (") that is not preceded by a slash (\) that is not preceded by another slash
    #this means a string can contains a \\ escape sequence
    #a string can contain a \" escape sequence
    #a string can end with  \\", but it does not end with \"
    string_re = re.compile(r'"((?!((?<!((?<!\\)\\))")).)*"', re.DOTALL)

    #'x' is passed as a marker, to mark the beginning and of strings
    #to preserve spacing around string beginning and end
    clean_file_contents = clean_string_from_regex(clean_file_contents, string_re, 'x')

    #remove character literals
    #a character literal is defined like a string (replacing single quotes with double quotes)
    #this is a big relaxation of the problem for C/C++
    char_re = re.compile(r'\'((?!((?<!((?<!\\)\\))\')).)*\'')

    clean_file_contents = clean_string_from_regex(clean_file_contents, char_re, 'x')

    #remove multi-line comments
    ml_comment_re = re.compile(r'(\s*)/\*((?!\*/).)*\*/', re.DOTALL)
    clean_file_contents = clean_string_from_regex(clean_file_contents, ml_comment_re, '')

    #remove single line comments
    sl_comment_re = re.compile(r'[ \t\r\f]*//.*$', re.MULTILINE)
    clean_file_contents = re.sub(sl_comment_re, '', clean_file_contents)

    #remove single-line and multi-line macros
    #a macro is #define statement followed by a few statements that have a slash in the line before it
    macro_re = re.compile(r'#define(\s+)(((?!\\).)+)((\\\n(((?!\\).)+))*)')
    clean_file_contents = clean_string_from_regex(clean_file_contents, macro_re, '')

    return clean_file_contents


def read_file(filename):
    """
    Reads contents of the file, returns a 4-tuple containing:
        1- raw file contents
        2- file contents without comments, strings and macros
        3- raw file contents as lines
        4- file contents without comments, strings and macros as lines

    """
    file_contents = open(filename).read()
    clean_file_contents = get_clean_file_contents(file_contents)

    file_lines = file_contents.split("\n")
    clean_file_lines = clean_file_contents.split("\n")

    return (file_contents, clean_file_contents, file_lines, clean_file_lines)


def get_all_files(targets):
    """
    Iterates over targets and gets names of all files

    """
    filenames = []
    for t in targets:
        #if directory: handle every file
        if os.path.isdir(t):
            for (path, _, files) in os.walk(t, followlinks=True):
                for f in files:
                    filenames.append(os.path.abspath(os.path.join(path, f)))

        #if file: process directly
        else:
            filenames.append(os.path.abspath(t))

    return filenames

def log_warning(filename, line_number, description, line_content = None):
    """
    Prints a warning to the standard output

    """
    if line_content == None:
        print "{0}:{1}: warning: {2}".format(filename, line_number, description)
    else:
        print "{0}:{1}: warning: {2}: {3}".format(filename, line_number, description, line_content)

    config.G_WARNING_COUNT += 1

def get_warning_count():
    """
    Returns the number of reported warnings

    """
    return config.G_WARNING_COUNT