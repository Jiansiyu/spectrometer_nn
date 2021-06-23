#!/bin/bash

yourfilenames=`ls *.Rmd`
for eachfile in $yourfilenames
do
   echo $eachfile
   Rscript -e "rmarkdown::render('${eachfile}')"
done