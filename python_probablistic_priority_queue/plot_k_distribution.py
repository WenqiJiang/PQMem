import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
# sns.set_theme(style="whitegrid")

from probablistic_priority_queue import get_k_distribution

plt.style.use('ggplot') 

default_colors = []
for i, color in enumerate(plt.rcParams['axes.prop_cycle']):
    default_colors.append(color["color"])
    print(color["color"], type(color["color"]))


N_L1_queue = 8
topK= 100

p_distribution, p_accumulated_distribution = get_k_distribution(
	N_in_stream=N_L1_queue, topK=topK)

x_tick_labels = np.arange(1, topK + 1)

fig, ax0 = plt.subplots(1, 1, figsize=(8, 2))
ax1 = ax0.twinx()
ax0.bar(x_tick_labels, p_distribution, width=1.0, color=default_colors[0])
ax1.plot(x_tick_labels, p_accumulated_distribution, color=default_colors[1])
# sns.barplot(x=x_tick_labels, y=p_distribution, ax=ax)

ax0.grid(False)
ax1.grid(False)

print("N input streams: {} topK: {}".format(N_L1_queue, topK))

plt.savefig('./img/k_distribution.png', transparent=False, dpi=200, bbox_inches="tight")
plt.show()