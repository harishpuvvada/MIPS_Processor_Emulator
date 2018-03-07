/*
Branch Predictor
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;

//access state:
#define NA 0 // no action





     struct config{

       int mSize;
      
       };



     class booltoint{

     public:

     booltoint(){}

    unsigned long int fromVector(vector<bool> vec){
    unsigned long int retval = 0;
    int i =0;
    for (vector<bool>::iterator it = vec.begin(); it!=vec.end(); it++){

            retval = retval+ ( vec[i] * pow(2,i));
            i++;

    }
    return retval;
    }

     };



int main(int argc, char* argv[]){


    
    config config_obj;
    ifstream param;
    param.open(argv[1]);

    booltoint booltoint_obj;


    while(!param.eof())  // read config file
    {

      param>>config_obj.mSize;

      }
    

    std::vector<std::vector<bool> > counterArray;

    int numrows = pow(2,config_obj.mSize);

    counterArray.resize(numrows);

    for(int a=0;a<=(counterArray.size()-1);a++){

        counterArray[a].resize(2,1);

      }

   
  int varTorNT = 2;    // variable to be set with a value '1' for taken and '0' for nottaken 
   

    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string(argv[2]) + ".out";
    
    traces.open(argv[2]);
    tracesout.open(outname.c_str());
    
    char var_orig;
    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;        
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;

    int misscounter=0;
    int numoftraces=0;


    if (traces.is_open()&&tracesout.is_open()){    
        while (getline (traces,line)){   // read mem access file and access Cache
            
            istringstream iss(line); 
            if (!(iss >> xaddr)) {break;}
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);

            var_orig = line[9] - '0';

            int orig_int = int(var_orig);

            vector<bool> tempbits(config_obj.mSize) ;



            for(int c=0;c<=config_obj.mSize-1;c++){

              tempbits[c] = accessaddr[c] ;

            }
           

             int tempbits_int = booltoint_obj.fromVector(tempbits);


             if(booltoint_obj.fromVector(counterArray[tempbits_int]) == 3){


                  varTorNT = 1;


                  if(varTorNT == orig_int){


                  }
                  else{

                    misscounter++;

                    counterArray[tempbits_int].at(0)=0;


                  }


             }

             else if(booltoint_obj.fromVector(counterArray[tempbits_int]) == 2){


                     varTorNT = 1;

                  if(varTorNT == orig_int){

                   counterArray[tempbits_int].at(0)=1;

                  }
                  else{
                    
                    misscounter++;

                   counterArray[tempbits_int].at(1)=0;

                    
                  }


             }

             else if(booltoint_obj.fromVector(counterArray[tempbits_int]) == 1){


                     varTorNT = 0;

                  if(varTorNT == orig_int){

                   counterArray[tempbits_int].at(0)=0;

                  }
                  else{

                   misscounter++;

                   counterArray[tempbits_int].at(1)=1;

                    
                  }



             }

             else{


                     varTorNT = 0;


                  if(varTorNT == orig_int){


                  }
                  else{

                   misscounter++;

                   counterArray[tempbits_int].at(0)=1;

                    
                  }


             }

            

             
            tracesout<<" "<< varTorNT << endl;  // Output hit/miss results for L1 and L2 to the output file;
             
             numoftraces++;


             
        }
        traces.close();
        tracesout.close(); 

        double percentage=(misscounter*100.0/numoftraces);
    }

    else cout<< "Unable to open trace or traceout file ";

   
    return 0;
}
