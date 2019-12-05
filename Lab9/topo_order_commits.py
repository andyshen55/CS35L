#!/usr/local/cs/bin/python3

from os import listdir, getcwd
from os.path import dirname
from sys import stderr

def topo_order():
    if not discover(getcwd()):
        print("Not inside a Git repository", file=stderr)
        return 1
    
def discover(path):
    if path == "/":
        return False
    return ".git" in listdir(path) or discover(dirname(path))
    
if __name__ == '__main__':
    topo_order()
