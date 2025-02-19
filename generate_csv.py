import subprocess
import psutil
import time
import threading
import csv
import argparse

cpu_usage = 0.0
memory_usage = 0.0
gpu_usage = 0.0
gpu_memory_usage = 0.0

def has_nvidia_gpu():
    try:
        subprocess.run(["nvidia-smi"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
        return True
    except subprocess.CalledProcessError:
        return False
    except FileNotFoundError:
        return False


def monitor_cpu(process_pid):
    global cpu_usage, memory_usage, gpu_usage, gpu_memory_usage
    proc_monitor = psutil.Process(process_pid)

    while True:
        cpu_usage = psutil.cpu_percent(interval=0.1)  # Calcola l'utilizzo della CPU
        memory_usage = proc_monitor.memory_info().rss / (1024 * 1024)  # Memoria in MB

        if has_nvidia_gpu():
            import GPUtil

            gpus = GPUtil.getGPUs()
            if gpus:
                gpu_usage = gpus[0].load  # Percentuale di utilizzo della GPU
                gpu_memory_usage = gpus[0].memoryUsed / 1024  # Memoria GPU in MB

        time.sleep(0.1)


for el in range(0, 5):

    process = subprocess.Popen(
        ["./build/main", "10000", "1000000", "300", "10000", "0", str(el)],
        stderr=subprocess.PIPE,
        text=True,
    )

    cpu_thread = threading.Thread(target=monitor_cpu, args=(process.pid,))
    cpu_thread.daemon = True
    cpu_thread.start()

    csv_filename = "benchmark_output" + str(el) + ".csv"
    with open(csv_filename, mode='w', newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow(['numLines', 'deltaTime', 'cpu_usage', 'cpu_memory', 'gpu_usage', 'gpu_memory'])
        index = 0
        while True:
            index += 1
            stderr_line = process.stderr.readline()
            if not stderr_line:
                break
            
            stderr_line = stderr_line[:-1].split(",")
            try:
                csv_writer.writerow([int(stderr_line[0]), float(stderr_line[1]), cpu_usage, memory_usage, gpu_usage, gpu_memory_usage])
            except:
                pass
            if index % 20:
                csv_file.flush()

    process.wait()