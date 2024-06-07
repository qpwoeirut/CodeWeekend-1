import api
import sys
import argparse
import tqdm
import time
import pathlib
import os
import subprocess

class bcolors:
    OK = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    RED= '\033[91m'
    YELLOW = '\033[33m'
    GREEN = '\033[32m'
    BLUE = '\033[96m'
    PURPLE = '\033[34m'
    PINK = '\033[35m'



"""
# TODO TESTCASES
#* Testcase 1: usper easy idk lorem ipsum
#? Testcase 2: usper medium idk lorem ipsum
#! Testcase 3: usper hard idk lorem ipsum
...
"""

NUM_TESTCASES = 5

GPP_PATH = "g++-14"


def compile_solution(solution):
    fileExt = pathlib.Path(solution).suffix
    fileName = pathlib.Path(solution).name
    if fileExt == '.cpp':
        #os.system(f"g++ -O3 -Wall -funroll-loops -flto -mcpu=native {solution} -o compile-dump/{fileName}")
        process = subprocess.run(f"{GPP_PATH} -O3 -Wall -funroll-loops -flto -mcpu=native {solution} -o compile-dump/{fileName}", shell=True, stderr=subprocess.PIPE)
        stderr = process.stderr.decode('utf-8')
        if stderr:
            print(f"Compilation error: {stderr}")
            sys.exit(1)
    else:
        pass

def run_testcase(solution, testcase):
    fileExt = pathlib.Path(solution).suffix
    fileName = pathlib.Path(solution).name
    #print(f"Running {fileName} on testcase {testcase}")
    if fileExt == '.py':
        #os.system(f"cat testcases/tc{testcase}.txt | python3 {solution} > output/{fileName}_OUT_TC{testcase}.txt")
        process = subprocess.run(f"cat testcases/tc{testcase}.txt | python3 {solution} > output/{fileName}_OUT_TC{testcase}.txt", shell=True, stderr=subprocess.PIPE)
        stderr = process.stderr.decode('utf-8')
        if stderr:
            print(f"Runtime error: {stderr}")
            sys.exit(1)
    elif fileExt == '.cpp':
        #os.system(f"./compile-dump{fileName} < testcases/tc{testcase}.txt > output/{fileName}_OUT_TC{testcase}.txt")
        process = subprocess.run(f"./compile-dump/{fileName} < testcases/tc{testcase}.txt > output/{fileName}_OUT_TC{testcase}.txt", shell=True, stderr=subprocess.PIPE)
        stderr = process.stderr.decode('utf-8')
        if stderr:
            print(f"Runtime error: {stderr}")
            sys.exit(1)
    else:
        #! add more file extensions if needed
        print(f"Invalid file extension: {fileExt}")
        sys.exit(1)

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

    #assert current directory is root of the project
    if not os.path.exists("testcases"):
        print("please run this script from the root of the project directory")
        sys.exit(1)

    if args.solution is None:
        print("Solution file not provided")
        sys.exit(1)

    compile_solution(args.solution)
    
    tcs = parse_testcase(args.testcase)

    colors = [bcolors.RED, bcolors.YELLOW, bcolors.GREEN, bcolors.BLUE, bcolors.PURPLE, bcolors.PINK]

    for i,tc in enumerate(tcs):
        #print("hi", i)
        print(colors[i%len(colors)], end="")
        #print(bcolors.RED)
        for j in tqdm.tqdm(range(args.trials), desc=f"Testcase {tc}"):
            #print(colors[j%len(colors)], end="", flush=True)
            run_testcase(args.solution, tc)
        #print(bcolors.ENDC, end="")
            




if __name__ == "__main__":
    main()

