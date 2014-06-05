import sys
from zipfile import ZipFile

def unzip(filename):
    with ZipFile(filename) as f:
        f.extractall()

if __name__ == '__main__':
    filename = sys.argv[1]
    unzip(filename)
