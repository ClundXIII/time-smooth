#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;

typedef double real;

int main(int argc, char **argv){

    vector<string> argList;

    for (int i=0; i<argc; i++){
        argList.push_back(argv[i]);
    }

    if (argList.size() != 6){
        cout << "Expected 5 args:" << endl
             << "1) filename" << endl
             << "2) last File number" << endl
             << "3) destination filename" << endl
             << "4) interval" << endl
             << "5) file ending from input file" << endl;
        return -1;
    }

    string baseFilename = argList.at(1);
    int last_file_num   = stoi(argList.at(2));
    string destinationFilename = argList.at(3);
    int interval = stoi(argList.at(4));
    string fileEnding = argList.at(5);


    cout << "reading x-Axis:" << endl;
    vector<real> xAxis;
    vector<vector<real>> buffer; //1st dimension: Q-Value/Angstroem/etc; 2nd dimension: time
    ///GET the 1st dimension:
    stringstream first_filename;
    first_filename << baseFilename << setfill('0') << setw(5) << 1 << fileEnding;
    ifstream first_f;
    first_f.open(first_filename.str());
    string tmpLine;
    while (getline(first_f,tmpLine)){
        if (tmpLine.at(0) == '#'){
            continue;
        }
        vector<real> tmpVec;
        for (int j=0; j<interval; j++){
            tmpVec.push_back(0);
        }

        stringstream streamline;
        streamline.str(tmpLine);
        real val1;
        streamline >> val1;
        xAxis.push_back(val1);
        buffer.push_back(tmpVec);
    }
    cout << "file: " << first_filename.str() << ", size: " << xAxis.size() << endl;

    cout << "done\nprocessing ..." << endl;
    int i=1;
    for (; i<(last_file_num/interval); i++){

        stringstream out_filename;
        out_filename << destinationFilename << setfill('0') << setw(5) << i << fileEnding;

        ofstream out_f;
        out_f.open(out_filename.str());

        for (int j=0; j<interval; j++){
            stringstream in_filename;
            in_filename << baseFilename << setfill('0') << setw(5) << ((i*interval)+j) << fileEnding;

            ifstream in_f;
            in_f.open(in_filename.str());

            int line_number = 0;
            string line;
            while (getline(in_f,line)){

                if (line.at(0) == '#'){
                    continue;
                }

                stringstream streamline;
                streamline.str(line);

                real val1, val2;
                streamline >> val1;
                streamline >> val2;

                buffer.at(line_number)[j] = val2;

                line_number++;
            }

            in_f.close();
        }

        for (int k=0; k<xAxis.size(); k++){
            real sum=0;
            for (int l=0; l<interval; l++){
                sum += buffer.at(k).at(l);
            }
            out_f << xAxis.at(k) << " " << sum << endl;
        }

    	out_f.flush();

        out_f.close();

        if ((i % (50/interval)) == 0){
            cout << "."; cout.flush();
        }

    }


    cout << "files: " << i << endl;

    return 0;
}
