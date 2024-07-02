import sys
import pandas as pd
df = pd.read_csv(sys.argv[1], sep=';', quotechar='"')
dic_ids = {}
for e in df['id']:
	if(e not in dic_ids):
		dic_ids[e] = 1
	else:
		dic_ids[e] += 1
ids = sorted(dic_ids)
print("id\tcant")
for i in ids:
	print("{}\t{}".format(i,dic_ids[i]))
