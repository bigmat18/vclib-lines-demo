import subprocess
import psutil
import time
import threading
import csv
import argparse
import os

cpu_usage = 0.0
memory_usage = 0.0
gpu_usage = 0.0
gpu_memory_usage = 0.0

def has_nvidia_gpu():
    try:
        subprocess.run(["nvidia-smi"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
        return True
    except subprocess.CalledProcessError: return False
    except FileNotFoundError: return False

def from_num_to_label(value):
    if(value == 0):   return "CPU"
    elif(value == 1): return "GPU"
    elif(value == 2): return "Instancing"
    elif(value == 3): return "Indirect"
    elif(value == 4): return "Texture"


def monitor_cpu(process_pid):
    global cpu_usage, memory_usage, gpu_usage, gpu_memory_usage
    proc_monitor = psutil.Process(process_pid)

    while True:
        cpu_usage = psutil.cpu_percent(interval=0.1)
        memory_usage = proc_monitor.memory_info().rss / (1024 * 1024)

        if has_nvidia_gpu():
            import GPUtil

            gpus = GPUtil.getGPUs()
            if gpus:
                gpu_usage = gpus[0].load
                gpu_memory_usage = gpus[0].memoryUsed / 1024

        time.sleep(0.1)


startNumPoints = 10
endNumPoints = 1000
framePerSteps = 300
stepSize = 10
objSelected = 0 
updatePerSecond = 10

for el in range(0, 5):

    process = subprocess.Popen(
        ["./build/main", str(startNumPoints), str(endNumPoints), str(framePerSteps), str(stepSize), str(objSelected), str(el), str(updatePerSecond)],
        stderr=subprocess.PIPE,
        text=True,
    )

    cpu_thread = threading.Thread(target=monitor_cpu, args=(process.pid,))
    cpu_thread.daemon = True
    cpu_thread.start()

    csv_filename = "data/bm_out_" + from_num_to_label(el)
    index = 0
    while(os.path.exists(csv_filename)):
        csv_filename += str(index)
    csv_filename += ".csv"

    with open(csv_filename, mode='w', newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow(['Lines num', 'Delta Time', 'CPU usage (%)', 'CPU memory (%)', 'GPU usage (%)', 'GPU memory (%)'])
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