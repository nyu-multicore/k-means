import subprocess
from pathlib import Path
from argparse import ArgumentParser
import pandas as pd
import glob
import re

dataset_names = ['dataset-10000', 'dataset-100000', 'dataset-1000000','dataset-5000000','dataset-10000000']
thread_counts = [1, 2, 5, 10, 50, 100]
thread_counts_disable_for_executables = ["kmeans_seq"]
sample_times = 10
n_clusters = [10]

script_path = Path(__file__)
script_dir = script_path.parent
project_dir = script_dir.parent
data_dir = project_dir / 'data'
release_dir = project_dir / 'cmake-build-release'
raw_output_path = data_dir / 'experiment_raw.csv'
avg_output_path = data_dir / 'experiment_avg.csv'

nmi_regex = re.compile(r'NMI: ([0-9Ee.+-]+)', re.MULTILINE)
time_regex = re.compile(r'Algorithm finished in: ([0-9Ee.+-]+) sec', re.MULTILINE)


def process_output(output):
    nmi_match = re.search(nmi_regex, output)
    time_match = re.search(time_regex, output)
    if not nmi_match or not time_match:
        print("Error: Could not find nmi and time in output:")
        print(output)
        exit(1)
    return float(nmi_match.group(1)), float(time_match.group(1))


def main():
    parser = ArgumentParser(description='Automatically run parallel experiments and collect results.')
    parser.add_argument("-e", "--exe", help="Name of executable to be run, if not set will run all executables found",
                        required=False)
    args = parser.parse_args()

    executables = glob.glob((release_dir / "kmeans*").__str__())

    executables = [Path(x) for x in executables]

    # filter out non-files
    executables = [x for x in executables if x.is_file()]
    # filter out files that are not executable
    executables = [x for x in executables if x.stat().st_mode & 0o111]
    if args.exe:
        executables = [x for x in executables if x.name == args.exe]
        if len(executables) == 0:
            print(f"No executable found with name \"{args.exe}\" found")
            exit(1)

    executables.sort()

    print("Executables: ", end="")
    print([x.name for x in executables])

    print("Datasets: ", end="")
    print(dataset_names)

    print("Thread counts: ", end="")
    print(thread_counts)

    print("N-Clusters: ", end="")
    print(n_clusters)

    raw_out_exes = []
    raw_out_datasets = []
    raw_out_n_clusters = []
    raw_out_threads = []
    raw_out_sample = []
    raw_out_nmi = []
    raw_out_time = []

    avg_out_exec = []
    avg_out_datasets = []
    avg_out_n_clusters = []
    avg_out_threads = []
    avg_out_nmi = []
    avg_out_time = []

    for exe in executables:
        for dataset_name in dataset_names:
            dataset_path = data_dir / f"{dataset_name}.txt"
            for n_cluster in n_clusters:
                label_path = data_dir / f"{dataset_name}.{n_cluster}.kmeans.txt"
                if exe.name in thread_counts_disable_for_executables:
                    thread_counts_pre = [1]
                else:
                    thread_counts_pre = thread_counts
                for thread_count in thread_counts_pre:
                    print(
                        f"Running {exe.name} on {dataset_name} with {thread_count} threads and {n_cluster} clusters...")
                    time_sum = 0
                    nmi_sum = 0
                    for sample_time in range(sample_times):
                        print(f"Sample #{sample_time}:", end="")
                        command = [exe.__str__(), str(thread_count), str(n_cluster), dataset_path.__str__(),
                                   label_path.__str__()]
                        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                        out, err = process.communicate()
                        if err:
                            print(err.decode("utf-8"))
                            exit(1)
                        output = out.decode("utf-8")
                        nmi, time = process_output(output)
                        print(f"NMI: {nmi},  Time: {time}")
                        nmi_sum += nmi
                        time_sum += time
                        raw_out_exes.append(exe.name)
                        raw_out_datasets.append(dataset_name)
                        raw_out_n_clusters.append(n_cluster)
                        raw_out_threads.append(thread_count)
                        raw_out_sample.append(sample_time)
                        raw_out_nmi.append(nmi)
                        raw_out_time.append(time)
                    nmi_avg = nmi_sum / sample_times
                    time_avg = time_sum / sample_times
                    print(f"Average NMI: {nmi_avg}, average time: {time_avg}")
                    avg_out_exec.append(exe.name)
                    avg_out_datasets.append(dataset_name)
                    avg_out_n_clusters.append(n_cluster)
                    avg_out_threads.append(thread_count)
                    avg_out_nmi.append(nmi_avg)
                    avg_out_time.append(time_avg)

    raw_df = pd.DataFrame(
        {"exec": raw_out_exes, "dataset": raw_out_datasets, "n_cluster": raw_out_n_clusters, "threads": raw_out_threads,
         "sample": raw_out_sample, "nmi": raw_out_nmi, "time": raw_out_time}
    )
    raw_df.to_csv(raw_output_path, index=False)

    avg_df = pd.DataFrame(
        {"exec": avg_out_exec, "dataset": avg_out_datasets, "n_cluster": avg_out_n_clusters, "threads": avg_out_threads,
         "nmi": avg_out_nmi, "time": avg_out_time}
    )
    avg_df.to_csv(avg_output_path, index=False)


if __name__ == '__main__':
    main()
