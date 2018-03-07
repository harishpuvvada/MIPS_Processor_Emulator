/*
Cache Simulator
Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
s = log2(#sets)   b = log2(block size)  t=32-s-b
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
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss


struct config{
       int L1blocksize;
       int L1setsize;
       int L1size;
       int L2blocksize;
       int L2setsize;
       int L2size;
       };

class Cache {

      public:

      int cachesize,sets,indexbits,offsetbits,tagbits;

      std::vector<std::vector<bool> > Tagarray;
      std::vector<std::vector<bool> > indexarray;
      std::vector<std::vector<bool> > offsetarray;
      vector<bool> validbitsarray;

      int a,wsss;

      Cache(){}

      Cache(int bs,int ss,int s) //constructor

      {

      wsss=ss;

      cachesize = s * 1024 ;    //total cache size

      sets = cachesize / (bs * ss) ;    //number of sets in one way

      indexbits = log2(sets);

      offsetbits =  log2(bs);

      tagbits = 32 - offsetbits - indexbits ;


      Tagarray.resize(sets);
      indexarray.resize(sets);
      offsetarray.resize(sets);
      validbitsarray.resize(sets,0);


      for(a=0;a<=(sets-1);a++){

        Tagarray[a].resize(tagbits);
        indexarray[a].resize(indexbits);
        offsetarray[a].resize(offsetbits);

      }

      }

     };

     class FinalCache{

     public:

      vector<Cache> waysvector;
      vector<int> waycounterarray;

      int j;

      FinalCache(int bs,int ss,int s){

      Cache temp(bs,ss,s);

      waysvector.resize(ss);

      for (j=0;j<=(ss-1);j++)
      {
           waysvector[j] = temp;

      }

      waycounterarray.resize((waysvector[0].sets),0);


      }

        void waycountermeth(int indexnum){

                   if(waycounterarray[indexnum]<waysvector[0].wsss-1){

                    waycounterarray[indexnum]++;

                    }

                     else{

                     waycounterarray[indexnum]=0;

                    }

          }




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

    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open(argv[1]);
    while(!cache_params.eof())  // read config file
    {
      cache_params>>dummyLine;
      cache_params>>cacheconfig.L1blocksize;
      cache_params>>cacheconfig.L1setsize;
      cache_params>>cacheconfig.L1size;
      cache_params>>dummyLine;
      cache_params>>cacheconfig.L2blocksize;
      cache_params>>cacheconfig.L2setsize;
      cache_params>>cacheconfig.L2size;
      }

   // Implement by you:
   // initialize the hirearch cache system with those configs
   // probably you may define a Cache class for L1 and L2, or any data structure you like


  FinalCache L1cache(cacheconfig.L1blocksize,cacheconfig.L1setsize,cacheconfig.L1size);

  FinalCache L2cache(cacheconfig.L2blocksize,cacheconfig.L2setsize,cacheconfig.L2size);


    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string("trace") + ".out";
//----testing
    ofstream test;
    string outname1;
    outname1=string("L1cachfstate.txt");
    test.open(outname1.c_str());

    ofstream test2;
    string outname2;
    outname2=string("L2cachfstate.txt");
    test2.open(outname2.c_str());

//-----testing
    traces.open(argv[2]);
    tracesout.open(outname.c_str());

    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;


    if (traces.is_open()&&tracesout.is_open()){

        while (getline (traces,line)){   // read mem access file and access Cache

            istringstream iss(line);
            if (!(iss >> accesstype >> xaddr)) {break;}
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
            cout<<accessaddr<<"-----"<<endl;

        int L1AcceState =0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
        int L2AcceState =0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;


           //L1 bits seperation and converting them to int

           vector<bool> L1offsetvalue(L1cache.waysvector[0].offsetbits) ;
           vector<bool> L1indexvalue(L1cache.waysvector[0].indexbits) ;
           vector<bool> L1tagvalue(L1cache.waysvector[0].tagbits) ;

           int a,b,c,d,e,f,k,l,x,y;

           booltoint booltoint_obj;



            for(d=0;d<=(L1cache.waysvector[0].offsetbits)-1;d++){
             L1offsetvalue[d]=accessaddr[d];
             }

            for(e=0;e<=(L1cache.waysvector[0].indexbits)-1;e++){
             L1indexvalue[e]=accessaddr[(L1cache.waysvector[0].offsetbits)+e];
             }

             for(f=0;f<=(L1cache.waysvector[0].tagbits)-1;f++){
             L1tagvalue[f]=accessaddr[(L1cache.waysvector[0].indexbits)+(L1cache.waysvector[0].offsetbits)+f];
             }


           unsigned long int L1OffsetVal_int=booltoint_obj.fromVector(L1offsetvalue);
           unsigned long int L1IndexVal_int=booltoint_obj.fromVector(L1indexvalue);
           unsigned long int L1TagVal_int=booltoint_obj.fromVector(L1tagvalue);


            cout<<"offset"<<L1OffsetVal_int<<endl;
            cout<<"index"<<L1IndexVal_int<<endl;
            cout<<"tag"<<L1TagVal_int<<endl;


           //L2 bits seperation and converting them to int

           vector<bool> L2offsetvalue(L2cache.waysvector[0].offsetbits) ;
           vector<bool> L2indexvalue(L2cache.waysvector[0].indexbits) ;
           vector<bool> L2tagvalue(L2cache.waysvector[0].tagbits) ;


             for(k=0;k<=(L2cache.waysvector[0].offsetbits)-1;k++){
             L2offsetvalue[k]=accessaddr[k];
             }

             for(l=0;l<=(L2cache.waysvector[0].indexbits)-1;l++){
             L2indexvalue[l]=accessaddr[(L2cache.waysvector[0].offsetbits)+l];
             }

             for(x=0;x<=(L2cache.waysvector[0].tagbits)-1;x++){
             L2tagvalue[x]=accessaddr[(L2cache.waysvector[0].indexbits)+(L2cache.waysvector[0].offsetbits)+x];
             }
  

            unsigned long int L2OffsetVal_int=booltoint_obj.fromVector(L2offsetvalue);
            unsigned long int L2IndexVal_int=booltoint_obj.fromVector(L2indexvalue);
            unsigned long int L2TagVal_int=booltoint_obj.fromVector(L2tagvalue);

            cout<<"offset"<<L2OffsetVal_int<<endl;
            cout<<"index"<<L2IndexVal_int<<endl;
            cout<<"tag"<<L2TagVal_int<<endl;


           // access the L1 and L2 Cache according to the trace;
              if (accesstype.compare("R")==0)

             {
                 //Implement by you:
                 // read access to the L1 Cache,
                 //  and then L2 (if required),
                 //  update the L1 and L2 access state variable;

            for(int y=0;y<=(cacheconfig.L1setsize-1);y++){

             if((booltoint_obj.fromVector((L1cache.waysvector[y].Tagarray[L1IndexVal_int]))==(L1TagVal_int))&&(L1cache.waysvector[y].validbitsarray[L1IndexVal_int])){

              cout<<"Read Hit in L1"<<endl;

              L1AcceState = 1;

              goto jumping;

              }

              }

              L1AcceState = 2;


              for(int g=0;g<=(cacheconfig.L2setsize-1);g++){

                 if((booltoint_obj.fromVector(L2cache.waysvector[g].Tagarray[L2IndexVal_int])==(L2TagVal_int))&&(L2cache.waysvector[g].validbitsarray[L2IndexVal_int])){

                   cout<<"Read Hit in L2"<<endl;

                    L2AcceState = 1;


                            for(int n=0;n<=(cacheconfig.L1setsize-1);n++){ // for checking if any way in L1 cache is empty

                                    if(!(L1cache.waysvector[n].validbitsarray[L1IndexVal_int])){

                                    L1cache.waysvector[n].Tagarray[L1IndexVal_int] = L1tagvalue; //storing tag

                                    L1cache.waysvector[n].validbitsarray[L1IndexVal_int] = 1; //setting the valid bit to 1

                                    goto jumping;

                                    }
                                                           
                             }

                                  //otherwise evict and write it to L2

                                  L1cache.waysvector[L1cache.waycounterarray[L1IndexVal_int]].Tagarray[L1IndexVal_int] = L1tagvalue; //storing new data in L1

                                  L1cache.waycountermeth(L1IndexVal_int);

                                  goto jumping;

                 }

                 }

                       L2AcceState = 2; // read miss


                             for(int n=0;n<=((cacheconfig.L2setsize)-1);n++){ // for checking if any way in L2 cache is empty


                                    if(!(L2cache.waysvector[n].validbitsarray[L2IndexVal_int]))

                                    {
 
                                           L2cache.waysvector[n].Tagarray[L2IndexVal_int] = L2tagvalue; //storing tag

                                           L2cache.waysvector[n].validbitsarray[L2IndexVal_int] = 1;

                                           goto mid1;

                                    }

                            }


                                L2cache.waysvector[L2cache.waycounterarray[L2IndexVal_int]].Tagarray[L2IndexVal_int] = L2tagvalue;

                                L2cache.waycountermeth(L2IndexVal_int);


                      mid1:    for(int n=0;n<=(cacheconfig.L1setsize-1);n++){ // for checking if any way in L1 cache is empty

                                    if(!(L1cache.waysvector[n].validbitsarray[L1IndexVal_int])){

                                            L1cache.waysvector[n].Tagarray[L1IndexVal_int] = L1tagvalue; //storing tag

                                            L1cache.waysvector[n].validbitsarray[L1IndexVal_int] = 1; //setting the valid bit to 1

                                            goto jumping;

                                    }

                             }
                                      //otherwise evict and write it to L2

                                     L1cache.waysvector[L1cache.waycounterarray[L1IndexVal_int]].Tagarray[L1IndexVal_int] = L1tagvalue; //storing new data in L1

                                     L1cache.waycountermeth(L1IndexVal_int);

                                     goto jumping;

                      }

             else
             {
                  //Implement by you:
                  // write access to the L1 Cache,
                  //and then L2 (if required),
                  //update the L1 and L2 access state variable;
                  //write Miss
              for(int g=0;g<=(cacheconfig.L1setsize-1);g++){

              if((booltoint_obj.fromVector((L1cache.waysvector[g].Tagarray[L1IndexVal_int]))==(L1TagVal_int))&&(L1cache.waysvector[g].validbitsarray[L1IndexVal_int])){
              //cachevalue == incoming value and valid bit is one for that way

                cout<<"write Hit in L1"<<endl;

                L1AcceState = 3;

                goto jumping;

              }

              }

              L1AcceState =4;


              for(int g=0;g<=(cacheconfig.L2setsize-1);g++){


              if((booltoint_obj.fromVector((L2cache.waysvector[g].Tagarray[L2IndexVal_int]))==(L2TagVal_int))&&(L2cache.waysvector[g].validbitsarray[L2IndexVal_int])){

                cout<<"write Hit in L2"<<endl;

                L2AcceState = 3;

                goto jumping;

              }

              }

              L2AcceState = 4;

            }


        jumping: tracesout<< L1AcceState << " " << L2AcceState << endl;  // Output hit/miss results for L1 and L2 to the output file;


        }

        traces.close();
        tracesout.close();
//---------------------------------testing

      vector<bool>::iterator ite;

      for(int u=0;u<=(cacheconfig.L1setsize)-1;u++){

        test<<" L1 Way "<<u<<endl;

        for(int y=0;y<=((L1cache.waysvector[0].sets)-1);y++){

           for(ite=L1cache.waysvector[u].Tagarray[y].begin();ite!=L1cache.waysvector[u].Tagarray[y].end();ite++){

            test<<*ite;

         }

         test<<endl;

        }
      test<<endl;
      test<<endl;
      test<<endl;

      }


        for(int u=0;u<=(cacheconfig.L2setsize)-1;u++){

        test2<<" L2 Way "<<u<<endl;

        for(int y=0;y<=((L2cache.waysvector[0].sets)-1);y++){

           for(ite=L2cache.waysvector[u].Tagarray[y].begin();ite!=L2cache.waysvector[u].Tagarray[y].end();ite++){

            test2<<*ite;

         }

         test2<<endl;

        }
      test2<<endl;
      test2<<endl;
      test2<<endl;

      }

// ------------------------------testing
    }

    else cout<< "Unable to open trace or traceout file ";

    return 0;

}
