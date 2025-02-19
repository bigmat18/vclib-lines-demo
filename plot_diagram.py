# import pandas as pd
# import matplotlib.pyplot as plt

# data = pd.read_csv("benchmark_output.csv", index_col=0)
# data["deltaTime"].plot()
# plt.show()

import subprocess

def get_gpu_utilization():
    try:
        result = subprocess.run(
            ["sudo", "powermetrics", "--samplers", "gpu_power", "--show-usage", "--duration", "1"],
            capture_output=True, text=True
        )
        return result.stdout
    except Exception as e:
        return str(e)

print(get_gpu_utilization())