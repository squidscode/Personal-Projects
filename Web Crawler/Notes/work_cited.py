import requests
from bs4 import BeautifulSoup

URL = "https://en.wikipedia.org/wiki/Ottawa"
page = requests.get(URL)

soup = BeautifulSoup(page.content, "html.parser")

print("Works Cited")
print("-----------\n")

refs = soup.find_all(class_="refbegin")

def recursive_print_content(content):
    ans_string = ''
    for item_of_content in content:
        if isinstance(item_of_content, str):
            ans_string += item_of_content
        else:
            ans_string += recursive_print_content(item_of_content)
    return ans_string

for ref in refs:
    ul = ref.find("ul")
    for li in ul:
        cite = li.find("cite")
        if cite != -1:
            print(recursive_print_content(cite))
            print()
