##################################################
########### PiCore Library for Bare metal code on
########### the Raspberry Pi 2  ##################
##################################################
# This is an open source library for programming 
# a bare application on a Raspberry Pi 2.
# This a playground for me to learn Embedded System
# programming using the C Programming Language.
# This projet comes with a linker script that is 
# C++ ready, I have tested it and run the C++ version
# this library. 
# Since some times already The Raspberry Pi 2/3 boot 
# in Hypervisor Mode. So I'm trying to figure out
# how to properly activate Interrupts.
# As of now, when an IRQ is thrown, an UNDEFINED
# INSTRUCTION is capture. I'm trying to figure out 
# the real cause of that.
## UPDATE 1 : I got a little time and used it to hack on
# my code find the issue: the source of the problem 
# was on using the gcc attribute :
# void f()  __attribute__((interrupt("IRQ"))).
# removing these options and handling register back up myself
# in assembly resolved the problem.

# If you use this code or part of code, just mention
# this project as the source.
# You are free to contribute to this project if you want
# to. 
####################################################
