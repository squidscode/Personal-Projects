import sys
import re
import json
import math
import matplotlib.pyplot as plt
import numpy as np

config = {}

if __name__ == "__main__":
    with open("./config/search_config.json", "r") as read_file:
        config = json.load(read_file)

    word_dictionary = {}
    with open("./data/tvshows(15).json", "r") as read_file:
        word_dictionary = json.load(read_file)

    search = input("Type your search query (separate words by commas or spaces): ")
    series_weight_dictionary = {}
    word_means = {}
    # Adding weights
    for word in search.split(" "):
        word = re.sub(r"[,.!?(\[.*?\])(\n)(\")]", "", word.lower())
        if word not in word_dictionary["word_map"]: continue
        # meaning = dictionary.meaning(word)
        # if "Adverb" in meaning or "Adjective" in meaning or "Verb" in meaning:
        #     continue
        mean = 0
        size = len(word_dictionary["word_map"][word])
        for series in word_dictionary["word_map"][word]:
            # weight = float(word_dictionary["word_map"][word][series]**0.5)/(word_dictionary["series_count"][series])
            weight = float(word_dictionary["word_map"][word][series]**0.5) * (10 / math.pow(size + 3, 0.5))
            mean += weight / size
            if series in series_weight_dictionary:
                series_weight_dictionary[series] = series_weight_dictionary[series] + weight
            else:
                series_weight_dictionary[series] = weight
        word_means[word] = mean
    # Creating and adding negative weights
    for word in word_means:
        for series in series_weight_dictionary:
            if series not in word_dictionary["word_map"][word]:
                weight = -word_means[word]
                series_weight_dictionary[series] = series_weight_dictionary[series] + weight

    # print(word_means)
    list_of_series = sorted(series_weight_dictionary.items(), key=lambda kv: (kv[1], kv[0]))
    list_of_series.reverse()
    
    data_x = []
    data_y = []
    count = 0
    for i in range(1, len(list_of_series) + 1, 1):
        if count >= 20: break
        print(f"{i}. {list_of_series[i - 1][0]}, weight: {list_of_series[i - 1][1]}, count: {word_dictionary['series_count'][list_of_series[i - 1][0]]}.")
        data_x.append(list_of_series[i - 1][0])
        data_y.append(list_of_series[i - 1][1])
        count += 1

    # plt.tick_params("x", rotation=90)
    # plt.subplots_adjust(bottom=0.465)

    # plt.bar(range(len(data_x)), data_y, align="center")
    # plt.xticks(range(len(data_x)), data_x)
    # plt.show()
