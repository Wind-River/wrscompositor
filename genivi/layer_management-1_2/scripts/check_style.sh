#!/bin/bash

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



DIRECTORY=$(cd `dirname $0` && pwd)
cd $DIRECTORY
cd ..

echo $PWD
echo "----------------------------------------"

TEMP_FILENAME=".check_style_temp.`date +%s%N`"

TARGET_FILES="`find -iname *.cpp -or -iname *.h -or -iname *.c | grep -vi \/tests\/ | grep -vi \/test\/ | grep -vi 3rdParty | grep -vi \/LayerManagerExamples\/ | grep -vi \config\/`"

for t_file in $TARGET_FILES
do
    python scripts/check_all_styles.py $t_file | tee $TEMP_FILENAME -a
done

LINES=`cat $TEMP_FILENAME | wc -l`

echo -e "\nTotal number of warnings: $LINES"
rm $TEMP_FILENAME