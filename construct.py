import pandas as pd
import os
import subprocess
import glob
import argparse


def list_cpp_files(start_dir):
    cpp_files = []
    cpp_files_size = []
    for root, _, files in os.walk(start_dir):
        for file in files:
            if file.endswith(".cpp"):
                cpp_files.append(os.path.join(root, file))
                cpp_files_size.append(os.path.getsize(os.path.join(root, file)))
    return cpp_files, cpp_files_size


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Script to construct the dataset")
    parser.add_argument("-rawdata_dir", type=str, default="data")
    parser.add_argument("-max_num_source_files", type=int, default=1)
    parser.add_argument("-timeout", type=int, default=5)
    args = parser.parse_args()

    cpp_files, cpp_files_size = list_cpp_files(args.rawdata_dir)
    cpp_files_size, cpp_files = map(list, zip(*sorted(zip(cpp_files_size, cpp_files))))

    for p in cpp_files[-1 * args.max_num_source_files :]:
        subprocess.run(["sh", "extractor.sh", p], capture_output=False, text=True)
        subprocess.run(["sh", "instrument.sh", p], capture_output=False, text=True)

        exe_files = glob.glob(os.path.join(p.split(".")[0], "*.o"))
        for e in exe_files:
            subprocess.run(
                [
                    "afl-fuzz",
                    "-i",
                    "../afl-tutorial/afl-2.52b/testcases/others/text/",
                    "-V",
                    str(args.timeout),
                    "-Q",
                    "-o",
                    e + "_out/",
                    e,
                ],
                capture_output=False, text=True
            )

        data = {"source":[], "output":[]}
        for e in exe_files:
            o = os.path.join(e + "_out/", "default/fuzzer_stats")
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

        pd.DataFrame.from_dict(data).to_csv(p.split(".")[0] + ".csv", index=False)
