#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.
bitset<32> ReturnRegister1,ReturnRegister2;
int is_loadword;
bitset<32> pc;



class RF
{
    public:
        bitset<32> ReadData1, ReadData2;
     	 RF()
    	{
          Registers.resize(32);
          Registers[0] = bitset<32> (0);
        }

        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {

          int writeenable=WrtEnable.to_ulong();

          if(writeenable){


            Registers[WrtReg.to_ulong()]=WrtData;


          }
          else{

          ReturnRegister1=Registers[RdReg1.to_ulong()];           //saving the Register's data and sending to main

          ReturnRegister2=Registers[RdReg2.to_ulong()];

          }



          return;

         }

	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {
                        rfout << Registers[j]<<endl;
                      }

                  }
                  else cout<<"Unable to open file";
                  rfout.close();

               }
	private:
            vector<bitset<32> >Registers;

};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {
                 // implement the ALU operations by you.
         int op1,op2,aluop_int,aluresult_int;
         op1= oprand1.to_ulong();
         op2 = oprand2.to_ulong();
         aluop_int = ALUOP.to_ulong();
         aluresult_int = ALUresult.to_ulong();

         if (aluop_int == 1) {
           aluresult_int = op1 + op2;
         }

        else if (aluop_int == 3) {
           aluresult_int = op1 - op2;
         }

         else if (aluop_int == 4) {
           aluresult_int = op1 & op2;    //AND operation
         }

         else if (aluop_int == 5) {
           aluresult_int = op1 | op2;     //OR operation
         }

         else if (aluop_int == 7) {
           aluresult_int = !(op1 | op2);    //NOR operation
         }


         std::bitset<32> ALUresult(aluresult_int);
                 return ALUresult;
               }
};

class INSMem
{
      public:
          bitset<32> Instruction;
          int counter;
          INSMem()
          {       IMem.resize(MemSize);
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {
                        IMem[i] = bitset<8>(line);
                        i++;
                        counter++;
                     }

                  }
                  else cout<<"Unable to open file";
                  imem.close();

                  }

          bitset<32> ReadMemory (bitset<32> ReadAddress)
              {
               // implement by you. (Read the byte at the ReadAddress and the following three byte).
                 cout<<"First line in Read memory"<<ReadAddress<<"\n";
                 int temp_pc= ReadAddress.to_ulong();
                 std::string str1=IMem[temp_pc].to_string<char,std::string::traits_type,std::string::allocator_type>();
                 std::string str2=IMem[temp_pc+1].to_string<char,std::string::traits_type,std::string::allocator_type>();
                 std::string str3=IMem[temp_pc+2].to_string<char,std::string::traits_type,std::string::allocator_type>();
                 std::string str4=IMem[temp_pc+3].to_string<char,std::string::traits_type,std::string::allocator_type>();

                 std::string str=str1+str2+str3+str4;

                 Instruction = bitset<32>(str);

               return Instruction;
              }

      private:
           vector<bitset<8> > IMem;

};

class DataMem
{
      public:
          bitset<32> readdata;
          DataMem()
          {
             DMem.resize(MemSize);
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();

          }
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
          {

               // implement by you.

                if(readmem==1){

                 int temp_ptr=Address.to_ulong();

                 std::string str1=DMem[temp_ptr].to_string<char,std::string::traits_type,std::string::allocator_type>();
                 std::string str2=DMem[temp_ptr+1].to_string<char,std::string::traits_type,std::string::allocator_type>();
                 std::string str3=DMem[temp_ptr+2].to_string<char,std::string::traits_type,std::string::allocator_type>();
                 std::string str4=DMem[temp_ptr+3].to_string<char,std::string::traits_type,std::string::allocator_type>();

                 std::string str=str1+str2+str3+str4;

                 bitset<32> readdata(str);

                 return readdata;

                }


                if(writemem==1){

                  int DMempointer = Address.to_ulong();

                  bitset<8> ArrsOfData[4] ;

                  int i,j,k,l,m;


                  for(i=0;i<=7;i++){
                  ArrsOfData[0].set(i,WriteData[24+i]);        //      writing data to MSB  byte
                  }
                  for(j=0;j<=7;j++){
                  ArrsOfData[1].set(j,WriteData[16+j]);        //       writing data to 2nd byte
                  }
                  for(k=0;k<=7;k++){
                  ArrsOfData[2].set(k,WriteData[8+k]);         //       writing data to 3rd byte
                  }
                  for(l=0;l<=7;l++){
                  ArrsOfData[3].set(l,WriteData[l]);           //       writing data to 4th byte
                  }

                  for(m=0;m<=3;m++){
                   DMem[DMempointer+m]=ArrsOfData[m];           // to put data from array to dmem
                  }

                }


          }

          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {
                        dmemout << DMem[j]<<endl;
                       }

                  }
                  else cout<<"Unable to open file";
                  dmemout.close();

               }

      private:
           vector<bitset<8> > DMem;

};



int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    long pc_int ;
    bitset<32> pc;
    int i,j,k,l,m,n,p,x,y,e,f;
    bitset<6> tempopcode ;
    bitset<6> Rtypeopcode;
    bitset<5> RegRs;
    bitset<5> RegRt;
    bitset<5> RegRd;                                // different sets of bits which we can use in later part of code
    bitset<5> Reg4;
    bitset<5> Reg5;
    bitset<16> imd;
    bitset<1> WrtEnable;
    bitset<32> WrtData;
    bitset<3> ALUop;
    bitset<3> LSBopcode;
    bitset<3> LSBInstruction;
    bitset<32> signextimmd;
    bitset<32> beq_signextimmd;
    bitset<32> jumpaddr;
    long temp_pc_int;
    long eltemp_pc_int;
    long jtemp_pc_int;
    bitset<32>next_pc;

  while(1){


       bitset<32> Instruction=myInsMem.ReadMemory(pc);


    if(Instruction==4294967295){                  //4294967295 - 11111111111111111111111111111111
        cout<<"Break is executed\n";
        break;
    }                                           // If current insturciton is "11111111111111111111111111111111", then break;
    else{
        cout<<"Break is not executed\n";
    }


    for(i=0;i<=5;i++){
        tempopcode.set(i,Instruction[26+i]);
    }
    for(x=0;x<=2;x++){
        LSBopcode.set(x,Instruction[26+x]);            //seperating 3 lsb bits of opcode for I type ALUop
    }
    for(f=0;f<=25;f++){
        jumpaddr.set(f,Instruction[f]);              // jump address with 2 lsb zeroes
    }
    for(j=0;j<=4;j++){
        RegRs.set(j,Instruction[21+j]);                // Rs
    }
    for(k=0;k<=4;k++){
        RegRt.set(k,Instruction[16+k]);                //Rt
    }
    for(l=0;l<=4;l++){
        RegRd.set(l,Instruction[11+l]);
    }
    for(m=0;m<=4;m++){
        Reg4.set(m,Instruction[6+m]);
    }
    for(n=0;n<=4;n++){
        Reg5.set(n,Instruction[n]);
    }
     for(y=0;y<=2;y++){
        LSBInstruction.set(y,Instruction[y]);              //3 lsb bits of function for Rtype
    }
    for(p=0;p<=15;p++){
      signextimmd.set(p,Instruction[p]);                   //For immediate in I type
    }
    for(e=0;e<=15;e++){
      beq_signextimmd.set(2+e,Instruction[e]);            //storing from 2nd bit so that it will be a four
    }




       if(tempopcode==4){                                  //BEQ

        int d=0;
        if(imd[15]==1){
        for(d=0;d<=13;d++){
        beq_signextimmd.set(18+d,1);                        //sign extending 1
        }
       }
       else{
        for(d=0;d<=13;d++){
        beq_signextimmd.set(18+d,0);                         //sign extending 0
        }
       }


       myRF.ReadWrite(RegRs,RegRt,0,0,0);                   // Global variables --  Return Registers will be updated

       if(ReturnRegister1==ReturnRegister2){

        temp_pc_int = pc.to_ulong();

        temp_pc_int = temp_pc_int + 4 + beq_signextimmd.to_ulong();    //addition of present pc + 4 + immediate value

        pc = std::bitset<32>(temp_pc_int);



            }

          else {

                    cout<<"Branch instruction is not executed";

                    eltemp_pc_int = pc.to_ulong();

                    eltemp_pc_int = eltemp_pc_int + 4 ;

                    pc = std::bitset<32>(eltemp_pc_int);


            }


       }


       else if(tempopcode==2){                                         //condition for j type --jump intruction


              jtemp_pc_int = pc.to_ulong();

              jtemp_pc_int = jtemp_pc_int + 4 ;                           //addition of present pc + 4

              next_pc = std::bitset<32>(jtemp_pc_int);
              int a;

              for(a=0;a<=3;a++){
              pc.set(28+a,next_pc[28+a]);
              }
              for(a=0;a<=25;a++){
              pc.set(2+a,jumpaddr[a]);
              }
              pc.set(0,0);
              pc.set(1,0);



        }


    else{


   if((tempopcode == 35)||(tempopcode == 43)){             //Load word or Store word 35-100011,43-101011

      ALUop = 001;

      if(tempopcode==35){

       cout<<"It is a I type Load-word instruction\n";

       is_loadword=1;

        int d=0;
        if(imd[15]==1){
        for(d=0;d<=15;d++){
        signextimmd.set(16+d,1);                              //sign extending 1
        }
       }
       else{
        for(d=0;d<=15;d++){
        signextimmd.set(16+d,0);
        }
       }


       myRF.ReadWrite(RegRs,RegRt,0,0,0);            // after this return register 1 (global variable) will have the data of Rs

       bitset<32> ALUResult=myALU.ALUOperation(ALUop,ReturnRegister1,signextimmd);    //adding rs and signextimmd

       bitset<32> DtoReg = myDataMem.MemoryAccess(ALUResult,0,1,0);

       myRF.ReadWrite(0,0,RegRt,DtoReg,1);


       }

       else{                                                 // executing store word

       cout<<"It is a I type Store-word instruction\n";

        int d=0;
        if(imd[15]==1){
        for(d=0;d<=15;d++){
        signextimmd.set(16+d,1);                             //sign extending 1
        }
       }
       else{
        for(d=0;d<=15;d++){
        signextimmd.set(16+d,0);
        }
       }

       myRF.ReadWrite(RegRs,RegRt,0,0,0);                 // after this return register 1 (global variable) will have the data of Rs

       bitset<32> ALUResult=myALU.ALUOperation(ALUop,ReturnRegister1,signextimmd);    //adding rs and signextimmd

       myDataMem.MemoryAccess(ALUResult,ReturnRegister2,0,1);                      //writing in to data memory

       }



       }

      else if(tempopcode == 0){                                          //Condition for Rtype Inst

      ALUop = LSBInstruction;

      if(ALUop.to_ulong()==1){                                               //ADDU

        myRF.ReadWrite(RegRs,RegRt,RegRd,0,0);                      //Global variables --  Return Registers will be updated

        bitset<32> ALUResult_R =myALU.ALUOperation(ALUop,ReturnRegister1,ReturnRegister2);

        myRF.ReadWrite(0,0,RegRd,ALUResult_R,1);


      }
      if(ALUop.to_ulong()==3){                                                //SUBU

        myRF.ReadWrite(RegRs,RegRt,RegRd,0,0);                        //Global variables --  Return Registers will be updated

        bitset<32> ALUResult_R =myALU.ALUOperation(ALUop,ReturnRegister1,ReturnRegister2);

        myRF.ReadWrite(0,0,RegRd,ALUResult_R,1);



      }

      if(ALUop.to_ulong()==4){                                                 //AND

        myRF.ReadWrite(RegRs,RegRt,RegRd,0,0);                           //Global variables --  Return Registers will be updated

        bitset<32> ALUResult_R =myALU.ALUOperation(ALUop,ReturnRegister1,ReturnRegister2);

        myRF.ReadWrite(0,0,RegRd,ALUResult_R,1);


      }

       if(ALUop.to_ulong()==5){                                                 //OR

        myRF.ReadWrite(RegRs,RegRt,RegRd,0,0);                             //Global variables --  Return Registers will be updated

        bitset<32> ALUResult_R =myALU.ALUOperation(ALUop,ReturnRegister1,ReturnRegister2);

        myRF.ReadWrite(0,0,RegRd,ALUResult_R,1);

       }

       if(ALUop.to_ulong()==7){                                                   //NOR

        myRF.ReadWrite(RegRs,RegRt,RegRd,0,0);                             //Global variables --  Return Registers will be updated

        bitset<32> ALUResult_R =myALU.ALUOperation(ALUop,ReturnRegister1,ReturnRegister2);

        myRF.ReadWrite(0,0,RegRd,ALUResult_R,1);


       }

    }

    else{                                                                        // ADDIU

       cout<<"It is a I type instruction(other than Load-Store)\n";

       ALUop = LSBopcode;

       if(tempopcode==9){                                                        // ADDIU

        int d=0;
        if(imd[15]==1){
        for(d=0;d<=15;d++){
        signextimmd.set(16+d,1);                                                 //sign extending 1
        }
       }
       else{
        for(d=0;d<=15;d++){
        signextimmd.set(16+d,0);
        }
       }

        myRF.ReadWrite(RegRs,RegRt,0,0,0);                                 //Global variables --  Return Registers will be updated

        bitset<32> ALUResult_I =myALU.ALUOperation(ALUop,ReturnRegister1,signextimmd);

        myRF.ReadWrite(0,0,RegRt,ALUResult_I,1);


       }

    }

    pc_int=pc.to_ulong();

    pc_int=pc_int+4;

    pc=std::bitset<32>(pc_int);

       }

  myRF.OutputRF();
                                    // dump RF

  }

  myDataMem.OutputDataMem();                         // dump data mem

  return 0;

}
