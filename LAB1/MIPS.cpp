#include<stdlib.h>
#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
#include<typeinfo>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> Rd1val, Rd2val; 
   
   	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            
            bitset<32> Rd1val;
            bitset<32> Rd2val;


            // What happens when we send zero as just a variablr , is there ever a zero in PIPELINE??

           	if (WrtEnable == 1)
        	{
        		Registers[WrtReg.to_ulong()] = WrtData;
        	}
  
	       	Rd1val	= Registers[RdReg1.to_ulong()];
         	Rd2val	= Registers[RdReg2.to_ulong()];
        	
        	
        	//return Rd1val,Rd2val;

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
             	if (ALUOP == 0b001)
             	{
             		ALUresult = bitset<32>(oprand1.to_ulong() + oprand2.to_ulong()); //ADDU	
             	}
             	else if (ALUOP == 0b011)
             	{
             		ALUresult = bitset<32>(oprand1.to_ulong() - oprand2.to_ulong()); //SUBU	
             	}
             	else if (ALUOP == 0b100)
             	{
             		ALUresult = bitset<32>(oprand1.to_ulong() & oprand2.to_ulong()); //AND	
             	}
             	else if (ALUOP == 0b101)
             	{
             		ALUresult = bitset<32>(oprand1.to_ulong() | oprand2.to_ulong()); //OR	
             	}
             	else if (ALUOP == 0b111)
             	{
             		ALUresult =  bitset<32>(~(oprand1.to_ulong() | oprand2.to_ulong())); //NOR	
             	}

                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          bitset<32> Instruction_byte;

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
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    

               // implement by you. (Read the byte at the ReadAddress and the following three byte).

              	Instruction = bitset<32> (0);

              	for (int i=3;i>=0;i--)
              	{
              	  Instruction_byte = bitset<32> ( IMem[ ReadAddress.to_ulong() + (3-i) ].to_ulong() );
  				  Instruction = Instruction_byte.to_ulong() << (8*i) | Instruction.to_ulong() ; 
  				  
              	}
              

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

          	// TO - DO
          	   if (readmem == 1){
               readdata = bitset<32>((DMem[Address.to_ulong()].to_ulong()) << 24 | (DMem[Address.to_ulong()+1].to_ulong()) << 16 | (DMem[Address.to_ulong()+2].to_ulong()) << 8 | (DMem[Address.to_ulong()+3].to_ulong())); 
               }
               
               else if (writemem == 1){
                (DMem[Address.to_ulong()]) = bitset<8> (WriteData.to_string().substr(0,8)); 
                (DMem[Address.to_ulong()+1]) = bitset<8> (WriteData.to_string().substr(8,8));
                (DMem[Address.to_ulong()+2]) = bitset<8> (WriteData.to_string().substr(16,8));
                (DMem[Address.to_ulong()+3]) = bitset<8> (WriteData.to_string().substr(24,8));
               }
               return readdata;         
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

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
                  ofstream fileout;
                  
                  fileout.open("Results.txt",std::ios_base::app);
                  if (fileout.is_open())
                  {
                  
                  fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;
                     
                  }
                  else cout<<"Unable to open file";
                  fileout.close();

}

   
int main()
{      
	
    bitset<32> PC = bitset<32> (0);
    bitset<32> Instruction;
    bitset<6> OPCODE;
    string conv_bit_to_string;
    bitset<32> ALUresult;
    bitset<32> Dmem_Load_Value; 

    bitset<32> SignExtImm = bitset<32> (0); //error debug

    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    while (1) //each loop body corresponds to one clock cycle.
	{
    	//cout <<  "PC value is "<< PC.to_ulong() << "\n";  

    	/****** Instruction fetfch stage **********/  
		Instruction = myInsMem.ReadMemory( PC.to_ulong() )	;
		//cout <<  "Inst mem value is "<< Instruction << "\n";
		conv_bit_to_string = Instruction.to_string<char,std::string::traits_type,std::string::allocator_type>();
		OPCODE = bitset<6>(conv_bit_to_string.substr(0,6));
		//cout << typeid(Instruction).name() << "\n";
		if (OPCODE  ==  0b000000)
		{
            cout << "R-TYPE" << "\n";  
			bitset<5> Rs = bitset<5>(conv_bit_to_string.substr(6,5));
			bitset<5> Rt = bitset<5>(conv_bit_to_string.substr(11,5));
			bitset<5> Rd = bitset<5>(conv_bit_to_string.substr(16,5));
            bitset<6> funct = bitset<6>(conv_bit_to_string.substr(26,6)); 
            cout << " Rs is " << Rs << " Rt is " << Rt << " Rd is "<< Rd << " funct is "<< funct<<"\n";
            
            myRF.ReadWrite(Rs, Rt, Rd, 0, 0); // Register addresses




		}
		else if (OPCODE == 0b000010 | OPCODE == 0b111111)
		{
			cout << "J-TYPE" << "\n"; 
			bitset<26> Address = bitset<26>(conv_bit_to_string.substr(6,26));
			cout <<  " Address is "<< Address<<"\n";
			if (OPCODE == 0b000010)
			{

				bitset<32> PC_J_4 = bitset<32> (PC.to_ulong() + 4);
				bitset<32> PC_MSB_4 = bitset<32>(PC_J_4.to_string().substr(0,4)); // Theoretically correct
				PC_MSB_4 = PC_MSB_4 << 28;
 				
 				bitset<32>Address_new = bitset<32>(Address.to_ulong());
 				Address_new = Address_new << 2;

				Address_new = Address_new | PC_MSB_4; 

				PC = Address_new; 
				continue;

			}
			else if (OPCODE == 0b111111)
			{
				cout <<  "Instructions complete \n";
				exit(0);
			}
		}
			
		else 
		{
			cout << "I-TYPE" << "\n"; 
			bitset<5> Rs = bitset<5>(conv_bit_to_string.substr(6,5));
			bitset<5> Rt = bitset<5>(conv_bit_to_string.substr(11,5));
			bitset<16> Immediate = bitset<16>(conv_bit_to_string.substr(16,16));
			//bitset<32> SignExtImm = bitset<32> (0); 



			cout << " Rs is " << Rs << " Rt is " << Rt << " Immediate is  "<< Immediate<<"\n";


			if (OPCODE == 0b100011)
			{
				cout << "LOAD_INSTRUCTION" << "\n";

				myRF.ReadWrite(Rs,0,0,0,0); // we are doing this to get Rd1val, WRONG
				bitset<32>Immediate = bitset<32>(Immediate);
				SignExtImm = bitset<32>(SignExtImm | Immediate);	
				if(Immediate.to_string().substr(0,1) == "1")
				{
					SignExtImm = (SignExtImm.to_ulong() | 0b11111111111111110000000000000000); 
				}
					
				ALUresult = myALU.ALUOperation(0b001,myRF.Rd1val,SignExtImm);

				Dmem_Load_Value = myDataMem.MemoryAccess(ALUresult,0,1,0); // dont know

				myRF.ReadWrite(0,0,Rt,Dmem_Load_Value,1);
				//complete

			}
			else if (OPCODE == 0b101011)
			{
				cout << "STORE_INSTRUCTION" << "\n";

				myRF.ReadWrite(Rs,Rt,0,0,0);

				bitset<32>Immediate = bitset<32>(Immediate);
				SignExtImm = bitset<32>(SignExtImm | Immediate);	
				if(Immediate.to_string().substr(0,1)=="1")
				{
					SignExtImm = (SignExtImm.to_ulong() | 0b11111111111111110000000000000000); 
				}

				ALUresult = myALU.ALUOperation(0b001,myRF.Rd1val,SignExtImm);

				Dmem_Load_Value = myDataMem.MemoryAccess(ALUresult,myRF.Rd1val,0,1); // Dmem_Load_Value IS not present

			} 
			else if (OPCODE == 0b000100)
			{
				cout << "BEQ_INSTRUCTION" << "\n";

				bitset<32> PC_I_4 = bitset<32> (PC.to_ulong() + 4);

				myRF.ReadWrite(Rs,Rt,0,0,0);

				if (myRF.Rd1val == myRF.Rd2val)
				{
					
					bitset<32>Immediate = bitset<32>(Immediate);
				    SignExtImm = bitset<32>(SignExtImm | Immediate);
					if(Immediate.to_string().substr(0,1)=="1")
					{
						SignExtImm = (SignExtImm.to_ulong() | 0b11111111111111110000000000000000); 
					}
					
					SignExtImm = SignExtImm << 2;
					PC = PC_I_4 | SignExtImm;
					continue;
				}

			}
			else if (OPCODE == 0b001001) 
			{
				cout << "ADDIU_INSTRUCTION" << "\n";

				myRF.ReadWrite(Rs, Rt, 0, 0, 0); // WRONG we do this to get Rd1val and RD2val

				bitset<32>Immediate = bitset<32>(Immediate);
				SignExtImm = bitset<32>(SignExtImm | Immediate);	
				if(Immediate.to_string().substr(0,1)=="1")
				{
					SignExtImm = (SignExtImm.to_ulong() | 0b11111111111111110000000000000000); 
				}
					

				ALUresult = myALU.ALUOperation(0b001,myRF.Rd1val,SignExtImm);

				myRF.ReadWrite(0,0,Rt,ALUresult,1);  // What happens if we send zero here??WRONG PIPELINE
				// Complete

			}


		}
	 
	
    // At the end of each cycle, fill in the corresponding data into "dumpResults" function to output files.
    // dumpResults(pc, , , , , , );      
    
    	PC = bitset<32> (PC.to_ulong() + 4);    
    }
	      //myRF.OutputRF(); // dump RF; 
          //myDataMem.OutputDataMem(); // dump data mem
      
          return 0;
        
}
