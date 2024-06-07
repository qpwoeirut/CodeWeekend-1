import api
import sys
import argparse
import tqdm
import time

class bcolors:
    OK = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

"""
# TODO TESTCASES
#* Testcase 1: usper easy idk lorem ipsum
#? Testcase 2: usper medium idk lorem ipsum
#! Testcase 3: usper hard idk lorem ipsum
...
"""

NUM_TESTCASES = 5


def run_testcase(solution, testcase):
    time.sleep(0.1)
    pass


def parse_testcase(testcase):
    testcase.strip()
    testcase = testcase.replace(' ', '')
    testcase = testcase.split(',')
    tcs = []
    for tc in testcase:
        if tc == '0':
            return list(range(1, NUM_TESTCASES+1))
        elif '-' in tc:
            begin, end = tc.split('-')
            assert int(begin) <= int(end), f"Invalid range: {begin}-{end}"
            assert 1 <= int(begin),  f"Invalid testcase number: {begin}"
            assert int(end) <= NUM_TESTCASES, f"Invalid testcase number: {end}"
            tcs += list(range(int(begin), int(end)+1))
        else:
            assert 1 <= int(tc) <= NUM_TESTCASES, f"Invalid testcase number: {tc}"
            tcs.append(int(tc))
    return tcs

def main():
    parser = argparse.ArgumentParser(description='Run testcases on solutions')
    
    parser.add_argument("--testcase", "-tc", type=str, default=0, help=f"Testcase number (1-{NUM_TESTCASES}), 0 for all, begin-end for range, `,` separated for multiple testcases")
    parser.add_argument("--solution", "-s", type=str, default=None, help="Path to solution file")
    parser.add_argument("--trials", "-t", type=int, default=1, help="Number of trials to run")

    args = parser.parse_args()

    print(args)

    if args.solution is None:
        print("Solution file not provided")
        sys.exit(1)

    
    tcs = parse_testcase(args.testcase)

    for tc in tcs:
        for j in tqdm.tqdm(range(args.trials), desc=f"Testcase {tc}"):
            run_testcase(args.solution, tc)





if __name__ == "__main__":
    main()

