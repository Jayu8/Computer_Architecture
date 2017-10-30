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
           	if (WrtEnable == 1)
        	{
        		Registers[WrtReg.to_ulong()] = WrtData;
        		//cout<<WrtData;
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
               int index = Address.to_ulong();
          	   if (writemem == 1)
          	   {
          	
	                DMem[index]   = bitset<8> (WriteData.to_string().substr(0,8)); 
	                DMem[index+1] = bitset<8> (WriteData.to_string().substr(8,8));
	                DMem[index+2] = bitset<8> (WriteData.to_string().substr(16,8));
	                DMem[index+3] = bitset<8> (WriteData.to_string().substr(24,8));
	           }
          	   else if (readmem == 1)
          	   {
               		readdata = bitset<32>((DMem[index].to_ulong()) << 24 | (DMem[index+1].to_ulong()) << 16 | (DMem[index+2].to_ulong()) << 8 | (DMem[index+3].to_ulong())); 
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
    bitset<1> WrtEnableRF = bitset<1> (0);
    bitset<5> Rs = 0;
    bitset<5> Rt = 0;
    bitset<5> Rd = 0;
    bitset<3> funct = 0 ;
    bitset<5> R_d = 0; 
    bitset<3> ALUOPTC=0; 
    bitset<16> Immediate = bitset<16>(0);
    bitset<32> SignExtImm = bitset<32> (0); 
    bitset<6> OPCODE_temp;
    bitset<6> OPCODE_NEW;
    bitset<32> Store_Rd;
    bitset<1> WriteMemEn=0;
    int WER=0;
    bitset<26> Address = 0;

    //bitset<32> SignExtImm = bitset<32> (0); //error debug

    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

   

    while (1) //each loop body corresponds to one clock cycle. PC.to_ulong() < 40
	{
        
        /***********     WRITE BACK STAGE    *********/
       // myRF.ReadWrite(0, 0, R_d, ALUresult, WrtEnableRF);  // Rs=0 , Rd=0 not required in WB




        //cout<< "                                                                      PC Value is "<< PC<<"\n";

        //cout << "\n Write Back  Stage" << "\n";        
        if(WER==1)
        {
          WrtEnableRF = 1;
        }


        
        if ( OPCODE_NEW == 0b100011 )   
        { 
           //cout << " LOAD Instruction\n";
           WriteMemEn = 0; 
           Dmem_Load_Value = myDataMem.MemoryAccess(ALUresult,0,1,WriteMemEn);
           myRF.ReadWrite(0, 0, R_d, Dmem_Load_Value, WrtEnableRF);
           //cout <<"R_d is " << R_d << "                                                 Loaded value   " <<Dmem_Load_Value << "\n";
           dumpResults(PC,R_d,Dmem_Load_Value,WrtEnableRF,0,0,0); //pc, addr, data,en       

         }
        else if (OPCODE_NEW == 0b101011)
        {
            //cout << " STORE Instruction\n";
            WriteMemEn = 1;
            Dmem_Load_Value = myDataMem.MemoryAccess(ALUresult,Store_Rd,0,WriteMemEn);
            dumpResults(PC,0,0,0,ALUresult,Store_Rd, WriteMemEn); //pc, addr, data,en       
            //cout <<"R_d is " << ALUresult << "                                               Storing value  " <<Store_Rd< "\n";
        }
        else if (OPCODE_NEW == 0b001001)
        {
        	myRF.ReadWrite(0, 0, R_d, ALUresult, WrtEnableRF);	
        	dumpResults(PC,R_d,ALUresult,WrtEnableRF,0,0,0);
        	// cout <<"R_d is " << R_d << "                                                 ADDIU instruction  " <<ALUresult << "\n";
        }
       else if(OPCODE_NEW != 0b101011 && OPCODE_NEW != 0b100011)
        {
           // cout << " OTHER than LOAD and STORE Instruction\n";
            myRF.ReadWrite(0, 0, R_d, ALUresult, WrtEnableRF);          
            dumpResults(PC,R_d,ALUresult,WrtEnableRF,0,0,0);
            //cout <<"R_d is " << R_d << "                                                 R type stored  " <<ALUresult << "\n";
        }
        

        /***********     READ REGISTERS AND EXECUTE STAGE    *********/
         //cout << "\n ID/ RF / Ex stage" << "\n";
        // For Read mode WrtEnable is 0
        WrtEnableRF = 0;
        WER = 0;
        
        
        // Read the"Rs_2 is"<<  R gi"Rt_2 is "<<ster values 
        //cout << "Rs_2 is"<< Rs << " Rt_2 is   "<< Rt<< "\n" ;
        myRF.ReadWrite( Rs, Rt, 0, 0, WrtEnableRF); 
        
        if(myRF.Rd1val == myRF.Rd2val && OPCODE == 0b000100)
        {
             //cout<<"                                            Branch Operation Started"<<"\n";
             bitset<18>Immediateval = bitset<18>(Immediate.to_ulong() << 2);
            if(Immediate.to_string().substr(0,1) == "1")
            {
              SignExtImm = (Immediateval.to_ulong() | 0b11111111111111000000000000000000);
            }
            else
            {
              SignExtImm = (Immediateval.to_ulong() | 0b00000000000000000000000000000000);            
            }
              PC = bitset<32> (PC.to_ulong()+SignExtImm.to_ulong());
              PC = PC.to_ulong() - 4;
              //dumpResults(PC,0,0,0,0,0,0);
              //cout<< "PC is value" << PC <<"\n";
              //cout<< "Branch is" << SignExtImm <<"\n";
              //exit(0);
        }
        else if (OPCODE == 0b000010)
        {
          
            bitset<32> jump_inst = bitset<32>((PC.to_ulong()-4));  
            bitset<4> offset =     bitset<4>(jump_inst.to_string().substr(0,3));
            bitset<32> off = bitset<32>(offset.to_ulong()<<28);
            bitset<32>Address_shift = bitset<32>(Address.to_ulong()<<2);
            bitset<32>Address_new = bitset<32>(Address_shift|off);
            PC = Address_new.to_ulong();
            //dumpResults(PC,0,0,0,0,0,0);
            //cout<< " jump PC "<< jump_inst<<"\n";
            //cout<< "Address"<<Address_new<<"\n";
            //cout<< "PC Value"<<PC<<"\n";
            //exit(0);
        
        }
        else
        {
            //cout<<"                                            No Branch Operation Started"<<"\n";
            R_d = Rd;
            if (OPCODE_temp != 0b000000)
            {
              R_d = Rt;
              Store_Rd =myRF.Rd2val;
              myRF.Rd2val = SignExtImm; 
              ALUOPTC = 1;          
            }
            OPCODE_NEW = OPCODE_temp;
            //cout << "Rs_2 value is"<< myRF.Rd1val << " Rt_2 value is/ Immediate "<< myRF.Rd2val<< "\n" ;
            // Get the ALU result (is there  any ?) 
            ALUresult= myALU.ALUOperation(ALUOPTC, myRF.Rd1val,myRF.Rd2val);
            // Condition telling when to we can write to RF.   
            if (ALUOPTC != 0)
              {
                 WER = 1;       
              }      
        }
        /***********     INSTRUCTION FETCH STAGE    *********/
        Instruction = myInsMem.ReadMemory( PC.to_ulong() )  ;
        conv_bit_to_string = Instruction.to_string<char,std::string::traits_type,std::string::allocator_type>();
        OPCODE = bitset<6>(conv_bit_to_string.substr(0,6));
        OPCODE_temp = OPCODE;
        //cout << "\n Instruction Fetch Stage" << "\n";
        //cout<< "OPCODE" << OPCODE << "\n";
        if (OPCODE  ==  0b000000)
        {
              //cout << "R-TYPE" << "\n";  
              Rs = bitset<5>(conv_bit_to_string.substr(6,5));
              Rt = bitset<5>(conv_bit_to_string.substr(11,5));
              Rd = bitset<5>(conv_bit_to_string.substr(16,5));
              funct = bitset<3>(conv_bit_to_string.substr(29,3)); 
              ALUOPTC = bitset<3>(funct);
             
              //cout << " Rs is " << Rs << " Rt is " << Rt << " Rd is "<< Rd << " funct is "<< funct<<"\n";
              //exit(0);
        }
         
        else if(OPCODE  ==  0b000010 | OPCODE  ==  0b111111 )
        {
              //cout << "J- TYPE" << "\n";
              if (OPCODE == 0b111111)
              {
                cout <<  "Instructions complete \n";
                break;
                
              }
              Address = bitset<26>(conv_bit_to_string.substr(6,26));
              //cout <<  " Address is "<< Address<<"\n";
        }

        else 
        {
              
              //cout << "I-TYPE" << "\n"; 
              Rs = bitset<5>(conv_bit_to_string.substr(6,5));
              Rt = bitset<5>(conv_bit_to_string.substr(11,5));
              Immediate = bitset<16>(conv_bit_to_string.substr(16,16));
              SignExtImm = bitset<32> (0); 
              bitset<32>Immediateval = bitset<32>(Immediate.to_ulong()|0x00);
              SignExtImm = bitset<32>(SignExtImm | Immediateval); 
              if(Immediate.to_string().substr(0,1) == "1")
                {
                  SignExtImm = (SignExtImm.to_ulong() | 0b11111111111111110000000000000000);

                }
              ALUOPTC = bitset<3>(OPCODE.to_string().substr(3,3));               
              //cout << " Rs is " << Rs << " Rt is " << Rt << " SignImmediate is  "<< SignExtImm<<"\n";     
          }
            /***********     UPDATING PC VALUE FOR EVER CYCLE   *********/
               PC = bitset<32> (PC.to_ulong() + 4); 
    }
    /***********    DUMPING FINAL STATE OF  RF and DATA MEM    *********/
	      myRF.OutputRF();   
        myDataMem.OutputDataMem();       	  
          return 0;       
}
