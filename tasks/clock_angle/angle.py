#!/usr/bin/env python2
"""
Write a function angle(time) which takes string of time in format “HH:MM” and return the angle between the minute and hour on hand on clock.
"""


def angle(s):
    hour, minutes = s.split(":")
    hour = int(hour) % 12
    minutes = int(minutes)
    minutes_abs_ang = minutes * (360 / 60)
    hour_abs_ang = hour * (360 / 12) + (minutes / 12.) * 60

    ang = minutes_abs_ang - hour_abs_ang
    if ang < 0.:
        ang *= -1.

    if ang > 180.:
        ang = 360 - ang

    return ang


def run():
    assert angle("00:00") == 0.


if __name__ == "__main__":
    run()
