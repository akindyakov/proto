#!/usr/bin/env python3

import argparse
import datetime
import hashlib
import logging
import pathlib


_log = logging.getLogger(__name__)
_log_format = "%(asctime)s %(name)s [%(process)d] %(levelname)s %(message)s"


class Event:
    def __init__(self, name, params):
        self.name = name
        self.params = params
        self.hashes = set()
        for key, value in self.params.items():
            m = hashlib.sha1()
            m.update(name.encode("utf-8"))
            m.update(key.encode("utf-8"))
            if isinstance(value, pathlib.Path):
            else:
                m.update(value.encode("utf-8"))
                self.hashes.add(m.hexdigest())


class EventTrap:
    def __init__(self, hashes, successors, action=None):
        self._hashes = hashes
        self._successors = list(successors)
        self._action = action

    def trap(self, event):
        triggered = all(h in event.hashes for h in self._hashes)
        if triggered:
            if self._action is not None:
                self._action(event)
            return self._successors
        return [self]


class EventEdge:
    def __init__(self, predecessor, name, params, repeatable):
        _log.debug("EventEdge created: %r %r", name, params)
        self._predecessor = predecessor
        self._successors = list()
        self._repeatable = repeatable

    def next(self, name, **params):
        _log.debug("Next: %r %r", name, params)
        successor = EventEdge(self, name, params, False)
        self._successors.append(successor)
        return successor

    def repeat(self, name, **params):
        _log.debug("Repeat: %r %r", name, params)
        successor = EventEdge(self, name, params, True)
        self._successors.append(successor)
        return successor

    def action(self, action_):
        #self._actions.append(action_)

    def _as_actionable(self):
        pass


class InitialEventEdge(EventEdge):
    def __init__(self):
        EventEdge.__init__(self, None, "initial")


def dfs():
    init = InitialEventEdge()
    init.next("first")#.action(lambda 
    init.next("second").next("third")
    return [] # init


def read_event():
    args = input("Enter event: ").strip().split()
    name = args[0]
    args = args[1:]
    params = {
        str(ind): value
        for ind, value in args
    }
    return Event(name, params)


def run(args):
    traps = dfs()
    while True:
        event = read_event()
        traps = [
            next_trap
            for trap in traps
            for next_trap in trap.trap(event)
        ]


def arguments():
    parser = argparse.ArgumentParser(description="")
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
