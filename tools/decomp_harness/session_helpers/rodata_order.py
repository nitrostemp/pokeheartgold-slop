#!/usr/bin/env python3
"""Predict MWCC 2.0/sp2p2 .rodata object order.

MWCC sorts a translation unit's .rodata objects by size (ascending) with a
heapsort over the objects in REVERSE creation order. Creation order is source
order: a file-scope const object is created at its definition, an anonymous
local initializer (e.g. `int list[5] = {...}` or `BgTemplate t = {...}`) when
its function is compiled. Because heapsort is unstable, the order among
same-size objects depends on the whole list, not only on their own order.

Verified on unk_02061284 (51 objects) and overlay_34 (8 objects).

Usage:
  rodata_order.py simulate NAME:SIZE ...      # creation order -> output order
  rodata_order.py solve TARGET_FILE           # brute-force a creation order
TARGET_FILE lines: "NAME SIZE [fixed]" in desired OUTPUT order; objects marked
"fixed" keep their relative creation order (e.g. anonymous initializers of one
function), the rest are permuted. Prints the first creation orders that work.
"""
import itertools
import sys


def heapsort(items, key):
    a = list(items)
    n = len(a)

    def sift(start, end):
        root = start
        while 2 * root + 1 <= end:
            child = 2 * root + 1
            swap = root
            if key(a[swap]) < key(a[child]):
                swap = child
            if child + 1 <= end and key(a[swap]) < key(a[child + 1]):
                swap = child + 1
            if swap == root:
                return
            a[root], a[swap] = a[swap], a[root]
            root = swap

    for start in range((n - 2) // 2, -1, -1):
        sift(start, n - 1)
    for end in range(n - 1, 0, -1):
        a[0], a[end] = a[end], a[0]
        sift(0, end - 1)
    return a


def simulate(creation_order, sizes):
    return heapsort(list(reversed(creation_order)), lambda k: sizes[k])


def solve(target, sizes, fixed, limit=10):
    movable = [n for n in target if n not in fixed]
    found = 0
    for perm in itertools.permutations(movable):
        # interleave the fixed block (kept in its given order) at every position
        for pos in range(len(perm) + 1):
            order = list(perm[:pos]) + fixed + list(perm[pos:])
            if simulate(order, sizes) == target:
                print(' '.join(order))
                found += 1
                if found >= limit:
                    return


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    if sys.argv[1] == 'simulate':
        names, sizes = [], {}
        for arg in sys.argv[2:]:
            name, size = arg.rsplit(':', 1)
            names.append(name)
            sizes[name] = int(size, 0)
        print(' '.join(simulate(names, sizes)))
    elif sys.argv[1] == 'solve':
        target, sizes, fixed = [], {}, []
        for line in open(sys.argv[2]):
            parts = line.split()
            if not parts:
                continue
            target.append(parts[0])
            sizes[parts[0]] = int(parts[1], 0)
            if len(parts) > 2 and parts[2] == 'fixed':
                fixed.append(parts[0])
        solve(target, sizes, fixed)


if __name__ == '__main__':
    main()
