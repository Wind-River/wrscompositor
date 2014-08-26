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



def check_binary_operators(filename, file_contents, file_lines, clean_file_contents, clean_file_lines):
    """
    Check if spacing around binary opeartors is consistent in file

    """
    #normal binary operators must all have 0 or 1 space between operator and operands
    binary_ops = ['=', r'\+', '%', r'\^', '==', '!=', '>=', '<=', '&&', r'\|\|', r'\|', '<<', '>>', r'\+=', '-=', r'\*=', '/=', '%=',  '&=', r'\|=', r'\^=', '<<=', '>>=', ':', r'\?', '/']
    #operators that are not handled
    #r'\*', '&', '>', '<', '-'
    #access binary operators must have no space between operator and operands
    binary_ops_re_text = '(' + "|".join(binary_ops) + ')'

    #initially: clean file contents from confusing content:
    #remove all preprocessor directives
    preprocessor_re = re.compile(r'#\s*\w+(\s+)(((?!\\).)+)((\\\n(((?!\\).)+))*)')
    clean_file_contents = clean_string_from_regex(clean_file_contents, preprocessor_re, '')
    clean_file_lines = clean_file_contents.split('\n')

    #re for NO space between operator and operands
    nospace_re = re.compile(r"\w{0}\w".format(binary_ops_re_text))
    #re for ONE space between operator and operands
    space_re = re.compile(r"\w\s{0}\s\w".format(binary_ops_re_text))
    #unbalanced is a match for binary op (\w\s*{0}\s*\w) that is neither no space (?!(\w{0}\w)) nor single space (?!(\w\s{0}\s\w))
    #this works because (?!...) does not consume strings 
    unbalanced_spaces_re = re.compile(r"(?!(\w{0}\w))(?!(\w\s{0}\s\w))(\w\s*{0}\s*\w)".format(binary_ops_re_text))

    access_ops_re_text = r'(\.|->|::)'
    bad_access_ops_re = re.compile(r"(\w\s+{0}\s*\w)|(\w\s*{0}\s+\w)".format(access_ops_re_text))

    #check if there is general inconsistance in the file
    nospace_match = re.search(nospace_re, clean_file_contents)
    space_match = re.search(space_re, clean_file_contents)

    #a single consistency in the file is enough to issue a warning
    if nospace_match != None and space_match != None:
        line1 = clean_file_contents[:nospace_match.start()].count("\n") + 1
        line2 = clean_file_contents[:space_match.start()].count("\n") + 1

        log_warning(filename, line1, "inconsistent spacing between operands and binary operator. All operands in a file must have same space (0 or 1 spaces) [Reported once per file]", "lines :{0} and {1}".format(line1, line2))

    for i in range(len(file_lines)):
        line = clean_file_lines[i]

        if re.search(unbalanced_spaces_re, line) != None:
            log_warning(filename, i + 1, "incorrect or unbalanced spaces between operands and binary operator", file_lines[i].strip(" "))

        if re.search(bad_access_ops_re, line) != None:
            log_warning(filename, i + 1, "unneeded space(s) around access/resolution operator", file_lines[i].strip(" "))

def check_semicolor(filename, file_contents, file_lines, clean_file_contents, clean_file_lines):
    """
    Checks that there are no spaces before semicolons, and that there is a space after it in case there is other content
    on the same line after it

    """
    #check if there is extra space before semicolon, or missing space after semicolon (if there is content after it like in for loop)
    space_before_re = re.compile(r"\s;");
    nospace_after_re = re.compile(r";((?! ).)");

    for i in range(len(file_lines)):
        line = clean_file_lines[i]

        if re.search(space_before_re, line) != None:
            log_warning(filename, i + 1, "unneeded space(s) before semicolon", file_lines[i].strip(" "))

        if re.search(nospace_after_re, line) != None:
            log_warning(filename, i + 1, "missing space after semicolon", file_lines[i].strip(" "))

def check_braces_spaces(filename, file_contents, file_lines, clean_file_contents, clean_file_lines):
    """
    Check that there are no unnecessary spaces around parentheses

    """
    #check if there are spaces between between right/left brace and contents
    bad_left_brace_re = re.compile(r"\(\s")
    bad_right_brace_re = re.compile(r"\s\)")

    for i in range(len(file_lines)):
        line = clean_file_lines[i]

        if re.search(bad_left_brace_re, line) != None:
            log_warning(filename, i + 1, "unneeded space(s) after left brace", file_lines[i].strip(" "))

        if re.search(bad_right_brace_re, line) != None:
            log_warning(filename, i + 1, "unneeded space(s) before right brace", file_lines[i].strip(" "))

def check_commas(filename, file_contents, file_lines, clean_file_contents, clean_file_lines):
    """
    Check that there is no space before commas, and there is exactly one space after coma

    """
    nospace_after_re = re.compile(r",((?!\s).)")
    too_many_spaces_after_re = re.compile(r",\s(\s+)")
    space_before_re = re.compile(r"\S(\s+),")

    for i in range(len(file_lines)):
        line = clean_file_lines[i]

        if re.search(nospace_after_re, line) != None:
            log_warning(filename, i + 1, "missing space after comma", file_lines[i].strip(" "))

        if re.search(too_many_spaces_after_re, line) != None:
            log_warning(filename, i + 1, "unneeded space(s) after comma", file_lines[i].strip(" "))

        if re.search(space_before_re, line) != None:
            log_warning(filename, i + 1, "unneeded space(s) before comma", file_lines[i].strip(" "))

def check_function_call_or_definition(filename, file_contents, file_lines, clean_file_contents, clean_file_lines):
    """
    Check that there is no space between function call/definition and left parenthese

    """
    #in bad function definition or call there exists a space (or more) between function name and left brace (the last \s+ in the expression)
    #the function name must not be a if, for, switch or while
    reserved_words = ['if', 'for', 'while', 'switch', 'return', 'delete']
    reserved_words_re_text = "(?<!\W" + ")(?<!\W".join(reserved_words) + ")"

    #the bad function is anything except the reserved words, and the closing brace is not directly followed by another open brace
    #(to differentiate function calls/defs from function pointer defs)
    #the \s+ is the fauled pattern that should be caught, because it means a space exists between function name and left parenthese
    bad_func_re_text = r"(\w+){0}(\s+)(\()".format(reserved_words_re_text)
    bad_func_re = re.compile(bad_func_re_text)

    #function pointer
    return_type_re_text = r'\w+\s*(&|\*)*\s*'
    func_ptr_re_text = r'\s*{0}\((\w|\*|:|\s)+\)(\s*)\('.format(return_type_re_text)

    func_ptr_re = re.compile(func_ptr_re_text)

    macro_re = re.compile(r"(\s*)#define")

    for i in range(len(file_lines)):
        line = clean_file_lines[i]

        #check all occurances in a line (a line can contain several function calls)
        for bad_func_match in bad_func_re.finditer(line):
            #print line[bad_func_match.start():], re.match(func_ptr_re, line[bad_func_match.start():]) != None
            if re.search(func_ptr_re, line) == None and re.match(macro_re, line) == None:
                log_warning(filename, i + 1, "unneeded space(s) between function name and left brace", file_lines[i].strip(" "))

def check_keyword_brace_space(filename, file_contents, file_lines, clean_file_contents, clean_file_lines):
    """
    Check that all occurances of (if, for,...etc) in file have a space between keyword and left brace or
    that all occurances do not have a space between keyword and left brace

    """
    #the keywords to be checked
    reserved_words = ['if', 'for', 'while', 'switch', 'return']
    #the \W makes sure the keyword is actually a keyword and not a part of a function name, e.g, if() would match but foo_if() would be filtered out
    reserved_words_re_text = r'\W(' + "|".join(reserved_words) + ')'

    nospace_re = re.compile(r"{0}\(".format(reserved_words_re_text))
    space_re = re.compile(r"{0}\s\(".format(reserved_words_re_text))
    too_many_space_re = re.compile(r"{0}\s(\s+)\(".format(reserved_words_re_text))

    #check if there is a general inconsistency (a signle occurance is in the file is enough to give a warning)
    nospace_match = re.search(nospace_re, clean_file_contents)
    space_match = re.search(space_re, clean_file_contents)
    if nospace_match != None and space_match != None:
        line1 = clean_file_contents[:nospace_match.start()].count("\n") + 1
        line2 = clean_file_contents[:space_match.start()].count("\n") + 1

        log_warning(filename, line1, "inconsistent spacing between keyword and left brace. All braces in a file must have same space (0 or 1 spaces) [Reported once per file]", "lines :{0} and {1}".format(line1, line2))

    for i in range(len(file_lines)):
        line = clean_file_lines[i]

        if re.search(too_many_space_re, line) != None:
            log_warning(filename, i + 1, "unneeded space(s) between keyword and left brace", file_lines[i].strip(" "))

def check_consistent_code_style(filename, file_contents, clean_file_contents, file_lines, clean_file_lines):
    check_function_call_or_definition(filename, file_contents, file_lines, clean_file_contents, clean_file_lines)
    check_semicolor(filename, file_contents, file_lines, clean_file_contents, clean_file_lines)
    check_keyword_brace_space(filename, file_contents, file_lines, clean_file_contents, clean_file_lines)
    check_braces_spaces(filename, file_contents, file_lines, clean_file_contents, clean_file_lines)
    check_commas(filename, file_contents, file_lines, clean_file_contents, clean_file_lines)
    check_binary_operators(filename, file_contents, file_lines, clean_file_contents, clean_file_lines)


if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories

\tGives warnings if there is inconsistency in spacing of commas, semicolons, operands, braces, function calls or definitions.
"""
        exit(0)

    for t in targets:
        if t[-2:] == ".h" or t[-4:] == ".cpp" or t[-2] == ".c":
            file_contents, clean_file_contents, file_lines, clean_file_lines = read_file(t)
            check_consistent_code_style(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
