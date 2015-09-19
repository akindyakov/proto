#!/usr/bin/env python3.4

import argparse
import logging
import sys
import hashlib

log = logging.getLogger(__name__)

def buildHash(finename, memory_limit=8 * 2**20):
    md5 = hashlib.md5()
    with open(filename, "r+b") as fin:
        chunk = fin.read(memory_limit)
        while chunk:
            md5.update(chunk)
    return md5.hexdigest()

def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument(
        '-p', '--path',
        type=str,
        help=''
    )
    parser.add_argument(
        '-l', '--log',
        type=str,
        metavar="FILE",
        help='Custom log file name',
    )
    args = parser.parse_args()

    if args.log is not None:
        log.debug("add custom log file")
        fh = logging.FileHandler(args.log)
        fh.setLevel(logging.DEBUG)
        fh.setFormatter(
            logging.Formatter('%(asctime)s %(name)s [%(process)d] %(levelname)s %(message)s')
        )
        log.addHandler(fh)


if __name__ == "__main__":
    try:
        log = logging.getLogger("main")
        log.setLevel(logging.DEBUG)

        ch = logging.StreamHandler()
        ch.setLevel(logging.ERROR)
        ch.setFormatter(
            logging.Formatter('%(asctime)s %(name)s %(levelname)s %(message)s')
        )
        log.addHandler(ch)

        main()

    except SystemExit:
        raise

    except:
        log.exception("Catched exception ")
        sys.exit(1)
