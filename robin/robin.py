#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import datetime
import hashlib
import logging
import sys
import pathlib
import socket


_log = logging.getLogger(__name__)
_log_format = "%(asctime)s %(name)s [%(process)d] %(levelname)s %(message)s"


def run(args):
    _log.debug("Message: %s", pathlib.Path.home())
    with open(args.filepath, "a") as fout:
        fout.write(
            "{tm} [{place}] -> {msg}\n".format(
                tm=datetime.datetime.now().strftime("%Y.%m.%d %a %H:%M:%S"),
                place = socket.gethostname(),
                msg=args.message,
            )
        )


def arguments():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument(
        "message",
        type=str,
        metavar="STR",
        help="Message to log",
    )
    parser.add_argument(
        "--filepath",
        type=str,
        metavar="FILE",
        default=pathlib.Path.home().joinpath("source/git.note/todo/day.log"),
        help="File",
    )
    parser.add_argument(
        "--log",
        type=str,
        metavar="FILE",
        help="Custom log file name",
    )
    return parser.parse_args()


def main():
    try:
        global _log
        _log = logging.getLogger("main")
        _log.setLevel(logging.DEBUG)
        ch = logging.StreamHandler()
        ch.setLevel(logging.DEBUG)
        ch.setFormatter(
            logging.Formatter(_log_format)
        )
        _log.addHandler(ch)
        args = arguments()
        if args.log is not None:
            _log.debug("add custom log file [%s]", args.log)
            filehandler = logging.FileHandler(args.log)
            filehandler.setLevel(logging.DEBUG)
            filehandler.setFormatter(
                logging.Formatter(_log_format)
            )
            _log.addHandler(filehandler)
        run(args)

    except SystemExit:
        raise

    except:
        _log.exception("Catched exception ")
        sys.exit(1)


if __name__ == "__main__":
    main()
