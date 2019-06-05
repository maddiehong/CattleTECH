#Python script to convert readings from test into csv file

import csv
def main():
    #open Feather m0 readings
    #NAME YOUR FILE HERE
    testResults = open("FILENAME.txt", "r")

    #Print just the transmission data
    #for testLine in testResults:
    #    if ("Transmission" in testLine):
    #        print(testLine)
    #    elif ("RSSI:" in testLine):
    #        print(testLine)

    #Create a clean csv
    #NAME YOUR NEW FILE HERE
    with open('NEWFILE.csv', 'w') as csvFile:
        filewriter = csv.writer(csvFile,delimiter=',',quoting=csv.QUOTE_ALL)
        filewriter.writerow(["Transmission #", "RSSI", "Transmit Time"])
        row = ["none", "none", "none"]
        prevTimer = 0
        for testLine in testResults:
            if ("Transmission" in testLine):
                splitItems = testLine.split(': ')
                row[0] = float(splitItems[-1][:-1])
            elif ("RSSI:" in testLine):
                splitItems = testLine.split(': ')
                row[1] = float(splitItems[-1][:-1])
            elif ("Timer:" in testLine):
                splitItems = testLine.split(': ')
                currTimer = float(splitItems[-1][:-2])
                row[2] = (currTimer - prevTimer)
                filewriter.writerow(row)
                #Reset row and Timer
                prevTimer = currTimer
                row = ["none", "none", "none"]
        print("New CSV File Created")
    testResults.close()
main()

#TESTS

#Test to print splitLine
#testLine = testResults.readlines()[49]
#lineItems = testLine.split()
#print("full line")
#print testLine
#print("words in line 50 ")
#for item in lineItems:
#    print(item)

#Test to print individual values
#    prevTimer = 0
#    row = ["none", "none","none"]
#    for testLine in testResults:
#        if ("Transmission" in testLine):
#            splitItems = testLine.split(': ')
#            print(splitItems)
#            row[0] = float(splitItems[-1][:-1])
#        elif ("RSSI:" in testLine):
#            splitItems = testLine.split(': ')
#            print(splitItems)
#            row[1] = float(splitItems[-1][:-1])
#        elif ("Timer:" in testLine):
#            splitItems = testLine.split(': ')
#            print(splitItems)
#            currTimer = float(splitItems[-1][:-2])
#            row[2] = (currTimer - prevTimer)
#            prevTimer = currTimer
#            print(row)
#            row = ["none", "none", "none"]
