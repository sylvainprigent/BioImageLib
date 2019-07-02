############################################################
#
# $Id$
#
# Copyright (c) bioImageLib 2019
#
# AUTHOR:
# Sylvain Prigent
# 

# Project name
project(blStatisticsTools)


## #################################################################
## Exe generation
## #################################################################
add_executable(blWilcoxon blWilcoxon.cpp )
target_link_libraries(blWilcoxon blTxtIO blStatistics blCore)

add_executable(blStudent blStudent )
target_link_libraries(blStudent blTxtIO blStatistics blCore)

