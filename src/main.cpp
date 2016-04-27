#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

#include "json.hpp"
#include "trie.h"

using namespace std;
using json = nlohmann::json;

// Returns time in seconds.
std::vector<std::vector<double>> benchmark_reads(std::string pathname);
std::vector<std::vector<double>> benchmark_subject(std::string pathname);

std::vector<std::vector<double>> benchmark_reads(std::string pathname)
{
    const int ITER_COUNT = 100;

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

        std::cout << "Testing " << error_rates[i] << " error rate of " << rate["rate"] << std::endl;
        for(json &reads : rate["cases"])
        {
            std::cout << "Running " << reads.size() << " queries..." << std::endl;
            start_time = std::chrono::high_resolution_clock::now();
            for(int iter = 0; iter < ITER_COUNT; iter++)
            {
                for(json &read : reads)
                {
                    Trie* trie = new Trie();
                    trie->addQuery(read);
                    delete trie;
                }
            }
            end_time = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            test_runs[i].push_back(duration.count() / (1000000000.0 * ITER_COUNT));
            std::cout << "Time: " << test_runs[i].back() << " s" << std::endl;
        }
    }

    return test_runs;
}

std::vector<std::vector<double>> benchmark_subject(std::string pathname)
{
    const int ITER_COUNT = 100;

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

    std::vector<std::string> error_rates = { "low", "high" };
    std::chrono::nanoseconds duration;
    std::vector<std::vector<double>> test_runs;

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
            for(int iter = 0; iter < ITER_COUNT; iter++)
            {
                Trie* trie = new Trie();
                for(json &read : rate["reads"])
                {
                    trie->addQuery(read);
                }
                start_time = std::chrono::high_resolution_clock::now();
                trie->searchTrieRecursively(trie->getRoot(), test["subject"], 1);
                end_time = std::chrono::high_resolution_clock::now();
                duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
                delete trie;
            }
            test_runs[i].push_back(duration.count() / (1000000000.0 * ITER_COUNT));
            std::cout << "Time: " << test_runs[i].back() << " s" << std::endl;
        }
    }

    return test_runs;
}

// Test program
int main()
{
    // Benchmark prefix trie construction
    std::cout << "Benchmarking reads..." << std::endl;
    benchmark_reads("read_tests.json");
    std::cout << "Done." << std::endl << std::endl;

    // Benchmark exhaustive search
    std::cout << "Benchmarking exhaustive search..." << std::endl;
    benchmark_subject("subj_tests.json");
    std::cout << "Done." << std::endl << std::endl;


	// Create trie
    Trie* trie = new Trie;

    // Add queries
    trie->addQuery("ATTGGTACACATTGGTACACATTGGTACACATTGGTACACATTGGTACAC");
    trie->addQuery("ACGGTGCGGCTTATTGGTACACATTGGTACACATTGGTACACATTGGTAC");
    trie->addQuery("ATAACCCATATTATTGGTACACATTGGTACACATTGGTACACATTGGTAC");
    trie->addQuery("CAAACCCCTTTTATTGGTACACATTGGTACACATTGGTACACATTGGTAC");

	// Search for subject
    string subject = "ACGGTGCGGCTTATTGGTACACATTGGTACACACTGGTACACATTGGTAC";
    trie->searchTrieRecursively(trie->getRoot(), subject, 1);

    cout << "Found " << subject << " at: " << trie->getIndex() << " with: " << trie->getMismatch() << " mismatches."  << endl;
    // TODO Create local variable that keeps track of best mismatches and index.

    // Free memory because we are good people
    delete trie;

    //input from file testing and tree building
    int* listSize = new int[5] {100,1000,10000,50000,100000};
    std::string fileName = "dengV2_";
    int listIndex = 0;

    //create new tree
    Trie* listTest = new Trie;

    //prefix of name of the test files
    std::string tmpFileName = fileName;

    //append the length of the list to the name of the file
    tmpFileName += to_string(listSize[listIndex]);

    //add mime type because cxx hates you
    tmpFileName += ".txt";

    //open the file
    ifstream file(tmpFileName);

    //debug
    //ofstream wfile("dengV2_index11.txt");

    //temp string to store references
    std::string* strTemp = new std::string[5];

    if (file.is_open()) {

        printf("\nfile was opened\n");

        int count = 0;

        for (string line; getline(file, line);) {

            //if(count %10 == 0)
            //   printf("Str count %d : %s\n",(int) line.length(), line.c_str());
            listTest->addQuery(line);

            if(count<5){

                strTemp[count++] = line;
            }

        }
    }else {
        printf("failed to open file");
        return 0;
    }

    //display tree info
    printf("\n%s %6d %s %6d %s\n","Trie was created with list of size ", listSize[listIndex],
           " the size of the trie is ", listTest->getSize(),"# of nodes");

    //test to see if known strings are in tree
    for(int j = 0; j < 5; j++) {

        listTest->searchTrieRecursively(listTest->getRoot(),strTemp[j],1);

        if(listTest->getMismatch() != 99999999) {
            printf("%s %s %s %d %s %d\n", "found the word", strTemp[j].c_str(),
                   "with #", listTest->getMismatch(), " mismatches at index ",
                   listTest->getIndex());
        }else{

            printf("\nDidn't find it recursively lets check our search function!\nBecause it should be in the list\n");

            if(listTest->strictSearch(strTemp[j])){

                printf("\nfound the word %s\n", strTemp[j].c_str());
            }else{

                printf("\nsearch didn't find the word, our trie might be broken\nor most likely both");
            }



        }
    }

    delete listTest;
    file.close();



    // Success
    return 0;
}

