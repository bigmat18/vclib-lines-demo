import pandas as pd
import matplotlib.pyplot as plt
import random, math
import numpy as np
from matplotlib.ticker import MaxNLocator

def remove_outliers(df, column, multiplier=1.5):
    # Calcola il primo e terzo quartile (Q1, Q3) e l'IQR
    Q1 = df[column].quantile(0.1)
    Q3 = df[column].quantile(0.99)
    IQR = Q3 - Q1

    # Calcola i limiti inferiore e superiore
    lower_bound = Q1 - multiplier * IQR
    upper_bound = Q3 + multiplier * IQR

    # Filtra i dati escludendo gli outliers
    return df[(df[column] >= lower_bound) & (df[column] <= upper_bound)]

index = ""
file_list = ["bm_out_CPU", "bm_out_GPU", "bm_out_Instancing", "bm_out_Indirect", "bm_out_Texture"]
file_list = ["static/" + el + index + ".csv" for el in file_list]

labels = ["CPU generated", "GPU generated", "Instancing based", "Indirect based", "Texture based"]
columns_to_plot = ["Delta Time"] #"Delta Time", "CPU usage (%)", "CPU memory", "GPU usage (%)", "GPU memory"
colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd"]

num_plots = 2
fig, axes = plt.subplots(len(columns_to_plot), 2 if num_plots == 3 else 1, figsize=(6, len(columns_to_plot) * 4))

index_ref = 0
col_ref = "Lines num"
dfref = pd.read_csv("data/" + file_list[index_ref])[:-1]
dfref = dfref.groupby(col_ref).mean()

for j, col in enumerate(columns_to_plot):
    if(len(columns_to_plot) != 1):
        axes_to_use = axes[j]
    else:
        axes_to_use = axes

    if num_plots == 1:
        ax1 = axes_to_use
    elif num_plots == 2:
        ax2 = axes_to_use
    elif num_plots == 3:
        ax1, ax2 = axes_to_use

    for i,f in enumerate(file_list):
        df = pd.read_csv("data/" + f)[:-1]

        df = df.groupby(col_ref).mean()

        xx = df.index
        yy = np.array(df[col])

        yyRef = list(dfref[col])
        if len(yyRef) < len(yy):
            yyRef = np.append(yyRef, [yyRef[-1]] * (len(yy) - len(yyRef)))
        elif len(yyRef) > len(yy):
            yyRef = yyRef[:len(yy)]

        yyRef = np.array(yyRef)
        yy_diff_percent = ((yy - yyRef) / yyRef) * 100

        if num_plots == 1 or num_plots == 3:
            if i == index_ref:
                ax1.plot(xx, np.zeros_like(yy_diff_percent), '--', color=colors[i], label=f"Riferimento: {labels[i]}")
            else:
                ax1.plot(xx, yy_diff_percent, label=labels[i], color=colors[i])

        if num_plots == 2 or num_plots == 3:
            yy *= 1000
            ax2.plot(xx, yy, label=labels[i], color=colors[i])

    if num_plots == 1 or num_plots == 3:
        ax1.set_title("Differenza rispetto al riferimento")
        ax1.set_xlabel(col_ref)
        ax1.set_ylabel("Diff. " + col + " (%) rispetto " + labels[index_ref])
        if(col == "Delta Time"):
            # ax1.set_ylim(-15, 5)
            ax1.yaxis.set_major_locator(MaxNLocator(nbins=15))
        ax1.legend()
        ax1.grid(True, which='both', color='gray', linestyle='-', linewidth=0.5, alpha=0.3) 

    if num_plots == 2 or num_plots == 3:
        ax2.set_title("Valori assoluti di " + col)
        ax2.set_xlabel(col_ref)
        ax2.set_ylabel(col + " (ms)")
        ax2.set_yscale('log')
        # ax2.set_xlim(0, 200000)
        # ax2.set_ylim(0, 10)
        ax2.yaxis.set_major_locator(MaxNLocator(nbins=15))
        ax2.legend()
        ax2.grid(True, which='both', color='gray', linestyle='-', linewidth=0.5, alpha=0.3) 

plt.savefig("static_log.pdf", format="pdf", dpi=1200, bbox_inches="tight")
plt.gcf().canvas.manager.set_window_title('Grafici Benchmark')
plt.tight_layout()
plt.show()