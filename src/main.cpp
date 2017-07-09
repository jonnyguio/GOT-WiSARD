// #include "mnist/mnist_reader.hpp"
#include <wann/WiSARD.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

#define DEBUG 0

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args)
    {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT> 
                            (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};

const char 
    *trainLabelsFilename = "./dados/labels/labels-treino-balanceadas.txt", 
    *trainWordRetinaFilename = "./dados/entradasParaRNSP/entradaTreinoPalavras.txt", 
    *trainTagRetinaFilename = "./dados/entradasParaRNSP/entradaTreinoTags.txt", 
    // *trainWordRetinaFilename = "./dados/entradaTreinoPalavraBalanceada.txt", 
    // *trainTagRetinaFilename = "./dados/entradaTreinoTagBalanceada.txt", 
    *testLabelsFilename = "./dados/labels/labels-teste.txt",
    *testWordRetinaFilename = "./dados/entradasParaRNSP/entradaTestePalavra.txt",
    *testTagRetinaFilename = "./dados/entradasParaRNSP/entradaTesteTag.txt";

const int trainSize = 200;
const int testSize = 47;
int numThreads = 4;
int numBitsAddrs = 16;
int threshold = 98;
bool bleaching = true;
bool extended = false;
int defaultBleaching = 1;
float confidenceThreshold = 0.1;

std::mutex resultMutex;

using namespace wann;

void executePartial(int, std::vector<std::vector<int>>, std::vector< std::vector<std::string> >*, WiSARD*);
void parallel(void);
void sequential(void);

void prepareDataWiSARD(const char, const char, std::vector<std::vector<int>>&, std::vector<std::string>&, bool);
void executePartial(int threadID, std::vector<std::vector<int>> array, std::vector< std::vector<std::string> > *result, WiSARD *w) {
//     std::vector<std::string> res;
//     if (DEBUG) std::cout << "Starting prediction..." << std::endl;
//     res = w->predict(array);
//     if (DEBUG) std::cout << "Finished predict, adding to list" << std::endl;
//     resultMutex.lock();
//     (*result)[threadID] = res;
//     resultMutex.unlock();
//     if (DEBUG) std::cout << "Exiting..." << std::endl;
}

void parallel() {

//     int count = 0;
//     std::vector<std::vector<int>> trainingImages, testImages, trainingImagesExtended, testImagesExtended;
//     std::vector<std::string> trainingLabels, testLabels, result;
//     std::vector<std::vector<int>>::const_iterator first, last;
//     std::vector<std::thread *> prediction;
//     std::vector< std::vector< std::string > > resultPartial(numThreads);

//     readLabelsMNIST(trainLabelsFilename, trainingLabels);
//     readImagesMNIST(trainImagesFilename, trainingImages);
//     readLabelsMNIST(testLabelsFilename, testLabels);
//     readImagesMNIST(testImagesFilename, testImages);

//     // std::thread tTrainLabels(readLabelsMNIST, std::ref(trainLabelsFilename), std::ref(trainingLabels));
//     // std::thread tTrainImages(readImagesMNIST, std::ref(trainImagesFilename), std::ref(trainingImages));
//     // std::thread tTestLabels(readLabelsMNIST, std::ref(testLabelsFilename), std::ref(testLabels));
//     // std::thread tTestImages(readImagesMNIST, std::ref(testImagesFilename), std::ref(testImages));

//     // tTrainLabels.join();
//     // tTrainImages.join();
//     // tTestLabels.join();
//     // tTestImages.join();

//     if (DEBUG) std::cout << "Preparing it to WiSARD..." << std::endl;
    
//     prepareToWisard(trainingImages, trainingImagesExtended);
//     prepareToWisard(testImages, testImagesExtended);

//     WiSARD *w = new WiSARD((extended) ? trainingImagesExtended[0].size() : trainingImages[0].size(), numBitsAddrs, bleaching, confidenceThreshold, defaultBleaching);

//     if (DEBUG) std::cout << "Training..." << std::endl;
//     w->fit((extended) ? trainingImagesExtended : trainingImages, trainingLabels);

//     if (DEBUG) std::cout << "Prediction..." << std::endl;

//     for (int i = 0; i < numThreads; i++) {
//         if (!extended) {
//             first = testImages.begin() + testImages.size() / numThreads * i;
//             last = testImages.begin() + testImages.size() / numThreads * (i + 1);
//         }
//         else {
//             first = testImagesExtended.begin() + testImagesExtended.size() / numThreads * i;
//             last = testImagesExtended.begin() + testImagesExtended.size() / numThreads * (i + 1);
//         }
//         std::vector<std::vector<int>> partial(first, last);

//         prediction.push_back(new std::thread(executePartial, i, partial, &resultPartial, w));
//     }

//     for (int i = 0; i < numThreads; i++) {
//         prediction[i]->join();
//     }

//     if (DEBUG) {
//         std::cout << "Finished, counting right answers (" << numThreads << "):" << std::endl; 
//         std::cout << "\t";
//         for (int i = 0; i < numThreads; i++) {
//             std::cout << resultPartial[i].size() << " ";
//         }
//         std::cout << std::endl;
//     }
//     if (DEBUG) std::cout << "Rights: ";
//     for (int i = 0; i < numThreads; i++) {
//         for (int j = 0; j < resultPartial[0].size(); j++) {
//             if (testLabels[i * resultPartial[0].size() + j] == resultPartial[i][j]) {
//                 count++;
//             }
//         }
//     }
//     std::cout << count << std::endl;

//     delete w;
}

void prepareDataWiSARD(int retinaLength, const char *dataFilename, const char *labelFilename, std::vector<std::vector<int>> &dataVec, std::vector<std::string> &labelVec, bool train) {
    int index, size = (train) ? trainSize : testSize;
    char temp;
    std::ifstream dataFile(dataFilename), labelFile(labelFilename);

    dataVec.resize(size, std::vector<int>(retinaLength));
    labelVec.resize(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < retinaLength; ++j) {
            dataFile.read((char *) &temp, sizeof(char));
            dataVec[i][j] = temp - 48;
            // std::cout << "temp " << temp << std::endl;
            // std::cout << "dataVec[" << i << "][" << j << "] = " << dataVec[i][j] << std::endl;
            dataFile.read((char *) &temp, sizeof(char));
            // std::cout << "temp " << temp << std::endl;
            dataFile.read((char *) &temp, sizeof(char));
            // std::cout << "temp " << temp << std::endl;
        }
    }
    // std::cout << "ué" << std::endl;
    for (int i = 0; i < size; ++i) {
        labelFile.read((char *) &temp, sizeof(char));
        labelFile.read((char *) &temp, sizeof(char));
        labelVec[i] = std::to_string((int) temp - 48);
        // std::cout << "labelVec[" << i << "] = " << labelVec[i] << " - " << temp << " - " << temp - 48 << std::endl;
        labelFile.read((char *) &temp, sizeof(char));
        labelFile.read((char *) &temp, sizeof(char));
        labelFile.read((char *) &temp, sizeof(char));
    }
}

void sequential() {
    int count = 0;
    std::vector<std::vector<int>> trainWord, testWord, trainTag, testTag;
    std::vector<std::string> trainWordLabels, trainTagLabels, testWordLabels, testTagLabels, resultTag, resultWord;

    prepareDataWiSARD(1009, trainWordRetinaFilename, trainLabelsFilename, trainWord, trainWordLabels, true);
    prepareDataWiSARD(29, trainTagRetinaFilename, trainLabelsFilename, trainTag, trainTagLabels, true);
    prepareDataWiSARD(1009, testWordRetinaFilename, testLabelsFilename, testWord, testWordLabels, false);
    prepareDataWiSARD(29, testTagRetinaFilename, testLabelsFilename, testTag, testTagLabels, false);

    WiSARD *wisardWord = new WiSARD(1009, numBitsAddrs, bleaching, confidenceThreshold, defaultBleaching);
    WiSARD *wisardTag = new WiSARD(29, numBitsAddrs, bleaching, confidenceThreshold, defaultBleaching);

    if (DEBUG) std::cout << "Training..." << std::endl;
    wisardWord->fit(trainWord, trainWordLabels);
    wisardTag->fit(trainWord, trainTagLabels);

    if (DEBUG) std::cout << "Prediction..." << std::endl;
    resultWord = wisardWord->predict(testWord);
    resultTag = wisardTag->predict(testTag);

    if (DEBUG) std::cout << "Rights words: ";
    for (int i = 0; i < testWordLabels.size(); ++i) {
        // std::cout << testWordLabels[i] << " - " << resultWord[i] << std::endl;
        if (testWordLabels[i] == resultWord[i]) {
            count++;
        }
    }    
    std::cout << count << std::endl;//<< " - " << (float) count / (float) testSize << std::endl;
    
    count = 0;
    if (DEBUG) std::cout << "Rights tags: ";
    for (int i = 0; i < testTagLabels.size(); ++i) {
        // std::cout << testTagLabels[i] << " - " << resultTag[i] << std::endl;
        if (testTagLabels[i] == resultTag[i]) {
            count++;
        }
    }    
    std::cout << count << std::endl;// " - " << (float) count / (float) testSize << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc > 1) {
        if (argc > 2) {
            numBitsAddrs = atoi(argv[2]);
            if (argc > 3) {
                if (argv[3][0] == '0') {
                    bleaching = false;
                }
                else {
                    defaultBleaching = atoi(argv[3]);
                }
                if (argc > 4) {
                    confidenceThreshold = atof(argv[4]);
                    if (argc > 5 ) {
                        numThreads = atoi(argv[5]);
                    }
                }
            }
        }
        if (argv[1][0] == '1') {
            std::cout << measure<std::chrono::milliseconds>::execution(parallel) << " miliseconds." << std::endl;
        }
        else {
            if (argv[1][0] == '0') {
               std::cout << measure<std::chrono::milliseconds>::execution(sequential) << " miliseconds." << std::endl;
            }
            else {
                std::cout << "Wrong argument. Use 0 for sequential or 1 for parallel execution" << std::endl;
                return 0;
            }
        }
    }
    else {
        std::cout << "usage: " << argv[0] << " <0 = sequential, 1 = parallel> [<numBitsAddrs> <bleaching, 0 = false, >1 = size of bleaching> <confidenceThreshold> <numThreads>]" << std::endl;
    }
    return 0;
}