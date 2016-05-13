import os

tmpStr = ""
num_reads = 0

with open("CS599_combo_reads.fasta" , 'r') as file:
    count = 0


    for line in file:
        if " "  not in line:
            tmpStr += line
            count+=1

        #print(str(count))
        if count == 25000:

            with open("combo_reads_2MM_long_and_short"+str(num_reads),'w') as ofile:

                for line in tmpStr.splitlines():
                    ofile.write(line[:49] +"\n")
                    ofile.write(line[49:]+"\n")
                    #print(line[:50], line[50:])

            num_reads +=1
            tmpStr = ""
            count = 0
            break

# print(tmpStr)
# if tmpStr:
#
#     with open("combo_reads_"+str(num_reads),'w') as ofile:
#
#         for line in tmpStr.splitlines():
#             ofile.write(line[:50] +"\n")
#             ofile.write(line[50:]+"\n")
#
#     #print(tmpStr)
