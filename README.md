# Personal-Projects
---
## Description
---
This respository is a collection of finished and work-in-progress projects. The projects are not language specific, and encompass a variety of different topics in computer science.
## Project: Web Crawler
---
### Module: web_crawler
Creates a dataset of mappings from words to keywords and the number of occurances of that word (ie. {"mario": {"Mario Kart": 3, "Super Mario Bros.": 4, ...}}) and a mapping of keywords to the number of words scrapped. 

The web_crawler can be dynamically configured using the "crawler_config.json" file ("src/config/crawler_config"). Examples of configurations can be found in the config file. Paragraphs can be targeted and links to pages can be disabled by specifying these configurations in the config file. 

```json
{"URL_wiki": "https://en.wikipedia.org", "URL_start": "https://en.wikipedia.org/wiki/List_of_television_programs:_", "URL_pages": ["numbers", "A", "B", "C", "D", "E", "F", "G", "H", "I-J", "K-L", "M", "N", "O", "P", "Q-R", "S", "T", "U-V-W", "X-Y-Z"], "invalid_contents": ["Categories", "Lists of television series", "Articles with short description", "Short description is different from Wikidata", "Add links"], "P_BUDGET": 15, "TAG_BUDGET": 20, "MAX_WORKERS": 4, "target_ids": ["Plot", "Premise", "Synopsis", "Overview", "Episodes", "Main", "Characters"], "ignore_words": ["a", "about", "all", "also", "as", "and", "at", "be", "because", "but", "by", "can", "come", "could", "day", "do", "even", "find", "first", "for", "from", "get", "give", "go", "have", "he", "her", "here", "him", "his", "how", "I", "if", "in", "into", "it", "its", "just", "know", "like", "look", "make", "man", "many", "me", "more", "my", "new", "no", "not", "now", "of", "on", "one", "only", "or", "other", "our", "out", "people", "say", "see", "she", "so", "some", "take", "tell", "than", "that", "the", "their", "them", "then", "there", "these", "they", "thing", "think", "this", "those", "to", "two", "up", "use", "very", "want", "way", "we", "well", "what", "when", "which", "who", "will", "with", "would", "year", "you", "your"], "output_file": "./data/tv_show_data.json"}
```