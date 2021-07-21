
import re
import sys

#sample = '"","","","","","Test"'

sample = sys.argv[1]
samplelist = re.split('[,]', sample)

samplelist = ['\"\"' if value == '' else value for value in samplelist]

print (samplelist)
print (sample)