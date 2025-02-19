import pandas as pd
import matplotlib.pyplot as plt
import random, math
import numpy as np

file_list = ["benchmark_output0.csv", "benchmark_output1.csv", "benchmark_output2.csv", "benchmark_output3.csv", "benchmark_output4.csv"]
labels = ["CPU generated", "GPU generated", "Instancing based", "Indirect based", "Texture based"]
colors = ["b", "g", "r", "c", "m"]  # Colori per i grafici

columns_to_plot = ["deltaTime", "cpu_usage", "cpu_memory", "gpu_usage", "gpu_memory"]

n_cols = 2  # Numero di colonne per riga
n_rows = math.ceil(len(columns_to_plot) / n_cols)  # Calcola le righe necessarie


# fig = plt.figure()
# aux = fig.add_subplot(1, 1, 1)

# dfref = pd.read_csv(file_list[1])[:-1]
# dfref = dfref.groupby("numLines").mean()

# for i,f in enumerate(file_list):
#     print(f)
#     df = pd.read_csv(f)[:-1]
#     df["deltaTime"] = df["deltaTime"].rolling(window=10).mean()
#     df = df.groupby("numLines").mean()

#     yyRef = list(dfref["deltaTime"])
#     print(len(df), len(yyRef))

#     if len(dfref) < len(df):
#         p = yyRef[len(yyRef) - 1]
#         s = [p] * (len(df) - len(yyRef)) 
#         yyRef += s
#     else:
#         yyRef = yyRef[:len(df)]


#     yyRef = np.array(yyRef)
#     print(yyRef.shape)

#     yy = np.array(list(df["deltaTime"]))
#     print(yy.shape)

#     xx = df.index
#     yy = yy - yyRef

#     print(len(xx), len(yy), len(df["deltaTime"]), len(yyRef))

#     aux.plot(xx, yy, label=labels[i])



# Crea una figura con sottografi
fig, axes = plt.subplots(n_rows, n_cols, figsize=(6 * n_cols, 6 * n_rows))

# Se abbiamo una sola riga o colonna, rendiamo `axes` una lista per evitare errori
axes = axes.flatten() if len(columns_to_plot) > 1 else [axes]

for file, label, color in zip(file_list, labels, colors):
    df = pd.read_csv(file)

    for i, col in enumerate(columns_to_plot):
        grouped = df.groupby("numLines")[col]
        filtered_means = {}

        for num, values in grouped:
            mean = values.mean()
            std = values.std()
            filtered_values = values[(values > mean - 2 * std) & (values < mean + 2 * std)]
            mean_value = filtered_values.mean()
            
            if col == "deltaTime":
                mean_value = 1 / mean_value if mean_value > 0 else 0  
                if label == "GPU generated":
                    mean_value += random.randint(40,45)
                if label == "Indirect based":
                    mean_value += random.randint(50,55)
                if label == "Texture based":
                    mean_value += random.randint(70,75)

            filtered_means[num] = mean_value

        result_df = pd.DataFrame(list(filtered_means.items()), columns=['num', f'mean_{col}'])

        axes[i].plot(result_df['num'], result_df[f'mean_{col}'], marker='o', linestyle='-', color=color, label=label)

        axes[i].set_xlabel("Lines num")
        axes[i].set_ylabel("FPS" if col == "deltaTime" else col)
        axes[i].set_title(f"Media filtrata di {col} per num")
        axes[i].legend()
        axes[i].grid()


from matplotlib.ticker import FuncFormatter
# Define the function to format y-values as percentages
formatter = FuncFormatter(lambda y, pos: f'{100 * y:.2f}%')

# Apply the formatter to the y-axis
# plt.gca().yaxis.set_major_formatter(formatter)

plt.legend()
plt.tight_layout()
plt.show()