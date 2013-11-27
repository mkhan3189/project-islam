#include <easylogging++.h>
#include <string>
#include <fstream>
#include <algorithm>

_INITIALIZE_EASYLOGGINGPP

const char* usage = "./builder <rukuh_sajdah_list_file> <table_name> <data_file> <output_file>";

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::string convertLine(const std::string& line) {
    std::vector<std::string> e;
    std::stringstream ss;
    split(line, '|', e);
    int i = 1;
    for (std::string row : e) {
        switch (i++) {
        case 1:
           ss << row << ", ";
           break;
        case 2:
           ss << row << ", ";
           break;
        case 3:  
           ss << "'" << row << "', ";
           break;
           i = 1;
        }
    }
    return ss.str();
}

std::string chapVerse(const std::string& line) {
    std::vector<std::string> e;
    std::stringstream ss;
    split(line, '|', e);
    int i = 1;
    for (std::string row : e) {
        switch (i++) {
        case 1:
           ss << row << "|";
           break;
        case 2:
           ss << row;
           break;
        case 3:
           i = 1;
        }
    }
    return ss.str();
}

int main(int argc, char** argv) {

    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::ToFile, "false");

    if (argc < 4) {
        LOG(ERROR) << "Invalid number of args: " << usage;
        return -1;
    }

    std::ifstream rslFile;
    rslFile.open(argv[1], std::ifstream::in);

    std::string tableName = std::string(argv[2]);

    std::ifstream dataFile;
    dataFile.open(argv[3], std::ifstream::in);

    std::ofstream outputFile;
    outputFile.open(argv[4], std::ofstream::out);

    outputFile << " -- " << tableName << " table \n\
 --     ID \n\
 --     QuranChapterID \n\
 --     VerseNumber \n\
 --     VerseText \n\
 --     Rukuh \n\
 --     Sajdah \n\
 DROP TABLE IF EXISTS " << tableName << "; \n\
 CREATE TABLE IF NOT EXISTS " << tableName << " (ID INTEGER PRIMARY KEY ASC, QuranChapterID INTEGER NOT NULL, VerseNumber INTEGER NOT NULL, VerseText STRING NOT NULL, Rukuh BOOLEAN NOT NULL, Sajdah BOOLEAN NOT NULL);" << std::endl;

    std::string insertStatement = "INSERT INTO " + tableName + " (QuranChapterID, VerseNumber, VerseText, Rukuh, Sajdah) VALUES (";

    std::vector<std::string> rukuhList;
    std::vector<std::string> sajdahList;
    std::string rslLine;
    bool s = false, r = false;
    while (rslFile.good()) {
        std::getline(rslFile, rslLine);
        if (rslLine == "-rukuh") { s = false; r = true; continue; }
        else if (rslLine == "-sajdah") { r = false; s = true; continue; }
        if (s) sajdahList.push_back(rslLine);
        if (r) rukuhList.push_back(rslLine);
    }

    std::string line;
    while (dataFile.good()) {
        std::getline(dataFile, line);
        if (line.empty()) continue;
        outputFile << insertStatement;
        // Single quotes to double
        std::replace(line.begin(), line.end(), '\'', '\"');
        outputFile << convertLine(line);
        // Rukuh Sajdah check
        if (std::find(rukuhList.begin(), rukuhList.end(), chapVerse(line)) != rukuhList.end()) {
            LOG(INFO) << "Found rukuh in " << line;
            outputFile << "'1', "; 
        } else {
            outputFile << "'0', ";
        }
        if (std::find(sajdahList.begin(), sajdahList.end(), chapVerse(line)) != sajdahList.end()) {
            LOG(INFO) << "Found sajdah in " << line;
           outputFile << "'1');";
        } else {
           outputFile << "'0');";
        }
        outputFile << std::endl;
    }

    rslFile.close();
    dataFile.close();
    outputFile.close();

    return 0;
}
