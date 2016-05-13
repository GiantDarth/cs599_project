import random as rand

charSet = {'A','T','C','G'}


for i in range(5):
    comboReadError = open("combo_reads_errorRate_5_"+str(i), "w")
    with open("combo_reads_"+str(i),'r') as cfile:

        tmpString = ""
        for line in cfile:
            # print(line)
            for char in line:
                tmpRand = rand.random()

                if tmpRand <= 0.05 and char != '\n':
                    # print("original char" ,char)
                    rchar = set(char)
                    tmpset = rchar^charSet
                    tmpReplChar = tmpset.pop()
                    # print("replacement char ", tmpReplChar)
                    tmpString += tmpReplChar
                else:
                    tmpString += char
                    #print(tmpString)


    comboReadError.write(tmpString)

    comboReadError.close()

