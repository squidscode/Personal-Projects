import requests
import re
import sys
import json
from bs4 import BeautifulSoup
import concurrent.futures


URL_wiki = "https://en.wikipedia.org"
URL_start = "https://en.wikipedia.org/wiki/List_of_television_programs:_"
URL_pages = ["numbers", "A", "B", "C", "D", "E", "F", "G", "H", "I-J",
"K-L", "M", "N", "O", "P", "Q-R", "S", "T", "U-V-W", "X-Y-Z"]
invalid_contents = ["Categories", "Lists of television series", "Articles with short description",
"Short description is different from Wikidata", "Add links"]
P_BUDGET = 4
TAG_BUDGET = 20
MAX_WORKERS = 4
target_ids = ["Plot", "Premise", "Synopsis", "Overview", "Episodes", "Main", "Characters"]
word_dictionary = {"word_map": {}, "series_count": {}}
# 100 most common words without "time".
ignore_words = {"a","about","all","also","as","and","at","be","because","but","by","can","come","could","day","do",
"even","find","first","for","from","get","give","go","have","he","her","here","him","his","how","I","if","in","into",
"it","its","just","know","like","look","make","man","many","me","more","my","new","no","not","now","of","on","one",
"only","or","other","our","out","people","say","see","she","so","some","take","tell","than","that","the","their","them",
"then","there","these","they","thing","think","this","those","to","two","up","use","very","want","way","we","well",
"what","when","which","who","will","with","would","year","you","your"}

def recursive_print_content(content):
    ans_string = ''
    for item_of_content in content:
        if isinstance(item_of_content, str):
            ans_string += item_of_content
        else:
            ans_string += recursive_print_content(item_of_content)
    return ans_string

def href_TV_show_list_link(href):
    return href and re.compile("/wiki/List_of_television_programs:_")

def href_wiki_TV_show_link(href):
    return href and re.compile("/wiki/..").search(href) and not re.compile("List_of").search(href)

def has_title(title):
    return title

def valid_a_tag(tag):
    return len(a.contents) > 0 and isinstance(a.contents[0], str) and invalid_contents.count(a.contents[0]) == 0

def no_class(class_):
    return not class_

def decode_words(series, p):
    para = recursive_print_content(p).split(" ")
    word_count = 0
    for word in para:
        word = re.sub(r"[,.!?(\[.*?\])(\n)(\")]", "", word.lower())
        if (len(word) <= 2 or word in ignore_words): continue
        if word in word_dictionary["word_map"] and word:
            if series in word_dictionary["word_map"][word]:
                word_dictionary["word_map"][word][series] = word_dictionary["word_map"][word][series] + 1
            else:
                word_dictionary["word_map"][word][series] = 1
        else:
            word_dictionary["word_map"][word] = {}
            word_dictionary["word_map"][word][series] = 1
        word_count += 1
    if series in word_dictionary["series_count"]:
        word_dictionary["series_count"][series] = word_dictionary["series_count"][series] + word_count
    else:
        word_dictionary["series_count"][series] = word_count
    

def target_section(series, soup, target_id, tag_budget, p_budget):
    target_span = soup.find("span", id=target_id)
    if target_span == None: 
        return (p_budget, {})
    target_h2 = target_span.parent
    seen_ps = set()
    print("HIT " + target_id + "!", end=" ")
    target_tag = target_h2.find_next_sibling()
    while tag_budget > 0 and p_budget > 0 and target_tag != None and target_tag.name != "h2":
        if target_tag.name == "p":
            decode_words(series, target_tag)
            seen_ps.add(target_tag) # add to seen_ps
            p_budget -= 1
        target_tag = target_tag.find_next_sibling()
        tag_budget -= 1
    return (p_budget, seen_ps)

# Plot -> Premise -> Synopsis -> Overview -> Episodes
def extract_page_content(series, soup):
    p_budget = P_BUDGET
    seen_ps = set()
    for target_id in target_ids:
        if p_budget == 0: break
        (p_budget, found_ps) = target_section(series, soup, target_id, TAG_BUDGET, p_budget)
        seen_ps = seen_ps.union(found_ps)
    if p_budget > 0:
        paras = soup.find_all("p", class_=no_class)
        for para in paras:
            if (p_budget == 0 or para in seen_ps): continue
            decode_words(series, para)
            p_budget -= 1

 
if __name__ == "__main__":
    count = 0
    # with concurrent.futures.ThreadPoolExecutor(max_workers=1) as executor:
    executor = concurrent.futures.ThreadPoolExecutor(max_workers=MAX_WORKERS)
    for URL_suffix in URL_pages:
        # if URL_suffix != "A": 
        #     continue
        try:
            page = requests.get(URL_start + URL_suffix)
        except:
            print(URL_start + URL_suffix + " ERROR!!")
            continue
        soup = BeautifulSoup(page.content, "html.parser")
        a_list = soup.find_all("a", href=href_wiki_TV_show_link, title=has_title)
        for a in a_list:
            # if count >= 10: break
            if valid_a_tag(a):
                print("Opening " + a["href"], end="... ")
                try:
                    subpage = requests.get(URL_wiki + a["href"])
                except:
                    print(URL_wiki + a["href"] + " ERROR!!")
                    continue
                subpage_soup = BeautifulSoup(subpage.content, "html.parser")
                # executor.submit(extract_page_content, recursive_print_content(a), subpage_soup)
                extract_page_content(recursive_print_content(a), subpage_soup)
                print("... DONE")
                count += 1
    executor.shutdown(wait=True, cancel_futures=False)

    print(word_dictionary)
    print("# of TV Shows: " + str(count))
    
    # Writing to "./WIP/Web\ Scraping/Data/television_programs.json"
    with open("./Data/television_programs.json", "w") as outfile:
        json.dump(word_dictionary, outfile)

 