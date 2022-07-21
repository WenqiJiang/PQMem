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

topK= 100
L1_length_list = []
input_stream_min = 2
inupt_stream_max = 64

for i in range(input_stream_min, inupt_stream_max + 1):
    N_L1_queue = i
    p_distribution, p_accumulated_distribution = get_k_distribution(
        N_in_stream=N_L1_queue, topK=topK)
    for k in range(1, topK + 1):
        if p_accumulated_distribution[k] > 0.9999:
            L1_length_list.append(k)
            break

print(L1_length_list)
print("Ensures 99.99 percent per queue same view")

x_tick_labels = np.arange(input_stream_min, inupt_stream_max + 1)

fig, ax0 = plt.subplots(1, 1, figsize=(8, 1.5))
ax0.plot(x_tick_labels, L1_length_list, color=default_colors[1])
# sns.barplot(x=x_tick_labels, y=p_distribution, ax=ax)

#ax0.grid(False)

plt.savefig('./img/queue_size_vs_input_stream.png', transparent=False, dpi=200, bbox_inches="tight")
plt.show()