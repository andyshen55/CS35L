#!/usr/local/cs/bin/python3

from os import listdir, getcwd, chdir
from os.path import dirname, join, abspath
from sys import stderr
from zlib import decompress

#sorry aaron i decided to spend my time studying instead of finishing this hw
def topo_order():
    git_dir = discover(getcwd())
    chdir(join(git_dir, ".git"))
    commit_tree = []
    
    branch_dir = join(getcwd(), "refs", "heads")
    branches = [join(branch_dir, branch) for branch in listdir(branch_dir)]
    print(branches)

    for branch in branches:
        dfs_commits(branch)

            
def dfs_commits(branch):
    with open(branch, 'r') as f:
        hash = f.read().strip('\n')
        hash_dir = join(getcwd(), "objects", hash[:2], hash[2:])

        with open(hash_dir, 'rb') as commit_blob:
            decompressed = decompress(commit_blob.read()).decode()
            for line in decompressed.splitlines():
                if "parent" in line:
                    parents = line.split()[1:]
                    
def discover(path):
    if path == "/":
        print("Not inside a Git repository", file=stderr)
        exit(1)

    if ".git" in listdir(path):
        return path
    
    return discover(dirname(path))

class CommitNode:
    def __init__(self, commit_hash):
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()
    
if __name__ == '__main__':
    topo_order()
