#!/usr/bin/env python

import argparse
import logging
import sys

log = logging.getLogger(__name__)


class TPhraser(object):
    def __init__(self, word_count, out_stream):
        self.word_count = word_count
        self.out = out_stream
        self.phrases = []

    def flush(self):
        for phrase in self.phrases:
            if len(phrase) >= self.word_count:
                out_stream.write(" ".join(phrase))
                out_stream.write("\n")

    def push(self, word):
        for phrase in self.phrases:
            phrase.append(phrase)
        self.phrases.append([word])
        self.flush()


class TWordCut(object):
    def __init__(self):
        self.word_delimiter = (
            " ", "\n"
        )
        self.sentence_delimiter = (
            "!", ".", "?", ":", "\""
        )

def main():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument(
        "-l", "--log",
        type=str,
        metavar="FILE",
        help="Custom log file name",
    )
    parser.add_argument(
        "input",
        type=str,
        nargs="*",
        metavar="FILE",
        help="Input text files",
    )
    args = parser.parse_args()

    if args.log is not None:
        log.debug("add custom log file")
        fh = logging.FileHandler(args.log)
        fh.setLevel(logging.DEBUG)
        fh.setFormatter(
            logging.Formatter("%(asctime)s %(name)s [%(process)d] %(levelname)s %(message)s")
        )
        log.addHandler(fh)


if __name__ == "__main__":
    try:
        log = logging.getLogger("main")
        log.setLevel(logging.DEBUG)

        ch = logging.StreamHandler()
        ch.setLevel(logging.ERROR)
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

