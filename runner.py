import api
import argparse
import json
import os
import pathlib
import subprocess
import sys
import tqdm


class bcolors:
    OK = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    RED = '\033[91m'
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

NUM_TESTCASES = 50

GPP_PATH = "g++-14"
FLAGS = ["-O3", "-Wall", "-funroll-loops", "-flto", "-march=native"]


def compile_solution(solution):
    if pathlib.Path(solution).suffix != '.cpp': return

    file_name = pathlib.Path(solution).name
    pathlib.Path("build").mkdir(exist_ok=True)
    process = subprocess.run([GPP_PATH, *FLAGS, solution, "-o", f"build/{file_name}"], stderr=subprocess.PIPE)
    stderr = process.stderr.decode('utf-8')
    if stderr:
        print(stderr)
    if process.returncode != 0:
        print("Compilation failed, stopping.")
        sys.exit(1)


def convert_txt_to_json(input_txt: str, output_json: str) -> None:
    def convert_line(line: list[str]) -> dict[str, str or int]:
        if line[0] == "move":
            assert len(line) == 3, f"unexpected number of tokens, expected 3, got {len(line)}"
            return {
                "type": line[0],
                "target_x": int(line[1]),
                "target_y": int(line[2])
            }
        elif line[0] == "attack":
            assert len(line) == 2, f"unexpected number of tokens, expected 2, got {len(line)}"
            return {
                "type": line[0],
                "target_id": int(line[1])
            }
        else:
            raise ValueError(f"unexpected action type: {line[0]}")

    with open(input_txt) as fin:
        lines = [line.strip().split() for line in fin]

    resp = {
        "moves": list(map(convert_line, lines))
    }

    with open(output_json, 'w') as fout:
        fout.write(json.dumps(resp))


def run_testcase(solution, testcase):
    file_extension = pathlib.Path(solution).suffix
    file_name = pathlib.Path(solution).name
    # print(f"Running {file_name} on testcase {testcase}")
    if file_extension == '.py':
        input_json = f"inputs/{format(testcase, '03')}.json"
        output_json = f"output/{file_name}_OUT_{format(testcase, '03')}.json"
        with open(input_json) as fin, open(output_json, 'w') as fout:
            process = subprocess.run(["python3", solution], stdin=fin, stdout=fout, stderr=subprocess.PIPE)
        stderr = process.stderr.decode('utf-8')
        if stderr:
            print(f"Runtime error: {stderr}")
            sys.exit(1)
    elif file_extension == '.cpp':
        input_txt = f"inputs/{format(testcase, '03')}.txt"
        output_txt = f"output/{file_name}_OUT_{format(testcase, '03')}.txt"
        with open(input_txt) as fin, open(output_txt, 'w') as fout:
            process = subprocess.run([f"./build/{file_name}"], stdin=fin, stdout=fout, stderr=subprocess.PIPE)
        stderr = process.stderr.decode('utf-8')
        if stderr:
            print(f"Runtime error: {stderr}")
            sys.exit(1)

        output_json = f"output/{file_name}_OUT_{format(testcase, '03')}.json"
        convert_txt_to_json(output_txt, output_json)
    else:
        #! add more file extensions if needed
        print(f"Invalid file extension: {file_extension}")
        sys.exit(1)

    api.submit(testcase, open(f"output/{file_name}_OUT_{format(testcase, '03')}.json").read())


def parse_testcase(testcase):
    testcase.strip()
    testcase = testcase.replace(' ', '')
    testcase = testcase.split(',')
    tcs = []
    for tc in testcase:
        if tc == '0':
            return list(range(1, NUM_TESTCASES + 1))
        elif tc == 'a':
            tcs += list(range(1, 26))
        elif tc == 'b':
            tcs += list(range(26, 51))
        elif '-' in tc:
            begin, end = tc.split('-')
            assert int(begin) <= int(end), f"Invalid range: {begin}-{end}"
            assert 1 <= int(begin), f"Invalid testcase number: {begin}"
            assert int(end) <= NUM_TESTCASES, f"Invalid testcase number: {end}"
            tcs += list(range(int(begin), int(end) + 1))
        else:
            assert 1 <= int(tc) <= NUM_TESTCASES, f"Invalid testcase number: {tc}"
            tcs.append(int(tc))
    return tcs


def main():
    parser = argparse.ArgumentParser(description='Run testcases on solutions')

    parser.add_argument("--testcase", "-tc", type=str, default=0,
                        help=f"Testcase number (1-{NUM_TESTCASES}), 0 for all, begin-end for range, `,` separated for multiple testcases, a for 1-25, b for 26-50")
    parser.add_argument("--solution", "-s", type=str, default=None, help="Path to solution file")
    parser.add_argument("--trials", "-t", type=int, default=1, help="Number of trials to run")

    args = parser.parse_args()

    # assert current directory is root of the project
    if not os.path.exists("api.py"):
        print("please run this script from the root of the project directory")
        sys.exit(1)

    if args.solution is None:
        print("Solution file not provided")
        sys.exit(1)

    compile_solution(args.solution)

    tcs = parse_testcase(args.testcase)

    colors = [bcolors.RED, bcolors.YELLOW, bcolors.GREEN, bcolors.BLUE, bcolors.PURPLE, bcolors.PINK]

    for i, tc in enumerate(tcs):
        print(colors[i % len(colors)], end="")
        for _ in tqdm.tqdm(range(args.trials), desc=f"Testcase {format(tc, '03')}"):
            # print(colors[j%len(colors)], end="", flush=True)
            run_testcase(args.solution, tc)
        # print(bcolors.ENDC, end="")


if __name__ == "__main__":
    main()
