#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
#include <ctime>

#include "json.hpp"
#include "trie.h"

using namespace std;
using json = nlohmann::json;

// Returns time in seconds.
std::vector<std::vector<double>> benchmark_reads(std::string pathname, unsigned short iterCount = 100);
std::vector<std::vector<double>> benchmark_subject(std::string pathname, unsigned short iterCount = 100);

std::vector<std::vector<double>> benchmark_reads(std::string pathname, unsigned short iterCount)
{
    json read_obj;
    std::ifstream file;
    file.open(pathname);
    if(file.is_open())
    {
        while(!file.eof())
        {
            file >> read_obj;
        }
    }
    file.close();

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

    std::vector<std::string> error_rates = { "low", "high" };
    std::chrono::nanoseconds duration;
    std::vector<std::vector<double>> test_runs;

    for(int i = 0; i < error_rates.size(); i++)
    {
        json &rate = read_obj[error_rates[i]];
        test_runs.push_back(std::vector<double>());

        //@todo store results for vizualization
        std::cout << "Testing " << error_rates[i] << " error rate of " << rate["rate"] << std::endl;
        for(json &reads : rate["cases"])
        {
            std::cout << "Running " << reads.size() << " queries..." << std::endl;
            start_time = std::chrono::high_resolution_clock::now();
            for(int iter = 0; iter < iterCount; iter++)
            {
                Trie* trie = new Trie();
                for(json &read : reads)
                {
                    trie->addQuery(read);
                }
                delete trie;
            }
            end_time = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            test_runs[i].push_back(duration.count() / (1000000000.0 * iterCount));
            std::cout << "Time: " << test_runs[i].back() << " s" << std::endl;
        }
    }

    return test_runs;
}

/*benchmark_subject
 * @param iterCount = unsigned short, # of mismatches to exhaustively search
 * @param pathname = path name to json file with chaos format
 *               // 0  x \\
 *              // _ '' _ \\
 *                  TTTT
 */
std::vector<std::vector<double>> benchmark_subject(std::string pathname, unsigned short iterCount, int trieSize, int subejectSearchRatio, bool singleSearch)
{
    json subj_tests;
    std::ifstream file;
    file.open(pathname);
    std::string subjectStr;
    std::string tmpStr;
    std::string header;
    int linecount = 0;
    if(file.is_open())

    {
        std::getline(file,header);
        while (std::getline(file,tmpStr))
        {
            subjectStr += tmpStr;
            linecount++;
        }
    }
    file.close();

    printf("\n line count is %d\n",linecount);

    std::chrono::time_point<std::chrono::system_clock> start_time, end_time;

    //@todo uncomment for low and high error rate benchmarks
    //std::vector<std::string> error_rates = { "low", "high" };
    //temp testing hard coded to test zero error rate
    //and to show everything works
    //@todo remove none
    std::vector<std::string> error_rates = {"none","one","two","three","four" };
    int combo_read_index = 1;

    std::vector<std::vector<double>> test_runs;
    std::vector<double> trieBuildTimes;
    std::vector<double> trieSearchTimes;

    printf("%s\n", error_rates[0].c_str());




       // int size = test["size"];
        for(int i = 0; i < combo_read_index ;i++)
        {
            //json &rate = test[error_rates[i]];
            std::cout << "Testing " << subjectStr.length() << " length subject " << error_rates[i] << ", w/ error rate of "
                << 0.00 << std::endl;



            Trie* trie = new Trie();

            clock_t trieBuildClockStart;
            trieBuildClockStart = clock();
            std::ifstream combo_read;
            combo_read.open("combo_reads_2MM_long_and_short"+to_string(i));
           // combo_read.open("combo_reads_"+to_string(i));

            bool adjustTrieSize = false;
            int trieSizeLimit =1;

            if(trieSize > 0) {
                adjustTrieSize = true;
                trieSizeLimit = trieSize;
            }



            if(combo_read.is_open()){
                std::string tmpLine;
                int count = 1;
                while(std::getline(combo_read,tmpLine)) {

                    trie->addQuery(tmpLine);
                    count++;
                    if(adjustTrieSize && trieSizeLimit % count ==0 ){

                        break;
                    }


                }
            }else
                printf("file could not be opened");

            double trieBuildDuration;
            trieBuildDuration = (clock() - trieBuildClockStart)/ (double) CLOCKS_PER_SEC;

            printf("Trie constructed in %f seconds\n", trieBuildDuration);

            trieBuildTimes.push_back(trieBuildDuration);

            //debug get the size of the trie
            printf("\nSize of the trie is %d\n", trie->getSize());
            clock_t start;
            start = clock();
            //keep track of the results
            std::vector<Trie::map> results;

            if(subejectSearchRatio <= 0){

                subejectSearchRatio = (int) subjectStr.length();
            }


            for(int i = 0; i < (int)subejectSearchRatio;i++){
                std::string tempSubStr =  subjectStr.substr((unsigned long long) i, 50);

                //debug
                //printf("%s\n", tempSubStr.c_str());
                //printf("\n%s %s\n", "trying a search", tempSubStr.c_str());
                if(!singleSearch)
                    //search for string get the results
                     results  = trie->searchTrieStack(tempSubStr , iterCount);
                else
                    trie->searchTrie(tempSubStr);
//debug
//
                //store index of mismatches

                int resultCount = 0;
                int numOfResults =0;

                //go through our results
                //display for debug,..
                //@todo store into file in format for visualization
                //@todo go through mismatch vector and coorelate to index
                for(Trie::map resMap : results){

                    std::string indexListStr ="";
                    std::vector<int> indexListMarks;
                    // Count total number of mismatches
                    int numMM = 0;
                    int tmpIndex = 0;
                    for (auto n : resMap.mismatches) {

                        numMM += n;
                        if(n == 1){

                            indexListMarks.push_back((i+tmpIndex));
                        }
                        tmpIndex++;
                    }
                    numOfResults++;

                }

                cout << "\nFound something! a total of " << numOfResults << " number of matching queries\n";

            }






            double duration1;
            duration1 = (clock() - start)/ (double) CLOCKS_PER_SEC;
            trieSearchTimes.push_back(duration1);


        }
//        break;
//    }

    test_runs.push_back(trieBuildTimes);
    test_runs.push_back(trieSearchTimes);


    return test_runs;
}

// Test program
int main()
{
    //const unsigned short ITER_COUNT = 100;
    // Benchmark prefix trie construction
    //std::cout << "Benchmarking reads..." << std::endl;
    //benchmark_reads("read_tests.json", ITER_COUNT);
    //std::cout << "Done." << std::endl << std::endl;

    // Benchmark exhaustive search
    //std::cout << "Benchmarking exhaustive search..." << std::endl;
    //benchmark_subject("subj_tests.json", ITER_COUNT);
    //std::cout << "Done." << std::endl << std::endl;


//	// Create trie
//    Trie* trie = new Trie;
//
//	// Take the subject from the file and put it into a string
//	std::ifstream subjectFile("subject.txt");
//	string subject;
//	std::getline(subjectFile, subject);
//
//	// Take every line from the input file and add it into the trie as a query
//	std::ifstream file("sequences.txt");
//	std::string str;
//
//	// Go through every line
//	std::vector<string> reads;
//	while (std::getline(file, str))
//	{
//		// Add to trie
//		trie->addQuery(str);
//		reads.push_back(str);
//	}
//
//	// Output file
//	std::ofstream out("output.txt");
//
//	// Split up the subject in groups of 50
//	for (int i = 0; i < subject.length() - 50; i++) {
//		string subjectSnip = subject.substr((unsigned long long)i, (unsigned long long) 50);
//		// Search the trie for the subject snippet
//		std::vector<Trie::map> solutions = trie->searchTrieStack(subjectSnip,1);
//
//		// For each match found
//		for (auto sol : solutions) {
//			// Count total number of mismatches
//			int size = 0;
//			for (auto n : sol.mismatches)
//				size += n;
//
//			// Print to file solution
//			out << "Found subject from index " << i << " to " << i + 50 << endl;
//			out << reads.at((unsigned long long)sol.index-1) << " was the read matched, index number " << sol.index-1 << " with: " << size << " mismatches." << endl;
//			// If there were mismatches, print out their locations
//			if (size != 0) {
//				out << "Mismatches at locations: < ";
//				for (int j = 0; j < 50; j++) {
//					if (sol.mismatches.at((unsigned long long)j) != 0) {
//						out << j + i << " ";
//					}
//				}
//				out << "> " << endl;
//			}
//		}
//	}
//
//    // Free memory because we are good people
//    delete trie;
//
//    // Success
//    return 0;



    //@todo both benchmarks now that everything seems to be working

    int trieSizeList[] = {0,0,25000};
    int subjectSearchList[] ={50000,25000,50000};
    std::string trySizeStr;
    std::string subjectSearchLengthStr;

    //base line test for speed of benchmarks
    for(int test = 0; test < 3;test++) {

        std::vector<std::vector<double>> tmpRes = benchmark_subject("BA.fasta", 1,trieSizeList[test],subjectSearchList[test],true);

        if(trieSizeList[test] == 0)
            trySizeStr  = "50000 queries";
        else
            trySizeStr  = "25000 queries";

        subjectSearchLengthStr = to_string(subjectSearchList[test]);


        printf("\nNon exhaustive searches completed : trie build times , query search times respectively\nNumber of queries : %s\nNumber of Searches : %s\n",trySizeStr.c_str(),subjectSearchLengthStr.c_str());
        for(std::vector<double> result : tmpRes){

            for(double dres : result){

                printf("%f ", dres);
            }
            printf("\n");

        }

    }


    //test for different size trie and searches
for(int mismatch = 0; mismatch <3;mismatch++) {

    for (int test = 0; test < 3; test++) {

        std::vector<std::vector<double>> tmpRes = benchmark_subject("combo_reads_0", (unsigned short)2, trieSizeList[test],
                                                                    subjectSearchList[test], false);
//for base line against known queries
//        std::vector<std::vector<double>> tmpRes = benchmark_subject("BA.fasta", (unsigned short)mismatch, trieSizeList[test],
//                                                                    subjectSearchList[test], false);

        if (trieSizeList[test] == 0)
            trySizeStr = "50000 queries";
        else
            trySizeStr = "25000 queries";

        subjectSearchLengthStr = to_string(subjectSearchList[test]);


        printf("\nExhaustive searches completed : with %d mismatches, \n trie build times , query search times respectively\nNumber of queries : %s\nNumber of Searches : %s\n",
               mismatch, trySizeStr.c_str(), subjectSearchLengthStr.c_str());
        for (std::vector<double> result : tmpRes) {

            for (double dres : result) {

                printf("%f ", dres);
            }
            printf("\n");

        }

    }

}





    return 0;
}
