# -*- coding: utf-8 -*-
# 根据 benchmark_results.md 的数据绘制 运行时间 vs 输入规模 对比图
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import numpy as np

plt.rcParams["font.sans-serif"] = ["Microsoft YaHei"]  # Windows 中文字体
plt.rcParams["axes.unicode_minus"] = False

# 数据来自 benchmark_results.md（1000 对查询，seed=42，单次运行）
datasets = ["NY", "BAY", "COL", "FLA", "CAL"]
n = np.array([264346, 321270, 435666, 1070376, 1890815])       # 顶点数
m = np.array([733846, 800172, 1057066, 2712798, 4657742])      # 弧数
t_binom = np.array([49065.7, 42003.1, 85496.5, 225262.0, 742975.4]) / 1000.0  # 总耗时 s
t_fib   = np.array([36947.4, 36522.4, 78106.8, 201912.1, 667803.4]) / 1000.0
a_binom = np.array([49.066, 42.003, 85.496, 225.262, 742.975])  # 平均 ms/对
a_fib   = np.array([36.947, 36.522, 78.107, 201.912, 667.803])
speedup = (t_binom - t_fib) / t_binom * 100

C1, C2 = "#4C72B0", "#DD8452"

# ---- 图1：总耗时对比（分组柱状图） ----
fig, ax = plt.subplots(figsize=(8.5, 5.2), dpi=300)
x = np.arange(len(datasets))
w = 0.36
b1 = ax.bar(x - w/2, t_binom, w, label="二项堆 (binomial heap)", color=C1)
b2 = ax.bar(x + w/2, t_fib,   w, label="斐波那契堆 (Fibonacci heap)", color=C2)
ax.bar_label(b1, fmt="%.1f", fontsize=8.5, padding=2)
ax.bar_label(b2, fmt="%.1f", fontsize=8.5, padding=2)
for i in range(len(datasets)):
    top = max(t_binom[i], t_fib[i])
    ax.annotate(f"提速 {speedup[i]:.1f}%", (i, top),
                textcoords="offset points", xytext=(0, 20),
                ha="center", fontsize=10, color="#B2182B", fontweight="bold")
ax.set_xticks(x)
ax.set_xticklabels([f"{d}\nn={n[i]:,}\nm={m[i]:,}" for i, d in enumerate(datasets)], fontsize=8.5)
ax.set_xlabel("数据集（按规模递增）")
ax.set_ylabel("1000 对查询总耗时 (s)")
ax.set_title("图1  Dijkstra 总耗时对比：二项堆 vs 斐波那契堆（1000 对查询）")
ax.set_ylim(0, max(t_binom) * 1.18)
ax.legend(loc="upper left")
ax.grid(axis="y", alpha=0.3)
fig.tight_layout()
fig.savefig(r"C:\Users\lenovo\Desktop\ads\plots\fig1_total_time.png")
plt.close(fig)

# ---- 图2：平均单对查询耗时 vs 输入规模（折线，双对数） ----
fig, ax = plt.subplots(figsize=(8.5, 5.2), dpi=300)
ax.plot(n, a_binom, "o-", color=C1, label="二项堆 (binomial heap)")
ax.plot(n, a_fib,   "s-", color=C2, label="斐波那契堆 (Fibonacci heap)")
for i, d in enumerate(datasets):
    ax.annotate(d, (n[i], a_binom[i]), textcoords="offset points", xytext=(0, 10),
                ha="center", fontsize=10)
ax.set_xscale("log")
ax.set_yscale("log")
ax.set_xticks(n)
ax.xaxis.set_major_formatter(mticker.FuncFormatter(lambda v, _: f"{int(v):,}"))
ax.xaxis.set_minor_formatter(mticker.NullFormatter())   # 关掉次要刻度标签，避免重叠
ax.minorticks_off()
ax.set_xlabel("输入规模：顶点数 n")
ax.set_ylabel("平均每对查询耗时 (ms)（log）")
ax.set_title("图2  平均单对查询耗时 vs 输入规模（双对数坐标）")
ax.set_ylim(20, 1200)
ax.legend(loc="upper left")
ax.grid(which="major", alpha=0.35)
fig.tight_layout()
fig.savefig(r"C:\Users\lenovo\Desktop\ads\plots\fig2_avg_time_vs_size.png")
plt.close(fig)

print("done:", speedup.round(1))
