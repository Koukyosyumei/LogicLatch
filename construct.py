import os
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
    parser = argparse.ArgumentParser(description='Script to construct the dataset')
    parser.add_argument('rawdata_dir', type=str)
    parser.add_argument("max_num_source_files", type=100)
    args = parser.parse_args()

    cpp_files, cpp_files_size = list_cpp_files(args.rawdata_dir)
    cpp_files_size, cpp_files = map(list, zip(*sorted(zip(cpp_files_size, cpp_files))))

    for p in cpp_files[-1 * args.max_num_source_files:]:
        pass