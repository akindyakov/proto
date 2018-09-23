#!/usr/bin/env python3

import argparse
import datetime
import hashlib
import logging
import pathlib
import sys


_log = logging.getLogger(__name__)
_log_format = "%(asctime)s %(name)s [%(process)d] %(levelname)s %(message)s"

PATH_PREFIX_SIZE = 5

class Placeholder:
    pass

class Event:
    def __init__(self, name, params):
        self.name = name
        self.params = params
        self.hashes = set()
        name_m = hashlib.sha1()
        name_m.update(name.encode("utf-8"))
        self.hashes.add(name_m.hexdigest())

        for key, value in self.params.items():
            param_m = name_m.copy()
            param_m.update(key.encode("utf-8"))
            if isinstance(value, pathlib.Path):
                for pref in self._prefixes_for_path(value):
                    pref_m = param_m.copy()
                    pref_m.update(
                        pref.as_posix().encode("utf-8")
                    )
                    self.hashes.add(pref_m.hexdigest())
                    _log.debug("Hash path prefix: %r -> %r", pref.as_posix(), pref_m.hexdigest())
            elif isinstance(value, Placeholder):
                pass
            else:
                param_m.update(value.encode("utf-8"))
                self.hashes.add(param_m.hexdigest())

    @staticmethod
    def _prefixes_for_path(path):
        prefixes = list()
        while path.name:
            prefixes.append(path)
            path = path.parent
        return prefixes[-PATH_PREFIX_SIZE:]

    def __repr__(self):
        return "{name!r} {params!r} {hashes!r}".format(
            name = self.name,
            params = self.params,
            hashes = self.hashes,
        )


class EventTrap:
    def __init__(self, hashes, successors, action=None, repeatable=False):
        self._hashes = hashes
        self._successors = list(successors)
        self._action = action
        self._repeatable = repeatable

    def trap(self, event):
        triggered = all(h in event.hashes for h in self._hashes)
        if triggered:
            if self._action is not None:
                self._action(event)
                if self._repeatable:
                    return [self]
                else:
                    return []
            if self._repeatable:
                return self._successors + [self]
            else:
                return self._successors
        return [self]


class EventEdge:
    def __init__(self, predecessor, name, params, repeatable):
        self._event = Event(name, params)
        _log.debug("EventEdge created: %r", self._event)
        self._predecessor = predecessor
        self._successors = list()
        self._repeatable = repeatable

    def one(self, name, **params):
        successor = EventEdge(self, name, params, False)
        self._successors.append(successor)
        return successor

    def many(self, name, **params):
        successor = EventEdge(self, name, params, True)
        self._successors.append(successor)
        return successor

    def action(self, action):
        trap = EventTrap(
            self._event.hashes,
            successors=[],
            action=action,
            repeatable=self._repeatable,
        )
        edge = self
        while edge._predecessor is not None:
            edge = edge._predecessor
            trap = EventTrap(
                edge._event.hashes,
                successors=[trap],
                action=None,
                repeatable=edge._repeatable,
            )
        return trap

    def null(self, action):
        return self.action(self, None)

def many(name, params):
    return EventEdge(None, name, params, True)


def one(name, params):
    return EventEdge(None, name, params, False)


def print_action(event):
    _log.debug(" -> Some final action on event: %r", event)


def dfs():
    traps = [
        many("first", {}).action(print_action),
        many("first", {}).one("second").action(print_action),
        many("first", {}).many("second").one("2").action(print_action),
        many("1", {}).one("2").one("3").action(print_action),
        many("open", {"path": pathlib.Path("/etc/netctl")}).action(print_action),
    ]
    return traps


def read_event():
    args = input("Enter event: ").strip().split()
    name = args[0]
    args = args[1:]
    params = dict()
    for ind, value in enumerate(args):
        key, value = value.split(":", 1)
        if key == "path":
            value = pathlib.Path(value)
        params[key] = value
    return Event(name, params)


def run(args):
    traps = dfs()
    while True:
        event = read_event()
        _log.debug("Event: %r", event)
        traps = [
            next_trap
            for trap in traps
            for next_trap in trap.trap(event)
        ]
        _log.debug("Traps count: %r", len(traps))


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
