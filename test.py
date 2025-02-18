import subprocess
import psutil
import time
import threading
import GPUtil

cpu_usage = 0.0
memory_usage = 0.0
gpu_usage = 0.0
gpu_memory_usage = 0.0

def monitor_cpu(process_pid):
    global cpu_usage, memory_usage, gpu_usage, gpu_memory_usage
    proc_monitor = psutil.Process(process_pid)

    while True:
        cpu_usage = psutil.cpu_percent(interval=0.1)  # Calcola l'utilizzo della CPU
        memory_usage = proc_monitor.memory_info().rss / (1024 * 1024)  # Memoria in MB

        gpus = GPUtil.getGPUs()
        if gpus:
            gpu_usage = gpus[0].load  # Percentuale di utilizzo della GPU
            gpu_memory_usage = gpus[0].memoryUsed / 1024  # Memoria GPU in MB

        time.sleep(0.1)

process = subprocess.Popen(
    ["./build/main"],
    stderr=subprocess.PIPE,
    text=True,
)

cpu_thread = threading.Thread(target=monitor_cpu, args=(process.pid,))
cpu_thread.daemon = True
cpu_thread.start()

while True:
    stderr_line = process.stderr.readline()
    if not stderr_line:
        break
    
    print(f"CPU Usage: {cpu_usage:.2f}% | Memory Usage: {memory_usage:.2f} MB | GPU Usage: {gpu_usage:.2f}% | GPU Memory: {gpu_memory_usage:.2f} MB | STDERR: {stderr_line.strip()}")
    
process.wait()