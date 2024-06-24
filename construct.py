import os

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
    cpp_files, cpp_files_size = list_cpp_files("data")
    cpp_files_size, cpp_files = map(list, zip(*sorted(zip(cpp_files_size, cpp_files))))
    print(cpp_files[-10:])
    print(cpp_files_size[-10:])