#!/usr/bin/env python3

"""
shuf.py - GNU shuf command, implemented in Python

Name: Thant Zin Oo
UID: 705172144
"""

import random, sys
import optparse

class shuf:
    def __init__(self, repeat, count, inputList):
        self.repeat = repeat
        self.count = count
        self.inputList = inputList

    def shuffle(self):
        random.shuffle(self.inputList)
        
        #if repeat is specified, selection without replacement
        if self.repeat:
            while self.count > 0:
                sys.stdout.write(random.choice(self.inputList))
                self.count -= 1
                
        else:
            #avoid duplicate lines by outputting shuffled lines in order
            for i, line in enumerate(self.inputList):
                if i == self.count or i == len(self.inputList):
                    break
                sys.stdout.write(self.inputList[i])

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Shuffles input by outputting a random permutation of its input lines."""

    #argument handling/parsing
    parser = optparse.OptionParser(version=version_msg, usage=usage_msg)

    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines to be repeated")

    parser.add_option("-n", "--head-count", metavar='\b',
                      action="store", dest="count", default=sys.maxsize,
                      help="output at most COUNT lines")

    parser.add_option("-i", "--input-range", metavar='\b',
                      action="store", dest="inputRange", default="",
                      help="treat each number LO through HI as an input line")

    #tuple unpacking to get options dict and args list
    options, args = parser.parse_args(sys.argv[1:])
    
    #exception handling for inputRange and count flags
    try:
        inputRange = options.inputRange
    except:
        parser.error("invalid INPUTRANGE: {0}".format(options.inputRange))
       
    try:
        count = int(options.count)
    except:
        parser.error("invalid COUNT: {0}".format(options.count))

    if count < 0:
        parser.error("negative COUNT: {0}".format(count))
        
    #error checking for input range violations 
    if len(inputRange):
        #check if hyphen range separator is present
        try:
            hyphen = inputRange.index('-')
        except ValueError as err:
            parser.error("invalid INPUTRANGE: {0}".format(options.inputRange))

        begin, end = inputRange.split("-")
        try:
            begin = int(begin)
            end = int(end)
        except ValueError as err:
            parser.error("invalid INPUTRANGE: {0}".format(options.inputRange))

        if begin > end:
            parser.error("invalid INPUTRANGE: {0}".format(options.inputRange))

        #construct list of values from given range
        inputList = [str(x) + "\n" for x in range(begin, end + 1)]

    #if no input range is given, check for stdin     
    else:
        #if no options given or single - option
        if not len(args) or (len(args) == 1 and args[0] == "-"):
            inputList = sys.stdin.readlines()

        #check if option is valid filename
        elif len(args) == 1:
            try:
                with open(args[0], 'r') as inFile:
                    #check if -i and file are provided
                    if options.inputRange != "":
                        parser.error("Error: both file and inputRange provided")
                    inputList = inFile.readlines()
                    
            except IOError as errno:
                parser.error("I/O error: {0}".format(errno))

        else:
            parser.error("wrong number of operands")
                          
    generator = shuf(options.repeat, count, inputList)
    generator.shuffle()
    
if __name__ == "__main__":
    main()
