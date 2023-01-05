/*

DSA 2: Section 2 - Dec. 6
Program 4 - Dynamic Programming (Merge)
Andy Kim

*/

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool dmc[1001][1001];    //  dynamic merge check

void dynamicMergeCheck(string &input_file, string &output_file);        // function declaration

int main() {
    string input_file, output_file;

    cout << "Enter name of input file: ";
    cin >> input_file;
    cout << "Enter name of output file: ";
    cin >> output_file;

    dynamicMergeCheck(input_file, output_file);         // execute dynamic merge check

    return 0;
}

// uses bottom-up dynamic programming to evaluate each letter of A and B with each letter of C in a global matrix to determine if C is a valid merge
void dynamicMergeCheck(string &input_file, string &output_file) {
    string A, B, C;
    ifstream input;
    ofstream output;

    input.open(input_file);
    output.open(output_file);
    if (!input) {cout << "Input file failed to open." << endl; exit(EXIT_FAILURE);}
    if (!output) {cout << "Output file failed to open." << endl; exit(EXIT_FAILURE);}

    while(input >> A && input >> B && input >> C) {     // look at every three lines/words
        // both A and B are empty
        if ((A == "") && (B =="") && (C == "")) {output << "\n";}
        // any 2 words are empty
        else if ((A == "") + (B == "") + (C == "") == 2) {output << "*** NOT A MERGE ***\n"; continue;}
        // length of C does not equal length sum of A and B
        else if (A.length() + B.length() != C.length()) {output << "*** NOT A MERGE ***\n"; continue;}
        // last letter of C doesn't match A or B
        else if ((A[A.length() - 1] != C[C.length() - 1]) && (B[B.length() - 1] != C[C.length() - 1])) {output << "*** NOT A MERGE ***\n"; continue;}

        // dynamic programming implementation
        for (int i = 0; i <= B.length(); i++) {         // mainly for determining which letters of which word occur in C first, also initializes entire array
            for (int j = 0; j <= A.length(); j++) {
                if (i == 0 && j > 0) {
                    if (C[j - 1] == A[j - 1]) {
                        if (j == 1) {
                            dmc[i][j] = true;
                            continue;
                        }
                        else {
                            dmc[i][j] = dmc[i][j - 1];
                            continue;
                        }
                    }
                }

                else if (j == 0 && i > 0) {
                    if (C[i - 1] == B[i - 1]) {
                        if(i == 1) {
                            dmc[i][j] = true;
                            continue;
                        }
                        else{
                            dmc[i][j] = dmc[i - 1][j];
                            continue;
                        }
                    }
                }
                dmc[i][j] = false;
            }
        }

        for (int i = 1; i <= B.length(); i++) {         // determine relationship between each letter of each word and if they exist in C, completes the array
            for (int j = 1; j <= A.length(); j++) {
                dmc[i][j] = false;
                if (dmc[i - 1][j] || dmc[i][j - 1]) {
                    if (A[j - 1] == B[i - 1] && A[j - 1] == C[i + j - 1]) {
                        dmc[i][j] = true;
                    }
                    else if (dmc[i - 1][j]) {
                        if (B[i - 1] == C[i + j - 1]) {
                            dmc[i][j] = true;
                        }
                    }
                    else if (dmc[i][j - 1]) {
                        if (A[j - 1] == C[i + j - 1]) {
                            dmc[i][j] = true;
                        }
                    }
                }
            }
        }

        // output parses through the finalized array, starting from the bottom right and capitalizes the letters of A in C
        if (dmc[B.length()][A.length()] == 1) {
            int m = B.length ();
            int n = A.length ();

            while (n > 0 && m >= 0) {
                if (dmc[m][n] && (m == 0 || !(dmc[m - 1][n]))) {
                    C[m + n - 1] = toupper(C[m + n - 1]);
                    n--;
                }
                else {m--;}
            }
            output << C << endl;
        }

        else  {output << "*** NOT A MERGE ***" << endl;}

    }
    input.close();
    output.close();
}
