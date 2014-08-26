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
from common_modules.config import G_MAX_INDENTATION_THRESHOLD
from common_modules.config import G_TAB_SEQUENCE

def get_correct_indentation_list(clean_file_lines):
    #It is important to keep track of the changes made to the indent level because
    #curly braces of namespaces do not affect indentation, while curly braces of
    #other statements like if, for and while increase/decease indentation
    indent_levels_stack = [0]

    #current indent level
    indent_level = 0

    #indent level for every line
    indentation_levels = [0] * len(clean_file_lines)

    #get correct indentation for every line
    for i in range(len(clean_file_lines)):
        line = clean_file_lines[i]

        def get_former_line():
            """
            Gets the last previous non-empty line

            """
            if i > 0:
                for j in range(i - 1, 0 , -1):
                    if len(clean_file_lines[j].strip(" \t\n\f\r")):
                        return clean_file_lines[j]
            return ""

        #previous non-empty line
        former_line = get_former_line()

        # the next part of the code decreases the indent level
        #
        # the level of indent decrease when right curly braces are found
        # in the line.
        #
        # if the line begins with a right curly brace then the line ITSELF
        # gets a decreased indent level
        #
        # if the line has some content on the same line before the right
        # curly brace (which is a bad practice, but nevertheless) the line
        # ITSELF does not get a decreased level of indentation, but the
        # following lines get a decreased level of indentation

        #check if the line begins with a right curly brace
        line_begins_with_right_brace = re.match(r"\s*\}", line) != None

        #if the line DOES NOT begin with right curly brace then it has the same level
        #of indentation as the previous line(s)
        if not line_begins_with_right_brace:
            indentation_levels[i] = indent_level

        #this is to decrease the level of indentation in case there is a right curly brace "}"
        #the level of indentation is decreased by popping elements from the indentation level stack
        #a loop is to account for cases where there are several braces on same line
        for _ in range(0, line.count("}") - line.count("{")):
            indent_level = indent_levels_stack.pop()

        #if the line begins with right curly brace then it gets a decreased
        #level of indentation
        if line_begins_with_right_brace:
            indentation_levels[i] = indent_level

        # the next part of the code increases the indent level
        #
        # indent level increases when a left curly brace is found
        #
        # an exception to this if the curly brace is the beginning
        # of a namespace or an extern block, in this case the indent
        # level does not increase

        # A flag is used in the very specific case of having several left
        # curly braces after a namespace/extern block (which is a bad practice)
        #
        # Initially the flag is False,soo that the first curly brace would not
        # increase indentation, after that the flag is raised (set to True)
        # so that every new left curly brace would incraese indentation
        flag = False

        #increase the level of indentation of there are open left curly braces "{"
        #a loop is to account for cases where there are several braces on same line
        for _ in range(0, line.count("{") - line.count("}")):
            #push the current level of indentation to the stack
            indent_levels_stack.append(indent_level)

            #if there is no namesapce or extern declaration before curly brace: increase indentation
            #that is because namespaces do not increase the level of indentation
            namespace_or_extern_re_text = r'\s*(namespace|(extern((\s*\S*\s*)?)(\s*)($|\{)))'
            namespace_or_extern_re = re.compile(namespace_or_extern_re_text)

            #if the current line contains a namespace/extern declation
            if re.match(namespace_or_extern_re, line) != None:
                #increase the level of indentation only if the flag is raised
                #(if it is NOT the first curly brace on line)
                if flag:
                    indent_level += 1
                else:
                    #raise the flag (so that next curly braces would increase indentation)
                    flag = True

            #otherwise: if the previous line contains namesapce/extern declaration
            elif re.match(namespace_or_extern_re, former_line) != None:
                #the indent level does not increase only if the previous line does not contain
                #a left curly brace, and if this is the first curly brace on the current line (flag is still False)
                #if either of the conditions is false it is safe to increase the indent level
                if re.search(r'(namespace|extern)((?!\{).)*\{', former_line) == None and not flag:
                    flag = True
                else:
                    indent_level += 1

            #otherwise: just increase indent level
            else:
                indent_level += 1

    return indentation_levels

def check_indentation(filename, file_contents, clean_file_contents, file_lines, clean_file_lines):
    """
    Checks if all lines in a file (except multi-line comments, strings and macros) have correct indentation

    """
    def is_statement_supplement(line, former_line, parentheses_count):
        """
        Checks if the statement is the completion of a previous statement, like arethmatic expressions broken on several lines, or for loop
        parts on separate lines. It also includes checks that the previous statement is well-closed (ends by semicolon or curly brace)

        IMPORTANT: the first line of a for loop (or an arethmatic expression...etc) broken on several lines would still not be considered
        a statement supplement

        """
        statement_supplement = False
        #include statements not preceded by a semicolon in previous line
        statement_supplement |= former_line != None and re.search(re.compile(r";(\s*)$"), former_line) == None
        #exclude lines containing block beginnings and ends
        statement_supplement &= re.search(re.compile(r"(\{|\})(\s*)$"), line) == None
        #exclude statements preceded by block beginnings and ends
        statement_supplement &= former_line != None and re.search(re.compile(r"(\{|\})(\s*)$"), former_line) == None
        #exclude preprocessor directives (they dunt have to end with a ; for example)
        statement_supplement &= not is_preprocessor_directive(line)
        #exclude statements preceded by preprocessor directives that do not end with a slash \
        statement_supplement &= former_line != None and re.match(re.compile(r"(\s*)#((?!\/).)*\/$"), former_line) == None

        #filter out the case where a line is preceeded by a label statement
        fl_label_statement = is_label_statement(former_line)
        statement_supplement &= not fl_label_statement

        #include statements that are contained inside parentheses
        statement_supplement |= parentheses_count > 0

        return statement_supplement

    def is_macro_body(former_line):
        """
        Checks if the line belongs to a macro body by checking that the line before it ends with a slash

        """
        return former_line != None and re.search(r"\\(\s*)$", former_line) != None

    def is_label_statement(line):
        """
        Checks if line starts with a label/case definition

        """
        label_statement = False
        if line != None:
            #case statements -> case id : expr that does not begin with a : (to avoid matching with :: operator)
            label_statement = re.match(r'(\s*)case(\s*)((?!:).)*(\s*):(?!:)', line) != None
            #label statements -> label : expr that does not begin with a : (to avoid matching with :: operator)
            label_statement |= re.match(r'(\s*)(\w+)(\s*):(?!:)', line) != None
        return label_statement

    def is_preprocessor_directive(line):
        """
        Checks if a line is a preprocessor directive

        """
        #preproccessor directives start with a #
        return re.match(r"(\s*)#", line) != None

    #keeps count of open (non-closed) left parentheses
    parentheses_count = 0

    #configured tab character
    tab_character = G_TAB_SEQUENCE[0]
    tab_character_count = G_TAB_SEQUENCE[1]

    #get list of correct levels of indentation for every line
    correct_indent_levels = get_correct_indentation_list(clean_file_lines)

    #indent level of current line and previous line
    correct_il = 0
    previous_il = 0

    #check every line for correct indentation
    for i in range(len(clean_file_lines)):
        line = clean_file_lines[i]
        #correct indent level of current line
        correct_il = correct_indent_levels[i]

        #actual indent level of the line
        actual_il = 0

        #get the actual line indentation
        #this regex searches for the first character in line that is not a space
        #if no match found this means a zero level of indentation
        indent_match = re.search("(?!{0})".format(tab_character), line)
        if indent_match:
            actual_il = 1.0 * indent_match.start() / tab_character_count

        def get_former_line():
            """
            Gets the last previous non-empty line

            """
            if i > 0:
                for j in range(i - 1, 0 , -1):
                    if len(clean_file_lines[j].strip(" \t\n\f\r")):
                        return clean_file_lines[j]
            return ""

        #previous non-empty line
        former_line = get_former_line()

        #make several checks about the line
        statement_supplement = is_statement_supplement(line, former_line, parentheses_count)
        label_statement = is_label_statement(line)
        fl_label_statement = is_label_statement(former_line)
        preprocessor_directive = is_preprocessor_directive(line)
        macro_body = is_macro_body(former_line)

        #if at non-empty line: check if indentation is correct
        if len(line.strip(" \t\n\r")) > 0:
            if macro_body:
                #do not check anything, just ignore line
                None
            elif preprocessor_directive:
                #preproccessor directives must have 0 level indentation
                if actual_il != 0:
                    log_warning(filename, i + 1, "incorrect indentation", file_lines[i].strip(" "))
            elif label_statement:
                #label statements must have one level less indentation than the normal indentation level
                if actual_il != correct_il - 1:
                    log_warning(filename, i + 1, "incorrect indentation", file_lines[i].strip(" "))
            elif (not statement_supplement) and actual_il != correct_il:
                #a complete statement (NON supplement statement) must be at the normal level of indentation
                log_warning(filename, i + 1, "incorrect indentation", file_lines[i].strip(" "))
            elif statement_supplement and actual_il < correct_il:
                #statement supplements must have a level of indentation at least equal to the normal level of indentation
                log_warning(filename, i + 1, "incorrect indentation", file_lines[i].strip(" "))

            #warning if indentation level is too much
            if correct_il == G_MAX_INDENTATION_THRESHOLD + 1 and previous_il < correct_il:
                log_warning(filename, i + 1, "code block has too much indentation [maximum level of indentation is {0}]".format(G_MAX_INDENTATION_THRESHOLD),)

        #update open parentheses count
        parentheses_count += line.count("(") - line.count(")")

        #update previous indent level
        previous_il = correct_il


if __name__ == "__main__":
    targets = sys.argv[1:]
    targets = get_all_files(targets)

    if len(targets) == 0:
        print """
\t**** No input provided ****
\tTakes a list of files/directories as input and performs specific style checking on all files/directories

\tGives warnings if lines (except multi-line comments and macros) do not have correct indentation.
\tIt also checks if code blocks have too much indentation (too deep). Code should have a maximum of {0} indentation levels.
""".format(G_MAX_INDENTATION_THRESHOLD)
        exit(0)

    for t in targets:
        if t[-2:] == ".h" or t[-4:] == ".cpp" or t[-2] == ".c":
            file_contents, clean_file_contents, file_lines, clean_file_lines = read_file(t)
            check_indentation(t, file_contents, clean_file_contents, file_lines, clean_file_lines)
