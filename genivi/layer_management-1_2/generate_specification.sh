#!/bin/bash

LATEX_PATH=`pwd`/doc_generated/latex
PDF_PATH=`pwd`/doc_generated
PDF_NAME=IVI_LayerManagement_Design_v3_0_MG0001


#==============================================================================
echo "running doxygen..."
#==============================================================================
doxygen

#==============================================================================
echo "patching latex main document..."
#==============================================================================
# doxygen will generate more sections of the document
# than neccessary for the specififcation document.
# This processing step removes the unneccessary sections
# from the main latex file, so they won't be included in
# the resulting PDF file.

MAIN_PAGE=`cat $LATEX_PATH/refman.tex`
echo "$MAIN_PAGE" \
    | grep -Ev "{struct|{class" \
    | grep -Ev "printindex" \
    | grep -Ev "{action}|{LayerManagement Command Action Overview}" \
    | grep -Ev "{frequency}|{LayerManagement Command Frequency Overview}" \
    | grep -Ev "{Class Documentation}" \
> $LATEX_PATH/refman.tex


#==============================================================================
echo "creating PDF from latex documentation..."
#==============================================================================
cd $LATEX_PATH
make 1> /dev/null
cd -


#==============================================================================
echo "applying name to PDF document..."
#==============================================================================
cp $LATEX_PATH/refman.pdf $PDF_PATH/$PDF_NAME.pdf


#==============================================================================
echo "The LayerManagement specification was stored here:"
#==============================================================================
echo "$PDF_PATH/$PDF_NAME.pdf"

