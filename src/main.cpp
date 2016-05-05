#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <string>

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
std::vector<std::vector<double>> benchmark_subject(std::string pathname, unsigned short iterCount)
{
    json subj_tests;
    std::ifstream file;
    file.open(pathname);
    if(file.is_open())
    {
        while (!file.eof())
        {
            file >> subj_tests;
        }
    }
    file.close();

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

    //@todo uncomment for low and high error rate benchmarks
    //std::vector<std::string> error_rates = { "low", "high" };
    //temp testing hard coded to test zero error rate
    //and to show everything works
    //@todo remove none
    std::vector<std::string> error_rates = {"none" };
    std::chrono::nanoseconds duration;
    std::vector<std::vector<double>> test_runs;
    printf("%s\n", error_rates[0].c_str());


    for(json &test : subj_tests)
    {
        test_runs.push_back(std::vector<double>());

        int size = test["size"];
        for(int i = 0; i < error_rates.size(); i++)
        {
            json &rate = test[error_rates[i]];
            std::cout << "Testing " << size / 1000 << "k subject w/ " << error_rates[i] << " error rate of "
                << rate["rate"] << std::endl;
            duration = std::chrono::nanoseconds(0);

            Trie* trie = new Trie();


            std::string tempSubject = test["subject"];

            //printf("\n%s\n", tempSubject.c_str());

            //loop through strings  of the reads array
            for(std::string read : rate["reads"])
            {
               //printf("\n%s\n",read.c_str());
                trie->addQuery(read);
            }

            //debug get the size of the trie
            printf("\nSize of the trie is %d\n", trie->getSize());

            //keep track of the results
            std::vector<Trie::map> results;

            //use the iter for number of mismatches
            for(int iter = 0; iter < iterCount; iter++)
            {
                //start the time before the search
                start_time = std::chrono::high_resolution_clock::now();
                //loop through the whole string exhaustively searching for matches

                for(int i = 0; i < tempSubject.length();i++){
                    std::string tempSubStr =  tempSubject.substr((unsigned long long) i, 50);

                    //debug
                    //printf("%s\n", tempSubStr.c_str());
                    //printf("\n%s %s\n", "trying a search", tempSubStr.c_str());

                    //search for string get the results
                    results  = trie->searchTrieStack(tempSubStr , iter);
//debug
//                    if(trie->searchTrie(tempSubStr) >=0 ){
//
//                        printf("\nSubString is in trie \n%s\n",tempSubStr.c_str());
//                    }

                    //stop the time our search is over
                    end_time = std::chrono::high_resolution_clock::now();
                    duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
                   // printf("size of results %d\n",(int)results.size());

                    //go through our results
                    //display for debug,..
                    //@todo store into file in format for visualization
                    //@todo go through mismatch vector and coorelate to index
                    for(Trie::map resMap : results){

			// Count total number of mismatches
                        int numMM = 0;
                        for (auto n : resMap.mismatches)
                                numMM += n;

                        printf("Positive Query at :%d with # %d  of mismatches", resMap.index,numMM);

                    }
                }

            }

            //@todo store to file for visualization
            test_runs[i].push_back(duration.count() / (1000000000.0 * iterCount));
            std::cout << "Time: " << test_runs[i].back() << " s" << std::endl;

            delete trie;
        }
    }

    return test_runs;
}

int runTest(int toleranceLimit) {

    // Declaration of Trie
    Trie* trie = new Trie;

    // File IO for Genome and Reads
        // Genome
	std::ifstream subjectFile("Genome.txt");
	std::string subject;
        // Because fasta files start with header, read that line to skip it
	std::getline(subjectFile, subject);
        // Then get the actual line we want
        std::getline(subjectFile, subject);

	// Reads
	std::ifstream file("Reads.txt");
	std::string str;
        //Because fasta files start with header, read that line to skip it
        std::getline(file, str);

	// Output file
	std::ofstream out("output.txt");

    // Number of Reads per Trie
    int sizeTrie = 1000;
    // Testing purposes
    int segment = 1;
    int reads = 1;
    // While there are more reads to search
    while (std::getline(file, str))
    {
        // Because reads all have headers, skip any line that starts with >
        if (str[0] == '>') {
            continue;
        }
        if(reads < sizeTrie){
            // Add to trie
            trie->addQuery(str);
            reads++;
        }
        else {
            // Break the subject into 50 mers and search for each
            for (int i = 0; i <= subject.size() - 50; i++) {
		std::string subjectSnip = subject.substr(i, i + 50);
		//Search the trie for the subject snippet
		std::vector<Trie::map> solutions = trie->searchTrieStack(subjectSnip, toleranceLimit);
                //printToFile(solutions, out, i);
            }
            // After we did all searches, we delete the trie to make room for
            // the next trie
            trie->~Trie();
            trie = new Trie;
            reads = 0;

            //reads.clear();
            if (segment % 100 == 0) {
                std::cout << "Finished search segment of " << sizeTrie << " reads. " << segment << " number of segments finished. " << std::endl;
            }
            segment++;
            // Even though we searched, we still took in a read from the file
            //so we must add that to the new trie and reads
            trie->addQuery(str);
            reads++;
        }
    }
    return 0;
}

void printToFile(std::vector<Trie::map> solutions, std::ofstream out, int i) {
    // For each match found
    for (Trie::map sol : solutions) {
            // Count total number of mismatches
            int size = 0;
            for (int n : sol.mismatches) {
                   size += n;
            }
            // Print to file solution
            out << "Found subject from index " << i << " to " << i + 50 << std::endl;
            out << "The read matched, index number " << sol.index << " with: " << size << " mismatches." << std::endl;
            // If there were mismatches, print out their locations
            if (size != 0) {
                   out << "Mismatches at locations: < ";
                   for (int j = 0; j < 50; j++) {
                           if (sol.mismatches.at(j) != 0) {
                                out << j + i << " ";
                            }
                    }
                    out << "> " << std::endl;
            }
    }
}

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



    // //@todo both benchmarks now that everything seems to be working
    // std::vector<std::vector<double>> tmpRes = benchmark_subject("subj_tests_no_errors.json",3);
    //
    // for(std::vector<double> result : tmpRes){
    //
    //     for(double dres : result){
    //
    //         printf("%f\n", dres);
    //     }
    // }



    runTest(1);

    // Success
    return 0;
}
