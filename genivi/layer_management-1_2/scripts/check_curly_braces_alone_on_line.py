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

def remove_tollerable_braces(clean_file_contents):
    """
    Cleans file contents from code blocks that should not be checked:
        1- Array initialization blocks
        2- Empty functions

    """
    #array assignment begins with a left curly brace ({) followed by any number of characters that is not a semicolon
    #it ends by a right curly brace (}) and a semicolon (;)
    assignment_re = re.compile(r'=(\s*)(\{)((?!;).)*(\})(\s*);', re.DOTALL)
    #remove array assignments from file contents
    no_assig_contents = clean_string_from_regex(clean_file_contents, assignment_re, '')

    #remove braces that are used for implementing empty functions
    empty_func_re = re.compile(r'\{\s*\}')
    no_empty_func_contents = clean_string_from_regex(no_assig_contents, empty_func_re, '')

    return no_empty_func_contents

def check_curly_braces_alone_on_line(filename, file_contents, clean_file_contents, file_lines, clean_file_lines):
    """
    Checks that there is no unnecessary code on same line with curly braces.

    """
    clean_file_contents = remove_tollerable_braces(clean_file_contents)
    clean_file_lines = clean_file_contents.split('\n')

    #keep a stack booleans to indicate where type declarations are made
    typedecl_stack = []

    #a type declaration contains the words struct, class, union or enum
    #it can also have the typdef keyword in the beginning
    typedecl_re = re.compile(r"(?!<\w)(typedef)?(\s*)(struct|class|enum|union)(?!\w)")

    #some statements are allowed to have curly brace on same
    #(do..while) blocks, namespaces and extern blocks
    permissible_re = re.compile(r'''(\s*)
                                (
                                    do|                     # do keyword
                                    (namespace((?!\{).)*)|  # namespace keyword 
                                    (extern((?!\{).)*)      # extern keyword
                                )?
                                (\s*)\{(\s*)                # open curly brace
                                (\\?)(\s*)                  # backslash indicates a multiline macro or multiline string
                                $''', re.VERBOSE)

    for i in range(len(file_lines)):
        line = clean_file_lines[i]

        def get_former_line():
            """
            Gets the last non-empty line

            """
            if i > 0:
                for j in range(i - 1, 0 , -1):
                    if len(clean_file_lines[j].strip(" \t\n\f\r")):
                        return clean_file_lines[j]
            return ""

        former_line = get_former_line()

        if line.count("{"):
            #check if the line contains a type declaration
            typedecl = re.search(typedecl_re, former_line)
            typedecl_stack.append(typedecl)

            #make a warning if line does not follow the permissible set of expressions
            if re.match(permissible_re, line) == None :
                log_warning(filename, i + 1, "other code on same line with curly brace", file_lines[i].strip(" "))

        if line.count("}"):
            typedecl = typedecl_stack.pop()

            if typedecl:
                #if the right curly brace marks the end of type declaration:
                #it is possible to have code AFTER the curly brace in case of type declarations
                #like instantiating a structure at end of structure definition
                if re.match(r"(\s*)\}", line) == None:
                    log_warning(filename, i + 1, "other code on same line with curly brace", file_lines[i].strip(" "))
            else:
                #if the right curly brace does not mark the end of type declaration:
                #the line must begin with the right curly brace, can possibly have a while expression and ends by semicolon
                #this has to be the end of line ($) to avoid having a second statement on the same line
                if re.match(r"(\s*)\}(\s*)((while((?!;).)*)?)(;?)(\s*)(\\?)(\s*)$", line) == None:
                    log_warning(filename, i + 1, "other code on same line with curly brace", file_lines[i].strip(" "))

if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories

\tGives warnings if unnecessary code exists on the same line with curly braces. 
"""
        exit(0)

    for t in targets:
        if t[-2:] == ".h" or t[-4:] == ".cpp" or t[-2] == ".c":
            file_contents, clean_file_contents, file_lines, clean_file_lines = read_file(t)
            check_curly_braces_alone_on_line(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
