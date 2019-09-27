from bs4 import BeautifulSoup
import requests
page = requests.get("https://pokemondb.net/pokedex/all")

soup = BeautifulSoup(page.content, 'html.parser')

indexes = soup.find_all('td', class_='cell-num cell-fixed')
names = soup.find_all('a', class_='ent-name')
stat_totals = soup.find_all('td', class_='cell-total')

rstats = soup.find_all(lambda rstats: rstats.name == 'td' and rstats['class'] == ['cell-num'] and rstats['class']!=['cell-fixed'])
stats = []

for i in range(0,len(rstats),6):
	group = []
	for t in range(6):
		group.append(rstats[i+t])
	stats.append(group)

for i in range(186):
	print(indexes[i].text + "," + names[i].text + "," + stat_totals[i].text + "," + stats[i][0].text + "," + stats[i][1].text + "," + stats[i][2].text + "," + stats[i][3].text + "," + stats[i][4].text + "," + stats[i][5].text)
