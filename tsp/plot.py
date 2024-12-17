import matplotlib.pyplot as plt

# Data: Time cases, Time algos, Length algos
time_cases = [[0.0, 0.0009946823120117188, 0.0, 0.0, 0.18243765830993652], [0.002000093460083008, 0.0, 0.0, 0.0, 0.2012476921081543], [0.010731935501098633, 0.0006885528564453125, 0.0005071163177490234, 0.0, 0.20952129364013672], [0.10413336753845215, 0.0044155120849609375, 0.0005414485931396484, 0.0, 0.22581100463867188], [0.9740419387817383, 0.023928403854370117, 0.0009164810180664062, 0.0, 0.24103140830993652], [10.7581045627594, 0.14615583419799805, 0.0022170543670654297, 0.0, 0.05326128005981445], [128.65434622764587, 0.24402546882629395, 0.0053060054779052734, 0.0, 0.06147623062133789], [None, 1.4672844409942627, 0.013678550720214844, 0.0, 0.07088494300842285], [None, 11.585455894470215, 0.031452178955078125, 0.0, 0.0818336009979248], [None, 8.709895372390747, 0.08594083786010742, 0.0, 0.09062767028808594], [None, 39.01952624320984, 0.1794734001159668, 0.0, 0.10034418106079102], [None, 633.083566904068, 0.39189743995666504, 0.0, 0.11131834983825684], [None, None, 0.8726253509521484, 0.0, 0.12484335899353027], [None, None, 1.9339728355407715, 0.0, 0.13399910926818848], [None, None, 4.349163055419922, 0.0, 0.1496438980102539], [None, None, 9.755097389221191, 0.0, 0.16228556632995605], [None, None, 23.681161880493164, 0.0, 0.17757868766784668], [None, None, 52.184656620025635, 0.0, 0.19826245307922363], [None, None, 117.53075122833252, 0.0, 0.21074581146240234], [None, None, None, 0.0005497932434082031, 0.2277524471282959], [None, None, None, 0.0, 0.2367391586303711], [None, None, None, 0.0, 0.33812856674194336], [None, None, None, 0.0, 0.4555027484893799], [None, None, None, 0.00038242340087890625, 0.589613676071167], [None, None, None, 0.0009267330169677734, 0.7455720901489258], [None, None, None, 0.0010056495666503906, 0.906726598739624], [None, None, None, 0.0, 1.3245882987976074], [None, None, None, 0.0010030269622802734, 1.8279197216033936], [None, None, None, 0.0009415149688720703, 2.434755563735962], [None, None, None, 0.0011115074157714844, 3.1155693531036377], [None, None, None, 0.0011279582977294922, 3.933720588684082]]
time_algos = [[0.0, 0.002000093460083008, 0.010731935501098633, 0.10413336753845215, 0.9740419387817383, 10.7581045627594, 128.65434622764587, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], [0.0009946823120117188, 0.0, 0.0006885528564453125, 0.0044155120849609375, 0.023928403854370117, 0.14615583419799805, 0.24402546882629395, 1.4672844409942627, 11.585455894470215, 8.709895372390747, 39.01952624320984, 633.083566904068, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], [0.0, 0.0, 0.0005071163177490234, 0.0005414485931396484, 0.0009164810180664062, 0.0022170543670654297, 0.0053060054779052734, 0.013678550720214844, 0.031452178955078125, 0.08594083786010742, 0.1794734001159668, 0.39189743995666504, 0.8726253509521484, 1.9339728355407715, 4.349163055419922, 9.755097389221191, 23.681161880493164, 52.184656620025635, 117.53075122833252, None, None, None, None, None, None, None, None, None, None, None, None], [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0005497932434082031, 0.0, 0.0, 0.0, 0.00038242340087890625, 0.0009267330169677734, 0.0010056495666503906, 0.0, 0.0010030269622802734, 0.0009415149688720703, 0.0011115074157714844, 0.0011279582977294922], [0.18243765830993652, 0.2012476921081543, 0.20952129364013672, 0.22581100463867188, 0.24103140830993652, 0.05326128005981445, 0.06147623062133789, 0.07088494300842285, 0.0818336009979248, 0.09062767028808594, 0.10034418106079102, 0.11131834983825684, 0.12484335899353027, 0.13399910926818848, 0.1496438980102539, 0.16228556632995605, 0.17757868766784668, 0.19826245307922363, 0.21074581146240234, 0.2277524471282959, 0.2367391586303711, 0.33812856674194336, 0.4555027484893799, 0.589613676071167, 0.7455720901489258, 0.906726598739624, 1.3245882987976074, 1.8279197216033936, 2.434755563735962, 3.1155693531036377, 3.933720588684082]]
length_algos = [[1582.4613345811579, 1444.627274255333, 2380.7544958265967, 2285.9077552117037, 1808.4886583297691, 2062.072766687941, 2503.7368897993365, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], [1582.461334581158, 1444.6272742553333, 2380.754495826597, 2285.907755211704, 1808.4886583297691, 2062.0727666879416, 2503.7368897993365, 2735.831857039737, 2311.532943259378, 2448.8784661130708, 2346.6713295541235, 2447.6745211142616, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], [1582.461334581158, 1444.6272742553333, 2380.7544958265967, 2285.907755211704, 1808.4886583297691, 2062.0727666879416, 2503.7368897993365, 2735.831857039737, 2311.532943259378, 2448.8784661130703, 2346.671329554124, 2447.6745211142616, 2940.693075268449, 2794.218721392321, 2454.588023914843, 2443.666138011168, 2820.102298108523, 3249.8011414328575, 2655.9209911165744, None, None, None, None, None, None, None, None, None, None, None, None], [1582.461334581158, 1463.0851860215098, 2423.3898587851104, 2622.779747641379, 2435.1566221823264, 2386.6934396402476, 2728.702458165603, 3523.33283867265, 2368.689011549295, 2484.4000794895833, 2406.3671596097693, 2736.6605446073168, 3775.520665390783, 3432.910418564477, 2466.1602046930793, 2907.899060014057, 3452.6099633951508, 3956.096421777196, 2722.8103087985846, 3670.039690516195, 4051.119277676303, 4349.601342544569, 4525.171704136049, 4775.172681012701, 4829.85762473364, 5178.900983721793, 5644.295125714173, 5630.150146180161, 6568.028528937022, 6910.695906913219, 7641.7047147797375], [1582.4613345811579, 1444.6272742553333, 2380.7544958265967, 2285.907755211704, 1808.4886583297691, 2062.0727666879416, 2503.7368897993365, 2735.831857039737, 2430.238188035762, 2448.8784661130708, 2363.6726383934915, 2535.053118081561, 2940.6930752684493, 3202.5937594326087, 2454.588023914843, 2551.073347332327, 2982.053262683901, 3672.368380270163, 3252.9564318407733, 3940.9832002690737, 4349.059371930779, 4319.970545931097, 5136.731086646554, 6304.120608695771, 6633.846634059791, 6368.696101392085, 8898.397611738264, 11667.500657264121, 13942.758308114006, 11906.08643249913, 14758.325304790877]]

# Helper function to plot a dataset
def plot_data(data, index, title, xlabel, ylabel, labels, filename, x_labels=None, log_scale=False):
    plt.figure(figsize=(10, 6))
    for i, sequence in enumerate(data):
        # Remove None values from the data
        x_values = [index[j] for j in range(len(sequence)) if sequence[j] is not None]
        y_values = [sequence[j] for j in range(len(sequence)) if sequence[j] is not None]
        plt.plot(x_values, y_values, label=labels[i])
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    if x_labels:  # If x_labels is provided, use it for x-axis ticks
        plt.xticks(index, x_labels)
    if log_scale:  # Apply logarithmic scale to y-axis if specified
        plt.yscale('log')
    plt.grid(True)
    # Place the legend outside the plot
    plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
    plt.tight_layout()
    plt.savefig(filename)
    plt.show()

case_index = [i for i in range(5, 25)] + [25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100]
case_labels = [f"N={i}" for i in case_index]
algo_index = [0, 1, 2, 3, 4]
algo_labels = ["Brute Force", "Backtracking", "Dynamic Programming", "Greedy", "Genetic"]

# Plot results
plot_data(time_cases, algo_index, "Execution Time by Test Case", "Algorithm", "Time (s)", case_labels, "pics/time_cases.png", x_labels=algo_labels)
plot_data(time_algos, case_index, "Execution Time by Algorithm", "Case scale", "Log Time (s)", algo_labels, "pics/time_algos.png", log_scale=True)
plot_data(length_algos, case_index, "Path Length by Algorithm", "Case scale", "Path Length", algo_labels, "pics/length_algos.png")