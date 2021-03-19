data = [1.6, 3.4, 5.5, 9.4]

f = open("myfile.txt", "x")


for value in data:
    record = str(value)
    f.write(record)
    f.write("\n")

f.close()