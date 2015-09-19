#!/usr/bin/env python3

import argparse
import logging
import sys
import hashlib
import datetime

log = logging.getLogger(__name__)

cfg = {
    "akindyakov_source": {
        "local": True,
        "path": "/home/akindyakov/bare.repo",
        "tail_size": 14,
        "time_interval": datetime.timedelta(days=1),
        "storage": {
            "index": ["phobos", "deimos"],
        },
    },
    "akindyakov_photo": {
        "inplace": True,
        "local": False,
    },
    "akindyakov_doc": {
        "inplace": True,
        "local": False,
    },
    "akindyakov_video": {
        "inplace": True,
        "local": False,
    },
    "akindyakov_video": {
        "inplace": True,
        "local": False,
    },
}

def check_time():
    pass

def check():
    pass

def main():
    parser = argparse.ArgumentParser(description='')
    parser.add_argument(
        '--backup-server-host',
        type=str,
        help=''
    )
    parser.add_argument(
        '--backup-server-port',
        type=str,
        help=''
    )
    parser.add_argument(
        '--backup-server-path',
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
