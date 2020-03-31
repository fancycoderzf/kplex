import os
import re
import xlwt

input_path = "/home/hushan/Algorithm/k-plex/result/" 
whole_file = []

for file in os.listdir(input_path): 
    path = os.path.join(input_path, file)
    whole_file.append(path)
#whole_file = [os.path.join(input_path, file) for file in os.listdir(input_path)]
whole_file.sort()

workbook = xlwt.Workbook(encoding = 'utf-8')
worksheet = workbook.add_sheet('Result')
worksheet.write(0, 0, 'Algorithm')
worksheet.write(0, 1, 'Name')
worksheet.write(0, 2, 'K')
worksheet.write(0, 3, 'Size')
worksheet.write(0, 4, 'Time')

i = 1
for w in whole_file:
    with open(w,'rb') as f:
        s = str(f.readlines())
        content = re.sub("\[|]|{|}|'| ","",s).strip("\\n")
        result = content.split(",")
        j = 0;
        for x in result:
            data = x.split(":")
            worksheet.write(i, j, data[1])
            j = j+1
    i = i+1

workbook.save('result/kplex.xls')