import pandas as pd
import os
import subprocess
import glob
import argparse
from concurrent.futures import ProcessPoolExecutor


def list_cpp_files(start_dir):
    cpp_files = []
    cpp_files_size = []
    for root, _, files in os.walk(start_dir):
        for file in files:
            # if root[-1] in ["D", "E", "F"] and file.endswith(".cpp"):
            if file.endswith(".cpp"):
                cpp_files.append(os.path.join(root, file))
                cpp_files_size.append(os.path.getsize(os.path.join(root, file)))
    return cpp_files, cpp_files_size


def process_file(p, timeout, output_root_dir, default_dir, quiet_stdout, quiet_stderr):
    stdout, stderr = None, None
    if quiet_stdout:
        stdout = subprocess.DEVNULL
    if quiet_stderr:
        stderr = subprocess.DEVNULL

    subprocess.run(
        ["sh", "extractor.sh", p, output_root_dir],
        capture_output=False,
        text=True,
        stdout=stdout,
        stderr=stderr,
    )
    subprocess.run(
        ["sh", "instrument.sh", p, output_root_dir],
        capture_output=False,
        text=True,
        stdout=stdout,
        stderr=stderr,
    )

    file_basename = p.split("/")[-1].split(".")[0]
    exe_files = glob.glob(
        os.path.join(os.path.join(output_root_dir, file_basename), "*.out")
    )
    for e in exe_files:
        subprocess.run(
            [
                "afl-fuzz",
                "-i",
                default_dir,
                "-V",
                str(timeout),
                "-o",
                e + "_result/",
                e,
            ],
            stdout=stdout,
            stderr=stderr,
            capture_output=False,
            text=True,
        )

    data = {"source": [], "output": []}
    for e in exe_files:
        try:
            o = os.path.join(e + "_result/", "default/fuzzer_stats")
            data["source"].append(p)
            data["output"].append(o)
            with open(o, "r") as file:
                for line in file:
                    key, value = line.split(":", 1)
                    key = key.strip()
                    value = value.strip()
                    if key not in data:
                        data[key] = [value]
                    else:
                        data[key].append(value)
        except:
            pass

    pd.DataFrame.from_dict(data).to_csv(
        os.path.join(os.path.join(output_root_dir, file_basename), "stats.csv"),
        index=False,
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Script to construct the dataset")
    parser.add_argument("--rawdata_dir", type=str, default="data")
    parser.add_argument("--max_num_source_files", type=int, default=1)
    parser.add_argument("--timeout", type=int, default=30)
    parser.add_argument("--output_root_dir", type=str, default="result")
    parser.add_argument(
        "--default_testcase",
        type=str,
        default="../afl-tutorial/afl-2.52b/testcases/others/text/",
    )
    parser.add_argument("--quiet_stdout", action="store_true")
    parser.add_argument("--quiet_stderr", action="store_true")
    args = parser.parse_args()

    cpp_files, cpp_files_size = list_cpp_files(args.rawdata_dir)
    cpp_files_size, cpp_files = map(list, zip(*sorted(zip(cpp_files_size, cpp_files))))

    files_to_process = cpp_files[-1 * args.max_num_source_files :]

    """
    process_file(files_to_process[-1],
                args.timeout,
                args.output_root_dir,
                args.default_testcase,
                args.quiet_stdout, 
                args.quiet_stderr)
    """

    with ProcessPoolExecutor() as executor:
        futures = [
            executor.submit(
                process_file,
                p,
                args.timeout,
                args.output_root_dir,
                args.default_testcase,
                args.quiet_stdout,
                args.quiet_stderr,
            )
            for p in files_to_process
        ]
        for future in futures:
            result = future.result()
