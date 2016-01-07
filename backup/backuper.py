#!/usr/bin/env python3

import argparse
import datetime
import logging
import os
import subprocess
import sys
import tempfile

log = logging.getLogger(__name__)

storage = {
    "index": ["phobos", "deimos"],
}

class TRsyncError(Exception):
    def __init__(self, returncode):
        self.returncode = returncode

    def __repr__(self):
        return "TRsyncError(returncode={})".format(self.returncode)

class TTarError(Exception):
    def __init__(self, returncode):
        self.returncode = returncode

    def __repr__(self):
        return "TTarError(returncode={})".format(self.returncode)


def irsync(from_path, to_path, opts):
    cmd = " ".join([
        "rsync",
        opts,
        from_path,
        to_path,
    ])
    log.debug("run: %s", cmd)
    with subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as proc:
        for line in proc.stdout:
            yield line
        for line in proc.stderr:
            log.debug("Popen stderr: %s", line)
        proc.wait()
        if proc.returncode != 0:
            raise TRsyncError(proc.returncode)


def pack(path, tfile):
    """
        tar --create path --gzip --file tfile
    """
    cmd = [
        "tar",
        "--create", path,
        "--gzip",
        "--absolute-names",
        "--file", tfile,
    ]
    log.debug(" ".join(cmd))
    with subprocess.Popen(" ".join(cmd), shell=True, stderr=subprocess.PIPE) as proc:
        for line in proc.stderr:
            log.debug("Popen stderr: %s", line)
        proc.wait()
        if proc.returncode != 0:
            raise TTarError(proc.returncode)


def implace_backup(args):
    opts = (
        " --recursive"
        " --times"
        " --sparse"
    )
    dst = ""
    if not args.local:
        opts += """ --rsh='ssh -p {}'""".format(args.backup_port)
        dst = "{}:{}".format(
            args.backup_host,
            args.backup_path,
        )
    else:
        dst = args.backup_path
    for i in irsync(args.what, dst, opts):
        log.info(i)


def tail_backup(args):
    with tempfile.NamedTemporaryFile(mode='w+b', prefix="tmp.", suffix='.tar.gz') as tmp:
        pack(args.what, tmp.name)
        fname = datetime.datetime.now().strftime(args.backup_file_fmt) + ".tar.gz"
        if not args.local:
            dst = "{}:{}".format(
                args.backup_host,
                os.path.join(args.backup_path, fname),
            )
            for _ in irsync(tmp.name, dst, """--rsh='ssh -p {}'""".format(args.backup_port)): pass
        else:
            dst = os.path.join(args.backup_path, fname)
            for _ in irsync(tmp.name, dst, ""): pass


def main():
    args = arguments()
    if args.log is not None:
        log.debug("add custom log file [%s]", args.log)
        filehandler = logging.FileHandler(args.log)
        filehandler.setLevel(logging.DEBUG)
        filehandler.setFormatter(
            logging.Formatter("%(asctime)s %(name)s [%(process)d] %(levelname)s %(message)s")
        )
        log.addHandler(filehandler)

    if args.tail_size == 0:
        implace_backup(args)
    else:
        tail_backup(args)


def arguments():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument(
        "--what",
        required=True,
        metavar="PATH",
        type=str,
        help="Backuping directory",
    )
    parser.add_argument(
        "--backup-host",
        type=str,
        help=""
    )
    parser.add_argument(
        "--backup-port",
        type=str,
        help=""
    )
    parser.add_argument(
        "--backup-path",
        required=True,
        type=str,
        help=""
    )
    parser.add_argument(
        "--local",
        action="store_true",
        help="Local mode",
    )
    parser.add_argument(
        "--backup-file-fmt",
        type=str,
        help=".tar.gz file fmt",
    )
    parser.add_argument(
        "--tail-size",
        type=int,
        default=0,
        help="Tail size for filestorage. Default: %(default)s",
    )
    parser.add_argument(
        "--log",
        type=str,
        metavar="FILE",
        help="Custom log file name",
    )
    return parser.parse_args()


if __name__ == "__main__":
    try:
        log = logging.getLogger("main")
        log.setLevel(logging.DEBUG)

        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        ch.setFormatter(
            logging.Formatter("%(asctime)s %(name)s %(levelname)s %(message)s")
        )
        log.addHandler(ch)

        main()

    except SystemExit:
        raise

    except:
        log.exception("Catched exception ")
        sys.exit(1)
